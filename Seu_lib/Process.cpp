#include "StdAfx.h"
#include "Process.h"
#include <tlhelp32.h>//???

void ProcessList(char *pBuf, LPMsgHead lpMsgHead)
{
	//?
	HANDLE hToken;
    OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken);
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid);
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);

	HANDLE hSnapshot = NULL;
	HANDLE hModule = NULL;
	HANDLE hProcess = NULL;
	//????????heap?飨module????
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnapshot == NULL)
	{
		lpMsgHead->dwCmd  = CMD_PROCLISTERR;
		lpMsgHead->dwSize = 0;
		return;
	}

	PROCESSENTRY32 PInfo;
	MODULEENTRY32 MInfo;
	PInfo.dwSize=sizeof(PROCESSENTRY32);
	MInfo.dwSize=sizeof(MODULEENTRY32);

	ProcessInfo Info;
	DWORD dwLen = 0;
	BOOL bContinue = FALSE;
	bContinue = Process32First(hSnapshot, &PInfo);
	while(bContinue)
	{
		hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PInfo.th32ProcessID);
		Module32First(hModule, &MInfo);
		CloseHandle(hModule);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,PInfo.th32ProcessID); 
		memset(&Info, 0, sizeof(ProcessInfo));
		Info.dwPid = PInfo.th32ProcessID;
		Info.dwThreads = PInfo.cntThreads;
		Info.dwPriClass= GetPriorityClass(hProcess);
		lstrcpyn(Info.FileName, PInfo.szExeFile, 32);
		lstrcpyn(Info.FilePath, MInfo.szExePath, 128);
		//д?
		memcpy(pBuf+dwLen,&Info,sizeof(ProcessInfo));
		dwLen += sizeof(ProcessInfo);

		bContinue = Process32Next(hSnapshot, &PInfo); 	
	}

	if (hSnapshot)
		CloseHandle(hSnapshot);

	lpMsgHead->dwCmd  = 0;
	lpMsgHead->dwSize = dwLen;
}

void ProcessKill(char *pBuf, LPMsgHead lpMsgHead)
{
	// ???y?
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, lpMsgHead->dwExtend1);
	if (hProcess != NULL)
	{
		TerminateProcess(hProcess, 0);
	}

	lpMsgHead->dwCmd  = CMD_PROCKILLERR;
	lpMsgHead->dwSize = 0;

	CloseHandle(hProcess);

	return;
}
