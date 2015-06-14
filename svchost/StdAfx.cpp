// stdafx.cpp : source file that includes just the standard includes
//	svchost.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

BOOL OpenUserDesktop()
{
	HDESK   hdeskCurrent;
	HDESK   hdesk;
	HWINSTA hwinstaCurrent;
	HWINSTA hwinsta;
	
	hwinstaCurrent = GetProcessWindowStation();
    if (hwinstaCurrent == NULL)
		return FALSE;
	
    hdeskCurrent = GetThreadDesktop(GetCurrentThreadId());
	if (hdeskCurrent == NULL)
		return FALSE;
	
    hwinsta = OpenWindowStation(_T("winsta0"), FALSE,
		WINSTA_ACCESSCLIPBOARD   |
		WINSTA_ACCESSGLOBALATOMS |
		WINSTA_CREATEDESKTOP     |
		WINSTA_ENUMDESKTOPS      |
		WINSTA_ENUMERATE         |
		WINSTA_EXITWINDOWS       |
		WINSTA_READATTRIBUTES    |
		WINSTA_READSCREEN        |
		WINSTA_WRITEATTRIBUTES);
    if (hwinsta == NULL)
		return FALSE;
	
    if (!SetProcessWindowStation(hwinsta))
		return FALSE;
	
    hdesk = OpenDesktop(_T("default"), 0, FALSE,
		DESKTOP_CREATEMENU      |
		DESKTOP_CREATEWINDOW    |
		DESKTOP_ENUMERATE       |
		DESKTOP_HOOKCONTROL     |
		DESKTOP_JOURNALPLAYBACK |
		DESKTOP_JOURNALRECORD   |
		DESKTOP_READOBJECTS     |
		DESKTOP_SWITCHDESKTOP   |
		DESKTOP_WRITEOBJECTS);
	if (hdesk == NULL)
		return FALSE;
	
	SetThreadDesktop(hdesk);
	
	return TRUE;
}

