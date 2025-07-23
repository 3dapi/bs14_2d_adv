// Implementation of the CD3DApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


static CD3DApplication*	g_pD3DApp;


CD3DApplication::CD3DApplication()
{
	g_pD3DApp	= this				;
	m_hInst		= NULL				;											// Window Instance

	strcpy(m_sCls, "McApi Window")	;											// Window Class Name

	m_hWnd		= NULL				;											// Main Window Handle
	m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;								// Window Style
	m_dScnX		= 504				;											// Screen Width
	m_dScnY		= 504				;											// Screen Height
	m_bShowCusor= TRUE				;											// Show Cusor


	m_pD3D			= NULL			;			// 1. Direct3D
	m_pd3dDevice	= NULL			;			// 2. Direct3D Device
	m_bWindow		= TRUE			;			// 3. Window Mode?
	m_pd3dSprite	= NULL			;			// 2D Sprite
}

CD3DApplication::~CD3DApplication()
{

}


INT CD3DApplication::Create( HINSTANCE hInst)
{
	m_hInst	= hInst;

	WNDCLASS wc =								// Register the window class
	{
		CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, m_hInst
		, LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1))
		, LoadCursor(NULL,IDC_ARROW)
		, (HBRUSH)GetStockObject(WHITE_BRUSH)
		, NULL
		, m_sCls
	};


	RegisterClass( &wc );


	RECT rc;									//Create the application's window

	SetRect( &rc, 0, 0, m_dScnX, m_dScnY);
	AdjustWindowRect( &rc, m_dWinStyle, false );

	int iScnSysW = ::GetSystemMetrics(SM_CXSCREEN);
	int iScnSysH = ::GetSystemMetrics(SM_CYSCREEN);

	m_hWnd = CreateWindow( m_sCls
		, m_sCls
		, m_dWinStyle
		,	(iScnSysW - (rc.right-rc.left))/2
		,	(iScnSysH - (rc.bottom-rc.top))/2
		,	(rc.right-rc.left)
		,	(rc.bottom-rc.top)
		, NULL
		, NULL
		, m_hInst
		, NULL );

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCusor);



	// D3D생성
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return -1;

	// D3D를 통해서 화면 모드를 찾는다.
	D3DDISPLAYMODE d3ddm;
    if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
        return E_FAIL;


	// 디바이스를 생성하기 위해서는 프리젠트 파라메터 구조체가 필요
	// 전부 0으로 셋팅한다음 일부만 변수세팅

	memset(&m_d3dParam, 0, sizeof(m_d3dParam));
	m_d3dParam.Windowed					= m_bWindow;
	m_d3dParam.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_d3dParam.BackBufferFormat			= d3ddm.Format;
	m_d3dParam.BackBufferWidth				= m_dScnX;
	m_d3dParam.BackBufferHeight			= m_dScnY;
	m_d3dParam.EnableAutoDepthStencil		= TRUE;
	m_d3dParam.AutoDepthStencilFormat		= D3DFMT_D16;
	m_d3dParam.FullScreen_RefreshRateInHz	= m_bWindow? 0: 60;


	// D3DADAPTER_DEFAULT: 대부분의 그래픽카드는 모노 듀얼일 경우 이부분을 수정
	// D3DDEVTYPE_HAL : 하드웨어 가속(가장 큰 속도)을 받을 것인가.. 하드웨어 지
	// 원이 없을 경우 D3D는 소프트웨어로 이를 대체 할 수 있다.

	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING, &m_d3dParam, &m_pd3dDevice ) ) )
	{
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dParam, &m_pd3dDevice ) ) )
		{
			m_pD3D->Release();
			m_pD3D = NULL;
			return -1;
		}
	}


	// DX의 스프라이트는 디바이스가 생성된 후에 만들어야 한다.
	if(FAILED(D3DXCreateSprite(m_pd3dDevice, &m_pd3dSprite)))
	{
		SAFE_RELEASE(	m_pd3dDevice	);
		SAFE_RELEASE(	m_pD3D			);
		return -1;
	}



	if(FAILED(Init()))
		return -1;


	return 0;
}

void CD3DApplication::Cleanup()
{
	Destroy();

	// 디바이스를 릴리즈하기전에 먼저 스프라이트를 해제해야 한다.
	SAFE_RELEASE(	m_pd3dSprite	);
	SAFE_RELEASE(	m_pd3dDevice	);
	SAFE_RELEASE(	m_pD3D			);

	if(m_hInst)
		UnregisterClass( m_sCls, m_hInst);
}



LRESULT WINAPI CD3DApplication::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc(hWnd, msg, wParam, lParam);
}


LRESULT CD3DApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_KEYDOWN:
		{
			switch(wParam)
			{
				case VK_ESCAPE:
				{
					SendMessage(hWnd, WM_DESTROY, 0,0);
					break;
				}
			}

			return 0;

		}

		case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage( 0 );
			return 0;
		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}




INT CD3DApplication::Run()
{
	MSG msg;
	memset( &msg, 0, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if(FAILED(FrameMove()))
				break;

			if(FAILED(Render()))
				break;

			// 후면버퍼 전면버퍼 교체( flipping)
			if(FAILED(m_pd3dDevice->Present( 0, 0, 0, 0)))
				break;

		}
	}

	Cleanup();

	UnregisterClass( m_sCls, m_hInst);

	return 1;
}




HRESULT CD3DApplication::Init()
{
	return 1;
}



HRESULT CD3DApplication::Destroy()
{
	return 1;
}


HRESULT CD3DApplication::FrameMove()
{
	return 1;
}



HRESULT CD3DApplication::Render()
{
	return 1;
}