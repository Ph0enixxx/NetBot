#if !defined(AFX_FILETRANDLG_H__2BB379B9_8CA0_4614_AB4A_CB04C057A94E__INCLUDED_)
#define AFX_FILETRANDLG_H__2BB379B9_8CA0_4614_AB4A_CB04C057A94E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileTranDlg.h : header file
//
#include "./ExClass/AutoLock.h"
#include "../Seu_lib/Command.h"
#include <winsock.h>
/////////////////////////////////////////////////////////////////////////////
// CFileTranDlg dialog

class CFileTranDlg : public CDialog
{
// Construction
public:
	CFileTranDlg(CWnd* pParent = NULL);   // standard constructor
	void AddTranTask(SOCKET s, BOOL bDown = TRUE);

// Dialog Data
	//{{AFX_DATA(CFileTranDlg)
	enum { IDD = IDD_FILETRAN_DLG };
	CListCtrl	m_TaskList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileTranDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD __ThreadFileDown(SOCKET s);
	DWORD __ThreadFileUp(SOCKET s);
	void  AddTaskToList(SOCKET s, LPCTSTR szDst, LPCTSTR szSrc, DWORD dwSize, BOOL bDown = TRUE);
	void  UpdataTaskToList(SOCKET s, DWORD dwFileLen, DWORD dwRecvLen, DWORD dwStartTime);
	void  ErrorTaskToList(SOCKET s,LPCTSTR szError);

	CString __MakeFileSizeString(DWORD dwSize);
	CString __MakeTimeString(DWORD dwTime);

	CImageList m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CFileTranDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETRANDLG_H__2BB379B9_8CA0_4614_AB4A_CB04C057A94E__INCLUDED_)
