// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	m_pLcSprite	= NULL;
	m_pInput	= NULL;

	m_pD3DXFont	= NULL;
	m_pFntIdx	= NULL;

	m_pTile		= NULL;
	m_pTx		= NULL;
	m_prcSc		= NULL;

	m_nTab		= 0;
	m_hWrk		= NULL;
	m_hWrk1		= NULL;
	m_hWrkTb	= NULL;
	m_hImgSc	= NULL;

	m_bIdx		= 0;
	m_vcScI		= D3DXVECTOR2(0,0);
	m_vcScM		= D3DXVECTOR2(0,0);
	m_iImgY		= 0;
	m_iImgX		= 0;
	m_nImgSc	= 0;
	m_nImgDs	= -1;

	SetRect(&m_rcMkSc, 0, 0, 32, 32);
}


HRESULT CMain::Init()
{
	// 툴의 기본 객체 생성
	SAFE_NEWCREATE1(m_pLcSprite, CLcSprite, m_pd3dSprite);
	SAFE_NEWCREATE1(m_pInput, CLcInput, m_hWnd);

	SAFE_NEWCREATE2(m_pTile, CLcTile, "data/mapindex.txt", m_pLcSprite);
	
	SAFE_NEWCREATE2(m_pTx, CLcTexture, m_pd3dDevice, "texture/tilemap.bmp");

	
	int x,y;
	int i=0;

	m_iW	= 32;
	m_iImgY = m_pTx->GetImageHeight()/ m_iW;
	m_iImgX = m_pTx->GetImageWidth()/ m_iW;
	
	m_prcSc = new RECT[ m_iImgY * m_iImgX];
		
	for(y=0; y<m_iImgY; ++y)
	{
		for(x=0; x<m_iImgX; ++x)
		{
			SetRect(&m_prcSc[i], x*m_iW, y*m_iW, x*m_iW+m_iW, y*m_iW+m_iW);
			++i;
		}
	}

	m_rcDs = m_prcSc[m_iImgY * m_iImgX-1];

	


	m_IL=ImageList_LoadImage(m_hInst, MAKEINTRESOURCE(IDB_WORK), 16, 1, RGB(255,255,255),IMAGE_BITMAP,0);
	m_hWrk = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_WORK), m_hWnd, CMain::WndWrk);
	ShowWindow(m_hWrk, SW_SHOW);
	
	m_hImgSc = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_IMGSRC), m_hWnd, CMain::WndImgSc);
	ShowWindow(m_hImgSc, SW_SHOW);
	SetFocus(m_hWnd);



	// Swap Chain 생성
	if(FAILED(m_SpWnd.Create((HWND)GetDlgItem(m_hImgSc, IDC_TX), m_pd3dDevice)))
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

	D3DXFONT_DESC hFont2 =
	{
		12, 0
		, FW_NORMAL, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE
		, "Arial"
	};

	D3DXCreateFontIndirect(m_pd3dDevice, &hFont2, &m_pFntIdx);

	return 0;
}


HRESULT CMain::Destroy()
{
	m_SpWnd.Destroy();
	
	SAFE_RELEASE(	m_pD3DXFont	);
	SAFE_RELEASE(	m_pFntIdx	);

	SAFE_DELETE(	m_pLcSprite	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pTile		);
	
	SAFE_DELETE_ARRAY(	m_prcSc	);

	SAFE_DESTROY_WINDOW(	m_hWrk1		);
	SAFE_DESTROY_WINDOW(	m_hWrk		);
	SAFE_DESTROY_WINDOW(	m_hImgSc	);

	return 0;
}


HRESULT CMain::FrameMove()
{
	if(m_pInput)
		m_pInput->FrameMove();

	if(m_pTile)
		m_pTile->FrameMove();

	UpdateSc();
	
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

	m_pLcSprite->Begin();

	m_pLcSprite->Draw(m_pTx->GetTexture()
						, &m_rcDs
						, NULL
						, NULL
						, 0
						, &(m_pTile->m_vcPds+m_pTile->m_vcDsM)
						, D3DXCOLOR(1,1,1,0.3f));

	m_pLcSprite->End();


	RECT	rt	= { 710, 10, m_dScnW-10, 50};
	char	sMsg[128];
	sprintf(sMsg, "FPS: %4.f", m_fFps);

	m_pD3DXFont->DrawText(NULL, sMsg, -1, &rt, 0, D3DXCOLOR(1,1,0,1));



	RECT		rc;
	D3DXVECTOR2	vcP;
	int			x,y;

	if(m_bIdx)
	{
		for(y=0; y<m_pTile->m_nTileNumY; ++y)
		{
			for(x=0; x<m_pTile->m_nTileNumX; ++x)
			{
				INT		idx = m_pTile->m_pTile[y][x].nIdx;

				vcP.x = FLOAT(x* m_iW);
				vcP.y = FLOAT(y* m_iW);
				
				vcP += m_pTile->m_vcPds;
				
				char	sMsg[128];

				rc.left		= LONG(vcP.x);
				rc.right	= LONG(vcP.x + m_iW);
				rc.top		= LONG(vcP.y);
				rc.bottom	= LONG(vcP.y + m_iW);

				sprintf(sMsg, "%d", idx);
				m_pFntIdx->DrawText(NULL, sMsg, -1, &rc, 0, D3DXCOLOR(1,1,0,1));
			}
		}
	}
	
	if(FAILED(m_pd3dDevice->EndScene()))
		return -1;

	if(FAILED(m_pd3dDevice->Present( 0, 0, 0, 0)))
		return -1;
	
	return 0;
}



INT CMain::UpdateSc()
{
	HWND hWnd = GetActiveWindow();

	if(m_hImgSc == hWnd)
	{
		if(m_pInput->KeyState(VK_RIGHT))
			m_vcScI.x += 32;

		if(m_pInput->KeyState(VK_LEFT))
			m_vcScI.x -= 32;

		if(m_pInput->KeyState(VK_DOWN))
			m_vcScI.y -= 32;

		if(m_pInput->KeyState(VK_UP))
			m_vcScI.y += 32;


		if(m_pInput->BtnPress(0))
		{
			D3DXVECTOR3 vcP = m_pInput->GetMousePos();
			RECT	rc1;
			RECT	rc2;
			int		iX;
			int		iY;
			FLOAT	x;
			FLOAT	y;

			GetWindowRect(m_hWnd, &rc1);
			GetWindowRect( (HWND)GetDlgItem(m_hImgSc, IDC_TX), &rc2);

			x	= vcP.x - (rc2.left - rc1.left)+3-m_vcScI.x ;
			y	= vcP.y - (rc2.top - rc1.top) +41-m_vcScI.y;
			iX	= int(x/32.f);
			iY	= int(y/32.f);

			if(iX>=0 && iX<m_iImgX && iY>=0 && iY<m_iImgY)
			{
				m_vcScM.x = iX * 32.f;
				m_vcScM.y = iY * 32.f;

				m_nImgSc	= iX + m_iImgX * iY;
				SetDlgItemInt(m_hImgSc, IDC_IMGIDX, m_nImgSc, 0);
			}
		}
		
	}



	m_SpWnd.Begin();
	
	m_pd3dDevice->BeginScene();
	m_pd3dDevice->Clear( 0L, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,  0x00446699, 1.0f, 0L );

	RECT	rt	= m_pTx->GetImageRect();

	m_pLcSprite->Begin();

	m_pLcSprite->Draw(m_pTx->GetTexture()
						, &rt
						, NULL
						, NULL
						, 0
						, &m_vcScI
						, D3DXCOLOR(1,1,1,1));

	m_pLcSprite->Draw(m_pTx->GetTexture()
						, &m_rcDs
						, NULL
						, NULL
						, 0
						, &(m_vcScM+m_vcScI)
						, D3DXCOLOR(1,1,1,0.3f));

	m_pLcSprite->End();

	m_pd3dDevice->EndScene();

	m_SpWnd.End();

	return 0;
}