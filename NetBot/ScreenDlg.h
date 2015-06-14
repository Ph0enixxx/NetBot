#if !defined(AFX_SCREENDLG_H__DE33F2CD_25F8_4B5C_97BA_231CC91E770A__INCLUDED_)
#define AFX_SCREENDLG_H__DE33F2CD_25F8_4B5C_97BA_231CC91E770A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenDlg.h : header file
//

#include "./ExControl/XPictureBox.h"

#include "../Seu_lib/Command.h"
#include "XScreenXor.h"

#include "../Seu_lib/zconf.h"
#include "../Seu_lib/zlib.h"
#pragma comment(lib,"../Seu_lib/zlib.lib")//图象无损数据压缩使用zlib库函数。

#include <winsock.h>
/////////////////////////////////////////////////////////////////////////////
// CScreenDlg dialog

class CScreenDlg : public CDialog
{
// Construction
public:
	CScreenDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void SetConnSocket(SOCKET miansocket,SOCKET helpsocket);
	
// Dialog Data
	//{{AFX_DATA(CScreenDlg)
	enum { IDD = IDD_SCREEN_DLG };
	CButton	m_ctl;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_Color;
	BOOL m_bStop;
	BOOL m_bCtrl;//是否允许控制
	SOCKET m_ConnSocket;
	SOCKET m_HelpSocket;
	XPictureBox m_PicBox;
	XScreenXor  m_ScreenXor;

	HANDLE hRecvScreenThread;
	DWORD RecvScreen();
	static DWORD __stdcall RecvScreenThread(void* pThis);

	void ScreenStop();
protected:
	// Generated message map functions
	//{{AFX_MSG(CScreenDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnStart();
	afx_msg void OnCheckCtrl();
	afx_msg void OnBtnSaveBmp();
	//}}AFX_MSG
	afx_msg void OnSelectScreenBytes(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENDLG_H__DE33F2CD_25F8_4B5C_97BA_231CC91E770A__INCLUDED_)
