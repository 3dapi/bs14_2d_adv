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
}


HRESULT CMain::Init()
{
	HRESULT hr=0;

	m_pLcSprite = new CLcSprite;
	if(FAILED(m_pLcSprite->Create(m_pd3dSprite)))
		return -1;

	SAFE_NEWCREATE1(m_pInput, CLcInput, m_hWnd);
	SAFE_NEWCREATE2(m_pTile, CLcTile, "data/map.txt", m_pLcSprite);
	
	

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
	SAFE_DELETE(	m_pLcSprite	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pTile		);

	return 0;
}


HRESULT CMain::FrameMove()
{
	if(m_pInput)
		m_pInput->FrameMove();

	if(m_pTile)
		m_pTile->FrameMove();


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


	if(FAILED(m_pd3dDevice->Present( 0, 0, 0, 0)))							// 마지막으로 후면버퍼를 전면버퍼로 교체한다.( flipping)
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



