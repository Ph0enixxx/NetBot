// CmdEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CmdEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdEdit

CCmdEdit::CCmdEdit()
{
	m_clrFore=RGB(255,255,255);//�ְ�ɫ
	m_clrBack=RGB(0,0,0);//������ɫ
	m_brush.CreateSolidBrush(m_clrBack);
}

CCmdEdit::~CCmdEdit()
{
}


BEGIN_MESSAGE_MAP(CCmdEdit, CEdit)
	//{{AFX_MSG_MAP(CCmdEdit)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdEdit message handlers

HBRUSH CCmdEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	pDC->SetTextColor(m_clrFore);
	pDC->SetBkColor(m_clrBack);
	return (HBRUSH)m_brush.GetSafeHandle();
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

void CCmdEdit::ClearEdit()
{
	this->SetWindowText("");
}

void CCmdEdit::AddText(LPCTSTR szText)
{
	CString strContext;
	GetWindowText(strContext);

	strContext += szText;
	this->SetWindowText(strContext);

	LineScroll(GetLineCount());//������λ��
	SetFocus();
	SetSel(strContext.GetLength(), -1);//���λ�ã�	
}