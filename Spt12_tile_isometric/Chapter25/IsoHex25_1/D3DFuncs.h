/*****************************************************************************
D3DFuncs.h
Ernest S. Pazera
17NOV2000
Requires the use of d3d8.lib, ddraw.lib and dxguid.lib
Also requires GDICanvas.h/cpp and DDFuncs.h/cpp
*****************************************************************************/
//include guard
#ifndef D3DFUNCS_H
#define D3DFUNCS_H

#include "DDFuncs.h"
#include "d3d.h"

//create the IDirect3D7 interface
LPDIRECT3D7 LPD3D_Create(LPDIRECTDRAW7 lpdd);

//clean up an IDirect3D7
void LPD3D_Release(LPDIRECT3D7* lpd3d);

//create the Device
LPDIRECT3DDEVICE7 LPD3DDEV_Create(LPDIRECT3D7 lpd3d,LPDIRECTDRAWSURFACE7 lpdds);

//set up the viewport
void LPD3DDEV_SetViewport(LPDIRECT3DDEVICE7 lpd3ddev,DWORD x,DWORD y,DWORD width,DWORD height);

//clear out the viewport
void LPD3DDEV_Clear(LPDIRECT3DDEVICE7 lpd3ddev,D3DCOLOR color);

//draw triangle list
void LPD3DDEV_DrawTriangleList(LPDIRECT3DDEVICE7 lpd3ddev,D3DTLVERTEX* pvertices,DWORD dwvertexcount);

//draw triangle strip
void LPD3DDEV_DrawTriangleStrip(LPDIRECT3DDEVICE7 lpd3ddev,D3DTLVERTEX* pvertices,DWORD dwvertexcount);

//clean up a device
void LPD3DDEV_Release(LPDIRECT3DDEVICE7* lpd3ddev);

//primary surface as a 3d rendering target
LPDIRECTDRAWSURFACE7 LPDDS_CreatePrimary3D(LPDIRECTDRAW7 lpdd,DWORD dwBackBufferCount);

//back buffer as a 3d rendering target
LPDIRECTDRAWSURFACE7 LPDDS_GetSecondary3D(LPDIRECTDRAWSURFACE7 lpdds);

//create a texture
LPDIRECTDRAWSURFACE7 LPDDS_CreateTexture(LPDIRECTDRAW7 lpdd,DWORD dwWidth,DWORD dwHeight);

//create a texture with a particular pixel format
LPDIRECTDRAWSURFACE7 LPDDS_CreateTexturePixelFormat(LPDIRECTDRAW7 lpdd,DWORD dwWidth,DWORD dwHeight,LPDDPIXELFORMAT lpddpf);

//set vertex data
void VERTEX_Set(D3DTLVERTEX* pVert,D3DVALUE x,D3DVALUE y, D3DCOLOR color, D3DVALUE tu, D3DVALUE tv);

#endif