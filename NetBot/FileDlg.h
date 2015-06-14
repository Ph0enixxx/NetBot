#if !defined(AFX_FILEDLG_H__C46534F9_E674_4C77_8E98_7A74DB92C4F1__INCLUDED_)
#define AFX_FILEDLG_H__C46534F9_E674_4C77_8E98_7A74DB92C4F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileDlg.h : header file
//
#include "ExControl/TrueColorToolBar.h"
#include "../Seu_lib/Command.h"
#include "SortList.h"

#include <winsock.h>
/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog

class CFileDlg : public CDialog
{
// Construction
public:
	CFileDlg(CWnd* pParent = NULL);   // standard constructor
	~CFileDlg();
public:
	void SetConnSocket(SOCKET socket);
	void StatusTextOut(int iPane, LPCTSTR ptzFormat, ...);

	void OnWorkBegin();
	void OnWorkEnd();
	int  GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected = FALSE);
	CString ExtractNameFromFullPath(CString szFullPath);

// Dialog Data
	//{{AFX_DATA(CFileDlg)
	enum { IDD = IDD_FILE_DLG };
	CSortListCtrl	m_FileList;
	CTreeCtrl	m_FileTree;
	CString	m_CurrPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	SOCKET  m_ConnSocket;

	MsgHead m_MsgHead;           //��Ϣͷ
	char  * m_Buffer;            //���ݻ�����

	CString m_SendPath;
	UINT    m_ViewStyle;
	BOOL    bCopySel;   //������ʾ�����Ƿ�ѡ��
	CString m_CopyPath; //����ѡ�µ��ļ�ȫ·��

	CImageList imgListTree;

	HANDLE m_hWorkThread;
	DWORD ListDriver();
	DWORD ListDirectory();
	DWORD FileDelete();
	DWORD FileExecNormal();
	DWORD FileExecHide();
	DWORD FilePaste();
	DWORD FileUpLoad();
	DWORD FileDownLoad();

	void GetRootDrivers();
	void StopWork();
protected:
	CTrueColorToolBar m_wndToolBar;
	CStatusBarCtrl    m_wndStatusBar;
	CMenu            m_FileMenu;

	//CSortListCtrl m_FileList;

	// Generated message map functions
	//{{AFX_MSG(CFileDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileRunnormal();
	afx_msg void OnFileRunhide();
	afx_msg void OnFileCopy();
	afx_msg void OnFilePaste();
	afx_msg void OnFileDelete();
	afx_msg void OnFileUpload();
	afx_msg void OnFileDownload();
	afx_msg void OnFileRefursh();
	afx_msg void OnSelchangedFiletree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnAddrGo();
	afx_msg void OnColumnclickFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnFileBackup();
	afx_msg void OnFileViewChg();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDLG_H__C46534F9_E674_4C77_8E98_7A74DB92C4F1__INCLUDED_)
