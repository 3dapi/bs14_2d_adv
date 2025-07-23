/*****************************************************************************
IsoHex14_3.cpp
Ernest S. Pazera
22JUL2000
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
#define WINDOWTITLE "IsoHex 14-4"

//map dimensions
const int MAPWIDTH=40;
const int MAPHEIGHT=40;

enum IsoDirection{
ISO_NORTH=0,
ISO_NORTHEAST=1,
ISO_EAST=2,
ISO_SOUTHEAST=3,
ISO_SOUTH=4,
ISO_SOUTHWEST=5,
ISO_WEST=6,
ISO_NORTHWEST=7
};

//mousemap directions
enum MouseMapDirection{
	MM_CENTER=0,
	MM_NW=1,
	MM_NE=2,
	MM_SW=3,
	MM_SE=4
};

//mousemap structure
struct CMouseMap
{
	POINT ptSize;
	POINT ptRef;
	MouseMapDirection* mmdLookUp;
};

//////////////////////////////////////////////////////////////////////////////
//PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
bool Prog_Init();//game data initalizer
void Prog_Loop();//main game loop
void Prog_Done();//game clean up
POINT DiamondMap_TilePlotter(POINT ptMap,int iTileWidth,int iTileHeight);
POINT DiamondMap_TileWalker(POINT ptStart,IsoDirection Dir);
POINT DiamondMap_MouseMapper(POINT ptMouse,CMouseMap* pmm);
void SetUpSpaces();//sets up spaces
void SetUpMap();
void DrawMap();
void ShowIsoCursor();
void LoadMouseMap(CMouseMap* pmm,LPCTSTR lpszFileName);
void ClipScreenAnchor();

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
CTileSet tsCursor;//cursor
int iTileMap[MAPWIDTH][MAPHEIGHT];
POINT ptCursor;//cursor position

//spaces
RECT rcWorldSpace;//worldspace
RECT rcScreenSpace;//screen space(also, view space)
RECT rcAnchorSpace;//anchor space
POINT ptScreenAnchor;//screen anchor
POINT ptScreenAnchorScroll;//scrolling for the screen anchor

//mouse map
CMouseMap mmMouseMap;


//////////////////////////////////////////////////////////////////////////////
//WINDOWPROC
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TheWindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//which message did we get?
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			//grab mouse coordinate
			POINT ptMouse;
			ptMouse.x=LOWORD(lParam);
			ptMouse.y=HIWORD(lParam);
			//mousemap the mouse coordinates
			ptCursor=DiamondMap_MouseMapper(ptMouse,&mmMouseMap);

			//clip cursor to tilemap
			if(ptCursor.x<0) ptCursor.x=0;
			if(ptCursor.y<0) ptCursor.y=0;
			if(ptCursor.x>MAPWIDTH-1) ptCursor.x=MAPWIDTH-1;
			if(ptCursor.y>MAPHEIGHT-1) ptCursor.y=MAPHEIGHT-1;

			//check for scrolling zones
			ptScreenAnchorScroll.x=0;
			ptScreenAnchorScroll.y=0;
			//top
			if(ptMouse.y<8)
			{
				ptScreenAnchorScroll.y=-(8-ptMouse.y);
			}
			//bottom
			if(ptMouse.y>472)
			{
				ptScreenAnchorScroll.y=(ptMouse.y-472);
			}
			//left
			if(ptMouse.x<8)
			{
				ptScreenAnchorScroll.x=-(8-ptMouse.x);
			}
			//right
			if(ptMouse.x>632)
			{
				ptScreenAnchorScroll.x=(ptMouse.x-632);
			}

			return(0);
		}break;
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
	tsIso.Load(lpdd,"IsoHex14_4-1.bmp");
	tsCursor.Load(lpdd,"IsoHex14_4-2.bmp");
	//load in mousemap
	LoadMouseMap(&mmMouseMap,"IsoHex14_4-3.bmp");
	//set up mousemap reference point
	POINT ptMap;
	ptMap.x=0;
	ptMap.y=0;
	mmMouseMap.ptRef=DiamondMap_TilePlotter(ptMap,mmMouseMap.ptSize.x,mmMouseMap.ptSize.y);
	mmMouseMap.ptRef.x+=tsIso.GetTileList()[0].rcDstExt.left;
	mmMouseMap.ptRef.y+=tsIso.GetTileList()[0].rcDstExt.top;

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
	//deallocate lookup table for mousemap
	delete [] mmMouseMap.mmdLookUp;
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
	//clear out back buffer
	DDBLTFX ddbltfx;
	DDBLTFX_ColorFill(&ddbltfx,0);
	lpddsBack->Blt(NULL,NULL,NULL,DDBLT_WAIT | DDBLT_COLORFILL,&ddbltfx);

	//scroll the map
	ptScreenAnchor.x+=ptScreenAnchorScroll.x;
	ptScreenAnchor.y+=ptScreenAnchorScroll.y;
	ClipScreenAnchor();

	//show the board
	DrawMap();

	//show the cursor
	ShowIsoCursor();

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
	//the y loop is outside, because we must blit in horizontal rows
	for(int y=0;y<MAPHEIGHT;y++)
	{
		for(int x=0;x<MAPWIDTH;x++)
		{
			//get pixel coordinate for map position
			ptMap.x=x;
			ptMap.y=y;
			ptTile=DiamondMap_TilePlotter(ptMap,mmMouseMap.ptSize.x,mmMouseMap.ptSize.y);
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

	//initialize cursor
	ptCursor.x=0;
	ptCursor.y=0;
}

POINT DiamondMap_TileWalker(POINT ptStart, IsoDirection Dir)
{
	switch(Dir)
	{
	case ISO_NORTH:
		{
			ptStart.x--;
			ptStart.y--;
		}break;
	case ISO_NORTHEAST:
		{
			ptStart.y--;
		}break;
	case ISO_EAST:
		{
			ptStart.x++;
			ptStart.y--;
		}break;
	case ISO_SOUTHEAST:
		{
			ptStart.x++;
		}break;
	case ISO_SOUTH:
		{
			ptStart.x++;
			ptStart.y++;
		}break;
	case ISO_SOUTHWEST:
		{
			ptStart.y++;
		}break;
	case ISO_WEST:
		{
			ptStart.x--;
			ptStart.y++;
		}break;
	case ISO_NORTHWEST:
		{
			ptStart.x--;
		}break;
	}
	return(ptStart);
}

void ShowIsoCursor()
{
	//copy cursor position
	POINT ptMap=ptCursor;

	//plot cursor position
	POINT ptPlot=DiamondMap_TilePlotter(ptMap,mmMouseMap.ptSize.x,mmMouseMap.ptSize.y);
	
	//put the cursor image
	tsCursor.PutTile(lpddsBack,ptPlot.x-ptScreenAnchor.x,ptPlot.y-ptScreenAnchor.y,0);
}

void LoadMouseMap(CMouseMap* pmm,LPCTSTR lpszfilename)
{
	//create canvas
	CGDICanvas gdic;
	//load file
	gdic.Load(NULL,lpszfilename);
	//assign width/height
	pmm->ptSize.x=gdic.GetWidth();
	pmm->ptSize.y=gdic.GetHeight();
	//allocate space for the lookup table
	pmm->mmdLookUp= new
		MouseMapDirection[gdic.GetWidth()*gdic.GetHeight()];
	//colorref values for filling lookup
	COLORREF crNW=GetPixel(gdic,0,0);
	COLORREF crNE=GetPixel(gdic,gdic.GetWidth()-1,0);
	COLORREF crSW=GetPixel(gdic,0,gdic.GetHeight()-1);
	COLORREF crSE=GetPixel(gdic,gdic.GetWidth()-1, gdic.GetHeight()-1);
	//test pixel color
	COLORREF crTest;
	//scan convert bitmap into lookup values
	for(int y=0;y<gdic.GetHeight();y++)
	{
		for(int x=0;x<gdic.GetWidth();x++)
		{
			//grab test pixel
			crTest=GetPixel(gdic,x,y);
			//set lookup to default
			pmm->mmdLookUp[x+y*pmm->ptSize.x]=MM_CENTER;
			//check colors
			if(crTest==crNW)
				pmm->mmdLookUp[x+y*pmm->ptSize.x]=MM_NW;
			if(crTest==crNE)
				pmm->mmdLookUp[x+y*pmm->ptSize.x]=MM_NE;
			if(crTest==crSW)
				pmm->mmdLookUp[x+y*pmm->ptSize.x]=MM_SW;
			if(crTest==crSE)
				pmm->mmdLookUp[x+y*pmm->ptSize.x]=MM_SE;
		}
	}
}

void ClipScreenAnchor()
{
	//clip to left
	if(ptScreenAnchor.x<rcAnchorSpace.left) ptScreenAnchor.x=rcAnchorSpace.left;
	//clip to top
	if(ptScreenAnchor.y<rcAnchorSpace.top) ptScreenAnchor.y=rcAnchorSpace.top;
	//clip to right
	if(ptScreenAnchor.x>rcAnchorSpace.right) ptScreenAnchor.x=rcAnchorSpace.right;
	//clip to bottom
	if(ptScreenAnchor.y>rcAnchorSpace.bottom) ptScreenAnchor.y=rcAnchorSpace.bottom;
}

POINT DiamondMap_MouseMapper(POINT ptMouse,CMouseMap* pmm)
{
	//step1:screen to world
	ptMouse.x+=ptScreenAnchor.x;
	ptMouse.y+=ptScreenAnchor.y;

	//step2:relative world coordinates
	ptMouse.x-=pmm->ptRef.x;
	ptMouse.y-=pmm->ptRef.y;

	//step3:mousemap coords
	POINT ptMouseMapCoarse;
	POINT ptMouseMapFine;
	//coarse mousemap coords
	ptMouseMapCoarse.x=ptMouse.x/pmm->ptSize.x;
	ptMouseMapCoarse.y=ptMouse.y/pmm->ptSize.y;
	//fine mousemap coords
	ptMouseMapFine.x=ptMouse.x%pmm->ptSize.x;
	ptMouseMapFine.y=ptMouse.y%pmm->ptSize.y;
	//adjust for negative fine coordinates
	if(ptMouseMapFine.x<0)
	{
		ptMouseMapFine.x+=pmm->ptSize.x;
		ptMouseMapCoarse.x--;
	}
	if(ptMouseMapFine.y<0)
	{
		ptMouseMapFine.y+=pmm->ptSize.y;
		ptMouseMapCoarse.y--;
	}

	//step4:coarse tilewalk
	POINT ptMap;
	ptMap.x=0;
	ptMap.y=0;
	//move north
	while(ptMouseMapCoarse.y<0)
	{
		ptMouseMapCoarse.y++;
		ptMap=DiamondMap_TileWalker(ptMap,ISO_NORTH);
	}
	//move south
	while(ptMouseMapCoarse.y>0)
	{
		ptMouseMapCoarse.y--;
		ptMap=DiamondMap_TileWalker(ptMap,ISO_SOUTH);
	}
	//move east
	while(ptMouseMapCoarse.x>0)
	{
		ptMouseMapCoarse.x--;
		ptMap=DiamondMap_TileWalker(ptMap,ISO_EAST);
	}
	//move west
	while(ptMouseMapCoarse.x<0)
	{
		ptMouseMapCoarse.x++;
		ptMap=DiamondMap_TileWalker(ptMap,ISO_WEST);
	}

	//step5:fine tilewalk
	//get lookup value
	MouseMapDirection mmd=pmm->mmdLookUp[ptMouseMapFine.x+ptMouseMapFine.y*pmm->ptSize.x];
	switch(mmd)
	{
	case MM_CENTER:
		{
			//do nothing
		}break;
	case MM_NW:
		{
			//move northwest
			ptMap=DiamondMap_TileWalker(ptMap,ISO_NORTHWEST);
		}break;
	case MM_NE:
		{
			//move northeast
			ptMap=DiamondMap_TileWalker(ptMap,ISO_NORTHEAST);
		}break;
	case MM_SW:
		{
			//move southwest
			ptMap=DiamondMap_TileWalker(ptMap,ISO_SOUTHWEST);
		}break;
	case MM_SE:
		{
			//move southeast
			ptMap=DiamondMap_TileWalker(ptMap,ISO_SOUTHEAST);
		}break;
	}

	//return map coordinate
	return(ptMap);
}
