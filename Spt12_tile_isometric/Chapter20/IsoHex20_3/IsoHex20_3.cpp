/*****************************************************************************
IsoHex20_3.cpp
Ernest S. Pazera
20OCT2000
Start a WIN32 Application Workspace, add in this file
Requires GDICanvas.h and GDICanvas.cpp
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//INCLUDES
//////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   
#include "GDICanvas.h"

//////////////////////////////////////////////////////////////////////////////
//DEFINES
//////////////////////////////////////////////////////////////////////////////
//name for our window class
#define WINDOWCLASS "ISOHEX20"
//title of the application
#define WINDOWTITLE "IsoHex 20-3"

//////////////////////////////////////////////////////////////////////////////
//PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
bool Prog_Init();//game data initalizer
void Prog_Loop();//main game loop
void Prog_Done();//game clean up
void ShowMap(HDC hdc);//show sample map

//////////////////////////////////////////////////////////////////////////////
//GLOBALS
//////////////////////////////////////////////////////////////////////////////
HINSTANCE hInstMain=NULL;//main application handle
HWND hWndMain=NULL;//handle to our main window

//gdi canvases
CGDICanvas gdicTexture;//texture image
CGDICanvas gdicIsoShape;//shape of the iso image
CGDICanvas gdicSource;//tiled texture goes here
CGDICanvas gdicTiles;//tiles go here
//columns and rows
int iColumns;
int iRows;

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
			return(0);//handled, so return 0
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
			ShowMap(hdc);

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
	hWndMain=CreateWindowEx(0,WINDOWCLASS,WINDOWTITLE, WS_BORDER | WS_SYSMENU | WS_VISIBLE,0,0,640,480,NULL,NULL,hInstMain,NULL);

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
	//load texture canvas
	gdicTexture.Load(NULL,"texture.bmp");

	//load iso shape
	gdicIsoShape.Load(NULL,"IsoShape.bmp");

	//grab dc from main window
	HDC hdc=GetDC(hWndMain);

	//calculate rows and columns of tiles
	iColumns=gdicTexture.GetWidth()/gdicIsoShape.GetWidth();
	iRows=2*gdicTexture.GetHeight()/gdicIsoShape.GetHeight();

	//create tiled texture canvas
	gdicSource.CreateBlank(hdc,gdicTexture.GetWidth()*2,gdicTexture.GetHeight()*2);

	//create tile canvas
	gdicTiles.CreateBlank(hdc,gdicIsoShape.GetWidth()*iColumns,gdicIsoShape.GetHeight()*iRows);

	//restore dc to main window
	ReleaseDC(hWndMain,hdc);

	//create tiled texture image
	//upper left
	BitBlt(gdicSource,0,0,gdicTexture.GetWidth(),gdicTexture.GetWidth(),gdicTexture,0,0,SRCCOPY);
	//upper right
	BitBlt(gdicSource,gdicTexture.GetWidth(),0,gdicTexture.GetWidth(),gdicTexture.GetWidth(),gdicTexture,0,0,SRCCOPY);
	//lower left
	BitBlt(gdicSource,0,gdicTexture.GetHeight(),gdicTexture.GetWidth(),gdicTexture.GetWidth(),gdicTexture,0,0,SRCCOPY);
	//lower right
	BitBlt(gdicSource,gdicTexture.GetWidth(),gdicTexture.GetHeight(),gdicTexture.GetWidth(),gdicTexture.GetWidth(),gdicTexture,0,0,SRCCOPY);

	//rip out the tiles
	int x;
	int y;
	int plotx;
	int ploty;

	//loop through columns
	for(x=0;x<iColumns;x++)
	{
		//loop through rows
		for(y=0;y<iRows;y++)
		{
			//determine plotx and ploty
			plotx=x*gdicIsoShape.GetWidth()+(y&1)*gdicIsoShape.GetWidth()/2;
			ploty=y*gdicIsoShape.GetHeight()/2;

			//bring plotx and ploty within appropriate range
			plotx%=gdicTexture.GetWidth();
			ploty%=gdicTexture.GetHeight();

			//check for negatives
			if(plotx<0) plotx+=gdicTexture.GetWidth();
			if(ploty<0) ploty+=gdicTexture.GetHeight();

			//grab this part of the source surface, and place it on the tile surface
			BitBlt(gdicTiles,x*gdicIsoShape.GetWidth(),y*gdicIsoShape.GetHeight(),gdicIsoShape.GetWidth(),gdicIsoShape.GetHeight(),gdicSource,plotx,ploty,SRCCOPY);

			//modulate with the iso shape
			BitBlt(gdicTiles,x*gdicIsoShape.GetWidth(),y*gdicIsoShape.GetHeight(),gdicIsoShape.GetWidth(),gdicIsoShape.GetHeight(),gdicIsoShape,0,0,SRCAND);
		}
	}

	return(true);//return success
}

//////////////////////////////////////////////////////////////////////////////
//CLEANUP
//////////////////////////////////////////////////////////////////////////////
void Prog_Done()
{
}

//////////////////////////////////////////////////////////////////////////////
//MAIN GAME LOOP
//////////////////////////////////////////////////////////////////////////////
void Prog_Loop()
{
}

void ShowMap(HDC hdc)//show sample map
{
	//vars
	int x;
	int y;
	int tilex;
	int tiley;
	int plotx;
	int ploty;

	//clear out dc
	RECT rcFill;
	GetClientRect(hWndMain,&rcFill);

	//fill rect
	FillRect(hdc,&rcFill,(HBRUSH)GetStockObject(BLACK_BRUSH));

	//loops
	for(x=0;x<10;x++)
	{
		for(y=0;y<20;y++)
		{
			//calculate plotted positions
			plotx=x*gdicIsoShape.GetWidth()+(y&1)*gdicIsoShape.GetWidth()/2;
			ploty=y*gdicIsoShape.GetHeight()/2;
			//calculate which tile to use
			tilex=(x%iColumns)*gdicIsoShape.GetWidth();
			tiley=(y%iRows)*gdicIsoShape.GetHeight();
			//blit the tile
			BitBlt(hdc,plotx,ploty,gdicIsoShape.GetWidth(),gdicIsoShape.GetHeight(),gdicTiles,tilex,tiley,SRCPAINT);
		}
	}
}
