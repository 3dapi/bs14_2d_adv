//IsoMouseMap.h
#ifndef __ISOMOUSEMAP_H__
#define __ISOMOUSEMAP_H__

#include <windows.h>
#include "IsoTilePlotter.h"
#include "IsoTileWalker.h"
#include "IsoScroller.h"

//mousemap directions
typedef enum {
	MM_CENTER=0,
	MM_NW=1,
	MM_NE=2,
	MM_SW=3,
	MM_SE=4
} MOUSEMAPDIRECTION;

//mousemap class
class CMouseMap
{
private:
	//width and height of lookup
	int iWidth;
	int iHeight;

	//reference point(adjustment for the upper left of tile 0,0)
	POINT ptRef;

	//lookup table
	MOUSEMAPDIRECTION* mmdLookUp;

	//scroller
	CScroller* pScroller;
	
	//walker
	CTileWalker* pTileWalker;
public:
	//constructor
	CMouseMap();
	//destructor
	~CMouseMap();

	//load mousemap
	void Load(LPCTSTR lpszFileName);//used with iso and hex maps
	void Create(int iWidth,int iHeight);//used with rectangular maps
	//destroy mousemap
	void Destroy();

	//width/height
	int GetWidth();
	int GetHeight();

	//reference
	POINT* GetReferencePoint();
	void SetReferencePoint(POINT* pptRefPt);
	void CalcReferencePoint(CTilePlotter* pTilePlotter,RECT* prcExtent);

	//map the mouse
	POINT MapMouse(POINT ptMouse);

	//scroller
	CScroller* GetScroller();
	void SetScroller(CScroller* pScroller);

	//walker
	CTileWalker* GetTileWalker();
	void SetTileWalker(CTileWalker* pTileWalker);
};

#endif