// Interface for the CLcChar class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _LcChar_H_
#define _LcChar_H_

class CLcChar
{
protected:
	CLcSprite*	m_pLcSprite	;
	CLcTexture*	m_pTex;

	RECT		m_rcA[9][7];

public:
	INT			m_iY;
	INT			m_iX;
	DWORD		m_dBgn;
	D3DXVECTOR2	m_vcP;
	D3DXVECTOR2	m_vcS;
	FLOAT		m_fSp;

	INT			m_nAni;


public:
	CLcChar();
	virtual ~CLcChar();

	INT		Create(CLcSprite* pSprite, char* sTex);
	void	Destroy();

	INT		FrameMove();
	void	Render();

};

#endif