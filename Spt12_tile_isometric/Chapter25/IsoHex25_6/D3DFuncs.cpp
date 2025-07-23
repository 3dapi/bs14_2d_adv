#include "d3dfuncs.h"

//create the IDirect3D7 interface
LPDIRECT3D7 LPD3D_Create(LPDIRECTDRAW7 lpdd)
{
	LPDIRECT3D7 lpd3d;
	//get the idirect3d pointer
	lpdd->QueryInterface(IID_IDirect3D7,(void**)&lpd3d);//ICKY COM STUFF!
	//return the pointer
	return(lpd3d);
}

//clean up an IDirect3D7
void LPD3D_Release(LPDIRECT3D7* lpd3d)
{
	//safe release
	if(*lpd3d)
	{
		(*lpd3d)->Release();
		(*lpd3d)=NULL;
	}
}

//create the Device
LPDIRECT3DDEVICE7 LPD3DDEV_Create(LPDIRECT3D7 lpd3d,LPDIRECTDRAWSURFACE7 lpdds)
{
	LPDIRECT3DDEVICE7 lpd3ddev;
	//create the idirect3ddevice(hack method)
	if(FAILED(lpd3d->CreateDevice(IID_IDirect3DTnLHalDevice,lpdds,&lpd3ddev)))//try tnl
		if(FAILED(lpd3d->CreateDevice(IID_IDirect3DHALDevice,lpdds,&lpd3ddev)))//no tnl, try hal
			if(FAILED(lpd3d->CreateDevice(IID_IDirect3DMMXDevice,lpdds,&lpd3ddev)))//no hal, try mmp
				if(FAILED(lpd3d->CreateDevice(IID_IDirect3DRGBDevice,lpdds,&lpd3ddev)))//no mmx, resort to rgb
					lpd3ddev=NULL;
	//return the pointer
	return(lpd3ddev);
}

//set up the viewport
void LPD3DDEV_SetViewport(LPDIRECT3DDEVICE7 lpd3ddev,DWORD x,DWORD y,DWORD width,DWORD height)
{
	//set up the view port
	D3DVIEWPORT7 vp;
	vp.dwX=x;
	vp.dwY=y;
	vp.dwWidth=width;
	vp.dwHeight=height;
	vp.dvMinZ=0.0;
	vp.dvMaxZ=1.0;
	//set the viewport for the device
	lpd3ddev->SetViewport(&vp);
}

//clear out the viewport
void LPD3DDEV_Clear(LPDIRECT3DDEVICE7 lpd3ddev,D3DCOLOR color)
{
	//clear out the device
	lpd3ddev->Clear(0,NULL,D3DCLEAR_TARGET,color,0,0);
}


//draw triangle list
void LPD3DDEV_DrawTriangleList(LPDIRECT3DDEVICE7 lpd3ddev,D3DTLVERTEX* pvertices,DWORD dwvertexcount)
{
	//draw the triangle list
	lpd3ddev->DrawPrimitive(D3DPT_TRIANGLELIST,D3DFVF_TLVERTEX,pvertices,dwvertexcount,0);
}

//draw triangle strip
void LPD3DDEV_DrawTriangleStrip(LPDIRECT3DDEVICE7 lpd3ddev,D3DTLVERTEX* pvertices,DWORD dwvertexcount)
{
	//draw the triangle strip
	lpd3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP,D3DFVF_TLVERTEX,pvertices,dwvertexcount,0);
}

//clean up a device
void LPD3DDEV_Release(LPDIRECT3DDEVICE7* lpd3ddev)
{
	//safe release
	if(*lpd3ddev)
	{
		(*lpd3ddev)->Release();
		(*lpd3ddev)=NULL;
	}
}

//primary surface as a 3d rendering target
LPDIRECTDRAWSURFACE7 LPDDS_CreatePrimary3D(LPDIRECTDRAW7 lpdd,DWORD dwBackBufferCount)
{
	LPDIRECTDRAWSURFACE7 lpdds;
	//create primary surface
	DDSURFACEDESC2 ddsd;
	memset(&ddsd,0,sizeof(DDSURFACEDESC2));
	ddsd.dwSize=sizeof(DDSURFACEDESC2);
	ddsd.dwFlags=DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.dwBackBufferCount=dwBackBufferCount;
	ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
	lpdd->CreateSurface(&ddsd,&lpdds,NULL);
	//return the surface pointer
	return(lpdds);
}

//back buffer as a 3d rendering target
LPDIRECTDRAWSURFACE7 LPDDS_GetSecondary3D(LPDIRECTDRAWSURFACE7 lpdds)
{
	LPDIRECTDRAWSURFACE7 lpddsBack;
	//create back buffer
	DDSCAPS2 ddscaps;
	memset(&ddscaps,0,sizeof(DDSCAPS2));
	ddscaps.dwCaps=DDSCAPS_BACKBUFFER | DDSCAPS_3DDEVICE;
	lpdds->GetAttachedSurface(&ddscaps,&lpddsBack);
	//return the new surface
	return(lpddsBack);
}

//create a texture
LPDIRECTDRAWSURFACE7 LPDDS_CreateTexture(LPDIRECTDRAW7 lpdd,DWORD dwWidth,DWORD dwHeight)
{
	LPDIRECTDRAWSURFACE7 lpdds;
	//create texture surface
	DDSURFACEDESC2 ddsd;
	memset(&ddsd,0,sizeof(DDSURFACEDESC2));
	ddsd.dwSize=sizeof(DDSURFACEDESC2);
	ddsd.dwFlags=DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth=dwWidth;
	ddsd.dwHeight=dwHeight;
	ddsd.ddsCaps.dwCaps=DDSCAPS_TEXTURE;
	lpdd->CreateSurface(&ddsd,&lpdds,NULL);
	//return the surface pointer
	return(lpdds);
}

//create a texture with a particular pixel format
LPDIRECTDRAWSURFACE7 LPDDS_CreateTexturePixelFormat(LPDIRECTDRAW7 lpdd,DWORD dwWidth,DWORD dwHeight,LPDDPIXELFORMAT lpddpf)
{
	LPDIRECTDRAWSURFACE7 lpdds;
	//create texture surface
	DDSURFACEDESC2 ddsd;
	memset(&ddsd,0,sizeof(DDSURFACEDESC2));
	ddsd.dwSize=sizeof(DDSURFACEDESC2);
	ddsd.dwFlags=DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.dwWidth=dwWidth;
	ddsd.dwHeight=dwHeight;
	ddsd.ddsCaps.dwCaps=DDSCAPS_TEXTURE;
	memcpy(&ddsd.ddpfPixelFormat,lpddpf,sizeof(DDPIXELFORMAT));
	lpdd->CreateSurface(&ddsd,&lpdds,NULL);
	//return the surface pointer
	return(lpdds);
}

//set vertex data
void VERTEX_Set(D3DTLVERTEX* pVert,D3DVALUE x,D3DVALUE y, D3DCOLOR color, D3DVALUE tu, D3DVALUE tv)
{
	//xyz
	pVert->sx=x;
	pVert->sy=y;
	pVert->sz=0.5;
	//rhw
	pVert->rhw=1.0;
	//color
	pVert->color=color;
	//specular
	pVert->specular=0;
	//texture coords
	pVert->tu=tu;
	pVert->tv=tv;
}
