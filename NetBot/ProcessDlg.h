#if !defined(AFX_PROCESSDLG_H__8F3DFF89_2CC2_483C_B704_5A344BB59DFE__INCLUDED_)
#define AFX_PROCESSDLG_H__8F3DFF89_2CC2_483C_B704_5A344BB59DFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessDlg.h : header file
//
#include "../Seu_lib/Command.h"

#include <winsock.h>
/////////////////////////////////////////////////////////////////////////////
// CProcessDlg dialog

class CProcessDlg : public CDialog
{
// Construction
public:
	CProcessDlg(CWnd* pParent = NULL);   // standard constructor
	void SetConnSocket(SOCKET socket);
	void StatusTextOut(int iPane,LPCTSTR ptzFormat, ...);

	void OnWorkBegin();
	void OnWorkEnd();
// Dialog Data
	//{{AFX_DATA(CProcessDlg)
	enum { IDD = IDD_PROCESS_DLG };
	CListCtrl	m_ProcessList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	SOCKET  m_ConnSocket;
	MsgHead m_MsgHead;           //消息头
	char    m_Buffer[128 * 1024]; //数据缓冲区

	HANDLE m_hWorkThread;
	DWORD ListProcess();
	DWORD KillProcess();

	void StopWork();
	CString __MakePriority(DWORD dwPriClass);
protected:
	CStatusBarCtrl    m_wndStatusBar;
	// Generated message map functions
	//{{AFX_MSG(CProcessDlg)
	afx_msg void OnGetprocessBtn();
	afx_msg void OnKillprocessBtn();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnClickProcessList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSDLG_H__8F3DFF89_2CC2_483C_B704_5A344BB59DFE__INCLUDED_)
