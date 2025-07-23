//
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


INT_PTR CALLBACK CMain::WndWrk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rc1;
	RECT rc2;

	INT iWidth;
	INT iHeight;
	INT iX;
	INT iY;
	
	TCITEM tie;
	
	WPARAM	wparHi;
	WPARAM	wparLo;
	
	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);
	
	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			GetWindowRect(GHWND, &rc1);
			GetWindowRect(hWnd, &rc2);
			
			iWidth = rc2.right - rc2.left;
			iHeight=  rc2.bottom- rc2.top;

			iX = rc1.left - iWidth;
			iY = rc1.top;// + iHeight;
			
			MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);
			GetWindowRect(hWnd, &rc2);

			GMAIN->m_hwWrkTb = GetDlgItem(hWnd, IDC_WORKTAB);

			TabCtrl_SetImageList(GMAIN->m_hwWrkTb, GMAIN->m_IL);
			
			tie.mask=TCIF_TEXT | TCIF_IMAGE;
			tie.pszText="Work 1";
			tie.iImage=0;
			TabCtrl_InsertItem(GMAIN->m_hwWrkTb,0,&tie);
			tie.pszText="Work 2";
			tie.iImage=1;
			TabCtrl_InsertItem(GMAIN->m_hwWrkTb,1,&tie);

			// 자식 윈도우 붙이기..
			GMAIN->m_hwWrk1 = CreateDialog( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_WORK1), hWnd,WndWrk1);

			ShowWindow(GMAIN->m_hwWrk1, SW_SHOW);
			GetWindowRect(GMAIN->m_hwWrkTb, &rc1);
			iX = 5;
			iY = 5;

			iWidth = rc1.right - rc1.left - iX * 2;
			iHeight = rc1.bottom - rc1.top - 40 - iY * 2;
			MoveWindow(GMAIN->m_hwWrk1, iX, iY, iWidth, iHeight, TRUE);

			return TRUE;
		}
		
		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case TCN_SELCHANGE:
				{
					GMAIN->m_nTab=TabCtrl_GetCurSel(GMAIN->m_hwWrkTb);

					GetWindowRect(GMAIN->m_hwWrkTb, &rc1);
					iX = 5;
					iY = 5;

					iWidth = rc1.right - rc1.left - iX * 2;
					iHeight = rc1.bottom - rc1.top - 40 - iY * 2;

					if (GMAIN->m_hwWrk)
					{
						ShowWindow(GMAIN->m_hwWrk1, SW_HIDE);
					}
					
					if (GMAIN->m_nTab==0)
					{
						ShowWindow(GMAIN->m_hwWrk1, SW_SHOW);
						MoveWindow(GMAIN->m_hwWrk1, iX, iY, iWidth, iHeight, TRUE);
						InvalidateRect(GMAIN->m_hwWrk1, NULL, TRUE);
					}
	
					break;
				}
			}
			
			break;
		}
		
		case WM_CLOSE:
		{
			break;
		}
	}
	
	return(FALSE);
}