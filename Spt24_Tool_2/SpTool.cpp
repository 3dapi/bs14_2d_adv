

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


				case IDM_SAVE:
				{
					if(m_pTile)
						m_pTile->SaveMapTxt();

					break;
				}
			}
			
			
			break;
		}


		case WM_MOVE:
		{
			RECT	rc1;
			RECT	rc2;
			RECT	rc3;
			int iX, iY;
			int iW, iH;

			if(!m_hWrk || !m_hImgSc)
				break;

			GetWindowRect(hWnd, &rc1);
			GetWindowRect(m_hWrk, &rc2);
			GetWindowRect(m_hImgSc, &rc3);

			iW = rc2.right - rc2.left;
			iH = rc2.bottom- rc2.top;

			iX = rc1.left -iW;
			iY = rc1.top;

			MoveWindow(m_hWrk, iX, iY, iW, iH, TRUE);

			iW = rc3.right - rc3.left;
			iH = rc3.bottom- rc3.top;

//			iX = rc1.left -iW;
			iY = rc1.bottom;

			MoveWindow(m_hImgSc, iX, iY, iW, iH, TRUE);

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