/*****************************************************************************
DDFuncs.cpp
Ernest S. Pazera
21MAY2000
Requires the use of ddraw.lib and dxguid.lib
Also requires GDICanvas.h/cpp
*****************************************************************************/

#include "DDFuncs.h"

///////////////////////////////////////////////////////////////////////////////
//DDSURFACEDESC2 functions
///////////////////////////////////////////////////////////////////////////////

//clean out a DDSD
void DDSD_Clear(DDSURFACEDESC2* pddsd)
{
	//clear the ddsd to all zeros
	memset(pddsd,0,sizeof(DDSURFACEDESC2));

	//set the size
	pddsd->dwSize=sizeof(DDSURFACEDESC2);
}

//set up a DDSD for a primary surface, no back buffer
void DDSD_PrimarySurface(DDSURFACEDESC2* pddsd)
{
	//clean out the ddsd
	DDSD_Clear(pddsd);

	//set flags
	pddsd->dwFlags=DDSD_CAPS;

	//set caps
	pddsd->ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;

}

//set up a DDSD for a primary surface, with any number of backbuffers
void DDSD_PrimarySurfaceWBackBuffer(DDSURFACEDESC2* pddsd, DWORD dwBackBufferCount)
{
	//clean out the ddsd
	DDSD_Clear(pddsd);

	//set flags
	pddsd->dwFlags=DDSD_CAPS | DDSD_BACKBUFFERCOUNT;

	//set caps
	pddsd->ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

	//set back buffer count
	pddsd->dwBackBufferCount=dwBackBufferCount;
}

//set up a DDSD for an offscreen surface, specify width and height
void DDSD_OffscreenSurface(DDSURFACEDESC2* pddsd,DWORD dwWidth, DWORD dwHeight)
{
	//clean out the ddsd
	DDSD_Clear(pddsd);

	//set flags
	pddsd->dwFlags=DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	//set caps
	pddsd->ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;

	//set width and height
	pddsd->dwWidth=dwWidth;
	pddsd->dwHeight=dwHeight;

}

///////////////////////////////////////////////////////////////////////////////
//DDSCAPSC2 functions
///////////////////////////////////////////////////////////////////////////////

//clean out ddscaps
void DDSCAPS_Clear(DDSCAPS2* pddscaps)
{
	//clear to all zeros
	memset(pddscaps,0,sizeof(DDSCAPS2));
}

//set up ddscaps for back buffer
void DDSCAPS_BackBuffer(DDSCAPS2* pddscaps)
{
	//clean out structure
	DDSCAPS_Clear(pddscaps);

	//specify back buffer caps
	pddscaps->dwCaps=DDSCAPS_BACKBUFFER;
}

///////////////////////////////////////////////////////////////////////////////
//DDBLTFX functions
///////////////////////////////////////////////////////////////////////////////

//clean out a ddbltfx structure
void DDBLTFX_Clear(DDBLTFX* pddbltfx)
{
	//set to all zeros
	memset(pddbltfx,0,sizeof(DDBLTFX));

	//set dwsize
	pddbltfx->dwSize=sizeof(DDBLTFX);
}

//set up a ddbltfx for a color fill
void DDBLTFX_ColorFill(DDBLTFX* pddbltfx,DWORD dwColor)
{
	//clean out structure
	DDBLTFX_Clear(pddbltfx);

	//set fill color member
	pddbltfx->dwFillColor=dwColor;
}

///////////////////////////////////////////////////////////////////////////////
//DDPIXELFORMAT functions
///////////////////////////////////////////////////////////////////////////////

//clean a ddpixelformat structure
void DDPF_Clear(DDPIXELFORMAT* pddpf)
{
	//set to all zeros
	memset(pddpf,0,sizeof(DDPIXELFORMAT));

	//set size
	pddpf->dwSize=sizeof(DDPIXELFORMAT);
}

///////////////////////////////////////////////////////////////////////////////
//Direct Draw Initialization/Cleanup functions
///////////////////////////////////////////////////////////////////////////////

//create direct draw interface
LPDIRECTDRAW7 LPDD_Create(HWND hWnd,DWORD dwCoopLevel)
{
	//declare object pointer
	LPDIRECTDRAW7 lpdd;

	//create object
	DirectDrawCreateEx(NULL,(void**)&lpdd,IID_IDirectDraw7,NULL);

	//set cooperative level
	lpdd->SetCooperativeLevel(hWnd,dwCoopLevel);

	//return pointer to object
	return(lpdd);
}

//safe release of direct draw
void LPDD_Release(LPDIRECTDRAW7* lplpdd)
{
	//safe release
	if(*lplpdd)
	{
		(*lplpdd)->Release();
		(*lplpdd)=NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//Surface Creation/Cleanup functions
///////////////////////////////////////////////////////////////////////////////

//primary surface
LPDIRECTDRAWSURFACE7 LPDDS_CreatePrimary(LPDIRECTDRAW7 lpdd,DWORD dwBackBufferCount)
{
	//declare surface declaration
	DDSURFACEDESC2 ddsd;

	if(dwBackBufferCount)
	{
		//has back buffer
		DDSD_PrimarySurfaceWBackBuffer(&ddsd,dwBackBufferCount);
	}
	else
	{
		//has no back buffer
		DDSD_PrimarySurface(&ddsd);
	}

	//declare pointer
	LPDIRECTDRAWSURFACE7 lpdds;
	
	//create the surface
	lpdd->CreateSurface(&ddsd,&lpdds,NULL);

	//return the surface
	return(lpdds);
}

//back buffer
LPDIRECTDRAWSURFACE7 LPDDS_GetSecondary(LPDIRECTDRAWSURFACE7 lpdds)
{
	//surface object pointer
	LPDIRECTDRAWSURFACE7 lpddsback;

	//caps structure
	DDSCAPS2 ddscaps;

	//fill in caps
	DDSCAPS_BackBuffer(&ddscaps);

	//retrieve attached surface
	lpdds->GetAttachedSurface(&ddscaps,&lpddsback);

	//return surface
	return(lpddsback);
}

//offscreen
LPDIRECTDRAWSURFACE7 LPDDS_CreateOffscreen(LPDIRECTDRAW7 lpdd,DWORD dwWidth,DWORD dwHeight)
{
	//set up ddsd
	DDSURFACEDESC2 ddsd;
	DDSD_OffscreenSurface(&ddsd,dwWidth,dwHeight);

	//create surface
	LPDIRECTDRAWSURFACE7 lpdds;
	lpdd->CreateSurface(&ddsd,&lpdds,NULL);

	//return surface
	return(lpdds);
}

//offscreen with a bitmap from a file
LPDIRECTDRAWSURFACE7 LPDDS_LoadFromFile(LPDIRECTDRAW7 lpdd,LPCTSTR lpszFileName)
{
	//load the bitmap
	CGDICanvas gdic;
	gdic.Load(NULL,lpszFileName);

	//create offscreen surface with same width and height
	LPDIRECTDRAWSURFACE7 lpdds=LPDDS_CreateOffscreen(lpdd,gdic.GetWidth(),gdic.GetHeight());

	//retrieve hdc for surface
	HDC hdcSurf;
	lpdds->GetDC(&hdcSurf);

	//blit image onto surface
	BitBlt(hdcSurf,0,0,gdic.GetWidth(),gdic.GetHeight(),gdic,0,0,SRCCOPY);

	//release dc
	lpdds->ReleaseDC(hdcSurf);

	//return surface
	return(lpdds);
}

//reloading a bitmap from file to a surface
void LPDDS_ReloadFromFile(LPDIRECTDRAWSURFACE7 lpdds, LPCTSTR lpszFileName)
{
	//load the bitmap
	CGDICanvas gdic;
	gdic.Load(NULL,lpszFileName);

	//retrieve hdc for surface
	HDC hdcSurf;
	lpdds->GetDC(&hdcSurf);

	//blit image onto surface
	BitBlt(hdcSurf,0,0,gdic.GetWidth(),gdic.GetHeight(),gdic,0,0,SRCCOPY);

	//release dc
	lpdds->ReleaseDC(hdcSurf);
}

//safe release
void LPDDS_Release(LPDIRECTDRAWSURFACE7* lplpdds)
{
	//safe release
	if(*lplpdds)
	{
		(*lplpdds)->Release();
		(*lplpdds)=NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//Color Key Setting functions
///////////////////////////////////////////////////////////////////////////////

void LPDDS_SetSrcColorKey(LPDIRECTDRAWSURFACE7 lpdds,DWORD dwColor)
{
	//set up color key
	DDCOLORKEY ddck;
	ddck.dwColorSpaceHighValue=dwColor;
	ddck.dwColorSpaceLowValue=dwColor;

	//set the color key
	lpdds->SetColorKey(DDCKEY_SRCBLT,&ddck);
}

///////////////////////////////////////////////////////////////////////////////
//Color Conversion Functions
///////////////////////////////////////////////////////////////////////////////

//from dd pixel to colorref
COLORREF ConvertDDColor(DWORD dwColor, DDPIXELFORMAT* pddpf)
{
	//extract color components
	DWORD dwRed=dwColor & pddpf->dwRBitMask;
	DWORD dwGreen=dwColor & pddpf->dwGBitMask;
	DWORD dwBlue=dwColor & pddpf->dwBBitMask;

	//multiply color components by max colorref value(255)
	dwRed*=255;
	dwGreen*=255;
	dwBlue*=255;

	//divide by masks
	dwRed/=pddpf->dwRBitMask;
	dwGreen/=pddpf->dwGBitMask;
	dwBlue/=pddpf->dwBBitMask;

	//return converted color
	return(RGB(dwRed,dwGreen,dwBlue));
}

//from colorref to dd pixel
DWORD ConvertColorRef(COLORREF crColor, DDPIXELFORMAT* pddpf)
{
	//extract color components
	DWORD dwRed=GetRValue(crColor);
	DWORD dwGreen=GetGValue(crColor);
	DWORD dwBlue=GetBValue(crColor);

	//multiply color components by max ddpixel value (the mask)
	dwRed*=pddpf->dwRBitMask;
	dwGreen*=pddpf->dwGBitMask;
	dwBlue*=pddpf->dwBBitMask;

	//divide by max colorref(255)
	dwRed/=255;
	dwGreen/=255;
	dwBlue/=255;

	//logical and with mask, to avoid fractions
	dwRed&=pddpf->dwRBitMask;
	dwGreen&=pddpf->dwGBitMask;
	dwBlue&=pddpf->dwBBitMask;

	//merge together, and return the result
	return(dwRed | dwGreen | dwBlue);
}

///////////////////////////////////////////////////////////////////////////////
//Clipper utilities
///////////////////////////////////////////////////////////////////////////////

//create a IDirectDraw7 clipper, and fill it with the information from a region
LPDIRECTDRAWCLIPPER LPDDCLIP_Create(LPDIRECTDRAW7 lpdd, HRGN hrgn)
{
	//create clipper
	LPDIRECTDRAWCLIPPER lpddclip;
	lpdd->CreateClipper(0,&lpddclip,NULL);

	//get size of buffer
	DWORD dwBufSize=GetRegionData(hrgn,0,NULL);

	//allocate buffer
	LPRGNDATA lprd=(LPRGNDATA)malloc(dwBufSize);

	//retrieve buffer
	GetRegionData(hrgn,dwBufSize,lprd);

	//set clip list
	lpddclip->SetClipList(lprd,0);

	//release buffer
	free(lprd);

	//return clipper
	return(lpddclip);
}

//safe release of an IDirectDrawClipper
void LPDDCLIP_Release(LPDIRECTDRAWCLIPPER* lplpddclip)
{
	//safe release
	if(*lplpddclip)
	{
		(*lplpddclip)->Release();
		(*lplpddclip)=NULL;
	}
}
