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

	m_pD3DXFont	= NULL;
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


	D3DXFONT_DESC hFont =
	{
		12, 0
		, FW_BOLD, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE
		, "Arial"
	};

	D3DXCreateFontIndirect(m_pDev, &hFont, &m_pD3DXFont);


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

	SAFE_RELEASE(	m_pD3DXFont	);
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
	HWND hWnd = GetActiveWindow();

	if(GHWND == hWnd)
	{
		if(GINPUT->BtnPress(0))
		{
			D3DXVECTOR3 vcD = 5*GINPUT->GetMouseEps();
			m_vcP.x += m_nTileWidth* int(vcD.x/m_nTileWidth);
			m_vcP.y += m_nTileWidth* int(vcD.y/m_nTileWidth);
//			m_vcP.x += vcD.x;
//			m_vcP.y += vcD.y;
		}
	}

	return 1;
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


	if(GMAIN->m_bIdx)
	{
		for(y=iBgnY; y<m_nTileNumY; ++y)
		{
			for(x=iBgnX; x<m_nTileNumX; ++x)
			{
				CLcTile::LcTile*	pTile = &m_pTile[y][x];

				D3DXVECTOR2	vcP(0,0);

				vcP	= m_vcP + pTile->p;
				
				char	sMsg[128];
				RECT	rc;

				rc.left		= LONG(vcP.x);
				rc.right	= LONG(vcP.x+m_nTileWidth);
				rc.top		= LONG(vcP.y);
				rc.bottom	= LONG(vcP.y +m_nTileWidth);

				sprintf(sMsg, "%d", m_pTile[y][x].nIdx);
				m_pD3DXFont->DrawText(NULL, sMsg, -1, &rc, 0, D3DXCOLOR(1,1,0,1));
			}
		}
	}
}





void CLcTile::Save()
{
	INT x, y;

	FILE*	fp;

	char	sFile[256];
	sprintf(sFile, "data/%s", m_sMap);

	fp = fopen(sFile, "wt");

	fprintf(fp,"Texture		%s", m_sBmp);
	fprintf(fp,"TileWidth	%d", &m_nTileWidth);
	fprintf(fp,"TileMaxX	%d", &m_nTileMaxX);
	fprintf(fp,"TileMaxY	%d", &m_nTileMaxY);
	fprintf(fp,"TileNumX	%d", &m_nTileNumX);
	fprintf(fp,"TileNumY	%d", &m_nTileNumY);


	for(y=0; y<m_nTileNumY; ++y)
	{
		for(x=0; x<m_nTileNumX; ++x)
		{
			CLcTile::LcTile*	pTile = &m_pTile[y][x];

			INT idx = pTile->nIdx;

			fprintf(fp, "%5d", idx);
		}

		fprintf(fp, "\n");
	}

	fclose(fp);
}

