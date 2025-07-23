//
//
////////////////////////////////////////////////////////////////////////////////


#pragma warning( disable : 4996)


#pragma once


#ifndef __STDAFX_H_
#define __STDAFX_H_

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "D3DApp.h"
#include "resource.h"

#include "LcSwpWn.h"

#include "Main.h"

#endif