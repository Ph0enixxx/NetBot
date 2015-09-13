// XScreenXor.h: interface for the XScreenXor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSCREENXOR_H__8CEE63D4_ACD7_456D_9539_465B47DC3530__INCLUDED_)
#define AFX_XSCREENXOR_H__8CEE63D4_ACD7_456D_9539_465B47DC3530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XScreenXor  
{
public:
	XScreenXor();
	virtual ~XScreenXor();

public:
	//��ȡ���õĺ���
	void  SetColor(int iColor);
	void  InitGlobalVar();

	int   GetInfoSize() const;
	int   GetBmpSize() const;
	BYTE* GetBmpData();
	BYTE* GetBmpSaveData();
	void  CaputreFrame(DWORD dwFrame);
	void  CaputreFrameFirst(DWORD dwFrame);
	void  CaputreFrameNext(DWORD dwFrame);

	//��ʾ���õĺ���
	void SetInfoSize(const int iInfoSize);
	void SetBmpSize(const int iBmpSize);
	void LoadBmpData(const BYTE* pData);
	void LoadBmpSaveData(const BYTE* pSaveData);

	void XorFrame();
	HBITMAP GetBitmapFromData();

private:
	void SaveScreenBits();

	BOOL OpenUserDesktop();
	BOOL CloseUserDesktop();

private:
	BYTE* m_pData;    //bmp��ɫ����ָ��
	BYTE* m_pDataSave;//���ǰһ֡λͼ����

	int	m_BmpSize;    //��¼ͼ�����ݵĴ�С
	int m_InfoSize;   //λͼͷ+��ɫ���С

	int m_ScrWidth;   //��Ļ���
	int m_ScrHeight;  //��Ļ�߶�

	int DestWidth;
	int DestHeigth;

	int MAXWIDTH;
	float Radio;

	int m_nColor;     //ͼ����ɫλ��

	HDESK   hdeskCurrent;
    HDESK   hdesk;
    HWINSTA hwinstaCurrent;
    HWINSTA hwinsta;

	BITMAPINFOHEADER bi;        //λͼ��Ϣͷ

	HDC              hMemDC, hScreenDC;
	HBITMAP          hBitmap;
};

#endif // !defined(AFX_XSCREENXOR_H__8CEE63D4_ACD7_456D_9539_465B47DC3530__INCLUDED_)
