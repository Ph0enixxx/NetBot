#if !defined(AFX_SERVERDLG_H__F79ACBA2_0E25_4EB1_BA23_9B12BB2A35B2__INCLUDED_)
#define AFX_SERVERDLG_H__F79ACBA2_0E25_4EB1_BA23_9B12BB2A35B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerDlg.h : header file
//
#include "./ExClass/IniFile.h"
/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);   // standard constructor
	char  ServerUrl[256];
	
	char Path[512];

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DLG };
	CListBox	m_LogList;
	CProgressCtrl	m_ServerProgress;
	CString	m_Url;
	CString	m_ServiceName;
	CString	m_ServiceDisp;
	CString	m_ServiceDesc;
	CString	m_port;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected: 
	UINT CompressType;
	CIniFile m_Ini;
	void ReadIniFile();


	
	void CompressFsg();
	void CompressUpx();
protected:
	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	//}}AFX_MSG
	afx_msg void OnCompressType(UINT nID);
	afx_msg void OnRelpaceService(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__F79ACBA2_0E25_4EB1_BA23_9B12BB2A35B2__INCLUDED_)
