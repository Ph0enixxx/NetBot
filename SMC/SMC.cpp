// SMC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "..\\..\\debug.h"

void xorCode(char *soure, int dLen, char Key)
{
    for (int i = 0; i < dLen; i++)
    {
        soure[i] ^= Key;
    }
}

void SMCRun(char *pBuf,char key)
{
    const char *szSecName = ".SMC";
    short nSec;
    PIMAGE_DOS_HEADER pDosHeader;
    PIMAGE_NT_HEADERS pNtHeader;
    PIMAGE_SECTION_HEADER pSec;
    pDosHeader=(PIMAGE_DOS_HEADER)pBuf;
    pNtHeader=(PIMAGE_NT_HEADERS)&pBuf[pDosHeader->e_lfanew];
    nSec=pNtHeader->FileHeader.NumberOfSections;
    pSec=(PIMAGE_SECTION_HEADER)&pBuf[ sizeof(IMAGE_NT_HEADERS)+pDosHeader->e_lfanew];
    for (int i=0;i<nSec;i++)
    {
        if (strcmp((char *)&pSec->Name,szSecName)==0)
        {
            int pack_size;
            char *packStart; 
            pack_size=pSec->SizeOfRawData;
            packStart = &pBuf[pSec->VirtualAddress];
            //VirtualProtect(packStart,pack_size,PAGE_EXECUTE_READWRITE,&old);
            xorCode(packStart,pack_size,key);
            //AfxMessageBox(_T("SMC���ܳɹ���"));
            return;
        }
        pSec++;
    }
}

void SMCEncrypt(HANDLE hFile, char key)
{
    HANDLE hMap;
    const char *szSecName = ".SMC";
    char *pBuf;
    int size;
    short nSec;
    PIMAGE_DOS_HEADER pDosHeader;
    PIMAGE_NT_HEADERS pNtHeader;
    PIMAGE_SECTION_HEADER pSec;
	
    size = GetFileSize(hFile,0);
    hMap=CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,size,NULL);
    if (hMap==INVALID_HANDLE_VALUE)
    {
		ErrMsg(_T("ӳ��ʧ��"));
		return ;
    }
    pBuf=(char *)MapViewOfFile(hMap,FILE_MAP_WRITE|FILE_MAP_READ,0,0,size);
    if(!pBuf)
	{
		ErrMsg(_T("ӳ��ʧ��"));
		return ;
    }
    pDosHeader=(PIMAGE_DOS_HEADER)pBuf;
    pNtHeader=(PIMAGE_NT_HEADERS)&pBuf[pDosHeader->e_lfanew];
    if (pNtHeader->Signature!=IMAGE_NT_SIGNATURE)
    {
        ErrMsg(_T("������Ч��win32 ��ִ���ļ�"));
        UnmapViewOfFile(pBuf);
		CloseHandle(hMap);
		return;
    }
    nSec=pNtHeader->FileHeader.NumberOfSections;
    pSec=(PIMAGE_SECTION_HEADER)&pBuf[ sizeof(IMAGE_NT_HEADERS)+pDosHeader->e_lfanew];
    for (int i=0;i<nSec;i++)
    {
        if (strcmp((char *)&pSec->Name,szSecName)==0)
        {
            int pack_size;
            char *packStart; 
            pack_size=pSec->SizeOfRawData;
            packStart = &pBuf[pSec->PointerToRawData];
            xorCode(packStart,pack_size,key);
            ErrMsg(_T("SMC���ܳɹ���"));
            UnmapViewOfFile(pBuf);
			CloseHandle(hMap);
			return;
        }
        pSec++;
    }
    ErrMsg(_T("δ�ҵ� .SMC ��"));

    UnmapViewOfFile(pBuf);
    CloseHandle(hMap);
    return ;
}

#pragma code_seg(".SMC")
void Fun1()
{
    MessageBoxA(NULL,"����ִ�б������㷨��",NULL,MB_OK);
}
#pragma code_seg()
#pragma comment(linker, "/SECTION:.SMC,ERW")

int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	return 0;
}

