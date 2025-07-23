/*****************************************************************************
IsoHex20_1.cpp
Ernest S. Pazera
19SEP2000
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
#define WINDOWTITLE "IsoHex 20-1"

//////////////////////////////////////////////////////////////////////////////
//PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
bool Prog_Init();//game data initalizer
void Prog_Loop();//main game loop
void Prog_Done();//game clean up
void ShowMap(HDC hdc);//show the map

//////////////////////////////////////////////////////////////////////////////
//GLOBALS
//////////////////////////////////////////////////////////////////////////////
HINSTANCE hInstMain=NULL;//main application handle
HWND hWndMain=NULL;//handle to our main window

//gdi canvases
CGDICanvas gdicSquare;//square tile
CGDICanvas gdicIso;//isometric tile
CGDICanvas gdicIsoMask;//isometric mask

//which map to show
bool bShowIso=true;//true if iso, false if square

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
			//if '1', show iso
			if(wParam=='1')
			{
				bShowIso=true;
				InvalidateRect(hWndMain,NULL,TRUE);
			}
			//if '2', show square
			if(wParam=='2')
			{
				bShowIso=false;
				InvalidateRect(hWndMain,NULL,TRUE);
			}
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
	hWndMain=CreateWindowEx(0,WINDOWCLASS,WINDOWTITLE, WS_BORDER | WS_SYSMENU | WS_VISIBLE,0,0,320,240,NULL,NULL,hInstMain,NULL);

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
	//grab hdc from main window
	HDC hdc=GetDC(hWndMain);

	//load the square tile
	gdicSquare.Load(hdc,"IsoHex20_1.bmp");

	//create the iso tile
	gdicIso.CreateBlank(hdc,64,32);
	gdicIsoMask.CreateBlank(hdc,64,32);

	//scan convert the square image into the iso image
	int tx,ty;//texture coordinates
	int x,y;//pixel coordinates
	int tempx;//for plotting a row of pixels
	COLORREF crColor;

	//clear out iso image
	for(x=0;x<64;x++)
	{
		for(y=0;y<32;y++)
		{
			SetPixelV(gdicIso,x,y,RGB(0,0,0));//iso is black
			SetPixelV(gdicIsoMask,x,y,RGB(255,255,255));//mask is black
		}
	}


	//loop through x texture coords
	for(tx=0;tx<16;tx++)
	{
		//loop through y texture coords
		for(ty=0;ty<16;ty++)
		{
			//grab the color from the texture
			crColor=GetPixel(gdicSquare,tx,ty);
			//calculate x and y
			x=30+tx*2-ty*2;
			y=tx+ty;
			//loop through four pixel positions
			for(tempx=x;tempx<(x+4);tempx++)
			{
				SetPixelV(gdicIso,tempx,y,crColor);//set color on iso picture
				SetPixelV(gdicIsoMask,tempx,y,0);//set to black on mask
			}
		}
	}

	//show the map
	ShowMap(hdc);

	//release the hdc back to the main window
	ReleaseDC(hWndMain,hdc);

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

void ShowMap(HDC hdc)
{
	//plot iso tiles using staggered calculations
	for(int y=0;y<20;y++)
	{
		for(int x=0;x<20;x++)
		{
			//if iso
			if(bShowIso)
			{
				//put mask
				BitBlt(hdc,x*64+(y&1)*32-32,y*16-16,64,32,gdicIsoMask,0,0,SRCAND);
				//put image
				BitBlt(hdc,x*64+(y&1)*32-32,y*16-16,64,32,gdicIso,0,0,SRCINVERT);
			}
			else
			{
				//square
				BitBlt(hdc,x*16,y*16,16,16,gdicSquare,0,0,SRCCOPY);
			}
		}
	}
}
