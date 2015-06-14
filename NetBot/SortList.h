#include "StdAfx.h"
#include "afxcmn.h"

#include <string.h>

class CSortListCtrl : public CListCtrl
{
public:
	CSortListCtrl(void);
public:
	~CSortListCtrl(void);

public:
	enum DATA_TYPE{INT_TYPE = 0, STRING_TYPE, DOUBLE_TYPE };
public:
	//�Ƿ�Ϊ����
	bool m_bAsc;
	//��ǰ���е���
	int m_nSortedCol;

public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
