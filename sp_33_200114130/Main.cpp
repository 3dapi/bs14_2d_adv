// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	strcpy(this->m_sCls, "200114130 장청명 PushPush Game ** 판 새로 시작 R키");
	m_pInput			= NULL;

	m_GameStCur	= IGameStage::GMST_BEGIN	;	// Current State
	m_GameStOld	= IGameStage::GMST_BEGIN	;	// Old State

	for(int i=0; i< IGameStage::GMST_TOT; ++i)
		m_pGameSt[i] = 0;
}

CMain::~CMain()
{

}


HRESULT CMain::Init()
{
	D3DXFONT_DESC hFont =
	{
		14, 0
		, FW_BOLD, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE
		, "굴림체"
	};

	D3DXCreateFontIndirect(GDEVICE, &hFont, &m_pD3DXFont);
	
	
	SAFE_NEWINIT(	m_pInput, CMcInput);
	

	// 1. 최초에는 GameBegin을 생성한다.
	SAFE_NEWINIT(	m_pGameSt[m_GameStOld],	CGameBegin);

	return 1;
}



HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pD3DXFont	);
	SAFE_DELETE(	m_pInput	);

	for(int i=0; i< IGameStage::GMST_TOT; ++i)
		SAFE_DELETE(	m_pGameSt[i]	);

	
	return 1;
}



HRESULT CMain::FrameMove()
{
	if(m_pInput)
		m_pInput->FrameMove();

	SAFE_FRAMEMOVE(	m_pGameSt[m_GameStCur]	);


	if(m_GameStOld != m_GameStCur)
	{
		SAFE_DELETE(	m_pGameSt[m_GameStOld]	);
		
		m_pInput->Init();

		switch(m_GameStCur)
		{
			case IGameStage::GMST_BEGIN:
				m_pGameSt[m_GameStCur] = new CGameBegin;
				break;

			case IGameStage::GMST_PLAY:
				m_pGameSt[m_GameStCur] = new CGamePlay;
				break;

			case IGameStage::GMST_END:
				m_pGameSt[m_GameStCur] = new CGameEnd;
				break;

		}

		if(FAILED(m_pGameSt[m_GameStCur]->Init()))
			return -1;
	}

	
	return 1;
}






HRESULT CMain::Render()
{
	if( NULL == m_pd3dDevice )
		return -1;


	// Phase가 다른 경우에는 Skip한다.
	if(m_GameStOld != m_GameStCur)
	{
		m_GameStOld = m_GameStCur;
		return 1;
	}
	
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,120,160), 1.0f, 0 );
	

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	SAFE_RENDER(	m_pGameSt[m_GameStCur]	);

	m_pd3dSprite->End();
	
	
	// EndScene
	m_pd3dDevice->EndScene();
	
	return 1;
}







LRESULT CMain::MsgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch( msg )
	{
		case WM_MOUSEWHEEL:
		{
			INT c= HIWORD(wParam);
			INT d= LOWORD(wParam);

			m_pInput->AddZ( short( HIWORD(wParam) ) );

			if(d)
			{
				m_pInput->SetMouseSt(6, true);
			}
			else
			{
				m_pInput->SetMouseSt(6, false);
			}

			return 0;
		}


		case WM_LBUTTONDOWN	:
		{
			m_pInput->SetMouseSt(0, true);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			m_pInput->SetMouseSt(1, true);
			return 0;
		}
		case WM_LBUTTONDBLCLK:
		{
			m_pInput->SetMouseSt(2, true);
			return 0;
		}

		case WM_RBUTTONDOWN	:
		{
			m_pInput->SetMouseSt(3, true);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			m_pInput->SetMouseSt(4, true);
			return 0;
		}
		case WM_RBUTTONDBLCLK:
		{
			m_pInput->SetMouseSt(5, true);
			return 0;
		}

		case WM_MBUTTONDOWN	:
		{
			m_pInput->SetMouseSt(6, true);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			m_pInput->SetMouseSt(7, true);
			return 0;
		}
		case WM_MBUTTONDBLCLK:
		{
			m_pInput->SetMouseSt(8, true);
			return 0;
		}
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}