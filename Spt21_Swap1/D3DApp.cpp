//
// File: D3DApp.cpp
// Desc:
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


static CD3DApplication* g_pD3DApp = NULL;

CD3DApplication::CD3DApplication()
{
	g_pD3DApp           = this;
	
	m_hWnd              = NULL;
	m_hMenu             = NULL;

	m_pD3D				= NULL;
	m_pd3dDevice		= NULL;
	m_pd3dSprite		= NULL;

	strcpy(m_sCls, "SpTool" );
	m_dScnW		= 1024;
	m_dScnH		= 768;
	
	m_dWinStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE;
}


CD3DApplication::~CD3DApplication()
{
}





LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );
}





HRESULT CD3DApplication::Create( HINSTANCE hInstance )
{
	m_hInst	= hInstance;
	
	if( m_hWnd == NULL)
	{
		WNDCLASS wndClass =
		{ 
			0,
				(WNDPROC)WndProc,
				0,
				0,
				hInstance,
				LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
				LoadCursor( NULL, IDC_ARROW ),
				(HBRUSH)GetStockObject(WHITE_BRUSH),
				NULL,
				m_sCls
		};
		
		RegisterClass( &wndClass );
		
		// Set the window's initial style
		m_dWinStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
			WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;
		
		// Set the window's initial width
		RECT rc;
		SetRect( &rc, 0, 0, m_dScnW, m_dScnH );
		AdjustWindowRect( &rc, m_dWinStyle, FALSE);
		
		
		// Create the render window
		m_hWnd = CreateWindow(	m_sCls
			,	m_sCls
			,	m_dWinStyle
			,	400//(iScnSysW - (rc.right-rc.left))/2
			,	10//(iScnSysH - (rc.bottom-rc.top))/2
			,	(rc.right-rc.left)
			,	(rc.bottom-rc.top)
			,	0
			,	NULL
			,	hInstance
			,	0 );
	}
	
	m_dWinStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	GetWindowRect( m_hWnd, &m_rcWinBounds );
	GetClientRect( m_hWnd, &m_rcWinClient );



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
	m_d3dParam.Windowed				= TRUE;
	m_d3dParam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_d3dParam.BackBufferFormat		= d3ddm.Format;
	m_d3dParam.BackBufferCount			= 2;
	m_d3dParam.EnableAutoDepthStencil	= TRUE;
	m_d3dParam.AutoDepthStencilFormat	= D3DFMT_D16;
	m_d3dParam.Flags					= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;		// Back Buffer를 이용하려면..
	
	
	// D3DADAPTER_DEFAULT: 대부분의 그래픽카드는 모노 듀얼일 경우 이부분을 수정
	// D3DDEVTYPE_HAL : 하드웨어 가속(가장 큰 속도)을 받을 것인가.. 하드웨어 지
	// 원이 없을 경우 D3D는 소프트웨어로 이를 대체 할 수 있다.
	
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING, &m_d3dParam, &m_pd3dDevice ) ) )
	{
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dParam, &m_pd3dDevice ) ) )
		{
			SAFE_RELEASE(	m_pd3dDevice	);
			SAFE_RELEASE(	m_pD3D			);
			return -1;
		}
	}
	
	// DX의 스프라이트는 디바이스가 생성된 후에 만들어야 한다.
	if(FAILED(D3DXCreateSprite(m_pd3dDevice, &m_pd3dSprite)))
	{
		SAFE_RELEASE(	m_pd3dDevice	);
		SAFE_RELEASE(	m_pD3D			);
		return E_FAIL;
	}

	if(FAILED(Init()))
		return -1;

	return 0;
}



LRESULT CD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_PAINT:
		break;
		
	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
			
		case IDM_EXIT:
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			return 0;
		}
		break;
		
	case WM_CLOSE:
		Destroy();
		HMENU hMenu;
		hMenu = GetMenu(hWnd);
		if( hMenu != NULL )
			DestroyMenu( hMenu );
		DestroyWindow( hWnd );
		PostQuitMessage(0);
		m_hWnd = NULL;
		return 0;
}

return DefWindowProc( hWnd, uMsg, wParam, lParam );
}



INT CD3DApplication::Run()
{
	HACCEL hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );
	
	MSG  msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	
	while( m_hWnd )
	{
		if(WM_QUIT == msg.message)
			break;

		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
		{
			if(0==TranslateAccelerator( m_hWnd, hAccel, &msg) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			if(FAILED(FrameMove()))
				break;

			if(FAILED(Render()))
				break;
		}
	}

	if( hAccel != NULL )
		DestroyAcceleratorTable( hAccel );
	
	return (INT)msg.wParam;
}
