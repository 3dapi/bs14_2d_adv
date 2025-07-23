// TileSet.h: interface for the CTileSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILESET_H__793CAFA2_45E3_11D4_A1EE_B431B7F27368__INCLUDED_)
#define AFX_TILESET_H__793CAFA2_45E3_11D4_A1EE_B431B7F27368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDICanvas.h"
#include "DDFuncs.h"

//tileset information structure
struct TILEINFO
{
	RECT rcSrc;//source rectangle
	POINT ptAnchor;//anchoring point
	RECT rcDstExt;//destination extent
};

class CTileSet  
{
private:
	//number of tiles in tileset
	DWORD dwTileCount;

	//tile array
	TILEINFO*  ptiTileList;

	//filename from which to reload
	LPSTR lpszReload;

	//offscreen plain directdrawsurface7
	LPDIRECTDRAWSURFACE7 lpddsTileSet;

public:
	//constructor
	CTileSet();

	//destructor
	~CTileSet();

	//load(initializer)
	void Load(LPDIRECTDRAW7 lpdd,LPSTR lpszLoad);

	//reload(restore)
	void Reload();

	//unload(uninitializer)
	void Unload();

	//get number of tiles
	DWORD GetTileCount();

	//get tile list
	TILEINFO* GetTileList();

	//get surface
	LPDIRECTDRAWSURFACE7 GetDDS();

	//retrieve filename
	LPSTR GetFileName();

	//blit a tile
	void PutTile(LPDIRECTDRAWSURFACE7 lpddsDst,int xDst,int yDst,int iTileNum);
};

#endif // !defined(AFX_TILESET_H__793CAFA2_45E3_11D4_A1EE_B431B7F27368__INCLUDED_)
