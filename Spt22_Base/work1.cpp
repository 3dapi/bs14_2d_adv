// Implementation of the CMcGrid work1
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"

INT_PTR CALLBACK WndWrk1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static	WPARAM	wparHi;
	static	WPARAM	wparLo;
	static	INT		nMst=-1;
	static	INT		nSub=-1;
	static	HWND	hTree;
	
	wparHi = HIWORD(wParam);
	wparLo = LOWORD(wParam);
	
	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			hTree = GetDlgItem(hWnd, IDC_PTC_TREE);

			return TRUE;
		}
		
		case WM_NOTIFY:
		{
			TVITEMEX	TvResEx;
			TVITEMEX	TvResPr;
			HTREEITEM	hResPrn;
			TCHAR		sResCap[30] = "\0";
			TCHAR		sResPrn[30] = "\0";
			
			LPNMHDR hdr;
			LPNMTREEVIEW ntv;
			hdr=(LPNMHDR)lParam;
			ntv=(LPNMTREEVIEW)lParam;			
			
			if(hdr->hwndFrom == hTree)
			{
				switch (hdr->code)
				{
					// 선택된 항목을 보여준다.
					case TVN_SELCHANGED:
					{
						TvResEx.mask=TVIF_PARAM | TVIF_TEXT;
						TvResEx.hItem=ntv->itemNew.hItem;
						TvResEx.pszText=sResCap;
						TvResEx.cchTextMax=sizeof(sResCap);
						TreeView_GetItem(hTree, &TvResEx);
						
						TCHAR sKind[256];
						TCHAR sName[256];
				
						hResPrn = TreeView_GetParent(hTree, TvResEx.hItem);
						
						if(hResPrn)
						{
							TvResPr.mask=TVIF_PARAM | TVIF_TEXT;
							TvResPr.hItem=hResPrn;
							TvResPr.pszText=sResPrn;
							TvResPr.cchTextMax=sizeof(sResPrn);
							TreeView_GetItem(hTree,&TvResPr);
							
							// 여기서 Work2의 데이터를 채워 넣는다

							TCHAR sIdMst[256];
							TCHAR sIdSub[256];
							
							sscanf(TvResEx.pszText, "%s %s", sIdMst, sIdSub);

							nMst = atoi(sIdMst);
							nSub = atoi(sIdSub);

						}
						
						else
						{
							sscanf(TvResEx.pszText, "%s %s", sKind, sName);
							
							SetDlgItemText(hWnd, IDC_PTC_TREE_NAME, sName);
							SetDlgItemText(hWnd, IDC_PTC_TREE_KIND, sKind);
						}
						
						break;
					}
				}
			}
			
			break;
		}	//case WM_NOTIFY:
		
		
		case WM_COMMAND:
		{
			switch(wparLo)
			{

				case IDC_PTC_TREE_SAVE:
				{
					MessageBox(hWnd,"Hello world", "Message", 0);
					break;
				}
				
			}

			break;
		}	// case WM_COMMAND:

		
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

