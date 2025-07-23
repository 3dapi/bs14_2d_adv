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
	
	D3DXVECTOR2			m_vcP		;	// Map Start Postion

public:
	D3DXIMAGE_INFO		m_TxImg		;
	LPDIRECT3DTEXTURE9	m_pTxMap	;

protected:
	LPD3DXFONT			m_pD3DXFont	;
	
public:
	CLcTile();
	virtual ~CLcTile();

	INT		Create(char* sFile, void* pSprite);
	void	Destroy();

	INT		FrameMove();
	void	Render();

	void	Save();

protected:
	INT		LoadMap();
};

#endif