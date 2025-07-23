// TileSet.cpp: implementation of the CTileSet class.
//
//////////////////////////////////////////////////////////////////////

#include "TileSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTileSet::CTileSet()
{
	//set all members to 0 or NULL
	dwTileCount=0;
	ptiTileList=NULL;
	lpddsTileSet=NULL;
	lpszReload=NULL;
}

CTileSet::~CTileSet()
{
	//unload the tileset
	Unload();
}

//load(initializer)
void CTileSet::Load(LPDIRECTDRAW7 lpdd,LPSTR lpszLoad)
{
	//unload any currently loaded tileset
	Unload();

	//copy filename
	lpszReload=strdup(lpszLoad);

	//load in the tileset
	lpddsTileSet=LPDDS_LoadFromFile(lpdd,lpszReload);


	//retrieve DDSURFACEDESC2 from surface
	DDSURFACEDESC2 ddsd;
	DDSD_Clear(&ddsd);
	lpddsTileSet->GetSurfaceDesc(&ddsd);

	//copy width and height
	int iWidth=ddsd.dwWidth;
	int iHeight=ddsd.dwHeight;

	//temporary variables for cell walls
	int* HCellWall=NULL;
	int HCellWallCount=0;
	int* VCellWall=NULL;
	int VCellWallCount=0;
	int xCount=0;
	int yCount=0;

	//grab the DC from the surface
	HDC hdc;
	lpddsTileSet->GetDC(&hdc);

	//temp variables for control colors
	COLORREF crTransparent=GetPixel(hdc,iWidth-1,0);
	COLORREF crWall=GetPixel(hdc,iWidth-1,1);
	COLORREF crAnchor=GetPixel(hdc,iWidth-1,2);
	COLORREF crInside=GetPixel(hdc,iWidth-1,3);
	COLORREF crInsideAnchor=GetPixel(hdc,iWidth-1,4);
	COLORREF crTest=RGB(0,0,0);

	//count the number of vertical cell walls
	//by scanning the top pixel row
	VCellWallCount=0;
	for(xCount=0;xCount<iWidth;xCount++)
	{
		//retrieve the pixel
		crTest=GetPixel(hdc,xCount,0);

		//test for transparent color
		if(crTest==crTransparent)
		{
			//increase wall counter
			VCellWallCount++;
		}
	}

	//allocate the vertical cell wall list
	VCellWall=new int[VCellWallCount];
	
	//find vertical cell walls
	VCellWallCount=0;
	for(xCount=0;xCount<iWidth;xCount++)
	{
		//retrieve the pixel
		crTest=GetPixel(hdc,xCount,0);

		//test for transparent color
		if(crTest==crTransparent)
		{
			//add x value to the list
			VCellWall[VCellWallCount]=xCount;

			//increase wall counter
			VCellWallCount++;
		}
	}

	//count the number of vertical cell walls
	//by scanning the top pixel row
	HCellWallCount=0;
	for(yCount=0;yCount<iHeight;yCount++)
	{
		//retrieve the pixel
		crTest=GetPixel(hdc,0,yCount);

		//test for transparent color
		if(crTest==crTransparent)
		{
			//increase wall counter
			HCellWallCount++;
		}
	}

	//allocate the vertical cell wall list
	HCellWall=new int[HCellWallCount];
	
	//find vertical cell walls
	HCellWallCount=0;
	for(yCount=0;yCount<iHeight;yCount++)
	{
		//retrieve the pixel
		crTest=GetPixel(hdc,0,yCount);

		//test for transparent color
		if(crTest==crTransparent)
		{
			//add x value to the list
			HCellWall[HCellWallCount]=yCount;

			//increase wall counter
			HCellWallCount++;
		}
	}

	//calculate number of rows, number of columns, and number of tiles
	int tilerows=HCellWallCount-1;
	int tilecolumns=VCellWallCount-1;
	int tilenum=0;
	dwTileCount=tilerows*tilecolumns;

	//allocate tile list
	ptiTileList=new TILEINFO[dwTileCount];

	//scan all tiles
	for(int rowcount=0;rowcount<tilerows;rowcount++)
	{
		for(int columncount=0;columncount<tilecolumns;columncount++)
		{
			//tile number
			tilenum=columncount+rowcount*tilecolumns;

			//temporary flag variable
			bool found=false;

			//scan top
			yCount=HCellWall[rowcount];

			//scan top for anchor
			found=false;
			for(xCount=VCellWall[columncount]+1;(!found)&&(xCount<VCellWall[columncount+1]-1);xCount++)
			{
				//grab pixel
				crTest=GetPixel(hdc,xCount,yCount);

				//check pixel for anchor
				if((crTest==crAnchor) || (crTest==crInsideAnchor))
				{
					found=true;
					ptiTileList[tilenum].ptAnchor.x=xCount;
				}
			}

			//scan top for first inside pixel
			found=false;
			for(xCount=VCellWall[columncount]+1;(!found)&&(xCount<VCellWall[columncount+1]-1);xCount++)
			{
				//grab pixel
				crTest=GetPixel(hdc,xCount,yCount);

				//check pixel for anchor
				if((crTest==crInside) || (crTest==crInsideAnchor))
				{
					found=true;
					ptiTileList[tilenum].rcSrc.left=xCount;
				}
			}

			//not found, use default
			if(!found)
			{
				ptiTileList[tilenum].rcSrc.left=VCellWall[columncount]+1;
			}

			//scan top for last inside pixel
			found=false;
			for(xCount=VCellWall[columncount+1]-1;(!found)&&(xCount>=VCellWall[columncount]+1);xCount--)
			{
				//grab pixel
				crTest=GetPixel(hdc,xCount,yCount);

				//check pixel for anchor
				if((crTest==crInside) || (crTest==crInsideAnchor))
				{
					found=true;
					ptiTileList[tilenum].rcSrc.right=xCount+1;
				}
			}

			//not found, use default
			if(!found)
			{
				ptiTileList[tilenum].rcSrc.left=VCellWall[columncount+1];
			}

			//scan left
			xCount=VCellWall[columncount];

			//scan left for anchor
			found=false;
			for(yCount=HCellWall[rowcount]+1;(!found)&&(yCount<HCellWall[rowcount+1]-1);yCount++)
			{
				//grab pixel
				crTest=GetPixel(hdc,xCount,yCount);

				//check pixel for anchor
				if((crTest==crAnchor) || (crTest==crInsideAnchor))
				{
					found=true;
					ptiTileList[tilenum].ptAnchor.y=yCount;
				}
			}

			//scan left for first inside pixel
			found=false;
			for(yCount=HCellWall[rowcount]+1;(!found)&&(yCount<HCellWall[rowcount+1]-1);yCount++)
			{
				//grab pixel
				crTest=GetPixel(hdc,xCount,yCount);

				//check pixel for anchor
				if((crTest==crInside) || (crTest==crInsideAnchor))
				{
					found=true;
					ptiTileList[tilenum].rcSrc.top=yCount;
				}
			}

			//not found, use default
			if(!found)
			{
				ptiTileList[tilenum].rcSrc.top=HCellWall[rowcount]+1;
			}

			//scan left for last inside pixel
			found=false;
			for(yCount=HCellWall[rowcount+1]-1;(!found)&&(yCount>=HCellWall[rowcount]+1);yCount--)
			{
				//grab pixel
				crTest=GetPixel(hdc,xCount,yCount);

				//check pixel for anchor
				if((crTest==crInside) || (crTest==crInsideAnchor))
				{
					found=true;
					ptiTileList[tilenum].rcSrc.bottom=yCount+1;
				}
			}

			//not found, use default
			if(!found)
			{
				ptiTileList[tilenum].rcSrc.bottom=HCellWall[rowcount+1];
			}

			//calculate the destination extents
			CopyRect(&ptiTileList[tilenum].rcDstExt,&ptiTileList[tilenum].rcSrc);
			OffsetRect(&ptiTileList[tilenum].rcDstExt,-ptiTileList[tilenum].ptAnchor.x,-ptiTileList[tilenum].ptAnchor.y);

		}
	}

	//release the dc of the surface
	lpddsTileSet->ReleaseDC(hdc);

	//set the transparent color
	LPDDS_SetSrcColorKey(lpddsTileSet,ConvertColorRef(crTransparent,&ddsd.ddpfPixelFormat));
}

//reload(restore)
void CTileSet::Reload()
{
	//reload image
	LPDDS_ReloadFromFile(lpddsTileSet,lpszReload);
}

//unload(uninitializer)
void CTileSet::Unload()
{
	//safely release the tile list
	if(ptiTileList)
	{
		delete [] ptiTileList;
		ptiTileList=NULL;
		dwTileCount=0;
	}

	//release the surface
	LPDDS_Release(&lpddsTileSet);

	//release the file name
	if(lpszReload)
	{
		free(lpszReload);
		lpszReload=NULL;
	}
}

//get number of tiles
DWORD CTileSet::GetTileCount()
{
	//return the tile count
	return(dwTileCount);
}

//get tile list
TILEINFO* CTileSet::GetTileList()
{
	//return the beginning of the tile list
	return(ptiTileList);
}

//get surface
LPDIRECTDRAWSURFACE7 CTileSet::GetDDS()
{
	//return the source surface
	return(lpddsTileSet);
}

//retrieve filename
LPSTR CTileSet::GetFileName()
{
	//return the file name
	return(lpszReload);
}

//blit a tile
void CTileSet::PutTile(LPDIRECTDRAWSURFACE7 lpddsDst,int xDst,int yDst,int iTileNum)
{
	//offset the desired tile's extent
	OffsetRect(&ptiTileList[iTileNum].rcDstExt,xDst,yDst);
	//blit the tile
	lpddsDst->Blt(&ptiTileList[iTileNum].rcDstExt,lpddsTileSet,&ptiTileList[iTileNum].rcSrc,DDBLT_WAIT | DDBLT_KEYSRC,NULL);
	//offset the desired tile's extent back
	OffsetRect(&ptiTileList[iTileNum].rcDstExt,-xDst,-yDst);
}

