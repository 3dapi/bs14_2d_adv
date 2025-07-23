

#include "_StdAfx.h"


INT_PTR CALLBACK CMain::WndImgSc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM	wparHi = HIWORD(wParam);
	WPARAM	wparLo = LOWORD(wParam);
	
	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			RECT rt1;
			RECT rt2;
			RECT rt3;

			INT iWidth;
			INT iHeight;
			INT iX;
			INT iY;

			GetWindowRect(GMAIN->GetHwnd(), &rt1);
			GetWindowRect(GMAIN->m_hWrk, &rt2);
			GetWindowRect(hWnd, &rt3);

			iWidth = rt3.right - rt3.left;
			iHeight=  rt3.bottom- rt3.top;
			iX = rt2.left;
			iY = rt2.bottom + 3;
			
			MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);

			return TRUE;
		}

		case WM_COMMAND:
		{
			break;
		}
		
		case WM_CLOSE:
		{
			break;
		}
	}
	
	return(FALSE);
}



INT_PTR CALLBACK CMain::WndCrt(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM	wparHi = HIWORD(wParam);
	WPARAM	wparLo = LOWORD(wParam);
	
	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			RECT rt1;
			RECT rt2;
			RECT rt3;

			INT iWidth;
			INT iHeight;
			INT iX;
			INT iY;

			GetWindowRect(GMAIN->GetHwnd(), &rt1);
			GetWindowRect(GMAIN->m_hWrk, &rt2);
			GetWindowRect(hWnd, &rt3);

			iWidth = rt3.right - rt3.left;
			iHeight=  rt3.bottom- rt3.top;
			iX = rt1.left+100;
			iY = rt1.top+100;
			
			MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDC_CRT_OK:
				{
					int iNx = GetDlgItemInt(hWnd, IDC_TILE_NUM_X, 0, 0);
					int iNy = GetDlgItemInt(hWnd, IDC_TILE_NUM_Y, 0, 0);

					GMAIN->m_pTile->CreateMap(iNx, iNy);

					SendMessage(hWnd, WM_CLOSE, 0, 0);

					break;
				}

				case IDC_CRT_CANCEL:
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
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
