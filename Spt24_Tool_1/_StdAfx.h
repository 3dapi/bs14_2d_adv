//
//
////////////////////////////////////////////////////////////////////////////////


#pragma warning( disable : 4996)


#pragma once


#ifndef __STDAFX_H_
#define __STDAFX_H_


#define STRICT
#define _WIN32_WINNT 0x0400

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>
#include <math.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#include "D3DApp.h"
#include "resource.h"


#define GHWND					g_pApp->GetHwnd()
#define GDEVICE					g_pApp->m_pd3dDevice
#define GINPUT					g_pApp->m_pInput

#include "_LcUtil/LcUtil.h"
#include "_LcUtil/LcInput.h"
#include "_LcUtil/LcTexture.h"
#include "_LcUtil/LcSprite.h"
#include "_LcUtil/LcSwpWn.h"


#include "LcTile.h"



#include "Main.h"

#endif

