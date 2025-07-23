//#pragma comment(linker,"/manifestdependency:\"type='win32'\
//						 name='Microsoft.Windows.Common-Controls'\
//						 version='6.0.0.0' processorArchitecture='*'\
//						 publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "_StdAfx.h"


CMain* g_pApp  = NULL;


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	CMain d3dApp;
	
	g_pApp  = &d3dApp;
	
	InitCommonControls();
	if( FAILED( d3dApp.Create( hInst ) ) )
		return 0;
	
	return d3dApp.Run();
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
			TCHAR strMsg[MAX_PATH];
			wsprintf( strMsg, TEXT("Right screen") );
			RECT rct;
			GetClientRect( hWnd, &rct );
			DrawText( hDC, strMsg, -1, &rct, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
			ReleaseDC( hWnd, hDC );

			break;
		}
		
		
	case WM_COMMAND:
		{
			switch(wparLo)
			{
				case ID_VIEW_WIRE:
					break;

				case ID_VIEW_SOLID:
					break;


				case IDM_VIEW_FIELD:
					break;
			}
			
			
			break;
		}
		
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}

