// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D2C70E6F_67DD_4EF5_BCF4_52F44599580B__INCLUDED_)
#define AFX_STDAFX_H__D2C70E6F_67DD_4EF5_BCF4_52F44599580B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define vipid 405

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define WINVER 0X502
#define _WIN32_WINNT 0x0502

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#ifndef _AFX_NO_AFXCMN_SUPPORT
	#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define DE
#include "..\\..\\debugh.h"

#include "SkinH.h"
#pragma comment(lib, "SkinH.lib")

#include "./ExClass/ThreadTemplate.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4200)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D2C70E6F_67DD_4EF5_BCF4_52F44599580B__INCLUDED_)
