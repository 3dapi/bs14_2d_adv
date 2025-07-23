/*****************************************************************************
IsoHex13_5.cpp
Ernest S. Pazera
19JUL2000
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
#define WINDOWCLASS "ISOHEX13"
//title of the application
#define WINDOWTITLE "IsoHex 13-5"

//map dimensions
const int MAPWIDTH=20;
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
POINT StagMap_TilePlotter(POINT ptMap,int iTileWidth,int iTileHeight);
POINT StagMap_TileWalker(POINT ptStart,IsoDirection Dir);
POINT StagMap_MouseMapper(POINT ptMouse,CMouseMap* pmm);
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
			ptCursor=StagMap_MouseMapper(ptMouse,&mmMouseMap);

			//clip cursor to tilemap
			if(ptCursor.x<0) ptCursor.x=0;
			if(ptCursor.y<0) ptCursor.y=0;
			//if(ptCursor.x>MAPWIDTH-1) ptCursor.x=MAPWIDTH-1;(eliminated)
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
	tsIso.Load(lpdd,"IsoHex13_5-1.bmp");
	tsCursor.Load(lpdd,"IsoHex13_5-2.bmp");
	//load in mousemap
	LoadMouseMap(&mmMouseMap,"IsoHex13_5-3.bmp");
	//set up mousemap reference point
	POINT ptMap;
	ptMap.x=0;
	ptMap.y=0;
	mmMouseMap.ptRef=StagMap_TilePlotter(ptMap,mmMouseMap.ptSize.x,mmMouseMap.ptSize.y);
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

POINT StagMap_TilePlotter(POINT ptMap,int iTileWidth,int iTileHeight)
{
	POINT ptReturn;
	//calculate pixel position for the map position given
	ptReturn.x=ptMap.x*iTileWidth+(ptMap.y & 1)*iTileWidth/2;
	ptReturn.y=ptMap.y*iTileHeight/2;
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
		for(int x=0;x<MAPWIDTH*2;x++)
		{
			//get pixel coordinate for map position
			ptMap.x=x;
			ptMap.y=y;
			ptTile=StagMap_TilePlotter(ptMap,mmMouseMap.ptSize.x,mmMouseMap.ptSize.y);
			//plot the tile(adjust for anchor)
			tsIso.PutTile(lpddsBack,ptTile.x-ptScreenAnchor.x,ptTile.y-ptScreenAnchor.y,iTileMap[x%MAPWIDTH][y]);
		}
	}
}

void SetUpSpaces()
{
	//set up screen space
	SetRect(&rcScreenSpace,0,0,640,480);
	
	//grab tile rectangle from tileset
	RECT rcTile1;
	RECT rcTile2;
	POINT ptPlot;
	POINT ptMap;

	//grab tiles from extents
	CopyRect(&rcTile1,&tsIso.GetTileList()[0].rcDstExt);
	CopyRect(&rcTile2,&tsIso.GetTileList()[0].rcDstExt);

	//move first tile to upper left position
	ptMap.x=0;
	ptMap.y=0;
	ptPlot=StagMap_TilePlotter(ptMap,mmMouseMap.ptSize.x,mmMouseMap.ptSize.y);
	OffsetRect(&rcTile1,ptPlot.x,ptPlot.y);

	//move first tile to lower right position
	ptMap.x=MAPWIDTH-1;
	ptMap.y=MAPHEIGHT-1;
	ptPlot=StagMap_TilePlotter(ptMap,mmMouseMap.ptSize.x,mmMouseMap.ptSize.y);
	OffsetRect(&rcTile2,ptPlot.x,ptPlot.y);

	//combine these two tiles into world space
	UnionRect(&rcWorldSpace,&rcTile1,&rcTile2);

	//copy worldspace to anchor space
	CopyRect(&rcAnchorSpace,&rcWorldSpace);

	//subtract out screenspace
	//adjust right edge(removed)
	//make sure right not less than left
	if(rcAnchorSpace.right<rcAnchorSpace.left) rcAnchorSpace.right=rcAnchorSpace.left;
	//adjust bottom edge
	rcAnchorSpace.bottom-=(rcScreenSpace.bottom-rcScreenSpace.top);
	//make sure bottom not less than top
	if(rcAnchorSpace.bottom<rcAnchorSpace.top) rcAnchorSpace.bottom=rcAnchorSpace.top;

	//adjust edges of anchorspace for "no jaggies"
	//add 1/2 height to top
	rcAnchorSpace.top+=(mmMouseMap.ptSize.y/2);
	//subtract 1/2 height from bottom
	rcAnchorSpace.bottom-=(mmMouseMap.ptSize.y/2);
	//add 1/2 width to left
	rcAnchorSpace.left+=(mmMouseMap.ptSize.x/2);
	//add 1/2 width from right(changed from 13_4
	//rcAnchorSpace.right+=(mmMouseMap.ptSize.x/2);

	//initialize screen anchor
	ptScreenAnchor.x=rcAnchorSpace.left;
	ptScreenAnchor.y=rcAnchorSpace.top;

	//initialize cursor
	ptCursor.x=0;
	ptCursor.y=0;
}

POINT StagMap_TileWalker(POINT ptStart, IsoDirection Dir)
{
	//depending on direction, move the point
	switch(Dir)
	{
	case ISO_NORTH:
		{
			ptStart.y-=2;
		}break;
	case ISO_NORTHEAST:
		{
			ptStart.x+=(ptStart.y&1);
			ptStart.y--;
		}break;
	case ISO_EAST:
		{
			ptStart.x++;
		}break;
	case ISO_SOUTHEAST:
		{
			ptStart.x+=(ptStart.y&1);
			ptStart.y++;
		}break;
	case ISO_SOUTH:
		{
			ptStart.y+=2;
		}break;
	case ISO_SOUTHWEST:
		{
			ptStart.x+=((ptStart.y&1)-1);
			ptStart.y++;
		}break;
	case ISO_WEST:
		{
			ptStart.x--;
		}break;
	case ISO_NORTHWEST:
		{
			ptStart.x+=((ptStart.y&1)-1);
			ptStart.y--;
		}break;
	}
	//return the point
	return(ptStart);
}

void ShowIsoCursor()
{
	//copy cursor position
	POINT ptMap=ptCursor;

	//plot cursor position
	POINT ptPlot=StagMap_TilePlotter(ptMap,mmMouseMap.ptSize.x,mmMouseMap.ptSize.y);
	
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
	//wrap to left
	if(ptScreenAnchor.x<rcAnchorSpace.left) ptScreenAnchor.x+=(rcAnchorSpace.right-rcAnchorSpace.left);
	//clip to top
	if(ptScreenAnchor.y<rcAnchorSpace.top) ptScreenAnchor.y=rcAnchorSpace.top;
	//wrap to right
	if(ptScreenAnchor.x>=rcAnchorSpace.right) ptScreenAnchor.x-=(rcAnchorSpace.right-rcAnchorSpace.left);;
	//clip to bottom
	if(ptScreenAnchor.y>=rcAnchorSpace.bottom) ptScreenAnchor.y=rcAnchorSpace.bottom-1;
}

POINT StagMap_MouseMapper(POINT ptMouse,CMouseMap* pmm)
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
		ptMap=StagMap_TileWalker(ptMap,ISO_NORTH);
	}
	//move south
	while(ptMouseMapCoarse.y>0)
	{
		ptMouseMapCoarse.y--;
		ptMap=StagMap_TileWalker(ptMap,ISO_SOUTH);
	}
	//move east
	while(ptMouseMapCoarse.x>0)
	{
		ptMouseMapCoarse.x--;
		ptMap=StagMap_TileWalker(ptMap,ISO_EAST);
	}
	//move west
	while(ptMouseMapCoarse.x<0)
	{
		ptMouseMapCoarse.x++;
		ptMap=StagMap_TileWalker(ptMap,ISO_WEST);
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
			ptMap=StagMap_TileWalker(ptMap,ISO_NORTHWEST);
		}break;
	case MM_NE:
		{
			//move northeast
			ptMap=StagMap_TileWalker(ptMap,ISO_NORTHEAST);
		}break;
	case MM_SW:
		{
			//move southwest
			ptMap=StagMap_TileWalker(ptMap,ISO_SOUTHWEST);
		}break;
	case MM_SE:
		{
			//move southeast
			ptMap=StagMap_TileWalker(ptMap,ISO_SOUTHEAST);
		}break;
	}

	//return map coordinate
	return(ptMap);
}
