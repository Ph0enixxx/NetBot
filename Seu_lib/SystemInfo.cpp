#include "StdAfx.h"
#include "SystemInfo.h"

BOOL GetSystemInfo(SysInfo& info)
{
	memset(&info,0,sizeof(SysInfo));
	/////get computer name///////
	DWORD iSize = 64;
	GetComputerName(info.cComputer,&iSize);
	///////get system version//////////
	char szSystem[32];
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
				if (osvi.dwMinorVersion == 0) lstrcpy(szSystem, "Win Vista");
				if (osvi.dwMinorVersion == 1) lstrcpy(szSystem, "Win 7");
				if (osvi.dwMinorVersion == 2) lstrcpy(szSystem, "Win 8");
				if (osvi.dwMinorVersion == 3) lstrcpy(szSystem, "Win 8.1");
			}
			else if (osvi.dwMajorVersion == 5)
			{
				if (osvi.dwMinorVersion == 0) lstrcpy(szSystem, "Win 2000");
				if (osvi.dwMinorVersion == 1) lstrcpy(szSystem, "Win XP");
				if (osvi.dwMinorVersion == 2) lstrcpy(szSystem, "Win 2003");
			}
			else if (osvi.dwMajorVersion == 10) lstrcpy(szSystem, "Windows 10");
            else if (osvi.dwMajorVersion <= 4) lstrcpy(szSystem, "Win NT");
         break;
      case VER_PLATFORM_WIN32_WINDOWS:
         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
                lstrcpy(szSystem, "Win 95");

         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
                lstrcpy(szSystem, "Win 98");
         break;
    }
	wsprintf(info.cOS,"%s SP%d (Build %d)",szSystem,osvi.wServicePackMajor,osvi.dwBuildNumber);
	////get memory size////////////////
	MEMORYSTATUS mem;
	mem.dwLength=sizeof(mem);
	GlobalMemoryStatus(&mem);
	wsprintf(info.cMemorySize,"%dMB",mem.dwTotalPhys/1024/1024+1);
	///////server version////////////////
	return TRUE;
}
