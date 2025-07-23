//IsoTilePlotter.cpp
#include "IsoTilePlotter.h"

//plotting function prototypes
POINT IsoHex_SlidePlotTile(POINT ptMap,int iTileWidth,int iTileHeight);
POINT IsoHex_StagPlotTile(POINT ptMap,int iTileWidth,int iTileHeight);
POINT IsoHex_DiamondPlotTile(POINT ptMap,int iTileWidth,int iTileHeight);
POINT IsoHex_RectPlotTile(POINT ptMap,int iTileWidth,int iTileHeight);

//constructor
CTilePlotter::CTilePlotter()
{
	//set default map type
	SetMapType(ISOMAP_RECTANGULAR);

	//set default tile size
	SetTileSize(1,1);
}

//destructor
CTilePlotter::~CTilePlotter()
{
	//intentionally blank
}

//set map type
void CTilePlotter::SetMapType(ISOMAPTYPE IsoMapType)
{
	//set the map type
	this->IsoMapType=IsoMapType;

	//set the proper plotting function
	switch(IsoMapType)
	{
	case ISOMAP_SLIDE:
		{
			this->IsoHexTilePlotterFn=IsoHex_SlidePlotTile;
		}break;
	case ISOMAP_STAGGERED:
		{
			this->IsoHexTilePlotterFn=IsoHex_StagPlotTile;
		}break;
	case ISOMAP_DIAMOND:
		{
			this->IsoHexTilePlotterFn=IsoHex_DiamondPlotTile;
		}break;
	case ISOMAP_RECTANGULAR:
		{
			this->IsoHexTilePlotterFn=IsoHex_RectPlotTile;
		}break;
	}
}

//get map type
ISOMAPTYPE CTilePlotter::GetMapType()
{
	//return the map type
	return(IsoMapType);
}

//set tile size
void CTilePlotter::SetTileSize(int iTileWidth,int iTileHeight)
{
	//set width and height
	this->iTileHeight=iTileHeight;
	this->iTileWidth=iTileWidth;
}

//get tile width
int CTilePlotter::GetTileWidth()
{
	//return width of tile
	return(iTileWidth);
}

//get tile height
int CTilePlotter::GetTileHeight()
{
	//return height of tile
	return(iTileHeight);
}

//plot a tile
POINT CTilePlotter::PlotTile(POINT ptMap)
{
	return(IsoHexTilePlotterFn(ptMap,iTileWidth,iTileHeight));
}

//actual tile plotting functions
//slide
POINT IsoHex_SlidePlotTile(POINT ptMap,int iTileWidth,int iTileHeight)
{
	//point plotted
	POINT ptPlot;

	//plot x
	ptPlot.x=ptMap.x*iTileWidth+ptMap.y*(iTileWidth>>1);

	//plot y
	ptPlot.y=ptMap.y*(iTileHeight>>1);

	//return plotted coordinate
	return(ptPlot);
}

//staggered
POINT IsoHex_StagPlotTile(POINT ptMap,int iTileWidth,int iTileHeight)
{
	//point plotted
	POINT ptPlot;

	//plot x
	ptPlot.x=ptMap.x*iTileWidth+(ptMap.y&1)*(iTileWidth>>1);

	//plot y
	ptPlot.y=ptMap.y*(iTileHeight>>1);

	//return plotted coordinate
	return(ptPlot);
}

//diamond
POINT IsoHex_DiamondPlotTile(POINT ptMap,int iTileWidth,int iTileHeight)
{
	//point plotted
	POINT ptPlot;

	//plot x
	ptPlot.x=(ptMap.x-ptMap.y)*(iTileWidth>>1);

	//plot y
	ptPlot.y=(ptMap.x+ptMap.y)*(iTileHeight>>1);

	//return plotted coordinate
	return(ptPlot);
}

//rectangular
POINT IsoHex_RectPlotTile(POINT ptMap,int iTileWidth,int iTileHeight)
{
	//point plotted
	POINT ptPlot;

	//plot x
	ptPlot.x=ptMap.x*iTileWidth;

	//plot y
	ptPlot.y=ptMap.y*iTileHeight;

	//return plotted coordinate
	return(ptPlot);
}

