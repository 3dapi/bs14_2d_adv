// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
:	m_nTab		(0)
,	m_hwWrk		(0)
,	htab_alter	(0)
,	hlst_scene	(0)
,	hlst_mscene	(0)
,	m_hwImgSc	(0)
{
}


HRESULT CMain::Init()
{
	SendMessage( m_hWnd, WM_PAINT, 0, 0 );

	m_IL=ImageList_LoadImage( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_WORK), 16, 1, RGB(255,255,255),IMAGE_BITMAP,0);

	m_hwWrk = CreateDialog( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_WORK), m_hWnd, WndWrk);
	ShowWindow(m_hwWrk, SW_SHOW);

	m_hwImgSc = CreateDialog( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_IMGSRC), m_hWnd, WndImgSrc);
	ShowWindow(m_hwImgSc, SW_SHOW);

	SetFocus(m_hWnd);

	return 0;
}


HRESULT CMain::Destroy()
{
	SAFE_DESTROY_WINDOW(hlst_scene);
	SAFE_DESTROY_WINDOW(m_hwWrk);

	SAFE_DESTROY_WINDOW(m_hwImgSc);

	return 0;
}