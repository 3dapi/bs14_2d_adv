////////////////////////////////////////////////////////////
//IsoTilePlotter.h
//24JUL2000
//ernest s. pazera
//delcarations for CTilePlotter
////////////////////////////////////////////////////////////
#ifndef __ISOTILEPLOTTER_H__
#define __ISOTILEPLOTTER_H__

#include <windows.h>
#include "IsoHexDefs.h"

////////////////////////////////////////////////////////////
//typedef for tile plotter function pointer type
////////////////////////////////////////////////////////////
typedef POINT (*ISOHEXTILEPLOTTERFN)(POINT ptMap,int iTileWidth,int iTileHeight);

////////////////////////////////////////////////////////////
//tile plotter class
////////////////////////////////////////////////////////////
class CTilePlotter
{
private:
	//type of map
	ISOMAPTYPE IsoMapType;

	//width and height of tiles
	int iTileWidth;
	int iTileHeight;

	//function called to calculate plotted tiles
	ISOHEXTILEPLOTTERFN IsoHexTilePlotterFn;

public:
	//constructor/destructor
	CTilePlotter();
	~CTilePlotter();

	//map type
	void SetMapType(ISOMAPTYPE IsoMapType);
	ISOMAPTYPE GetMapType();

	//tile size
	void SetTileSize(int iTileWidth,int iTileHeight);
	int GetTileWidth();
	int GetTileHeight();

	//plot a tile
	POINT PlotTile(POINT ptMap);
};

#endif