// FileTranDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "FileTranDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileTranDlg dialog


CFileTranDlg::CFileTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTranDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileTranDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFileTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileTranDlg)
	DDX_Control(pDX, IDC_TASK_LIST, m_TaskList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileTranDlg, CDialog)
	//{{AFX_MSG_MAP(CFileTranDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileTranDlg message handlers
BOOL CFileTranDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the small icon for this dialog
	SetIcon(AfxGetApp()->LoadIcon(IDI_OL_TRANS), FALSE);
	CenterWindow();
	// TODO: Add extra initialization here
	////////////////////////////////////////////////////////////////////////
	//create file tran list
	ListView_SetExtendedListViewStyle(m_TaskList.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_TaskList.InsertColumn(0, "Զ������", LVCFMT_LEFT, 120);
	m_TaskList.InsertColumn(1, "����·��", LVCFMT_LEFT, 140);
	m_TaskList.InsertColumn(2, "Զ��·��", LVCFMT_LEFT, 140);
	m_TaskList.InsertColumn(3, "�ļ���С", LVCFMT_LEFT, 70);
	m_TaskList.InsertColumn(4, "���״̬", LVCFMT_LEFT, 70);
	m_TaskList.InsertColumn(5, "��������", LVCFMT_LEFT, 70);
	m_TaskList.InsertColumn(6, "ʣ��ʱ��", LVCFMT_LEFT, 70);
	m_ImageList.Create(14, 14, ILC_COLOR24 | ILC_MASK, 2, 2);
	HICON hIcon0 = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_FILE_UP));
	m_ImageList.Add(hIcon0);
	hIcon0 = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_FILE_DOWN));
	m_ImageList.Add(hIcon0);
	m_TaskList.SetImageList(&m_ImageList, LVSIL_SMALL);

	return TRUE;
}

BOOL CFileTranDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int)pMsg->wParam;
		if (nVirtKey == VK_RETURN)
		{
			//����ǻس�����������Ҫ��������,����ʲôҲ����
			return TRUE;
		}
		if (nVirtKey == VK_ESCAPE)
		{
			//�����ESC����������Ҫ��������,����ʲôҲ����
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CFileTranDlg::AddTranTask(SOCKET s, BOOL bDown)
{
	if (s == INVALID_SOCKET)
		return;

	if (bDown)
		ThreadTemplate::StartThread<CFileTranDlg, DWORD>(this, &CFileTranDlg::__ThreadFileDown, s);
	else
		ThreadTemplate::StartThread<CFileTranDlg, DWORD>(this, &CFileTranDlg::__ThreadFileUp, s);
}

DWORD CFileTranDlg::__ThreadFileDown(SOCKET m_ConnSocket)
{
	HANDLE hDownFile = INVALID_HANDLE_VALUE;
	DWORD  dwDownFileSize = 0, dwBufSize = 4096, dwBytes, dwTotalRecv = 0;
	DWORD  dwStartTime;
	BYTE   RecvBuffer[4096];
	FileOpt m_FileOpt;

	//receive file message
	memset(&m_FileOpt, 0, sizeof(FileOpt));
	int nRet = recv(m_ConnSocket, (char *)&m_FileOpt, sizeof(FileOpt), 0);
	if (nRet <= 0)
	{
		shutdown(m_ConnSocket, 0x02);
		closesocket(m_ConnSocket);
		return 0;
	}

	dwDownFileSize = m_FileOpt.iSize;
	if (dwDownFileSize <= 0)
	{
		shutdown(m_ConnSocket, 0x02);
		closesocket(m_ConnSocket);
		return 0;
	}

	//��ʾ������Ϣ
	AddTaskToList(m_ConnSocket, m_FileOpt.cScrFile, m_FileOpt.cDstFile, m_FileOpt.iSize, TRUE);

	hDownFile = CreateFile(m_FileOpt.cDstFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hDownFile == INVALID_HANDLE_VALUE)
	{//���ش����ļ�ʧ��
		ErrorTaskToList(m_ConnSocket, "���ش����ļ�ʧ��");
		shutdown(m_ConnSocket, 0x02);
		closesocket(m_ConnSocket);
		return 0;
	}

	dwStartTime = GetTickCount(); //��ʼʱ��
	while (dwDownFileSize > 0)
	{
		nRet = recv(m_ConnSocket, (char*)&RecvBuffer, dwBufSize, 0);
		if (nRet == 0)
			break;
		if (nRet == SOCKET_ERROR)
		{
			ErrorTaskToList(m_ConnSocket, "�ļ�����ʧ��");
			break;
		}

		dwTotalRecv += nRet;
		//����������Ϣ
		UpdataTaskToList(m_ConnSocket, m_FileOpt.iSize, dwTotalRecv, dwStartTime);

		WriteFile(hDownFile, RecvBuffer, nRet, &dwBytes, NULL);
		dwDownFileSize = dwDownFileSize - dwBytes;
	}

	CloseHandle(hDownFile);
	shutdown(m_ConnSocket, 0x02);
	closesocket(m_ConnSocket);

	return 0;
}

DWORD CFileTranDlg::__ThreadFileUp(SOCKET m_ConnSocket)
{
	HANDLE hUpFile = INVALID_HANDLE_VALUE;
	DWORD  dwDownFileSize = 0, dwBufSize = 4096, dwBytes, dwTotalSend = 0;
	DWORD  dwStartTime;
	BYTE   SendBuffer[4096];
	FileOpt m_FileOpt;

	//receive file message
	memset(&m_FileOpt, 0, sizeof(FileOpt));
	int nRet = recv(m_ConnSocket, (char *)&m_FileOpt, sizeof(FileOpt), 0);
	if (nRet <= 0)
	{
		shutdown(m_ConnSocket, 0x02);
		closesocket(m_ConnSocket);
		return 0;
	}

	if (m_FileOpt.iSize <= 0)
	{
		shutdown(m_ConnSocket, 0x02);
		closesocket(m_ConnSocket);
		return 0;
	}

	hUpFile = CreateFile(m_FileOpt.cDstFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hUpFile == INVALID_HANDLE_VALUE)
	{   //��ȡ�����ļ�ʧ��
		ErrorTaskToList(m_ConnSocket, "��ȡ�����ļ�ʧ��");
		shutdown(m_ConnSocket, 0x02);
		closesocket(m_ConnSocket);
		return 2;
	}

	dwDownFileSize = GetFileSize(hUpFile, NULL);
	m_FileOpt.iSize = dwDownFileSize;
	//��ʾ������Ϣ
	AddTaskToList(m_ConnSocket, m_FileOpt.cDstFile, m_FileOpt.cScrFile, m_FileOpt.iSize, FALSE);

	dwStartTime = GetTickCount(); //��ʼʱ��
	//ѭ�������ļ�����
	while (dwDownFileSize > 0)//	if (bIsDownFile)
	{
		if (ReadFile(hUpFile, SendBuffer, 4096, &dwBytes, NULL))
		{
			nRet = send(m_ConnSocket, (char*)&SendBuffer, dwBytes, 0);//��������
			if (nRet == 0)
				break;
			if (nRet == SOCKET_ERROR)
			{
				ErrorTaskToList(m_ConnSocket, "�ļ�����ʧ��");
				break;
			}

			dwTotalSend += nRet;
			//����������Ϣ
			UpdataTaskToList(m_ConnSocket, m_FileOpt.iSize, dwTotalSend, dwStartTime);

			dwDownFileSize = dwDownFileSize - dwBytes;
		}
		else
		{//CMD_READFILEEOR
			break;
		}
	}

	CloseHandle(hUpFile);
	shutdown(m_ConnSocket, 0x02);
	closesocket(m_ConnSocket);

	return 0;
}

void CFileTranDlg::AddTaskToList(SOCKET s, LPCTSTR szDst, LPCTSTR szSrc, DWORD dwSize, BOOL bDown)
{
	sockaddr_in addr;
	int cb = sizeof(addr);
	int ir = getpeername(s, (sockaddr*)&addr, &cb);

	int iItem = m_TaskList.GetItemCount();
	m_TaskList.InsertItem(iItem, "", bDown);
	m_TaskList.SetItemData(iItem, s);
	m_TaskList.SetItemText(iItem, 0, inet_ntoa(addr.sin_addr));
	m_TaskList.SetItemText(iItem, 1, szSrc);
	m_TaskList.SetItemText(iItem, 2, szDst);
	m_TaskList.SetItemText(iItem, 3, __MakeFileSizeString(dwSize));
}

void CFileTranDlg::UpdataTaskToList(SOCKET s, DWORD dwFileLen, DWORD dwRecvLen, DWORD dwStartTime)
{
	DWORD dwEndTime = GetTickCount();
	DWORD dwUsedTime = dwEndTime - dwStartTime;
	if (dwUsedTime == 0)
		return;
	DWORD dwDownSpeed = dwRecvLen / dwUsedTime * 1000;
	DWORD dwRemainTime = (dwFileLen - dwRecvLen) / (dwRecvLen / dwUsedTime) / 1000;

	DWORD dwNum = m_TaskList.GetItemCount();
	for (DWORD dwItem = 0; dwItem < dwNum; dwItem++)
	{
		if (m_TaskList.GetItemData(dwItem) == s)
		{
			if (dwFileLen == dwRecvLen)
			{
				m_TaskList.SetItemText(dwItem, 4, _T("���"));
				m_TaskList.SetItemData(dwItem, 0);
			}
			else
				m_TaskList.SetItemText(dwItem, 4, __MakeFileSizeString(dwRecvLen));
			m_TaskList.SetItemText(dwItem, 5, __MakeFileSizeString(dwDownSpeed) + _T("/��"));
			m_TaskList.SetItemText(dwItem, 6, __MakeTimeString(dwRemainTime));
			break;
		}
	}
}

void CFileTranDlg::ErrorTaskToList(SOCKET s, LPCTSTR szError)
{
	DWORD dwNum = m_TaskList.GetItemCount();
	for (DWORD dwItem = 0; dwItem < dwNum; dwItem++)
	{
		if (m_TaskList.GetItemData(dwItem) == s)
		{
			m_TaskList.SetItemText(dwItem, 4, szError);
			m_TaskList.SetItemData(dwItem, 0);
			break;
		}
	}
}

CString CFileTranDlg::__MakeFileSizeString(DWORD dwSize)
{
	CString strRet;
	if (dwSize < 1024)
		strRet.Format(_T("%d�ֽ�"), dwSize);
	else if (dwSize > 1024 && dwSize < 1024 * 1024)
		strRet.Format(_T("%4.2f KB"), (float)dwSize / 1024);
	else if (dwSize > 1024 * 1024 && dwSize < 1024 * 1024 * 1024)
		strRet.Format(_T("%4.2f MB"), (float)dwSize / 1024 / 1024);
	else
		strRet.Format(_T("%4.2f GB"), (float)dwSize / 1024 / 1024 / 1024);
	return strRet;
}

CString CFileTranDlg::__MakeTimeString(DWORD dwTime)
{
	CString strRet;
	if (dwTime < 60)
		strRet.Format(_T("%d��"), dwTime);
	else if (dwTime < 60 * 60)
		strRet.Format(_T("%d����"), dwTime / 60);
	else if (dwTime < 60 * 60 * 24)
		strRet.Format(_T("%dСʱ"), dwTime / 60 / 60);
	else
		strRet.Format(_T("%d��"), dwTime / 60 / 60 / 24);
	return strRet;
}