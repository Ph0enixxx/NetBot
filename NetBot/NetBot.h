// NetBot.h : main header file for the SEU_RAT application
//

#if !defined(AFX_NETBOT_H__731C5865_C490_4EE8_86E3_B419C318409E__INCLUDED_)
#define AFX_NETBOT_H__731C5865_C490_4EE8_86E3_B419C318409E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "./ExControl/SplashScreenEx.h"
/////////////////////////////////////////////////////////////////////////////
// CNetBotApp:
// See NetBot.cpp for the implementation of this class
//

class CNetBotApp : public CWinApp
{
public:
	CNetBotApp();
	void DecryptData(unsigned char *szRec, unsigned long nLen, unsigned long key);
	CSplashScreenEx *pSplash;
	DWORD VipID;
	//////////////////////////////
	//
	void AttachImageList();
	void DetachImageList();
	CImageList m_SmallImgList;
	CImageList m_LargeImgList;
	//////////////////////////////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetBotApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNetBotApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETBOT_H__731C5865_C490_4EE8_86E3_B419C318409E__INCLUDED_)
