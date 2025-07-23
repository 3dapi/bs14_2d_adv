////////////////////////////////////////////////////////////
//IsoHexDefs.h
//24JUL2000
//Ernest S. Pazera
//fundamental enumerations for use with the IsoHexCore engine
////////////////////////////////////////////////////////////
#ifndef __ISOHEXDEFS_H__
#define __ISOHEXDEFS_H__

////////////////////////////////////////////////////////////
//Enumerations
////////////////////////////////////////////////////////////

//the isometric directions
typedef enum
{
	ISO_NORTH=0,
	ISO_NORTHEAST=1,
	ISO_EAST=2,
	ISO_SOUTHEAST=3,
	ISO_SOUTH=4,
	ISO_SOUTHWEST=5,
	ISO_WEST=6,
	ISO_NORTHWEST=7
} ISODIRECTION;

//directional turning macros
#define ISO_TURNRIGHT(dir,turn) (ISODIRECTION)(((int)(dir)+(turn))&7)
#define ISO_TURNLEFT(dir,turn) (ISODIRECTION)(((int)(dir)+(turn)*7)&7)

//iso map types
typedef enum
{
	ISOMAP_SLIDE,
	ISOMAP_STAGGERED,
	ISOMAP_DIAMOND,
	ISOMAP_RECTANGULAR
} ISOMAPTYPE;

#endif