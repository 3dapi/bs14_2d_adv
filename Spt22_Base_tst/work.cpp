// Implementation of the CMcGrid work
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"

INT_PTR CALLBACK WndWrk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM	wparHi = HIWORD(wParam);
	WPARAM	wparLo = LOWORD(wParam);
	
	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			{
				RECT rt1;
				RECT rt2;

				INT iWidth;
				INT iHeight;
				INT iX;
				INT iY;

				RECT trt={0, 0, 0, 0};

				GetWindowRect(g_pApp->GetHwnd(), &rt1);
				GetWindowRect(hWnd, &rt2);
			
				iWidth = rt2.right - rt2.left;
				iHeight=  rt2.bottom- rt2.top;

				iX = rt1.left - iWidth;
				iY = rt1.top;// + iHeight;
				MoveWindow(hWnd, iX, iY, iWidth, iHeight, TRUE);
			}


			HIMAGELIST hImage  = g_pApp->m_IL;
			HWND hw_parent = hWnd;
			INT tab_item_h = 20;
			INT tab_item_p =  4;

			INT tab_pos_y  = 20;
			INT tab_pos_x  = 15;
			INT tab_w      =  0;
			INT tab_h      = tab_item_h + tab_item_p;

			RECT rc_ref;
			GetClientRect(hw_parent, &rc_ref);
			tab_w = rc_ref.right  - rc_ref.left - 30;

			g_pApp->htab_alter = GetDlgItem(hw_parent, IDC_TAB_MERGE);								// creation parameters
			if(g_pApp->htab_alter)
			{
				TabCtrl_SetImageList(g_pApp->htab_alter, hImage);
				TabCtrl_SetItemSize (g_pApp->htab_alter, 100, tab_item_h);
				TabCtrl_SetPadding  (g_pApp->htab_alter,  10, tab_item_p);
				::MoveWindow(g_pApp->htab_alter, tab_pos_x, tab_pos_y, tab_w+2, tab_h, TRUE);

				TCITEM item;
				item.mask=TCIF_TEXT | TCIF_IMAGE;
				item.pszText="Work 1";	item.iImage=0;	TabCtrl_InsertItem(g_pApp->htab_alter, 0, &item);
				item.pszText="Work 2";	item.iImage=1;	TabCtrl_InsertItem(g_pApp->htab_alter, 1, &item);
				::UpdateWindow(g_pApp->htab_alter);
			}

			// list 0 ...
			g_pApp->hlst_scene = GetDlgItem(hw_parent, IDC_LST_SCENE);
			{
				INT h = 250;
				::MoveWindow(g_pApp->hlst_scene, tab_pos_x+1, tab_pos_y+tab_h +1, tab_w, h, TRUE);
				::ShowWindow(g_pApp->hlst_scene, SW_SHOW);
			}

			// list 1 ...
			g_pApp->hlst_mscene = GetDlgItem(hw_parent, IDC_LST_MSCENE);
			{
				INT h = 250;
				::MoveWindow(g_pApp->hlst_mscene, tab_pos_x+1, tab_pos_y+tab_h +1, tab_w, h, TRUE);
				::ShowWindow(g_pApp->hlst_mscene, SW_HIDE);
			}


			// list ..
			g_pApp->hlst_scene = GetDlgItem(hw_parent, IDC_LST_SCENE);
			{
				static LVCOLUMN lv ={LVCF_TEXT | LVCF_WIDTH | LVCF_FMT, 0, };
				lv.pszText = (LPSTR)"item";
				lv.fmt = LVCFMT_LEFT;
				lv.cx = 80;
				ListView_InsertColumn(g_pApp->hlst_scene, 1, &lv);

				lv.pszText = (LPSTR)"value";
				lv.fmt = LVCFMT_LEFT;
				lv.cx = 130;
				ListView_InsertColumn(g_pApp->hlst_scene, 2, &lv);

				LVCOLUMN COL={0};		//열 구조체 
				LVITEM   LI ={0};		//항목 구조체

				LI.mask = LVIF_TEXT;
				LI.iItem = 1;         //행 (전체 화면 - 캡션포함)
				LI.pszText = "111";  //문자열 값
				ListView_InsertItem(g_pApp->hlst_scene, &LI);
				ListView_SetItemText(g_pApp->hlst_scene, 0, 1, "aaaa");
			}

			g_pApp->hlst_mscene = GetDlgItem(hw_parent, IDC_LST_MSCENE);
			{
				static LVCOLUMN lv ={LVCF_TEXT | LVCF_WIDTH | LVCF_FMT, 0, };
				lv.pszText = (LPSTR)"item2";
				lv.fmt = LVCFMT_LEFT;
				lv.cx = 80;
				ListView_InsertColumn(g_pApp->hlst_mscene, 1, &lv);
			}

			return TRUE;
		}
		
		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case TCN_SELCHANGE:
				{
					RECT rc_tab;
					// 자식 윈도우 선택
					g_pApp->m_nTab=TabCtrl_GetCurSel(g_pApp->htab_alter);

					ShowWindow(g_pApp->hlst_scene, SW_HIDE);
					ShowWindow(g_pApp->hlst_mscene, SW_HIDE);
					
					if (g_pApp->m_nTab==0)
					{
						ShowWindow    (g_pApp->hlst_scene, SW_SHOW);
						InvalidateRect(g_pApp->hlst_scene, NULL, TRUE);
					}
					else
					{
						ShowWindow    (g_pApp->hlst_mscene, SW_SHOW);
						InvalidateRect(g_pApp->hlst_mscene, NULL, TRUE);
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