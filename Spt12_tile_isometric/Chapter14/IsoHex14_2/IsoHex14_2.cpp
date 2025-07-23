/*****************************************************************************
IsoHex14_2.cpp
Ernest S. Pazera
21JUL2000
Start a WIN32 Application Workspace, add in this file
Requires ddraw.lib and dxguid.lib
Needs DDFuncs.h/cpp, GDICanvas.h/cpp, and TileSet.h/cpp
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//INCLUDES
//////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   
#include "TileSet.h"

//////////////////////////////////////////////////////////////////////////////
//DEFINES
//////////////////////////////////////////////////////////////////////////////
//name for our window class
#define WINDOWCLASS "ISOHEX14"
//title of the application
#define WINDOWTITLE "IsoHex 14-2"

//map dimensions
const int MAPWIDTH=20;
const int MAPHEIGHT=20;

//////////////////////////////////////////////////////////////////////////////
//PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
bool Prog_Init();//game data initalizer
void Prog_Loop();//main game loop
void Prog_Done();//game clean up
POINT DiamondMap_TilePlotter(POINT ptMap,int iTileWidth,int iTileHeight);
void SetUpSpaces();//sets up spaces
void SetUpMap();
void DrawMap();

//////////////////////////////////////////////////////////////////////////////
//GLOBALS
//////////////////////////////////////////////////////////////////////////////
HINSTANCE hInstMain=NULL;//main application handle
HWND hWndMain=NULL;//handle to our main window
LPDIRECTDRAW7 lpdd=NULL;//directdraw
LPDIRECTDRAWSURFACE7 lpddsMain=NULL;//primary surface
LPDIRECTDRAWSURFACE7 lpddsBack=NULL;//back buffer
LPDIRECTDRAWCLIPPER lpddClipper=NULL;//clipper
CTileSet tsIso;//tileset
int iTileMap[MAPWIDTH][MAPHEIGHT];

//spaces
RECT rcWorldSpace;//worldspace
RECT rcScreenSpace;//screen space(also, view space)
RECT rcAnchorSpace;//anchor space
POINT ptScreenAnchor;//screen anchor


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
			//if escape hit, destroy window
			if(wParam==VK_ESCAPE) DestroyWindow(hWndMain);
			//handled, so return 0
			return(0);
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
	//set up ddraw
	lpdd=LPDD_Create(hWndMain,DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);
	lpdd->SetDisplayMode(640,480,16,0,0);

	//set up primary surface
	lpddsMain=LPDDS_CreatePrimary(lpdd,1);

	//set up back buffer
	lpddsBack=LPDDS_GetSecondary(lpddsMain);

	//create clipper
	lpdd->CreateClipper(0,&lpddClipper,NULL);
	lpddClipper->SetHWnd(0,hWndMain);
	lpddsBack->SetClipper(lpddClipper);

	//load in tileset
	tsIso.Load(lpdd,"IsoHex14_2.bmp");

	//set up spaces
	SetUpSpaces();

	//set up the tilemap
	SetUpMap();

	return(true);//return success
}

//////////////////////////////////////////////////////////////////////////////
//CLEANUP
//////////////////////////////////////////////////////////////////////////////
void Prog_Done()
{
	//clean up clipper
	LPDDCLIP_Release(&lpddClipper);
	//clean up primary surface
	LPDDS_Release(&lpddsMain);
	//clean up ddraw
	LPDD_Release(&lpdd);
}

//////////////////////////////////////////////////////////////////////////////
//MAIN GAME LOOP
//////////////////////////////////////////////////////////////////////////////
void Prog_Loop()
{
	//check for keys, and adjust screen anchor
	//up
	if(GetAsyncKeyState(VK_UP)<0)
	{
		if(ptScreenAnchor.y>rcAnchorSpace.top) ptScreenAnchor.y--;
	}
	//down
	if(GetAsyncKeyState(VK_DOWN)<0)
	{
		if(ptScreenAnchor.y<rcAnchorSpace.bottom) ptScreenAnchor.y++;
	}
	//right
	if(GetAsyncKeyState(VK_RIGHT)<0)
	{
		if(ptScreenAnchor.x<rcAnchorSpace.right) ptScreenAnchor.x++;
	}
	//left
	if(GetAsyncKeyState(VK_LEFT)<0)
	{
		if(ptScreenAnchor.x>rcAnchorSpace.left) ptScreenAnchor.x--;
	}

	//clear out back buffer
	DDBLTFX ddbltfx;
	DDBLTFX_ColorFill(&ddbltfx,0);
	lpddsBack->Blt(NULL,NULL,NULL,DDBLT_WAIT | DDBLT_COLORFILL,&ddbltfx);

	//show the board
	DrawMap();
	//flip
	lpddsMain->Flip(NULL,DDFLIP_WAIT);
}

POINT DiamondMap_TilePlotter(POINT ptMap,int iTileWidth,int iTileHeight)
{
	POINT ptReturn;
	//calculate pixel position for the map position given
	ptReturn.x=(ptMap.x-ptMap.y)*iTileWidth/2;
	ptReturn.y=(ptMap.x+ptMap.y)*iTileHeight/2;
	//return calculate point
	return(ptReturn);
}

void SetUpMap()
{
	//randomly set up the map
	for(int x=0;x<MAPWIDTH;x++)
	{
		for(int y=0;y<MAPHEIGHT;y++)
		{
			iTileMap[x][y]=rand()%(tsIso.GetTileCount());
		}
	}
}

void DrawMap()
{
	POINT ptTile;//tile pixel coordinate
	POINT ptMap;//map coordinate
	//get tile width and height
	int iTileWidth=tsIso.GetTileList()[0].rcSrc.right-tsIso.GetTileList()[0].rcSrc.left;
	int iTileHeight=tsIso.GetTileList()[0].rcSrc.bottom-tsIso.GetTileList()[0].rcSrc.top;
	//the y loop is outside, because we must blit in horizontal rows
	for(int y=0;y<MAPHEIGHT;y++)
	{
		for(int x=0;x<MAPWIDTH;x++)
		{
			//get pixel coordinate for map position
			ptMap.x=x;
			ptMap.y=y;
			ptTile=DiamondMap_TilePlotter(ptMap,iTileWidth,iTileHeight);
			//plot the tile(adjust for anchor)
			tsIso.PutTile(lpddsBack,ptTile.x-ptScreenAnchor.x,ptTile.y-ptScreenAnchor.y,iTileMap[x][y]);
		}
	}
}

void SetUpSpaces()
{
	//set up screen space
	SetRect(&rcScreenSpace,0,0,640,480);
	
	//get a few metrics from the tileset
	int iTileWidth=tsIso.GetTileList()[0].rcDstExt.right-tsIso.GetTileList()[0].rcDstExt.left;
	int iTileHeight=tsIso.GetTileList()[0].rcDstExt.bottom-tsIso.GetTileList()[0].rcDstExt.top;

	//grab tile rectangle from tileset
	RECT rcTile1;
	RECT rcTile2;
	RECT rcTile3;
	RECT rcTile4;
	RECT rcWS1;
	RECT rcWS2;
	POINT ptPlot;
	POINT ptMap;

	//grab tiles from extents
	CopyRect(&rcTile1,&tsIso.GetTileList()[0].rcDstExt);
	CopyRect(&rcTile2,&tsIso.GetTileList()[0].rcDstExt);
	CopyRect(&rcTile3,&tsIso.GetTileList()[0].rcDstExt);
	CopyRect(&rcTile4,&tsIso.GetTileList()[0].rcDstExt);

	//move first tile to top position
	ptMap.x=0;
	ptMap.y=0;
	ptPlot=DiamondMap_TilePlotter(ptMap,iTileWidth,iTileHeight);
	OffsetRect(&rcTile3,ptPlot.x,ptPlot.y);

	//move first tile to bottom position
	ptMap.x=MAPWIDTH-1;
	ptMap.y=MAPHEIGHT-1;
	ptPlot=DiamondMap_TilePlotter(ptMap,iTileWidth,iTileHeight);
	OffsetRect(&rcTile2,ptPlot.x,ptPlot.y);

	//move first tile to left position
	ptMap.x=0;
	ptMap.y=MAPHEIGHT-1;
	ptPlot=DiamondMap_TilePlotter(ptMap,iTileWidth,iTileHeight);
	OffsetRect(&rcTile4,ptPlot.x,ptPlot.y);

	//move first tile to right position
	ptMap.x=MAPWIDTH-1;
	ptMap.y=0;
	ptPlot=DiamondMap_TilePlotter(ptMap,iTileWidth,iTileHeight);
	OffsetRect(&rcTile3,ptPlot.x,ptPlot.y);

	//combine these four tiles into world space
	UnionRect(&rcWS1,&rcTile1,&rcTile2);
	UnionRect(&rcWS2,&rcTile3,&rcTile4);
	UnionRect(&rcWorldSpace,&rcWS1,&rcWS2);

	//copy worldspace to anchor space
	CopyRect(&rcAnchorSpace,&rcWorldSpace);

	//subtract out screenspace
	//adjust right edge
	rcAnchorSpace.right-=(rcScreenSpace.right-rcScreenSpace.left);
	//make sure right not less than left
	if(rcAnchorSpace.right<rcAnchorSpace.left) rcAnchorSpace.right=rcAnchorSpace.left;
	//adjust bottom edge
	rcAnchorSpace.bottom-=(rcScreenSpace.bottom-rcScreenSpace.top);
	//make sure bottom not less than top
	if(rcAnchorSpace.bottom<rcAnchorSpace.top) rcAnchorSpace.bottom=rcAnchorSpace.top;

	//initialize screen anchor
	ptScreenAnchor.x=0;
	ptScreenAnchor.y=0;
}
