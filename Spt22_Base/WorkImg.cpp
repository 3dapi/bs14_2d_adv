
#include "_StdAfx.h"

INT_PTR CALLBACK WndImgSrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			RECT rt2;
			RECT rt3;

			INT iWidth;
			INT iHeight;
			INT iX;
			INT iY;

			GetWindowRect(GMAIN->GetHwnd(), &rt1);
			GetWindowRect(GMAIN->m_hwWrk, &rt2);
			GetWindowRect(hWnd, &rt3);

			iWidth = rt3.right - rt3.left;
			iHeight=  rt3.bottom- rt3.top;
			iX = rt1.left - iWidth;
			iY = rt2.bottom + 3;
			
			MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);

			return TRUE;
		}
		
	
		case WM_HSCROLL:
		{
			INT Value =0;
			
			switch (wparLo)
			{
				case SB_LINELEFT:
				{
					break;
				}


				case SB_PAGELEFT:
				{
					break;
				}
			
				case SB_LINERIGHT:
				{
					break;
				}

				case SB_PAGERIGHT:
				{
					break;
				}

				case SB_THUMBTRACK:
				{
					break;
				}
			}
			
		
			break;
		}
		
		
		case WM_VSCROLL:
		{
//			HWND hScroll = GetDlgItem(hWnd, IDC_SCRLY);

			switch (wparLo)
			{
				case SB_LINELEFT:
				{
					break;
				}

				case SB_PAGELEFT:
				{
					break;
				}
			
				case SB_LINERIGHT:
				{
					break;
				}

				case SB_PAGERIGHT:
				{
					break;
				}

				case SB_THUMBTRACK:
				{
					break;
				}
			}
			
			break;
		}
		
		case WM_COMMAND:
		{
			break;
		}
		
		case WM_SYSCOMMAND:
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