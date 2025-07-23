// Implementation of the CD3DApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


static CD3DApplication* g_pD3DApp = NULL;


CD3DApplication::CD3DApplication()
{
	g_pD3DApp           = this;

	m_hWnd              = NULL;
	m_hMenu             = NULL;

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

		// Register the windows class
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
		AdjustWindowRect( &rc, m_dWinStyle, true );


		// Create the render window
		m_hWnd = CreateWindow(	m_sCls
			,	m_sCls
			,	m_dWinStyle
			,	300//(iScnSysW - (rc.right-rc.left))/2
			,	10//(iScnSysH - (rc.bottom-rc.top))/2
			,	(rc.right-rc.left)
			,	(rc.bottom-rc.top)
			,	0
			,	LoadMenu( hInstance, MAKEINTRESOURCE(IDR_MENU) )
			,	hInstance
			,	0 );
	}

	m_dWinStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	GetWindowRect( m_hWnd, &m_rcWinBounds );
	GetClientRect( m_hWnd, &m_rcWinClient );

	HRESULT hr;
	hr = Init();
	return hr;
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
		case IDM_TOGGLEFULLSCREEN:
			return 0;

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
		}
	}

	if( hAccel != NULL )
		DestroyAcceleratorTable( hAccel );

	return (INT)msg.wParam;
}
