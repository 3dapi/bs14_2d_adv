/*****************************************************************************
IsoHex10_3.cpp
Ernest S. Pazera
30JUN2000
Start a WIN32 Application Workspace, add in this file
Needs ddraw.lib and dxguid.lib
Needs GDICanvas.h/cpp
Needs DDFuncs.h/cpp
Art by Ari Feldman
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//INCLUDES
//////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>
#include "GDICanvas.h"
#include "ddraw.h"
#include "DDFuncs.h"
#include "TileSet.h"
   
//////////////////////////////////////////////////////////////////////////////
//DEFINES
//////////////////////////////////////////////////////////////////////////////
//name for our window class
#define WINDOWCLASS "ISOHEX10"
//title of the application
#define WINDOWTITLE "IsoHex 10-3, with art by Ari Feldman"

//map and tile constants
const int TILEWIDTH=32;
const int TILEHEIGHT=32;
const int MAPWIDTH=18;
const int MAPHEIGHT=18;

//panels
const int MAPPANELX=12;
const int MAPPANELY=12;
const int MAPPANELWIDTH=MAPWIDTH*TILEWIDTH;
const int MAPPANELHEIGHT=MAPHEIGHT*TILEHEIGHT;

const int TILEPANELX=604;
const int TILEPANELY=12;
const int TILEPANELCOLUMNS=6;
const int TILEPANELROWS=18;
const int TILEPANELWIDTH=TILEPANELCOLUMNS*TILEWIDTH;
const int TILEPANELHEIGHT=TILEPANELROWS*TILEHEIGHT;

//////////////////////////////////////////////////////////////////////////////
//PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
bool Prog_Init();//game data initalizer
void Prog_Loop();//main game loop
void Prog_Done();//game clean up

void ShowMapPanel();//show the map panel
void ShowTilePanel();//show the tile panel

//////////////////////////////////////////////////////////////////////////////
//GLOBALS
//////////////////////////////////////////////////////////////////////////////
HINSTANCE hInstMain=NULL;//main application handle
HWND hWndMain=NULL;//handle to our main window

//directdraw
LPDIRECTDRAW7 lpdd=NULL;
LPDIRECTDRAWSURFACE7 lpddsMain=NULL;
LPDIRECTDRAWSURFACE7 lpddsBack=NULL;

//tileset
CTileSet tsTileSet;

//tile map
int iTileMap[MAPWIDTH][MAPHEIGHT];

//tile selection
int iTileTop=0;
int iTileSelected=0;

//////////////////////////////////////////////////////////////////////////////
//WINDOWPROC
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TheWindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//which message did we get?
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			//point to contain mouse coords
			POINT ptMouse;
			ptMouse.x=LOWORD(lParam);
			ptMouse.y=HIWORD(lParam);

			//RECT used for zone checking
			RECT rcZone;

			//other variables
			int mapx=0;
			int mapy=0;
			int tilex=0;
			int tiley=0;
			int tilenum=0;

			//check the map panel
			SetRect(&rcZone,MAPPANELX,MAPPANELY,MAPPANELX+MAPPANELWIDTH,MAPPANELY+MAPPANELHEIGHT);
			if(PtInRect(&rcZone,ptMouse))
			{
				//in map panel
				//calculate what tile mouse is on
				mapx=(ptMouse.x-MAPPANELX)/TILEWIDTH;
				mapy=(ptMouse.y-MAPPANELY)/TILEHEIGHT;

				//change map tile to currently selected tile
				iTileMap[mapx][mapy]=iTileSelected;

				return(0);//handled
			}

			//check the tile panel
			SetRect(&rcZone,TILEPANELX,TILEPANELY,TILEPANELX+TILEPANELWIDTH,TILEPANELY+TILEPANELHEIGHT);
			if(PtInRect(&rcZone,ptMouse))
			{
				//calculate which tile was selected
				tilex=(ptMouse.x-TILEPANELX)/TILEWIDTH;
				tiley=(ptMouse.y-TILEPANELY)/TILEHEIGHT;
				tilenum=iTileTop+tilex+tiley*TILEPANELCOLUMNS;

				//check for valid tile
				if(tilenum<tsTileSet.GetTileCount())
				{
					//assign current tile
					iTileSelected=tilenum;
				}

				return(0);//handled
			}

			//scroll tileset up
			SetRect(&rcZone,TILEPANELX,0,TILEPANELX+TILEPANELWIDTH,TILEPANELY);
			if(PtInRect(&rcZone,ptMouse))
			{
				//check if we can scroll up
				if(iTileTop>0)
				{
					//scroll up
					iTileTop-=TILEPANELCOLUMNS;
				}
			}
			//scroll tileset down
			SetRect(&rcZone,TILEPANELX,TILEPANELY+TILEPANELHEIGHT,TILEPANELX+TILEPANELWIDTH,600);
			if(PtInRect(&rcZone,ptMouse))
			{
				//check if we can scroll down
				if((iTileTop+TILEPANELCOLUMNS)<tsTileSet.GetTileCount())
				{
					//scroll up
					iTileTop+=TILEPANELCOLUMNS;
				}
			}
			return(0);//handled
		}break;
	case WM_MOUSEMOVE:
		{
			//if the left button is down
			if(wParam & MK_LBUTTON)
			{
			//point to contain mouse coords
			POINT ptMouse;
			ptMouse.x=LOWORD(lParam);
			ptMouse.y=HIWORD(lParam);

			//RECT used for zone checking
			RECT rcZone;

			//other variables
			int mapx=0;
			int mapy=0;
			int tilex=0;
			int tiley=0;
			int tilenum=0;

			//check the map panel
			SetRect(&rcZone,MAPPANELX,MAPPANELY,MAPPANELX+MAPPANELWIDTH,MAPPANELY+MAPPANELHEIGHT);
			if(PtInRect(&rcZone,ptMouse))
			{
				//in map panel
				//calculate what tile mouse is on
				mapx=(ptMouse.x-MAPPANELX)/TILEWIDTH;
				mapy=(ptMouse.y-MAPPANELY)/TILEHEIGHT;

				//change map tile to currently selected tile
				iTileMap[mapx][mapy]=iTileSelected;

				return(0);//handled
			}

			//check the tile panel
			SetRect(&rcZone,TILEPANELX,TILEPANELY,TILEPANELX+TILEPANELWIDTH,TILEPANELY+TILEPANELHEIGHT);
			if(PtInRect(&rcZone,ptMouse))
			{
				//calculate which tile was selected
				tilex=(ptMouse.x-TILEPANELX)/TILEWIDTH;
				tiley=(ptMouse.y-TILEPANELY)/TILEHEIGHT;
				tilenum=iTileTop+tilex+tiley*TILEPANELCOLUMNS;

				//check for valid tile
				if(tilenum<tsTileSet.GetTileCount())
				{
					//assign current tile
					iTileSelected=tilenum;
				}
			}
			}
			return(0);//handled
		}break;
	case WM_KEYDOWN:
		{
			//on escape, destroy main window
			if(wParam==VK_ESCAPE)
			{
				DestroyWindow(hWndMain);
			}

			return(0);//handled
		}break;
	case WM_DESTROY://the window is being destroyed
		{

			//tell the application we are quitting
			PostQuitMessage(0);

			//handled message, so return 0
			return(0);

		}break;
	case WM_PAINT://the window needs repainting
		{
			//a variable needed for painting information
			PAINTSTRUCT ps;
			
			//start painting
			HDC hdc=BeginPaint(hwnd,&ps);

			/////////////////////////////
			//painting code would go here
			/////////////////////////////

			//end painting
			EndPaint(hwnd,&ps);
						
			//handled message, so return 0
			return(0);
		}break;
	}

	//pass along any other message to default message handler
	return(DefWindowProc(hwnd,uMsg,wParam,lParam));
}


//////////////////////////////////////////////////////////////////////////////
//WINMAIN
//////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	//assign instance to global variable
	hInstMain=hInstance;

	//create window class
	WNDCLASSEX wcx;

	//set the size of the structure
	wcx.cbSize=sizeof(WNDCLASSEX);

	//class style
	wcx.style=CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//window procedure
	wcx.lpfnWndProc=TheWindowProc;

	//class extra
	wcx.cbClsExtra=0;

	//window extra
	wcx.cbWndExtra=0;

	//application handle
	wcx.hInstance=hInstMain;

	//icon
	wcx.hIcon=LoadIcon(NULL,IDI_APPLICATION);

	//cursor
	wcx.hCursor=LoadCursor(NULL,IDC_ARROW);

	//background color
	wcx.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);

	//menu
	wcx.lpszMenuName=NULL;

	//class name
	wcx.lpszClassName=WINDOWCLASS;

	//small icon
	wcx.hIconSm=NULL;

	//register the window class, return 0 if not successful
	if(!RegisterClassEx(&wcx)) return(0);

	//create main window
	hWndMain=CreateWindowEx(0,WINDOWCLASS,WINDOWTITLE, WS_POPUP | WS_VISIBLE,0,0,320,240,NULL,NULL,hInstMain,NULL);

	//error check
	if(!hWndMain) return(0);

	//if program initialization failed, then return with 0
	if(!Prog_Init()) return(0);

	//message structure
	MSG msg;

	//message pump
	for(;;)	
	{
		//look for a message
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			//there is a message

			//check that we arent quitting
			if(msg.message==WM_QUIT) break;
			
			//translate message
			TranslateMessage(&msg);

			//dispatch message
			DispatchMessage(&msg);
		}

		//run main game loop
		Prog_Loop();
	}
	
	//clean up program data
	Prog_Done();

	//return the wparam from the WM_QUIT message
	return(msg.wParam);
}

//////////////////////////////////////////////////////////////////////////////
//INITIALIZATION
//////////////////////////////////////////////////////////////////////////////
bool Prog_Init()
{
	//create IDirectDraw7
	lpdd=LPDD_Create(hWndMain,DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);

	//set display mode
	lpdd->SetDisplayMode(800,600,16,0,0);

	//create primary surface
	lpddsMain=LPDDS_CreatePrimary(lpdd,1);

	//get back buffer
	lpddsBack=LPDDS_GetSecondary(lpddsMain);

	//clear out back buffer
	DDBLTFX ddbltfx;
	DDBLTFX_ColorFill(&ddbltfx,0);
	lpddsBack->Blt(NULL,NULL,NULL,DDBLT_WAIT | DDBLT_COLORFILL,&ddbltfx);

	//load in tileset
	tsTileSet.Load(lpdd,"IsoHex10_3.bmp");

	//clear out the map
	for(int mapx=0;mapx<MAPWIDTH;mapx++)
	{
		for(int mapy=0;mapy<MAPHEIGHT;mapy++)
		{
			iTileMap[mapx][mapy]=0;
		}
	}

	return(true);//return success
}

//////////////////////////////////////////////////////////////////////////////
//CLEANUP
//////////////////////////////////////////////////////////////////////////////
void Prog_Done()
{
	//destroy primary surface
	LPDDS_Release(&lpddsMain);

	//destroy IDirectDraw7
	LPDD_Release(&lpdd);
}

//////////////////////////////////////////////////////////////////////////////
//MAIN GAME LOOP
//////////////////////////////////////////////////////////////////////////////
void Prog_Loop()
{
	//show the map panel
	ShowMapPanel();
	//show tile selection panel
	ShowTilePanel();
	//flip
	lpddsMain->Flip(NULL,DDFLIP_WAIT);
}

//show the map panel
void ShowMapPanel()
{
	//clear out map panel
	//set up fill rect
	RECT rcFill;
	SetRect(&rcFill,MAPPANELX,MAPPANELY,MAPPANELX+MAPPANELWIDTH,MAPPANELY+MAPPANELHEIGHT);
	//set up ddbltfx
	DDBLTFX ddbltfx;
	DDBLTFX_ColorFill(&ddbltfx,0);
	lpddsBack->Blt(&rcFill,NULL,NULL,DDBLT_WAIT | DDBLT_COLORFILL,&ddbltfx);

	//loop through map
	for(int mapy=0;mapy<MAPHEIGHT;mapy++)
	{
		for(int mapx=0;mapx<MAPWIDTH;mapx++)
		{
			//put the tile
			tsTileSet.PutTile(lpddsBack,MAPPANELX+mapx*TILEWIDTH,MAPPANELY+mapy*TILEHEIGHT,iTileMap[mapx][mapy]);
		}
	}
}

//show the tile panel
void ShowTilePanel()
{
	//clear out map panel
	//set up fill rect
	RECT rcFill;
	SetRect(&rcFill,TILEPANELX,TILEPANELY,TILEPANELX+TILEPANELWIDTH,TILEPANELY+TILEPANELHEIGHT);
	//set up ddbltfx
	DDBLTFX ddbltfx;
	DDBLTFX_ColorFill(&ddbltfx,0);
	lpddsBack->Blt(&rcFill,NULL,NULL,DDBLT_WAIT | DDBLT_COLORFILL,&ddbltfx);

	//set tile counter to first tile
	int tilenum=iTileTop;
	//loop through columns and rows
	for(int tiley=0;tiley<TILEPANELROWS;tiley++)
	{
		for(int tilex=0;tilex<TILEPANELCOLUMNS;tilex++)
		{
			//check for tilenum's existence in tileset
			if(tilenum<tsTileSet.GetTileCount())
			{
				tsTileSet.PutTile(lpddsBack,TILEPANELX+tilex*TILEWIDTH,TILEPANELY+tiley*TILEHEIGHT,tilenum);
				//check for selected tile
				if(tilenum==iTileSelected)
				{
					//grab the dc
					HDC hdc;
					lpddsBack->GetDC(&hdc);
					//calculate outline rect
					RECT rcOutline;
					SetRect(&rcOutline,TILEPANELX+tilex*TILEWIDTH,TILEPANELY+tiley*TILEHEIGHT,TILEPANELX+tilex*TILEWIDTH+TILEWIDTH,TILEPANELY+tiley*TILEHEIGHT+TILEHEIGHT);

					//select a white pen into dc
					SelectObject(hdc,(HPEN)GetStockObject(WHITE_PEN));

					//place selection rectangle
					MoveToEx(hdc,rcOutline.left,rcOutline.top,NULL);
					LineTo(hdc,rcOutline.right-1,rcOutline.top);
					LineTo(hdc,rcOutline.right-1,rcOutline.bottom-1);
					LineTo(hdc,rcOutline.left,rcOutline.bottom-1);
					LineTo(hdc,rcOutline.left,rcOutline.top);

					//release the dc
					lpddsBack->ReleaseDC(hdc);
				}
			}
			//increase tile counter
			tilenum++;
		}
	}
}
