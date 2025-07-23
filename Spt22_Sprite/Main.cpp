// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
:	m_nTab		(0)
,	m_hwWrk		(0)
,	m_hwWrk1	(0)
,	m_hwWrkTb	(0)
,	m_hwImgSc	(0)
{
	m_pTx1			= NULL;
}


HRESULT CMain::Init()
{
	D3DXIMAGE_INFO	pImgInf;

	if( FAILED( D3DXCreateTextureFromFileEx(
		m_pd3dDevice
		, "Texture/seatosky2.bmp"
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, 0x0000001
		, 0x0000001
		, 0x00FFFFFF
		, &pImgInf
		, NULL
		, &m_pTx1
		)) )
	{
		MessageBox(NULL, "seatosky2.bmp file Could not find", "Err", 0);
		m_pTx1 = NULL;
		return -1;
	}

	SendMessage( m_hWnd, WM_PAINT, 0, 0 );

	m_IL=ImageList_LoadImage(m_hInst, MAKEINTRESOURCE(IDB_WORK), 16, 1, RGB(255,255,255),IMAGE_BITMAP,0);

	m_hwWrk = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_WORK), m_hWnd, WndWrk);
	ShowWindow(m_hwWrk, SW_SHOW);



	m_hwImgSc = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_IMGSRC), m_hWnd, WndImgSrc);
	ShowWindow(m_hwImgSc, SW_SHOW);

	SetFocus(m_hWnd);

	return 0;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pTx1	);

	SAFE_DESTROY_WINDOW(m_hwWrk1);
	SAFE_DESTROY_WINDOW(m_hwWrk);

	SAFE_DESTROY_WINDOW(m_hwImgSc);

	return 0;
}


HRESULT CMain::FrameMove()
{

	return 0;
}



HRESULT CMain::Render()
{
	if( NULL == m_pd3dDevice )
		return 0;

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0 );


	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


	RECT	rt1 = {0,0,800, 600};

	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pd3dSprite->Draw(m_pTx1, &rt1, NULL, NULL, D3DXCOLOR(1,1,1,1));
	m_pd3dSprite->End();


	if(FAILED(m_pd3dDevice->EndScene()))
		return -1;


	if(FAILED(m_pd3dDevice->Present( NULL, NULL, NULL, NULL )))
		return -1;

	return 0;
}