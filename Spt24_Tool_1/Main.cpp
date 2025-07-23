// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	m_pLcSprite	= NULL;
	m_pInput	= NULL;

	m_pD3DXFont	= NULL;

	m_pTile		= NULL;

	m_nTab		= 0;
	m_hwWrk		= NULL;
	m_hwWrk1	= NULL;
	m_hwWrkTb	= NULL;
	m_hwImgSc	= NULL;

	m_bIdx		= TRUE;
	m_vcPosI	= D3DXVECTOR2(0,0);
	m_vcPosM	= D3DXVECTOR2(0,0);
}


HRESULT CMain::Init()
{
	// 툴의 기본 객체 생성
	m_pLcSprite = new CLcSprite;
	if(FAILED(m_pLcSprite->Create(m_pd3dSprite)))
		return -1;

	SAFE_NEWCREATE1(m_pInput, CLcInput, m_hWnd);
	SAFE_NEWCREATE2(m_pTile, CLcTile, "data/mapindex.txt", m_pLcSprite);

	
	m_IL=ImageList_LoadImage(m_hInst, MAKEINTRESOURCE(IDB_WORK), 16, 1, RGB(255,255,255),IMAGE_BITMAP,0);

	m_hwWrk = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_WORK), m_hWnd, WndWrk);
	ShowWindow(m_hwWrk, SW_SHOW);

	
	
	m_hwImgSc = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_IMGSRC), m_hWnd, WndImgSrc);
	ShowWindow(m_hwImgSc, SW_SHOW);
	SetFocus(m_hWnd);


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


	HWND hWnd = GetActiveWindow();

	if(m_hwImgSc == hWnd)
	{
		if(m_pInput->BtnPress(0))
		{
			D3DXVECTOR3 vcD = 2*m_pInput->GetMouseEps();
			m_vcPosI.x += 32* int(vcD.x/32);
			m_vcPosI.y += 32* int(vcD.y/32);
		}


		if(m_pInput->BtnPress(1))
		{
			m_vcPosM.x += 32;
			m_vcPosM.y += 32;
		}
		
	}



	m_SpWnd.Begin();
	
	m_pd3dDevice->BeginScene();
	m_pd3dDevice->Clear( 0L, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,  0x00446699, 1.0f, 0L );

	RECT	rt	= { 0, 0, m_pTile->m_TxImg.Width, m_pTile->m_TxImg.Height};

	m_pLcSprite->Begin();
	m_pLcSprite->Draw(m_pTile->m_pTxMap, &rt, NULL, NULL, 0, &m_vcPosI, D3DXCOLOR(1,1,1,1));
//	m_pLcSprite->Draw(m_pTile->m_pTx, &m_pTile->m_rcMark, NULL, NULL, 0, &m_vcPosM, D3DXCOLOR(1,1,1,0.6f));
	m_pLcSprite->End();

	m_pd3dDevice->EndScene();

	m_SpWnd.End();

	return 0;
}



HRESULT CMain::Render()
{
	if( NULL == m_pd3dDevice )
		return 0;

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0 );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;
	
	if(m_pTile)
		m_pTile->Render();


	RECT	rt	= { 10, 10, m_dScnW-10, 50};
	TCHAR	sMsg[128];
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
		
		
		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case ID_VIEW_INDEX:
				{
					m_bIdx ^= 1;

					if(m_bIdx)
						CheckMenuItem(GetMenu(hWnd), ID_VIEW_INDEX, MF_CHECKED);
					else
						CheckMenuItem(GetMenu(hWnd), ID_VIEW_INDEX, MF_UNCHECKED);

					break;
				}
			}
			
			
			break;
		}
		
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}



INT_PTR CALLBACK WndAbout(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM		wparHi;
	WPARAM		wparLo;

	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);	

	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			return TRUE;
		}
		
		
		case WM_COMMAND:
		{
			switch(wparLo)
			{
			case IDOK:
				{
					SendMessage(hWnd, WM_CLOSE, 0,0);
					break;
				}
			}
			
			break;
		}
		
		
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			break;
		}
		
		
	}
	
	return(FALSE);
}