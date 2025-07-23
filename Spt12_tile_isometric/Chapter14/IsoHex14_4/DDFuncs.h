/*****************************************************************************
DDFuncs.h
Ernest S. Pazera
21MAY2000
Requires the use of ddraw.lib and dxguid.lib
Also requires GDICanvas.h/cpp
*****************************************************************************/
//include guard
#ifndef DDFUNCS_H
#define DDFUNCS_H

#include <windows.h>
#include "ddraw.h"
#include "GDICanvas.h"

///////////////////////////////////////////////////////////////////////////////
//DDSURFACEDESC2 functions
///////////////////////////////////////////////////////////////////////////////

//clean out a DDSD
void DDSD_Clear(DDSURFACEDESC2* pddsd);

//set up a DDSD for a primary surface, no back buffer
void DDSD_PrimarySurface(DDSURFACEDESC2* pddsd);

//set up a DDSD for a primary surface, with any number of backbuffers
void DDSD_PrimarySurfaceWBackBuffer(DDSURFACEDESC2* pddsd, DWORD dwBackBufferCount);

//set up a DDSD for an offscreen surface, specify width and height
void DDSD_OffscreenSurface(DDSURFACEDESC2* pddsd,DWORD dwWidth, DWORD dwHeight);

///////////////////////////////////////////////////////////////////////////////
//DDSCAPS2 functions
///////////////////////////////////////////////////////////////////////////////

//clean out ddscaps
void DDSCAPS_Clear(DDSCAPS2* pddscaps);

//set up ddscaps for back buffer
void DDSCAPS_BackBuffer(DDSCAPS2* pddscaps);

///////////////////////////////////////////////////////////////////////////////
//DDBLTFX functions
///////////////////////////////////////////////////////////////////////////////

//clean out a ddbltfx structure
void DDBLTFX_Clear(DDBLTFX* pddbltfx);

//set up a ddbltfx for a color fill
void DDBLTFX_ColorFill(DDBLTFX* pddbltfx,DWORD dwColor);

///////////////////////////////////////////////////////////////////////////////
//DDPIXELFORMAT functions
///////////////////////////////////////////////////////////////////////////////

//clean a ddpixelformat structure
void DDPF_Clear(DDPIXELFORMAT* pddpf);

///////////////////////////////////////////////////////////////////////////////
//Direct Draw Initialization/Cleanup functions
///////////////////////////////////////////////////////////////////////////////

//create direct draw interface
LPDIRECTDRAW7 LPDD_Create(HWND hWnd,DWORD dwCoopLevel);

//safe release of direct draw
void LPDD_Release(LPDIRECTDRAW7* lplpdd);

///////////////////////////////////////////////////////////////////////////////
//Surface Creation/Cleanup functions
///////////////////////////////////////////////////////////////////////////////

//primary surface
LPDIRECTDRAWSURFACE7 LPDDS_CreatePrimary(LPDIRECTDRAW7 lpdd,DWORD dwBackBufferCount);

//back buffer
LPDIRECTDRAWSURFACE7 LPDDS_GetSecondary(LPDIRECTDRAWSURFACE7 lpdds);

//offscreen
LPDIRECTDRAWSURFACE7 LPDDS_CreateOffscreen(LPDIRECTDRAW7 lpdd,DWORD dwWidth,DWORD dwHeight);

//offscreen with a bitmap from a file
LPDIRECTDRAWSURFACE7 LPDDS_LoadFromFile(LPDIRECTDRAW7 lpdd,LPCTSTR lpszFileName);

//reloading a bitmap from file to a surface
void LPDDS_ReloadFromFile(LPDIRECTDRAWSURFACE7 lpdds, LPCTSTR lpszFileName);

//safe release
void LPDDS_Release(LPDIRECTDRAWSURFACE7* lplpdds);

///////////////////////////////////////////////////////////////////////////////
//Color Key Setting functions
///////////////////////////////////////////////////////////////////////////////

void LPDDS_SetSrcColorKey(LPDIRECTDRAWSURFACE7 lpdds,DWORD dwColor);

///////////////////////////////////////////////////////////////////////////////
//Color Conversion Functions
///////////////////////////////////////////////////////////////////////////////

//from dd pixel to colorref
COLORREF ConvertDDColor(DWORD dwColor, DDPIXELFORMAT* pddpf);

//from colorref to dd pixel
DWORD ConvertColorRef(COLORREF crColor, DDPIXELFORMAT* pddpf);

///////////////////////////////////////////////////////////////////////////////
//Clipper utilities
///////////////////////////////////////////////////////////////////////////////

//create a IDirectDraw7 clipper, and fill it with the information from a region
LPDIRECTDRAWCLIPPER LPDDCLIP_Create(LPDIRECTDRAW7 lpdd, HRGN hrgn);

//safe release of an IDirectDrawClipper
void LPDDCLIP_Release(LPDIRECTDRAWCLIPPER* lplpddclip);

//include guard
#endif//ndef DDFUNCS_H
