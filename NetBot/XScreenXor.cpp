// XScreenXor.cpp: implementation of the XScreenXor class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "XScreenXor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XScreenXor::XScreenXor()
{
	m_pData = NULL;
	m_pDataSave = NULL;
	m_BmpSize = 0;
	m_InfoSize = 0;
	m_ScrWidth = 0;
	m_ScrHeigth = 0;
	m_nColor = 8;//Ĭ��256ɫ

	OpenUserDesktop();
}

XScreenXor::~XScreenXor()
{
	if (m_pData != NULL)
		delete[] m_pData;

	if (m_pDataSave != NULL)
		delete[] m_pDataSave;

	CloseUserDesktop();
}

void XScreenXor::SetColor(int iColor)
{
	if (iColor < 8)
		m_nColor = 4;
	else if (iColor < 16)
		m_nColor = 8;
	else if (iColor < 24)
		m_nColor = 16;
	else
		m_nColor = 24;
}

void XScreenXor::InitGlobalVar()
{
	//�����Ļ��С
	m_ScrWidth = GetSystemMetrics(SM_CXSCREEN);//λͼ���
	m_ScrHeigth = GetSystemMetrics(SM_CYSCREEN);//λͼ�߶�

	//����λͼͷ��С��λͼ��С
	int biSize = sizeof(BITMAPINFOHEADER);
	if (m_nColor > 8)
		m_InfoSize = biSize;
	else
		m_InfoSize = biSize + (1 << m_nColor) * sizeof(RGBQUAD);

	m_BmpSize = m_InfoSize + ((m_ScrWidth * m_nColor + 31) / 32 * 4) * m_ScrHeigth;

	//����λͼ�洢�ռ�
	if (m_pData != NULL)
		delete[] m_pData;
	m_pData = new BYTE[m_BmpSize];

	if (m_pDataSave != NULL)
		delete[] m_pDataSave;
	m_pDataSave = new BYTE[m_BmpSize];

}

int XScreenXor::GetInfoSize() const
{
	return m_InfoSize;
}

int XScreenXor::GetBmpSize() const
{
	return m_BmpSize;
}

BYTE* XScreenXor::GetBmpData()
{
	return m_pData;
}

BYTE* XScreenXor::GetBmpSaveData()
{
	return m_pDataSave;
}

void XScreenXor::SetInfoSize(const int iInfoSize)
{
	m_InfoSize = iInfoSize;
}

void XScreenXor::SetBmpSize(const int iBmpSize)
{
	m_BmpSize = iBmpSize;

	if (m_pData != NULL)
		delete[] m_pData;
	m_pData = new BYTE[m_BmpSize];

	if (m_pDataSave != NULL)
		delete[] m_pDataSave;
	m_pDataSave = new BYTE[m_BmpSize];
}

void XScreenXor::LoadBmpData(const BYTE* pData)
{
	int iCount = m_BmpSize / sizeof(DWORD);
	for (int i = 0; i < iCount; i++)
	{
		((PDWORD)m_pData)[i] = ((PDWORD)pData)[i];
	}
}

void XScreenXor::LoadBmpSaveData(const BYTE* pSaveData)
{
	int iCount = m_BmpSize / sizeof(DWORD);
	for (int i = 0; i < iCount; i++)
	{
		((PDWORD)m_pDataSave)[i] = ((PDWORD)pSaveData)[i];
	}
}

void  XScreenXor::CaputreFrame(DWORD dwFrame)
{
	//��ȡ��Ļ
	SaveScreenBits();

	int iCount = m_BmpSize / sizeof(DWORD);
	if (dwFrame == 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			((PDWORD)m_pDataSave)[i] = ((PDWORD)m_pData)[i];
		}
	}

	if (dwFrame > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			((PDWORD)m_pDataSave)[i] ^= ((PDWORD)m_pData)[i];
			PBYTE tmp = m_pData;
			m_pData = m_pDataSave;
			m_pDataSave = tmp;
		}
	}
}

//you should call function 'InitGlobalVar' first
void XScreenXor::SaveScreenBits()
{
	HDC              hMemDC, hScreenDC;
	HBITMAP          hBitmap;
	PBITMAPINFO      lpBmpInfo; //λͼ��Ϣ
	BITMAPINFOHEADER bi;        //λͼ��Ϣͷ

	//��ȡ����HDC
	hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScreenDC);
	//����һ������Ļ�豸��������ݵ�λͼ
	hBitmap = CreateCompatibleBitmap(hScreenDC, m_ScrWidth, m_ScrHeigth);
	// ����λͼѡ���ڴ��豸��������
	SelectObject(hMemDC, hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	::BitBlt(hMemDC, 0, 0, m_ScrWidth, m_ScrHeigth, hScreenDC, 0, 0, SRCCOPY);

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = m_ScrWidth;
	bi.biHeight = m_ScrHeigth;
	bi.biPlanes = 1;
	bi.biBitCount = m_nColor;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	lpBmpInfo = PBITMAPINFO(m_pData);
	//�����ݿ���ȥ
	memcpy(m_pData, &bi, sizeof(BITMAPINFOHEADER));

	::GetDIBits(
		hMemDC,
		hBitmap,
		0,
		m_ScrHeigth,
		m_pData + m_InfoSize,
		lpBmpInfo,
		DIB_RGB_COLORS);

	//���
	DeleteDC(hMemDC);
	DeleteDC(hScreenDC);
	DeleteObject(hBitmap);
}

void XScreenXor::XorFrame()
{
	//���²��쵽m_pData
	int iCount = m_BmpSize / sizeof(DWORD);
	for (int i = 0; i < iCount; i++)
	{
		((PDWORD)m_pData)[i] ^= ((PDWORD)m_pDataSave)[i];
	}
}

HBITMAP XScreenXor::GetBitmapFromData()
{
	HBITMAP hBitmap;
	PBITMAPINFO lpBmpInfo; //λͼ��Ϣ

	lpBmpInfo = PBITMAPINFO(m_pData);

	HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	// ����DDBλͼ
	hBitmap = CreateDIBitmap(
		hDC,
		&lpBmpInfo->bmiHeader,
		CBM_INIT,
		m_pData + m_InfoSize,
		lpBmpInfo,
		DIB_RGB_COLORS);

	DeleteDC(hDC);

	return hBitmap;
}

////////////////////////////////////////////////////////////////////////////
//��Ļ�������ְ���,����������ԭ��:
//һ��ϵͳ����������δ��½����.
//���Ǵ�����Ļ��������.
//��ʱ��Ҫ����ǰ�����л������������ץ��.
BOOL XScreenXor::OpenUserDesktop()
{
	hwinstaCurrent = GetProcessWindowStation();
	if (hwinstaCurrent == NULL)
		return FALSE;

	hdeskCurrent = GetThreadDesktop(GetCurrentThreadId());
	if (hdeskCurrent == NULL)
		return FALSE;

	hwinsta = OpenWindowStation(_T("winsta0"), FALSE,
		WINSTA_ACCESSCLIPBOARD |
		WINSTA_ACCESSGLOBALATOMS |
		WINSTA_CREATEDESKTOP |
		WINSTA_ENUMDESKTOPS |
		WINSTA_ENUMERATE |
		WINSTA_EXITWINDOWS |
		WINSTA_READATTRIBUTES |
		WINSTA_READSCREEN |
		WINSTA_WRITEATTRIBUTES);
	if (hwinsta == NULL)
		return FALSE;

	if (!SetProcessWindowStation(hwinsta))
		return FALSE;

	hdesk = OpenDesktop(_T("default"), 0, FALSE,
		DESKTOP_CREATEMENU |
		DESKTOP_CREATEWINDOW |
		DESKTOP_ENUMERATE |
		DESKTOP_HOOKCONTROL |
		DESKTOP_JOURNALPLAYBACK |
		DESKTOP_JOURNALRECORD |
		DESKTOP_READOBJECTS |
		DESKTOP_SWITCHDESKTOP |
		DESKTOP_WRITEOBJECTS);
	if (hdesk == NULL)
		return FALSE;

	SetThreadDesktop(hdesk);

	return TRUE;
}

BOOL XScreenXor::CloseUserDesktop()
{
	if (!SetProcessWindowStation(hwinstaCurrent))
		return FALSE;

	if (!SetThreadDesktop(hdeskCurrent))
		return FALSE;

	if (!CloseWindowStation(hwinsta))
		return FALSE;

	if (!CloseDesktop(hdesk))
		return FALSE;

	return TRUE;
}