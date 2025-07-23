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

	m_nImgDs	= -1;

	m_vcPds		= D3DXVECTOR2(0,0);
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

	if(FAILED(LoadMapTxt()))
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


	return 0;
}


INT CLcTile::FrameMove()
{
	HWND hWnd = GetActiveWindow();

	if(GHWND == hWnd)
	{
		if(GINPUT->KeyState(VK_LEFT))
			m_vcPds.x += 32;

		if(GINPUT->KeyState(VK_RIGHT))
			m_vcPds.x -= 32;

		if(GINPUT->KeyState(VK_DOWN))
			m_vcPds.y -= 32;

		if(GINPUT->KeyState(VK_UP))
			m_vcPds.y += 32;

		if(m_vcPds.x> INT(GMAIN->GetScnW()) -32)
			m_vcPds.x -= 32;

		if(m_vcPds.x< -(m_nTileNumX-1)*32)
			m_vcPds.x += 32;

		if(m_vcPds.y> INT(GMAIN->GetScnH())-32)
			m_vcPds.y -= 32;

		if(m_vcPds.y< -(m_nTileNumY-1)*32)
			m_vcPds.y += 32;

		if(GINPUT->BtnPress(0))
		{
			int nIdx = GMAIN->m_nImgSc;
			D3DXVECTOR3 vcP = GINPUT->GetMousePos();
			FLOAT	x,y;
			int		iX;
			int		iY;

			x = -m_vcPds.x + vcP.x;
			y = -m_vcPds.y + vcP.y;


			iX	= int(x/32.f);
			iY	= int(y/32.f);

			if(GINPUT->KeyState(VK_LCONTROL) && iX>=0 && iX<m_nTileNumX && iY>=0 && iY<m_nTileNumY)
			{
				m_vcDsM.x = iX * 32.f;
				m_vcDsM.y = iY * 32.f;


				m_nImgDs	= iX + GMAIN->m_iImgX * iY;

				m_pTile[iY][iX].nIdx = nIdx;

				SetRect(&GMAIN->m_prcSc[m_nImgDs], iX*GMAIN->m_iW, iY*GMAIN->m_iW, (iX+1)*GMAIN->m_iW, (iY+1)*GMAIN->m_iW);

				LcUtil_SetWindowTitle("%d", m_nImgDs);
			}
		}
	}

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
			
			INT idx = pTile->nIdx;
			vcP	= m_vcPds + pTile->p;

			m_pLcSprite->Draw(GMAIN->m_pTx->GetTexture()
								, &GMAIN->m_prcSc[idx]
								, NULL
								, NULL
								, 0
								, &vcP
								, D3DXCOLOR(1,1,1,1));
		}
	}

	m_pLcSprite->End();

}





void CLcTile::SaveMapTxt(char* sFile)
{
	INT x, y;

	FILE*	fp;

	if(sFile)
		fp = fopen(sFile, "wt");
	else
		fp = fopen(m_sMap, "wt");
	

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
}

void CLcTile::CreateMap(INT x, INT y)
{
	if(x<1 || x>128 || y<1 || y>128)
		return;
	
	Destroy();

	m_nTileNumX	= x;
	m_nTileNumY	= y;

	m_pTile = new CLcTile::LcTile*[m_nTileNumY];


	for(y=0; y<m_nTileNumY; ++y)
		m_pTile[y] = new CLcTile::LcTile[m_nTileNumX];


	SaveMapTxt(m_sMap);
}