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
	
	m_vcP		= D3DXVECTOR2(0,0);
	m_pTxMap	= NULL;
}


CLcTile::~CLcTile()
{
	Destroy();
}


INT CLcTile::Create(char* sFile, void* pSprite)
{
	m_pLcSprite = (CLcSprite*)pSprite;
	m_pDev = m_pLcSprite->GetDevice();

	strcpy(m_sMap, sFile);

	if(FAILED(LoadMap()))
		return -1;

	if( FAILED( LcUtil_TextureLoad(m_pDev, m_sBmp, &m_pTxMap, &m_TxImg, 0xFF000000) ) )
		return -1;

		
	return 0;
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
}





INT CLcTile::LoadMap()
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


	return 0;
}


INT CLcTile::FrameMove()
{
	return 0;
}


void CLcTile::Render()
{
	int			x,y;

	int			iBgnX	= -int(m_vcP.x/32);
	int			iBgnY	= -int(m_vcP.y/32);

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
			
			D3DXVECTOR2	vcP(0,0);

			vcP	= m_vcP + pTile->p;
			m_pLcSprite->Draw(m_pTxMap, &pTile->rc, NULL, NULL, 0, &vcP, D3DXCOLOR(1,1,1,1));
		}
	}

	m_pLcSprite->End();
}





