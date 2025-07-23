// Interface for the CLcTile class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcTile_H_
#define _LcTile_H_


class CLcTile
{
public:
	struct LcTile
	{
		INT			nIdx;		// Tile Index
		RECT		rc	;		// Tile Image Rect
		D3DXVECTOR2	p	;		// Tile Position

		DWORD		dAtt;

		LcTile()
		{
			nIdx		=-1;
			rc.left		= 0;
			rc.bottom	= 0;
			p.x			= 0;
			p.y			= 0;

			dAtt		= 0;
		}
	};

protected:
	LPDIRECT3DDEVICE9	m_pDev		;
	CLcSprite*			m_pLcSprite	;
	
	INT					m_nTileWidth;	// Tile Width
	INT					m_nTileMaxX	;	// Max Tile Number of X
	INT					m_nTileMaxY	;	// Max Tile Number of Y
	INT					m_nTileNumX	;	// Tile Number of X
	INT					m_nTileNumY	;	// Tile Number of Y

	LcTile**			m_pTile		;	// Tiles

	char				m_sMap[256]	;
	char				m_sBmp[256]	;

	CLcTexture*			m_pTxMap	;

public:
	D3DXVECTOR2			m_vcPds		;

	INT					m_iTotx		;
	INT					m_iToty		;



public:
	CLcTile();
	virtual ~CLcTile();

	INT		Create(char* sFile, void* pSprite);
	void	Destroy();

	INT		FrameMove();
	void	Render();

protected:
	INT		LoadMapTxt();
	INT		LoadMapBin(char* sFile);
	INT		SaveMapTxt(char* sFile);
	INT		SaveMapBin(char* sFile);
};

#endif