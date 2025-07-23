// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


#define SAFE_DESTROY_WINDOW(p)	{if(p)DestroyWindow(p);	}


CMain::CMain()
{
	m_hImgSc		= NULL;
	m_pTx1			= NULL;

	m_hSwW			= NULL;
	m_pSwS			= NULL;
	m_pSwC			= NULL;
	m_pSwD			= NULL;	
}


HRESULT CMain::Init()
{
	if( FAILED( D3DXCreateTextureFromFileEx(
		m_pd3dDevice
		, "Texture/Image.png"
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, 0x0000001
		, 0x0000001
		, 0x00FFFFFF
		, NULL
		, NULL
		, &m_pTx1
		)) )
	{
		return -1;
	}

	
	m_hImgSc = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_IMGSRC), m_hWnd, CMain::WndImgSrc);
	ShowWindow(m_hImgSc, SW_SHOW);
	SetFocus(m_hWnd);
	


	//Swap chain만들기
	// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼의 속성을 가져온다.
	D3DSURFACE_DESC		dscC;
	D3DSURFACE_DESC		dscD;
	LPDIRECT3DSURFACE9	pDevC=NULL;
	LPDIRECT3DSURFACE9	pDevD=NULL;

	m_pd3dDevice->GetRenderTarget(0, &pDevC);
	pDevC->GetDesc(&dscC);
	pDevC->Release();

	m_pd3dDevice->GetDepthStencilSurface(&pDevD);
	pDevD->GetDesc(&dscD);
	pDevD->Release();



	// 대상 윈도우 핸들을 얻는다.
	m_hSwW = (HWND)GetDlgItem(m_hImgSc, IDC_TX);

	// Present Parameter 속성을 설정한다.
	// BackBuffer Width, Height가 0이면 뷰포트가 자동으로
	// 윈도우 영역만큼 설정 된다.
	
	D3DPRESENT_PARAMETERS	d3dParam;
	memset(&d3dParam, 0, sizeof(D3DPRESENT_PARAMETERS));
	
	d3dParam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dParam.Windowed				= TRUE;
	d3dParam.BackBufferWidth		= 0;
	d3dParam.BackBufferHeight		= 0;
	d3dParam.BackBufferFormat		= dscC.Format;
	d3dParam.AutoDepthStencilFormat	= dscD.Format;
	d3dParam.hDeviceWindow			= m_hSwW;

	// Swap Chain을 생성 한다.
	if( FAILED(m_pd3dDevice->CreateAdditionalSwapChain(&d3dParam, &m_pSwS)))
		return -1;

	// Swap Chain으로 생성된 후면 버퍼의 색상 버퍼를 가져온다.
	if( FAILED(m_pSwS->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pSwC)))
		return -1;

	// Swap Chain을 위한 깊이, 스텐실 버퍼를 생성한다.
	if ( FAILED(m_pd3dDevice->CreateDepthStencilSurface(
								d3dParam.BackBufferWidth
								, d3dParam.BackBufferHeight
								, d3dParam.AutoDepthStencilFormat
								, D3DMULTISAMPLE_NONE, 0, 0
								, &m_pSwD
								, NULL)))
		return -1;


	return 0;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pTx1	);

	SAFE_RELEASE(	m_pSwS	);
	SAFE_RELEASE(	m_pSwC	);
	SAFE_RELEASE(	m_pSwD	);

	SAFE_DESTROY_WINDOW(m_hImgSc);

	return 0;
}


HRESULT CMain::FrameMove()
{
	// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼를 가져온다.
	LPDIRECT3DSURFACE9	pDevC=NULL;
	LPDIRECT3DSURFACE9	pDevD=NULL;

	m_pd3dDevice->GetRenderTarget(0, &pDevC);
	m_pd3dDevice->GetDepthStencilSurface(&pDevD);


	// Swap Chain의 색상버퍼와 깊이 버퍼로 변경한다.
	m_pd3dDevice->SetRenderTarget(0, m_pSwC);
	m_pd3dDevice->SetDepthStencilSurface(m_pSwD);
	

	// 장면을 연출한다.
	m_pd3dDevice->BeginScene();

	m_pd3dDevice->Clear( 0L, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,  0x00446699, 1.0f, 0L );

	RECT	rt1 = {0,0,256, 256};
	
	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pd3dSprite->Draw(m_pTx1, &rt1, NULL, NULL, D3DXCOLOR(1,1,1,1));
	m_pd3dSprite->End();

	m_pd3dDevice->EndScene();

	// Swap Chain에 저장된 데이터를 화면에 출력한다.
	m_pSwS->Present(0, 0, 0, 0, 0);


	// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼로 변경한다.
	m_pd3dDevice->SetRenderTarget(0, pDevC);
	m_pd3dDevice->SetDepthStencilSurface(pDevD);

	pDevC->Release();
	pDevD->Release();

	
	return 0;
}



HRESULT CMain::Render()
{
	if( NULL == m_pd3dDevice )
		return 0;

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00006699, 1.0f, 0 );
	
	
	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;
	
	
	RECT	rt1 = {0,0,256, 256};
	
	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pd3dSprite->Draw(m_pTx1, &rt1, NULL, NULL, D3DXCOLOR(1,1,1,1));
	m_pd3dSprite->End();
	

	
	if(FAILED(m_pd3dDevice->EndScene()))
		return -1;

	if(FAILED(m_pd3dDevice->Present( NULL, NULL, NULL, NULL )))
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




INT_PTR CALLBACK CMain::WndImgSrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM	wparHi;
	WPARAM	wparLo;
	
	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);
	
	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			RECT rt1;
			RECT rt3;

			INT iWidth;
			INT iHeight;
			INT iX;
			INT iY;

			GetWindowRect(GMAIN->GetHwnd(), &rt1);
			GetWindowRect(hWnd, &rt3);

			iWidth = rt3.right - rt3.left;
			iHeight=  rt3.bottom- rt3.top;
			iX = rt1.left - iWidth;
			iY = rt1.top;
			
			MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);

			return TRUE;
		}
	}	
	
	
	return(FALSE);
}