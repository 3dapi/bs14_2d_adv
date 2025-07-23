//isoscroller.cpp
#include "IsoScroller.h"

//constructor
CScroller::CScroller()
{
	//set all spaces to empty
	SetRectEmpty(&rcWorldSpace);
	SetRectEmpty(&rcScreenSpace);
	SetRectEmpty(&rcAnchorSpace);

	//set screen anchor to 0,0
	ptScreenAnchor.x=0;
	ptScreenAnchor.y=0;

	//set wrapping modes to none
	SetHWrapMode(WRAPMODE_NONE);
	SetVWrapMode(WRAPMODE_NONE);
}

//destructor
CScroller::~CScroller()
{
}

//screen space
//get screenspace
RECT* CScroller::GetScreenSpace()
{
	//return pointer to rect
	return(&rcScreenSpace);
}

//set screenspace
void CScroller::SetScreenSpace(RECT* prcNewScreenSpace)
{
	//copy rectangle
	CopyRect(&rcScreenSpace,prcNewScreenSpace);
}

//change screenspace
void CScroller::AdjustScreenSpace(int iLeftAdjust,int iTopAdjust,int iRightAdjust, int iBottomAdjust)
{
	//adjust left
	rcScreenSpace.left+=iLeftAdjust;
	//adjust top
	rcScreenSpace.top+=iTopAdjust;
	//adjust right
	rcScreenSpace.right+=iRightAdjust;
	//adjust bottom
	rcScreenSpace.bottom+=iBottomAdjust;
}

//retrieve width of screenspace
int CScroller::GetScreenSpaceWidth()
{
	//return width of rect
	return(rcScreenSpace.right-rcScreenSpace.left);
}

//retrieve height of screenspace
int CScroller::GetScreenSpaceHeight()
{
	//return height of rect
	return(rcScreenSpace.bottom-rcScreenSpace.top);
}

//world space
//get worldspace
RECT* CScroller::GetWorldSpace()
{
	//return pointer to rect
	return(&rcWorldSpace);
}

//set worldspace
void CScroller::SetWorldSpace(RECT* prcNewWorldSpace)
{
	//copy rectangle
	CopyRect(&rcWorldSpace,prcNewWorldSpace);
}

//change worldspace
void CScroller::AdjustWorldSpace(int iLeftAdjust,int iTopAdjust,int iRightAdjust, int iBottomAdjust)
{
	//adjust left
	rcWorldSpace.left+=iLeftAdjust;
	//adjust top
	rcWorldSpace.top+=iTopAdjust;
	//adjust right
	rcWorldSpace.right+=iRightAdjust;
	//adjust bottom
	rcWorldSpace.bottom+=iBottomAdjust;
}

//retrieve width of worldspace
int CScroller::GetWorldSpaceWidth()
{
	//return width of rect
	return(rcWorldSpace.right-rcWorldSpace.left);
}

//retrieve height of worldspace
int CScroller::GetWorldSpaceHeight()
{
	//return height of rect
	return(rcWorldSpace.bottom-rcWorldSpace.top);
}

//calc worldspace based on a tileplotter and map width and height
void CScroller::CalcWorldSpace(CTilePlotter* TilePlotter,RECT* prcExtent,int iMapWidth,int iMapHeight)
{
	//set worldspace rect to empty
	SetRectEmpty(&rcWorldSpace);

	//temporary rectangle
	RECT rcTemp;
	CopyRect(&rcTemp,prcExtent);

	//point for plotting
	POINT ptPlot;

	//map point
	POINT ptMap;

	//loop through map positions
	for(ptMap.x=0;ptMap.x<iMapWidth;ptMap.x++)
	{
		for(ptMap.y=0;ptMap.y<iMapHeight;ptMap.y++)
		{
			//plot the map point
			ptPlot=TilePlotter->PlotTile(ptMap);

			//adjust the temp rect
			OffsetRect(&rcTemp,ptPlot.x,ptPlot.y);

			//expand the boundaries of worldspace
			//left
			if(rcTemp.left<rcWorldSpace.left) rcWorldSpace.left=rcTemp.left;
			//top
			if(rcTemp.top<rcWorldSpace.top) rcWorldSpace.top=rcTemp.top;
			//right
			if(rcTemp.right>rcWorldSpace.right) rcWorldSpace.right=rcTemp.right;
			//bottom
			if(rcTemp.bottom>rcWorldSpace.bottom) rcWorldSpace.bottom=rcTemp.bottom;

			//adjust the temp rect back
			OffsetRect(&rcTemp,-ptPlot.x,-ptPlot.y);
		}
	}
}

//anchor space
//get anchorspace
RECT* CScroller::GetAnchorSpace()
{
	//return pointer to rect
	return(&rcAnchorSpace);
}

//set anchorspace
void CScroller::SetAnchorSpace(RECT* prcNewAnchorSpace)
{
	//copy rectangle
	CopyRect(&rcAnchorSpace,prcNewAnchorSpace);
}

//change anchorspace
void CScroller::AdjustAnchorSpace(int iLeftAdjust,int iTopAdjust,int iRightAdjust, int iBottomAdjust)
{
	//adjust left
	rcAnchorSpace.left+=iLeftAdjust;
	//adjust top
	rcAnchorSpace.top+=iTopAdjust;
	//adjust right
	rcAnchorSpace.right+=iRightAdjust;
	//adjust bottom
	rcAnchorSpace.bottom+=iBottomAdjust;
}

//retrieve width of anchorspace
int CScroller::GetAnchorSpaceWidth()
{
	//return width of rect
	return(rcAnchorSpace.right-rcAnchorSpace.left);
}

//retrieve height of anchorspace
int CScroller::GetAnchorSpaceHeight()
{
	//return height of rect
	return(rcAnchorSpace.bottom-rcAnchorSpace.top);
}

//calculates anchor space based on world space and screen space
void CScroller::CalcAnchorSpace()
{
	//copy worldspace
	CopyRect(&rcAnchorSpace,&rcWorldSpace);

	//subtract out screen space
	if(GetHWrapMode()!=WRAPMODE_WRAP) rcAnchorSpace.right-=GetScreenSpaceWidth();
	if(GetVWrapMode()!=WRAPMODE_WRAP) rcAnchorSpace.bottom-=GetScreenSpaceHeight();

	//make sure right!<left and bottom !<top
	if(rcAnchorSpace.right<=rcAnchorSpace.left) rcAnchorSpace.right=rcAnchorSpace.left+1;
	if(rcAnchorSpace.bottom<=rcAnchorSpace.top) rcAnchorSpace.bottom=rcAnchorSpace.top+1;
}

//anchor
//retrieve anchor
POINT* CScroller::GetAnchor()
{
	//return screen anchor
	return(&ptScreenAnchor);
}

//set anchor
void CScroller::SetAnchor(POINT* pptNewAnchor,bool bWrap)
{
	//copy position
	ptScreenAnchor.x=pptNewAnchor->x;
	ptScreenAnchor.y=pptNewAnchor->y;

	//optionally apply wrapping mode
	if(bWrap) WrapAnchor();
}

//move anchor
void CScroller::MoveAnchor(int iXAdjust,int iYAdjust,bool bWrap)
{
	//adjust screen anchor
	ptScreenAnchor.x+=iXAdjust;
	ptScreenAnchor.y+=iYAdjust;

	//optionally wrap/clip anchor
	if(bWrap) WrapAnchor();
}

//apply wrapmode to anchor
void CScroller::WrapAnchor()
{
	//horizontal wrapping
	switch(swmHorizontal)
	{
	case WRAPMODE_CLIP:
		{
			//clip to left
			if(ptScreenAnchor.x<rcAnchorSpace.left) ptScreenAnchor.x=rcAnchorSpace.left;
			//clip to right
			if(ptScreenAnchor.x>=rcAnchorSpace.right) ptScreenAnchor.x=rcAnchorSpace.right-1;
		}break;
	case WRAPMODE_WRAP:
		{
			//left wrapping
			while(ptScreenAnchor.x<rcAnchorSpace.left) ptScreenAnchor.x+=GetAnchorSpaceWidth();
			//right wrapping
			while(ptScreenAnchor.x>=rcAnchorSpace.right) ptScreenAnchor.x-=GetAnchorSpaceWidth();
		}break;
	}
	//vertical wrapping
	switch(swmVertical)
	{
	case WRAPMODE_CLIP:
		{
			//clip to top
			if(ptScreenAnchor.y<rcAnchorSpace.top) ptScreenAnchor.y=rcAnchorSpace.top;
			//clip to bottom
			if(ptScreenAnchor.y>=rcAnchorSpace.bottom) ptScreenAnchor.y=rcAnchorSpace.bottom-1;
		}break;
	case WRAPMODE_WRAP:
		{
			//top wrapping
			while(ptScreenAnchor.y<rcAnchorSpace.top) ptScreenAnchor.y+=GetAnchorSpaceWidth();
			//bottom wrapping
			while(ptScreenAnchor.y>=rcAnchorSpace.bottom) ptScreenAnchor.y-=GetAnchorSpaceWidth();
		}break;
	}
}

//conversion
//screen->world
POINT CScroller::ScreenToWorld(POINT ptScreen)
{
	//translate into plotspace coordinates
	ptScreen.x-=rcScreenSpace.left;
	ptScreen.y-=rcScreenSpace.top;

	//translate into world coordinates
	ptScreen.x+=ptScreenAnchor.x;
	ptScreen.y+=ptScreenAnchor.y;

	//return coordinates
	return(ptScreen);
}

//world->screen
POINT CScroller::WorldToScreen(POINT ptWorld)
{
	//translate into plotspace coordinates
	ptWorld.x-=ptScreenAnchor.x;
	ptWorld.y-=ptScreenAnchor.y;

	//translate into screen coordinates
	ptWorld.x+=rcScreenSpace.left;
	ptWorld.y+=rcScreenSpace.top;

	//return cooridinates
	return(ptWorld);
}

//wrap modes
//setters
//horizontal
void CScroller::SetHWrapMode(SCROLLERWRAPMODE ScrollerWrapMode)
{
	//set wrap mode
	swmHorizontal=ScrollerWrapMode;
}

//vertical
void CScroller::SetVWrapMode(SCROLLERWRAPMODE ScrollerWrapMode)
{
	//set wrap mode
	swmVertical=ScrollerWrapMode;
}

//getters
//horiz
SCROLLERWRAPMODE CScroller::GetHWrapMode()
{
	//return wrap mode
	return(swmHorizontal);
}

//vertical
SCROLLERWRAPMODE CScroller::GetVWrapMode()
{
	//return wrap mode
	return(swmVertical);
}

//validation
//world
bool CScroller::IsWorldCoord(POINT ptWorld)
{
	//check for valid coordinate
	return(PtInRect(&rcWorldSpace,ptWorld)!=FALSE);
}

//screen
bool CScroller::IsScreenCoord(POINT ptScreen)
{
	//check for valid coordinate
	return(PtInRect(&rcScreenSpace,ptScreen)!=FALSE);
}

//anchor
bool CScroller::IsAnchorCoord(POINT ptAnchor)
{
	//check for valid coordinate
	return(PtInRect(&rcAnchorSpace,ptAnchor)!=FALSE);
}
