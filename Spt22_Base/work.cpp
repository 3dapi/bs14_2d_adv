// Implementation of the CMcGrid work
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"

INT_PTR CALLBACK WndWrk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rt1;
	RECT rt2;

	INT iWidth;
	INT iHeight;
	INT iX;
	INT iY;
	
	TCITEM tie;
	RECT trt={0,0,0,0};
	
	WPARAM	wparHi;
	WPARAM	wparLo;
	
	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);
	
	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			GetWindowRect(g_pApp->GetHwnd(), &rt1);
			GetWindowRect(hWnd, &rt2);
			
			iWidth = rt2.right - rt2.left;
			iHeight=  rt2.bottom- rt2.top;

			iX = rt1.left - iWidth;
			iY = rt1.top;// + iHeight;
			
			MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);
			GetWindowRect(hWnd, &rt2);

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
			GMAIN->m_hwWrk1 = CreateDialog( GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_WORK1), hWnd,WndWrk1);

			ShowWindow(GMAIN->m_hwWrk1, SW_SHOW);
			GetWindowRect(GMAIN->m_hwWrkTb, &rt1);
			iX = 5;
			iY = 5;

			iWidth = rt1.right - rt1.left - iX * 2;
			iHeight = rt1.bottom - rt1.top - 40 - iY * 2;
			MoveWindow(GMAIN->m_hwWrk1, iX, iY, iWidth, iHeight, TRUE);

			return TRUE;
		}
		
		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case TCN_SELCHANGE:
				{
					// 자식 윈도우 선택
					GMAIN->m_nTab=TabCtrl_GetCurSel(GMAIN->m_hwWrkTb);

					GetWindowRect(GMAIN->m_hwWrkTb, &rt1);
					iX = 5;
					iY = 5;

					iWidth = rt1.right - rt1.left - iX * 2;
					iHeight = rt1.bottom - rt1.top - 40 - iY * 2;

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