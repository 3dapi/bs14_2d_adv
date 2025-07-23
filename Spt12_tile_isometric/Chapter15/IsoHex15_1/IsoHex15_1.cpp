/*****************************************************************************
IsoHex15_1.cpp
Ernest S. Pazera
01AUG2000
Start a WIN32 Application Workspace, add in this file
Requires the following libs:
ddraw.lib, dxguid.lib
Requires the following files:
DDFuncs.h.cpp, GDICanvas.h/cpp, IsoMouseMap.h/cpp,
IsoScroller.h/cpp, IsoTilePlotter.h/cpp, IsoTileWalker.h/cpp
TileSet.h/cpp. IsoHexCore.h, IsoHexDefs.h
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//INCLUDES
//////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>  
#include "DDFuncs.h"
#include "TileSet.h"
#include "IsoHexCore.h"
 

//////////////////////////////////////////////////////////////////////////////
//DEFINES
//////////////////////////////////////////////////////////////////////////////
//name for our window class
#define WINDOWCLASS "ISOHEX15"
//title of the application
#define WINDOWTITLE "IsoHex 15-1"

const int MAPWIDTH=20;
const int MAPHEIGHT=20;

//////////////////////////////////////////////////////////////////////////////
//PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
bool Prog_Init();//game data initalizer
void Prog_Loop();//main game loop
void Prog_Done();//game clean up

//////////////////////////////////////////////////////////////////////////////
//GLOBALS
//////////////////////////////////////////////////////////////////////////////
HINSTANCE hInstMain=NULL;//main application handle
HWND hWndMain=NULL;//handle to our main window

//directdraw
LPDIRECTDRAW7 lpdd=NULL;
LPDIRECTDRAWSURFACE7 lpddsMain=NULL;
LPDIRECTDRAWSURFACE7 lpddsBack=NULL;
LPDIRECTDRAWCLIPPER lpddClip=NULL;

//tilesets
CTileSet tsIso;//main tileset
CTileSet tsCursor;//cursor

//isohexcore components
CTilePlotter TilePlotter;//plotter
CTileWalker TileWalker;//walker
CScroller Scroller;//scroller
CMouseMap MouseMap;//mousemap

POINT ptCursor;//keep track of the cursor
POINT ptScroll;//keep track of how quickly we scroll

int iMap[MAPWIDTH][MAPHEIGHT];//map array

//////////////////////////////////////////////////////////////////////////////
//WINDOWPROC
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TheWindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//which message did we get?
	switch(uMsg)
	{
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				{
					DestroyWindow(hWndMain);
				}break;
			case '1':
				{
					//set up the iso engine for slide maps
					TileWalker.SetMapType(ISOMAP_SLIDE);//set walker to slide mapping

					TilePlotter.SetMapType(ISOMAP_SLIDE);//set plotter to slide mapping

					//recalculate the scroller
					Scroller.CalcWorldSpace(&TilePlotter,&tsIso.GetTileList()[0].rcDstExt,MAPWIDTH,MAPHEIGHT);
					Scroller.CalcAnchorSpace();

					//set the screen anchor back to zero
					Scroller.GetAnchor()->x=0;
					Scroller.GetAnchor()->y=0;
				}break;
			case '2':
				{
					//set up iso engine for staggered maps
					TileWalker.SetMapType(ISOMAP_STAGGERED);//set walker to staggered mapping

					TilePlotter.SetMapType(ISOMAP_STAGGERED);//set plotter to staggered mapping

					//recalculate the scroller
					Scroller.CalcWorldSpace(&TilePlotter,&tsIso.GetTileList()[0].rcDstExt,MAPWIDTH,MAPHEIGHT);
					Scroller.CalcAnchorSpace();

					//reset the anchor to (0,0)
					Scroller.GetAnchor()->x=0;
					Scroller.GetAnchor()->y=0;
				}break;
			case '3':
				{
					//set up iso engine for diamond maps
					TileWalker.SetMapType(ISOMAP_DIAMOND);//set walker to diamond map mode

					TilePlotter.SetMapType(ISOMAP_DIAMOND);//set plotter to diamond mapping

					//recalculate the scroller spaces
					Scroller.CalcWorldSpace(&TilePlotter,&tsIso.GetTileList()[0].rcDstExt,MAPWIDTH,MAPHEIGHT);
					Scroller.CalcAnchorSpace();

					//reset the anchor to (0,0)
					Scroller.GetAnchor()->x=0;
					Scroller.GetAnchor()->y=0;
				}break;
			}
		}break;
	case WM_MOUSEMOVE:
		{
			//grab mouse x and y
			int x=LOWORD(lParam);
			int y=HIWORD(lParam);

			//reset scrolling speeds to zero
			ptScroll.x=0;
			ptScroll.y=0;

			//left scroll?
			if(x<8) ptScroll.x=x-8;

			//upward scroll?
			if(y<8) ptScroll.y=y-8;

			//right scroll?
			if(x>=632) ptScroll.x=x-632;

			//downward scroll?
			if(y>=472) ptScroll.y=y-472;
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
	//create IDirectDraw object
	lpdd=LPDD_Create(hWndMain,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);

	//set display mode
	lpdd->SetDisplayMode(640,480,16,0,0);

	//create primary surface
	lpddsMain=LPDDS_CreatePrimary(lpdd,1);

	//get back buffer
	lpddsBack=LPDDS_GetSecondary(lpddsMain);

	//create clipper
	lpdd->CreateClipper(0,&lpddClip,NULL);

	//associate window with the clipper
	lpddClip->SetHWnd(0,hWndMain);

	//attach clipper to back buffer
	lpddsBack->SetClipper(lpddClip);

	//load in the mousemap
	MouseMap.Load("MouseMap.bmp");

	//set up the tile plotter
	TilePlotter.SetMapType(ISOMAP_DIAMOND);//diamond mode
	TilePlotter.SetTileSize(MouseMap.GetWidth(),MouseMap.GetHeight());//grab width and height from mousemap

	//set up tile walker to diamond mode
	TileWalker.SetMapType(ISOMAP_DIAMOND);

	//set up screeen space
	RECT rcTemp;
	SetRect(&rcTemp,0,0,640,480);
	Scroller.SetScreenSpace(&rcTemp);

	//load in tiles and cursor
	tsIso.Load(lpdd,"Tiles.bmp");
	tsCursor.Load(lpdd,"cursor.bmp");

	//grab tile extent from tileset
	CopyRect(&rcTemp,&tsIso.GetTileList()[0].rcDstExt);

	//calculate the worldspace
	Scroller.CalcWorldSpace(&TilePlotter,&rcTemp,MAPWIDTH,MAPHEIGHT);

	//calculate the mousemap reference point
	MouseMap.CalcReferencePoint(&TilePlotter,&rcTemp);

	//calculate anchor space
	Scroller.CalcAnchorSpace();

	//set wrap modes for scroller
	Scroller.SetHWrapMode(WRAPMODE_CLIP);
	Scroller.SetVWrapMode(WRAPMODE_CLIP);

	//set scroller anchor to (0,0)
	Scroller.GetAnchor()->x=0;
	Scroller.GetAnchor()->y=0;

	//attach scrolelr and tilewalker to mousemap
	MouseMap.SetScroller(&Scroller);
	MouseMap.SetTileWalker(&TileWalker);

	//set up the map to a random tilefield
	for(int x=0;x<MAPWIDTH;x++)
	{
		for(int y=0;y<MAPHEIGHT;y++)
		{
			iMap[x][y]=rand()%tsIso.GetTileCount();
		}
	}

	return(true);//return success
}

//////////////////////////////////////////////////////////////////////////////
//CLEANUP
//////////////////////////////////////////////////////////////////////////////
void Prog_Done()
{
	//release main/back surfaces
	LPDDS_Release(&lpddsMain);

	//release clipper
	LPDDCLIP_Release(&lpddClip);

	//release directdraw
	LPDD_Release(&lpdd);
}

//////////////////////////////////////////////////////////////////////////////
//MAIN GAME LOOP
//////////////////////////////////////////////////////////////////////////////
void Prog_Loop()
{
	//clear out backbuffer
	DDBLTFX ddbltfx;
	DDBLTFX_ColorFill(&ddbltfx,0);
	lpddsBack->Blt(NULL,NULL,NULL,DDBLT_WAIT | DDBLT_COLORFILL,&ddbltfx);
	
	//move the anchor based on scrolling speed
	Scroller.MoveAnchor(ptScroll.x,ptScroll.y);

	//plot our tiles
	POINT ptPlot;
	POINT ptMap;
	for(ptMap.y=0;ptMap.y<MAPHEIGHT;ptMap.y++)
	{
		for(ptMap.x=0;ptMap.x<MAPWIDTH;ptMap.x++)
		{
			//plot the tile
			ptPlot=TilePlotter.PlotTile(ptMap);

			//convert from world to screen
			ptPlot=Scroller.WorldToScreen(ptPlot);

			//put the tile
			tsIso.PutTile(lpddsBack,ptPlot.x,ptPlot.y,iMap[ptMap.x][ptMap.y]);
		}
	}

	//grab the mouse position
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	//map the mouse
	ptCursor=MouseMap.MapMouse(ptMouse);

	//clip the cursor to valid map coordinates
	if(ptCursor.x<0) ptCursor.x=0;
	if(ptCursor.y<0) ptCursor.y=0;
	if(ptCursor.x>(MAPWIDTH-1)) ptCursor.x=MAPWIDTH-1;
	if(ptCursor.y>(MAPHEIGHT-1)) ptCursor.y=MAPHEIGHT-1;

	//plot the cursor
	ptPlot=TilePlotter.PlotTile(ptCursor);
	//convert world to screen
	ptPlot=Scroller.WorldToScreen(ptPlot);
	//put the cursor on screen
	tsCursor.PutTile(lpddsBack,ptPlot.x,ptPlot.y,0);

	//flip to show the back buffer
	lpddsMain->Flip(0,DDFLIP_WAIT);
}

