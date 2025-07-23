// Implementation of the CLcTile class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CLcTile::CLcTile()
{
	m_pDev		= NULL;
	m_pLcSprite	= NULL;

	m_nTileWidth= 0;
	m_nTileMaxX	= 0;
	m_nTileMaxY	= 0;
	m_nTileNumX	= 0;
	m_nTileNumY	= 0;

	m_pTile		= NULL;
	m_pTxMap	= NULL;

	m_vcPds		= D3DXVECTOR2(-64,-160);
}


CLcTile::~CLcTile()
{
	Destroy();
}


void CLcTile::Destroy()
{
	if(m_pTile)
	{
		for(int y=0; y<m_nTileNumY; ++y)
		{
			SAFE_DELETE_ARRAY(	m_pTile[y]	);
		}

		SAFE_DELETE_ARRAY(	m_pTile	);
	}

	SAFE_DELETE(	m_pTxMap	);
}



INT CLcTile::Create(char* sFile, void* pSprite)
{
	m_pLcSprite = (CLcSprite*)pSprite;
	m_pDev = m_pLcSprite->GetDevice();

	strcpy(m_sMap, sFile);

	if(FAILED(LoadMapTxt()))
		return -1;

	if(FAILED(SaveMapBin("data/mapinf.bin")))
		return -1;

	if(FAILED(LoadMapBin("data/mapinf.bin")))
		return -1;

	SAFE_NEWCREATE2(m_pTxMap, CLcTexture, m_pDev, m_sBmp);

	return 0;
}



INT CLcTile::FrameMove()
{
	return 0;
}


void CLcTile::Render()
{
	D3DXVECTOR2	vcP;
	int			x,y;

	int			iBgnX	= -int(m_vcPds.x/32);
	int			iBgnY	= -int(m_vcPds.y/32);

	if(iBgnX<0)
		iBgnX=0;

	if(iBgnY<0)
		iBgnY=0;


	m_pLcSprite->Begin();
	
	for(y=iBgnY; y<m_nTileNumY; ++y)
	{
		for(x=iBgnX; x<m_nTileNumX; ++x)
		{
			CLcTile::LcTile*	pTile = &m_pTile[y][x];

			vcP	= m_vcPds + pTile->p;

			m_pLcSprite->Draw(m_pTxMap->GetTexture()
							, &pTile->rc
							, NULL
							, NULL
							, 0
							, &vcP
							, D3DXCOLOR(1,1,1,1));
		}
	}

	m_pLcSprite->End();

}



INT CLcTile::LoadMapTxt()
{
	FILE*	fp;

	fp = fopen(m_sMap, "rt");

	if(NULL == fp)
		return -1;

	int x,y;

	fscanf(fp,"%*s %s", m_sBmp);
	fscanf(fp,"%*s %d", &m_nTileWidth);
	fscanf(fp,"%*s %d", &m_nTileMaxX);
	fscanf(fp,"%*s %d", &m_nTileMaxY);
	fscanf(fp,"%*s %d", &m_nTileNumX);
	fscanf(fp,"%*s %d", &m_nTileNumY);

	m_pTile = new CLcTile::LcTile*[m_nTileNumY];


	for(y=0; y<m_nTileNumY; ++y)
	{
		m_pTile[y] = new CLcTile::LcTile[m_nTileNumX];

		for(x=0; x<m_nTileNumX; ++x)
		{
			fscanf(fp,"%d", &m_pTile[y][x].nIdx);
		}
	}
	
	fclose(fp);


	for(y=0; y<m_nTileNumY; ++y)
	{
		for(x=0; x<m_nTileNumX; ++x)
		{
			CLcTile::LcTile*	pTile = &m_pTile[y][x];

			INT	nX	= pTile->nIdx%m_nTileMaxX;
			INT	nY	= pTile->nIdx/m_nTileMaxX;

			pTile->p.x	= FLOAT(x*m_nTileWidth);
			pTile->p.y	= FLOAT(y*m_nTileWidth);

			pTile->rc.left	= nX*m_nTileWidth;
			pTile->rc.top	= nY*m_nTileWidth;
			pTile->rc.right	= nX*m_nTileWidth+m_nTileWidth;
			pTile->rc.bottom= nY*m_nTileWidth+m_nTileWidth;
		}
	}

	m_iTotx = m_nTileWidth * m_nTileNumX;
	m_iToty = m_nTileWidth * m_nTileNumY;

	return 0;
}


INT CLcTile::SaveMapTxt(char* sFile)
{
	INT x, y;

	FILE*	fp;

	if(sFile)
		fp = fopen(sFile, "wt");
	else
		fp = fopen(m_sMap, "wt");

	if(!fp)
		return -1;
	

	fprintf(fp,"Texture		%s\n", m_sBmp);
	fprintf(fp,"TileWidth	%d\n", m_nTileWidth);
	fprintf(fp,"TileMaxX	%d\n", m_nTileMaxX);
	fprintf(fp,"TileMaxY	%d\n", m_nTileMaxY);
	fprintf(fp,"TileNumX	%d\n", m_nTileNumX);
	fprintf(fp,"TileNumY	%d\n", m_nTileNumY);


	for(y=0; y<m_nTileNumY; ++y)
	{
		for(x=0; x<m_nTileNumX; ++x)
		{
			INT idx = m_pTile[y][x].nIdx;

			fprintf(fp, "%5d", idx);
		}

		fprintf(fp, "\n");
	}

	fclose(fp);

	return 0;
}


INT CLcTile::LoadMapBin(char* sFile)
{
	FILE*	fp;

	if(sFile)
		strcpy(m_sMap, sFile);

	fp = fopen(m_sMap, "rb");

	if(NULL == fp)
		return -1;

	int x,y;

	fread(m_sBmp, sizeof(char), sizeof(m_sBmp), fp);
	fread(&m_nTileWidth,sizeof(int), 1, fp);
	fread(&m_nTileMaxX, sizeof(int), 1, fp);
	fread(&m_nTileMaxY, sizeof(int), 1, fp);
	fread(&m_nTileNumX, sizeof(int), 1, fp);
	fread(&m_nTileNumY, sizeof(int), 1, fp);

	if(m_pTile)
	{
		for(int y=0; y<m_nTileNumY; ++y)
		{
			SAFE_DELETE_ARRAY(	m_pTile[y]	);
		}

		SAFE_DELETE_ARRAY(	m_pTile	);
	}


	m_pTile = new CLcTile::LcTile*[m_nTileNumY];

	for(y=0; y<m_nTileNumY; ++y)
	{
		m_pTile[y] = new CLcTile::LcTile[m_nTileNumX];

		fread(m_pTile[y], sizeof(CLcTile::LcTile), m_nTileNumX, fp);
	}
	
	fclose(fp);


	for(y=0; y<m_nTileNumY; ++y)
	{
		for(x=0; x<m_nTileNumX; ++x)
		{
			CLcTile::LcTile*	pTile = &m_pTile[y][x];

			INT	nX	= pTile->nIdx%m_nTileMaxX;
			INT	nY	= pTile->nIdx/m_nTileMaxX;

			pTile->p.x	= FLOAT(x*m_nTileWidth);
			pTile->p.y	= FLOAT(y*m_nTileWidth);

			pTile->rc.left	= nX*m_nTileWidth;
			pTile->rc.top	= nY*m_nTileWidth;
			pTile->rc.right	= nX*m_nTileWidth+m_nTileWidth;
			pTile->rc.bottom= nY*m_nTileWidth+m_nTileWidth;
		}
	}

	m_iTotx = m_nTileWidth * m_nTileNumX;
	m_iToty = m_nTileWidth * m_nTileNumY;

	return 0;
}


INT CLcTile::SaveMapBin(char* sFile)
{
	FILE*	fp;

	if(sFile)
		fp = fopen(sFile, "wb");
	else
		fp = fopen(m_sMap, "wb");
	
	if(!fp)
		return -1;

	fwrite(m_sBmp, sizeof(char), sizeof(m_sBmp), fp);
	fwrite(&m_nTileWidth,sizeof(int), 1, fp);
	fwrite(&m_nTileMaxX, sizeof(int), 1, fp);
	fwrite(&m_nTileMaxY, sizeof(int), 1, fp);
	fwrite(&m_nTileNumX, sizeof(int), 1, fp);
	fwrite(&m_nTileNumY, sizeof(int), 1, fp);

	for(int y=0; y<m_nTileNumY; ++y)
	{
		fwrite(m_pTile[y], sizeof(CLcTile::LcTile), m_nTileNumX, fp);
	}

	fclose(fp);

	return 0;
}