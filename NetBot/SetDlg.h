#if !defined(AFX_SETDLG_H__22924D22_6FCC_455A_A386_692050E3046F__INCLUDED_)
#define AFX_SETDLG_H__22924D22_6FCC_455A_A386_692050E3046F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDlg.h : header file
//
#include "./ExClass/IniFile.h"

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog
class CSetDlg : public CDialog
{
// Construction
public:
	CSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDlg)
	enum { IDD = IDD_SET_DLG };
	int		m_ListenPort;
	int		m_ConnectMax;
	BOOL	m_Skin;
	BOOL	m_China;
	BOOL	m_WIN7;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CIniFile m_Ini;
	void ReadIniFile();
	
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetDlg)
	afx_msg void OnBtnSetsave1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnSetsave4();
	afx_msg void OnCheckSkin();
	afx_msg void OnCheckChina();
	afx_msg void OnCheckWin7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDLG_H__22924D22_6FCC_455A_A386_692050E3046F__INCLUDED_)
