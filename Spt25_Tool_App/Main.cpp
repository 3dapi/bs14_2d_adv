// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	m_pD3DXFont	= NULL;
	m_pLcSprite	= NULL;
	m_pInput	= NULL;
	m_pTile		= NULL;
	m_pChar		= NULL;	
}


HRESULT CMain::Init()
{
	SAFE_NEWCREATE1(m_pLcSprite, CLcSprite, m_pd3dSprite);
	SAFE_NEWCREATE1(m_pInput, CLcInput, m_hWnd);
	SAFE_NEWCREATE2(m_pTile, CLcTile, "data/mapindex.txt", m_pLcSprite);
	SAFE_NEWCREATE2(m_pChar, CLcChar, m_pLcSprite, "Texture/OBJECT1.bmp");
	
	
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
	SAFE_RELEASE(	m_pD3DXFont	);

	SAFE_DELETE(	m_pLcSprite	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pTile		);
	SAFE_DELETE(	m_pChar		);

	return 0;
}



HRESULT CMain::Restore()
{
	m_pD3DXFont->OnResetDevice();
	
	return 0;
}


HRESULT CMain::Invalidate()
{
	m_pD3DXFont->OnLostDevice();

	return 0;
}


HRESULT CMain::FrameMove()
{
	SAFE_FRMMOV(	m_pInput	);

	if(m_pChar->m_vcP.x>650)
	{
		m_pTile->m_vcPds.x -= m_pChar->m_vcP.x-650;
		m_pChar->m_vcP.x = 650;

		if(m_pTile->m_iTotx+m_pTile->m_vcPds.x <800)
			m_pTile->m_vcPds.x = 800.f - m_pTile->m_iTotx;
	}

	if(m_pChar->m_vcP.x<50)
	{
		m_pTile->m_vcPds.x -= m_pChar->m_vcP.x-50;
		m_pChar->m_vcP.x =50;

		if(-m_pTile->m_vcPds.x<0)
			m_pTile->m_vcPds.x = 0;
	}


	if(m_pChar->m_vcP.y>480)
	{
		m_pTile->m_vcPds.y -= m_pChar->m_vcP.y-480;
		m_pChar->m_vcP.y = 480;

		if(m_pTile->m_iToty+m_pTile->m_vcPds.y <620)
			m_pTile->m_vcPds.y = 620.f - m_pTile->m_iToty;
	}

	if(m_pChar->m_vcP.y<50)
	{
		m_pTile->m_vcPds.y -= m_pChar->m_vcP.y-50;
		m_pChar->m_vcP.y =50;

		if(-m_pTile->m_vcPds.y<0)
			m_pTile->m_vcPds.y = 0;
	}

	SAFE_FRMMOV(	m_pTile		);
	SAFE_FRMMOV(	m_pChar		);

	return 0;
}



HRESULT CMain::Render()
{
	if( NULL == GDEVICE )
		return 0;

	GDEVICE->SetRenderState( D3DRS_FOGENABLE, FALSE );							// 안개
	GDEVICE->SetRenderState( D3DRS_LIGHTING, FALSE );							// 라이팅을 끈다.( 옵션)

	GDEVICE->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0 );

	if( FAILED( GDEVICE->BeginScene() ) )
		return -1;
	
	SAFE_RENDER(	m_pTile	);


	SAFE_RENDER(	m_pChar	);



	RECT	rt	= { 710, 10, m_dScnW-10, 50};
	char	sMsg[128];

	sprintf(sMsg, "FPS: %4.f", m_fFps);

	m_pD3DXFont->DrawText(NULL, sMsg, -1, &rt, 0, D3DXCOLOR(1,1,0,1));


	GDEVICE->EndScene();
	
	return 0;
}



LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static	WPARAM		wparHi;
	static	WPARAM		wparLo;
	static	HWND		hw;
	
	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);
	hw = (HWND)lParam;

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


