/*****************************************************************************
IsoHex24_1.cpp
Ernest S. Pazera
17NOV2000
Start a WIN32 Application Workspace, add in this file
Needs ddraw.lib, d3d8.lib and dxguid.lib
Needs GDICanvas.h/cpp
Needs DDFuncs.h/cpp
Needs D3DFuncs.h/cpp
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//INCLUDES
//////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   
#include <math.h>//sin and cos
#include "GDICanvas.h"
#include "ddraw.h"
#include "DDFuncs.h"
#include "d3d.h"
#include "d3dfuncs.h"

//////////////////////////////////////////////////////////////////////////////
//DEFINES
//////////////////////////////////////////////////////////////////////////////
//name for our window class
#define WINDOWCLASS "ISOHEX25"
//title of the application
#define WINDOWTITLE "IsoHex 25-1"

//screen attributes
const DWORD SCREENWIDTH=640;
const DWORD SCREENHEIGHT=480;
const DWORD SCREENBPP=16;

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

//IDirectDraw7 Pointer
LPDIRECTDRAW7 lpdd=NULL;

//surfaces
LPDIRECTDRAWSURFACE7 lpddsPrime=NULL;
LPDIRECTDRAWSURFACE7 lpddsBack=NULL;

//IDirect3D7
LPDIRECT3D7 lpd3d=NULL;

//IDirect3DDevice
LPDIRECT3DDEVICE7 lpd3ddev=NULL;

//vertices
D3DTLVERTEX vert[3];//three vertices

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
			//check for escape key
			if(wParam==VK_ESCAPE)
			{
				DestroyWindow(hWndMain);
			}

			return(0);//handled message
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
	lpdd=LPDD_Create(hWndMain,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);

	//set the display mode
	lpdd->SetDisplayMode(SCREENWIDTH,SCREENHEIGHT,SCREENBPP,0,0);

	//create primary surface
	lpddsPrime=LPDDS_CreatePrimary3D(lpdd,1);

	//create back buffer
	lpddsBack=LPDDS_GetSecondary3D(lpddsPrime);
	
	//get the idirect3d pointer
	lpd3d=LPD3D_Create(lpdd);

	//create the idirect3ddevice(hack method)
	lpd3ddev=LPD3DDEV_Create(lpd3d,lpddsBack);

	//set up viewport
	LPD3DDEV_SetViewport(lpd3ddev,0,0,SCREENWIDTH,SCREENHEIGHT);

	//////////////////////////////////
	//extra setup goes here
	//////////////////////////////////

	return(true);//return success
}

//////////////////////////////////////////////////////////////////////////////
//CLEANUP
//////////////////////////////////////////////////////////////////////////////
void Prog_Done()
{	
	//////////////////////////////////
	//extra cleanup goes here
	//////////////////////////////////

	//release IDirect3DDevice
	LPD3DDEV_Release(&lpd3ddev);

	//release IDirect3D 
	LPD3D_Release(&lpd3d);

	//clean up primary surface(this will clean up the back buffer, also)
	LPDDS_Release(&lpddsPrime);

	//clean up the dd pointer
	LPDD_Release(&lpdd);
}

//////////////////////////////////////////////////////////////////////////////
//MAIN GAME LOOP
//////////////////////////////////////////////////////////////////////////////
void Prog_Loop()
{
	//clear the viewport to black
	lpd3ddev->Clear(0,NULL,D3DCLEAR_TARGET,0,0,0);

	//start the scene
	lpd3ddev->BeginScene();

	//////////////////////////////////
	//rendering code goes here
	//////////////////////////////////

	//end the scene
	lpd3ddev->EndScene();

	//flip 
	lpddsPrime->Flip(NULL,DDFLIP_WAIT);
}

