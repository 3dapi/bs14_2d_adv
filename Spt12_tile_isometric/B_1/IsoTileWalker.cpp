//IsoTileWalker.cpp
#include "IsoTileWalker.h"

//prototypes for tilewalker functions
POINT IsoHex_SlideTileWalk(POINT ptStart,ISODIRECTION IsoDirection);
POINT IsoHex_StagTileWalk(POINT ptStart,ISODIRECTION IsoDirection);
POINT IsoHex_DiamondTileWalk(POINT ptStart,ISODIRECTION IsoDirection);
POINT IsoHex_RectTileWalk(POINT ptStart,ISODIRECTION IsoDirection);

//constructor
CTileWalker::CTileWalker()
{
	SetMapType(ISOMAP_RECTANGULAR);
}

//destructor
CTileWalker::~CTileWalker()
{
}

//set map type
void CTileWalker::SetMapType(ISOMAPTYPE IsoMapType)
{
	//store map type
	this->IsoMapType=IsoMapType;

	//set the tilewalker function
	switch(IsoMapType)
	{
	case ISOMAP_SLIDE:
		{
			IsoHexTileWalkerFn=IsoHex_SlideTileWalk;
		}break;
	case ISOMAP_STAGGERED:
		{
			IsoHexTileWalkerFn=IsoHex_StagTileWalk;
		}break;
	case ISOMAP_DIAMOND:
		{
			IsoHexTileWalkerFn=IsoHex_DiamondTileWalk;
		}break;
	case ISOMAP_RECTANGULAR:
		{
			IsoHexTileWalkerFn=IsoHex_RectTileWalk;
		}break;
	}
}

//get map type
ISOMAPTYPE CTileWalker::GetMapType()
{
	return(IsoMapType);
}

//tile walking
POINT CTileWalker::TileWalk(POINT ptStart,ISODIRECTION IsoDirection)
{
	return(IsoHexTileWalkerFn(ptStart,IsoDirection));
}

//tile walker functions
//slide
POINT IsoHex_SlideTileWalk(POINT ptStart,ISODIRECTION IsoDirection)
{
	//move ptStart depending on direction
	switch(IsoDirection)
	{
	case ISO_NORTH://move north
		{
			ptStart.y-=2;
			ptStart.x++;
		}break;
	case ISO_NORTHEAST://move northeast
		{
			ptStart.x++;
			ptStart.y--;
		}break;
	case ISO_EAST://move east
		{
			ptStart.x++;
		}break;
	case ISO_SOUTHEAST://move southeast
		{
			ptStart.y++;
		}break;
	case ISO_SOUTH://move south
		{
			ptStart.y+=2;
			ptStart.x--;
		}break;
	case ISO_SOUTHWEST://move southwest
		{
			ptStart.x--;
			ptStart.y++;
		}break;
	case ISO_WEST://move west
		{
			ptStart.x--;
		}break;
	case ISO_NORTHWEST://move northwest
		{
			ptStart.y--;
		}break;
	}
	//return the location moved to
	return(ptStart);
}

//staggered
POINT IsoHex_StagTileWalk(POINT ptStart,ISODIRECTION IsoDirection)
{
	//move ptStart depending on direction
	switch(IsoDirection)
	{
	case ISO_NORTH://move north
		{
			ptStart.y-=2;
		}break;
	case ISO_NORTHEAST://move northeast
		{
			ptStart.x+=((ptStart.y&1));
			ptStart.y--;
		}break;
	case ISO_EAST://move east
		{
			ptStart.x++;
		}break;
	case ISO_SOUTHEAST://move southeast
		{
			ptStart.x+=((ptStart.y&1));
			ptStart.y++;
		}break;
	case ISO_SOUTH://move south
		{
			ptStart.y+=2;
		}break;
	case ISO_SOUTHWEST://move southwest
		{
			ptStart.x+=((ptStart.y&1)-1);
			ptStart.y++;
		}break;
	case ISO_WEST://move west
		{
			ptStart.x--;
		}break;
	case ISO_NORTHWEST://move northwest
		{
			ptStart.x+=((ptStart.y&1)-1);
			ptStart.y--;
		}break;
	}
	//return the location moved to
	return(ptStart);
}

//diamond
POINT IsoHex_DiamondTileWalk(POINT ptStart,ISODIRECTION IsoDirection)
{
	//move ptStart depending on direction
	switch(IsoDirection)
	{
	case ISO_NORTH://move north
		{
			ptStart.x--;
			ptStart.y--;
		}break;
	case ISO_NORTHEAST://move northeast
		{
			ptStart.y--;
		}break;
	case ISO_EAST://move east
		{
			ptStart.x++;
			ptStart.y--;
		}break;
	case ISO_SOUTHEAST://move southeast
		{
			ptStart.x++;
		}break;
	case ISO_SOUTH://move south
		{
			ptStart.x++;
			ptStart.y++;
		}break;
	case ISO_SOUTHWEST://move southwest
		{
			ptStart.y++;
		}break;
	case ISO_WEST://move west
		{
			ptStart.x--;
			ptStart.y++;
		}break;
	case ISO_NORTHWEST://move northwest
		{
			ptStart.x--;
		}break;
	}
	//return the location moved to
	return(ptStart);
}

//rectangular
POINT IsoHex_RectTileWalk(POINT ptStart,ISODIRECTION IsoDirection)
{
	//move ptStart depending on direction
	switch(IsoDirection)
	{
	case ISO_NORTH://move north
		{
			ptStart.y--;
		}break;
	case ISO_NORTHEAST://move northeast
		{
			ptStart.y--;
			ptStart.x++;
		}break;
	case ISO_EAST://move east
		{
			ptStart.x++;
		}break;
	case ISO_SOUTHEAST://move southeast
		{
			ptStart.x++;
			ptStart.y++;
		}break;
	case ISO_SOUTH://move south
		{
			ptStart.y++;
		}break;
	case ISO_SOUTHWEST://move southwest
		{
			ptStart.x--;
			ptStart.y++;
		}break;
	case ISO_WEST://move west
		{
			ptStart.x--;
		}break;
	case ISO_NORTHWEST://move northwest
		{
			ptStart.x--;
			ptStart.y--;
		}break;
	}
	//return the location moved to
	return(ptStart);
}

