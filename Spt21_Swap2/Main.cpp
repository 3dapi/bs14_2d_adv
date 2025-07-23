// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"

#define SAFE_DESTROY_WINDOW(p)	{if(p)DestroyWindow(p);	}


CMain::CMain()
{
	m_hImgSc		= NULL;
	m_pTx1			= NULL;
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
	HWND hWndImg = (HWND)GetDlgItem(m_hImgSc, IDC_TX);


	if(FAILED(m_SpW.Create(hWndImg, m_pd3dDevice)))
		return -1;

	return 0;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pTx1	);

	SAFE_DESTROY_WINDOW(m_hImgSc);

	m_SpW.Destroy();

	return 0;
}


HRESULT CMain::FrameMove()
{
	m_SpW.Begin();
	
	m_pd3dDevice->BeginScene();
	
	m_pd3dDevice->Clear( 0L, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,  0x00446699, 1.0f, 0L );

	RECT	rt1 = {0,0,256, 256};
	
	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pd3dSprite->Draw(m_pTx1, &rt1, NULL, NULL, D3DXCOLOR(1,1,1,1));
	m_pd3dSprite->End();

	m_pd3dDevice->EndScene();

	m_SpW.End();

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