///isoscroller.h
//072500
//ernest s. pazera
//declarations for the cscroller class
#ifndef __ISOSCROLLER_H__
#define __ISOSCROLLER_H__

#include <windows.h>
#include "IsoTilePlotter.h"

//wrapping modes for anchors
typedef enum
{
	WRAPMODE_NONE,//no clipping of any kind is done to the anchor
	WRAPMODE_CLIP,
	WRAPMODE_WRAP
} SCROLLERWRAPMODE;

//world space/screen space management class
class CScroller
{
private:
	//screen space
	RECT rcScreenSpace;

	//world space
	RECT rcWorldSpace;

	//anchor space
	RECT rcAnchorSpace;

	//anchor
	POINT ptScreenAnchor;

	//wrapmodes
	SCROLLERWRAPMODE swmHorizontal;
	SCROLLERWRAPMODE swmVertical;
public:
	//constructor
	CScroller();
	//destructor
	~CScroller();

	//screen space
	RECT* GetScreenSpace();
	void SetScreenSpace(RECT* prcNewScreenSpace);
	void AdjustScreenSpace(int iLeftAdjust,int iTopAdjust,int iRightAdjust, int iBottomAdjust);
	int GetScreenSpaceWidth();
	int GetScreenSpaceHeight();

	//world space
	RECT* GetWorldSpace();
	void SetWorldSpace(RECT* prcNewWorldSpace);
	void AdjustWorldSpace(int iLeftAdjust,int iTopAdjust,int iRightAdjust, int iBottomAdjust);
	int GetWorldSpaceWidth();
	int GetWorldSpaceHeight();
	void CalcWorldSpace(CTilePlotter* TilePlotter,RECT* prcExtent,int iMapWidth,int iMapHeight);//calculates worldspace based on a tile plotter, a tile extent rectangle, and a map's height and width

	//anchor space
	RECT* GetAnchorSpace();
	void SetAnchorSpace(RECT* prcNewAnchorSpace);
	void AdjustAnchorSpace(int iLeftAdjust,int iTopAdjust,int iRightAdjust, int iBottomAdjust);
	int GetAnchorSpaceWidth();
	int GetAnchorSpaceHeight();
	void CalcAnchorSpace();//calculates anchor space based on world space and screen space

	//anchor
	POINT* GetAnchor();
	void SetAnchor(POINT* pptNewAnchor,bool bWrap=true);
	void MoveAnchor(int iXAdjust,int iYAdjust,bool bWrap=true);
	void WrapAnchor();//applies clipping or wrapping to anchor

	//conversion
	POINT ScreenToWorld(POINT ptScreen);
	POINT WorldToScreen(POINT ptWorld);

	//wrap modes
	void SetHWrapMode(SCROLLERWRAPMODE ScrollerWrapMode);
	void SetVWrapMode(SCROLLERWRAPMODE ScrollerWrapMode);
	SCROLLERWRAPMODE GetHWrapMode();
	SCROLLERWRAPMODE GetVWrapMode();

	//validation
	bool IsWorldCoord(POINT ptWorld);
	bool IsScreenCoord(POINT ptScreen);
	bool IsAnchorCoord(POINT ptAnchor);
};

#endif