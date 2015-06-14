#if !defined(AFX_VIDEODLG_H__65F00F16_9E66_40D4_A006_B457CABCD53B__INCLUDED_)
#define AFX_VIDEODLG_H__65F00F16_9E66_40D4_A006_B457CABCD53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoDlg.h : header file
//
#include "../Seu_lib/Command.h"
#include <winsock.h>

#include "../Seu_lib/zconf.h"
#include "../Seu_lib/zlib.h"
#pragma comment(lib,"../Seu_lib/zlib.lib")//图象无损数据压缩使用zlib库函数。

#include "./ExControl/XPictureBox.h"
#include "./ExClass/AviFile.h"
/////////////////////////////////////////////////////////////////////////////
// CVideoDlg dialog

class CVideoDlg : public CDialog
{
// Construction
public:
	CVideoDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void SetConnSocket(SOCKET socket);
	void StatusTextOut(int iPane, LPCTSTR ptzFormat, ...);

// Dialog Data
	//{{AFX_DATA(CVideoDlg)
	enum { IDD = IDD_VIDEO_DLG };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	SOCKET m_ConnSocket;
	BOOL m_bStop;
	XPictureBox m_PicBox;
	CAviFile* m_pAviFile;

	HANDLE hRecvVideoThread;
	DWORD RecvVideo();
	static DWORD __stdcall RecvVideoThread(void* pThis);

	BOOL VideoStart();
	void VideoStop();

	HBITMAP GetBitmapFromData(LPBITMAPINFO lpBmpInfo, BYTE* pDibData);

// Implementation
protected:
	CStatusBarCtrl    m_wndStatusBar;
	// Generated message map functions
	//{{AFX_MSG(CVideoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnRecords();
	afx_msg void OnBtnRecorde();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEODLG_H__65F00F16_9E66_40D4_A006_B457CABCD53B__INCLUDED_)
