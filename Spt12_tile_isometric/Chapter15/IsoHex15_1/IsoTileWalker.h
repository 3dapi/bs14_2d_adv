//IsoTileWalker.h
#ifndef __ISOTILEWALKER_H__
#define __ISOTILEWALKER_H__

#include "IsoHexDefs.h"
#include <windows.h>

//typedef for a function pointer to a tilewalker function
typedef POINT (*ISOHEXTILEWALKERFN)(POINT ptStart,ISODIRECTION IsoDirection);

//tilewalker class
class CTileWalker
{
private:
	//tile walker function pointer
	ISOHEXTILEWALKERFN IsoHexTileWalkerFn;

	//iso map type
	ISOMAPTYPE IsoMapType;
public:
	//constructor
	CTileWalker();
	//destructor
	~CTileWalker();

	//map type
	void SetMapType(ISOMAPTYPE IsoMapType);
	ISOMAPTYPE GetMapType();

	//tile walking
	POINT TileWalk(POINT ptStart,ISODIRECTION IsoDirection);
};

#endif