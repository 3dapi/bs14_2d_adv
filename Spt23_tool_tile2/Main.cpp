// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	m_pLcSprite	= NULL;
	m_pD3DXFont	= NULL;
	m_pInput	= NULL;

	m_pTile		= NULL;

	m_pTxTile	= NULL;

	m_nTab		= 0;
	m_hwWrk		= NULL;
	m_hwWrk1	= NULL;
	m_hwWrkTb	= NULL;
	m_hwImgSc	= NULL;
}


HRESULT CMain::Init()
{
	// 툴의 기본 객체 생성
	m_pLcSprite = new CLcSprite;
	if(FAILED(m_pLcSprite->Create(m_pd3dSprite)))
		return -1;

	SAFE_NEWCREATE1(m_pInput, CLcInput, m_hWnd);
	SAFE_NEWCREATE2(m_pTile, CLcTile, "data/map.txt", m_pLcSprite);



	// 작업 윈도우 생성
	SendMessage( m_hWnd, WM_PAINT, 0, 0 );

	m_IL=ImageList_LoadImage(m_hInst, MAKEINTRESOURCE(IDB_WORK), 16, 1, RGB(255,255,255),IMAGE_BITMAP,0);

	m_hwWrk = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_WORK), m_hWnd, CMain::WndWrk);
	ShowWindow(m_hwWrk, SW_SHOW);

	m_hwImgSc = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_IMGSRC), m_hWnd, CMain::WndImgSrc);
	ShowWindow(m_hwImgSc, SW_SHOW);

	SetFocus(m_hWnd);


	// Tile 이미지 생성
	SAFE_NEWCREATE2(m_pTxTile, CLcTexture, m_pd3dDevice, "Texture/map_rect002.bmp");



	// Swap Chain 생성
	if(FAILED(m_SpWnd.Create((HWND)GetDlgItem(m_hwImgSc, IDC_TX), m_pd3dDevice)))
		return -1;




	D3DXFONT_DESC hFont =
	{
		14, 0
		, FW_BOLD, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE
		, "굴림체"
	};

	D3DXCreateFontIndirect(m_pd3dDevice, &hFont, &m_pD3DXFont);

	return 0;
}


HRESULT CMain::Destroy()
{
	m_SpWnd.Destroy();

	SAFE_RELEASE(	m_pD3DXFont	);
	
	SAFE_DELETE(	m_pLcSprite	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pTile		);

	SAFE_DESTROY_WINDOW(	m_hwWrk1	);
	SAFE_DESTROY_WINDOW(	m_hwWrk		);
	SAFE_DESTROY_WINDOW(	m_hwImgSc	);

	return 0;
}


HRESULT CMain::FrameMove()
{
	if(m_pInput)
		m_pInput->FrameMove();

	if(m_pTile)
		m_pTile->FrameMove();


	static	DWORD	dTimeBgn = GetTickCount();
	DWORD	dTimeEnd= GetTickCount();

	if(dTimeEnd - dTimeBgn>20)
	{
		m_SpWnd.Begin();
		
		m_pd3dDevice->Clear( 0L, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,  0x00446699, 1.0f, 0L );

		m_pLcSprite->Begin();

		RECT	rc = m_pTxTile->GetImageRect();
		m_pLcSprite->Draw(	m_pTxTile->GetTexture()
							, &rc
							, NULL, NULL, 0, NULL, D3DXCOLOR(1,1,1,1));
		m_pLcSprite->End();

		m_SpWnd.End();

		dTimeBgn = dTimeEnd;
	}


	return 0;
}



HRESULT CMain::Render()
{
	if( NULL == m_pd3dDevice )
		return 0;

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0 );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;
	
	m_pLcSprite->Begin();

	if(m_pTile)
		m_pTile->Render();

	m_pLcSprite->End();


	RECT	rt	= { 10, 10, m_dScnW-10, 50};
	char	sMsg[128];
	sprintf(sMsg, "FPS: %4.f", m_fFps);
	m_pD3DXFont->DrawText(NULL, sMsg, -1, &rt, 0, D3DXCOLOR(1,1,0,1));

	
	if(FAILED(m_pd3dDevice->EndScene()))
		return -1;


	if(FAILED(m_pd3dDevice->Present( NULL, NULL, NULL, NULL )))
		return -1;
	
	return 0;
}








LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if(m_pInput)
		m_pInput->MsgProc(hWnd, msg, wParam, lParam);
	
	switch( msg )
	{
		case WM_PAINT:
		{
			HDC hDC = GetDC( hWnd );
			RECT rct;
			GetClientRect( hWnd, &rct );
			ReleaseDC( hWnd, hDC );

			break;
		}
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}



