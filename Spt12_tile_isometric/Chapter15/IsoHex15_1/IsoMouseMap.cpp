//IsoMouseMap.cpp
#include "IsoMouseMap.h"
#include "GDICanvas.h"

//constructor
CMouseMap::CMouseMap()
{
	//set defaults
	//scroller
	SetScroller(NULL);

	//walker
	SetTileWalker(NULL);

	//initialize mousemap to zeros
	mmdLookUp=NULL;
	iWidth=0;
	iHeight=0;

	//mousemap
	Create(1,1);

	//reference point
	ptRef.x=0;
	ptRef.y=0;
}

//destructor
CMouseMap::~CMouseMap()
{
	//destroy the lookup table
	Destroy();
}

//load mousemap
void CMouseMap::Load(LPCTSTR lpszFileName)
{
	//gdi canvas
	CGDICanvas gdic;

	//load in graphic
	gdic.Load(NULL,lpszFileName);

	//create mousemap
	Create(gdic.GetWidth(),gdic.GetHeight());

	//grab corner points
	COLORREF crNE=GetPixel(gdic,iWidth-1,0);
	COLORREF crNW=GetPixel(gdic,0,0);
	COLORREF crSE=GetPixel(gdic,iWidth-1,iHeight-1);
	COLORREF crSW=GetPixel(gdic,0,iHeight-1);

	//test point
	COLORREF crTest;

	//fill in lookup
	for(int x=0;x<iWidth;x++)
	{
		for(int y=0;y<iHeight;y++)
		{
			//grab pixel
			crTest=GetPixel(gdic,x,y);

			//check against corner points
			if(crTest==crNE) mmdLookUp[x+y*iWidth]=MM_NE;
			if(crTest==crSE) mmdLookUp[x+y*iWidth]=MM_SE;
			if(crTest==crNW) mmdLookUp[x+y*iWidth]=MM_NW;
			if(crTest==crSW) mmdLookUp[x+y*iWidth]=MM_SW;
		}
	}
}

//create a blank mousemap
void CMouseMap::Create(int iWidth,int iHeight)
{
	//destroy any old lookup table
	Destroy();

	//copy size
	this->iWidth=iWidth;
	this->iHeight=iHeight;

	//allocate buffer
	mmdLookUp=new MOUSEMAPDIRECTION[iWidth*iHeight];

	//clear out buffer
	for(int count=0;count<iWidth*iHeight;count++)
	{
		mmdLookUp[count]=MM_CENTER;
	}
}

//destroy mousemap
void CMouseMap::Destroy()
{
	//if allocated, deallocate
	if(mmdLookUp)
	{
		delete[] mmdLookUp;
		iWidth=0;
		iHeight=0;
	}
}

//width/height
int CMouseMap::GetWidth()
{
	//return width
	return(iWidth);
}

int CMouseMap::GetHeight()
{
	//return height
	return(iHeight);
}

//reference
//get the reference point
POINT* CMouseMap::GetReferencePoint()
{
	//return point
	return(&ptRef);
}

//set the reference point
void CMouseMap::SetReferencePoint(POINT* pptRefPt)
{
	//set new reference point
	ptRef.x=pptRefPt->x;
	ptRef.y=pptRefPt->y;
}

//calculate the reference point based on a tile plotter and an extent rect
void CMouseMap::CalcReferencePoint(CTilePlotter* pTilePlotter,RECT* prcExtent)
{
	//map point
	POINT ptMap;
	ptMap.x=0;
	ptMap.y=0;

	//plotted point
	ptRef=pTilePlotter->PlotTile(ptMap);

	//adjust ptRef by left and top of extent rect
	ptRef.x+=prcExtent->left;
	ptRef.y+=prcExtent->top;
}

//map the mouse
POINT CMouseMap::MapMouse(POINT ptMouse)
{
	//convert to world coordinates from screen coordinates
	//check for a scroller. if there is no scroller, then assume screen coordinates are the same as world coordinates
	if(pScroller)
	{
		//convert from screen to world
		ptMouse=pScroller->ScreenToWorld(ptMouse);
	}

	//subtract reference point
	ptMouse.x-=ptRef.x;
	ptMouse.y-=ptRef.y;

	//convert to mousemap coordinates
	POINT ptMouseMapCoarse;
	POINT ptMouseMapFine;

	//coarse coordinates
	ptMouseMapCoarse.x=ptMouse.x/GetWidth();
	ptMouseMapCoarse.y=ptMouse.y/GetHeight();

	//fine coordinates
	ptMouseMapFine.x=ptMouse.x%GetWidth();
	ptMouseMapFine.y=ptMouse.y%GetHeight();

	//adjust for negative fine coordinates
	if(ptMouseMapFine.x<0)
	{
		ptMouseMapFine.x+=iWidth;
		ptMouseMapCoarse.x--;
	}
	if(ptMouseMapFine.y<0)
	{
		ptMouseMapFine.y+=iHeight;
		ptMouseMapCoarse.y--;
	}

	//check for a tile walker. if there is no tile walker, convert from coarse coordinates directly into map coordinates
	POINT ptMap;
	ptMap.x=0;
	ptMap.y=0;
	if(pTileWalker)
	{
		//coarse tilewalk
		//north
		while(ptMouseMapCoarse.y<0)
		{
			ptMap=pTileWalker->TileWalk(ptMap,ISO_NORTH);
			ptMouseMapCoarse.y++;
		}
		//east
		while(ptMouseMapCoarse.x>0)
		{
			ptMap=pTileWalker->TileWalk(ptMap,ISO_EAST);
			ptMouseMapCoarse.x--;
		}
		//south
		while(ptMouseMapCoarse.y>0)
		{
			ptMap=pTileWalker->TileWalk(ptMap,ISO_SOUTH);
			ptMouseMapCoarse.y--;
		}
		//west
		while(ptMouseMapCoarse.x<0)
		{
			ptMap=pTileWalker->TileWalk(ptMap,ISO_WEST);
			ptMouseMapCoarse.x++;
		}
		//fine tilewalk
		switch(mmdLookUp[ptMouseMapFine.x+ptMouseMapFine.y*GetWidth()])
		{
		case MM_NE:
			{
				ptMap=pTileWalker->TileWalk(ptMap,ISO_NORTHEAST);
			}break;
		case MM_SE:
			{
				ptMap=pTileWalker->TileWalk(ptMap,ISO_SOUTHEAST);
			}break;
		case MM_SW:
			{
				ptMap=pTileWalker->TileWalk(ptMap,ISO_SOUTHWEST);
			}break;
		case MM_NW:
			{
				ptMap=pTileWalker->TileWalk(ptMap,ISO_NORTHWEST);
			}break;
		}
	}
	else
	{
		//coarse coordinates are map coordinates
		ptMap=ptMouseMapCoarse;
	}
	//return map coordinate
	return(ptMap);
}

//scroller
//get the scroller
CScroller* CMouseMap::GetScroller()
{
	//return the scroller
	return(pScroller);
}

//set the scroller
void CMouseMap::SetScroller(CScroller* pScroller)
{
	//set the scroller
	this->pScroller=pScroller;
}

//walker
//get the tile walker
CTileWalker* CMouseMap::GetTileWalker()
{
	//return walker
	return(pTileWalker);
}

//set the tile walker
void CMouseMap::SetTileWalker(CTileWalker* pTileWalker)
{
	//set the tile walker
	this->pTileWalker=pTileWalker;
}
