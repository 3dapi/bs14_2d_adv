
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

			GetWindowRect(g_pApp->GetHwnd(), &rt1);
			GetWindowRect(g_pApp->m_hwWrk, &rt2);
			GetWindowRect(hWnd, &rt3);

			iWidth = rt3.right - rt3.left;
			iHeight=  rt3.bottom- rt3.top;
			iX = rt1.left - iWidth;
			iY = rt2.bottom + 3;
			
			MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);


			

			return TRUE;
		}

		case WM_PAINT:
		{
			HWND hwnd_prev = GetDlgItem(hWnd, IDC_PREV);
			HDC hDC = GetDC(hwnd_prev);
			CHAR strMsg[MAX_PATH]={0};
			sprintf(strMsg, "Hello world");
			//RECT rct;
			//GetClientRect(hwnd_prev, &rct);
			//DrawText(hDC, strMsg, -1, &rct, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

			TextOut(hDC, 20, 20, strMsg, strlen(strMsg));
			ReleaseDC(hwnd_prev, hDC);

			break;
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