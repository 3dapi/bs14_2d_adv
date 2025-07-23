// Interface for the CLcSprite class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcSprite_H_
#define _LcSprite_H_

class CLcSprite
{
protected:
	LPD3DXSPRITE	m_pDxSprite;			// 2D DX Sprite

public:
	CLcSprite();
	virtual ~CLcSprite();

	INT		Create(LPD3DXSPRITE);

	INT		Begin(DWORD = D3DXSPRITE_ALPHABLEND);
	INT		End();

	INT		Draw(LPDIRECT3DTEXTURE9 pTex
					, const RECT* pSrcRect
					, const D3DXVECTOR2* pScaling		// Scaling
					, const D3DXVECTOR2* pCenter		// Rotation Center
					, FLOAT	fAngle						// Degree.
					, const D3DXVECTOR2* pPosition		// Translation
					, D3DCOLOR Color
					);


	LPDIRECT3DDEVICE9	GetDevice();
};

#endif


