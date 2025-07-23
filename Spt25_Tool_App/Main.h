// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

class CMain : public CD3DApplication
{
public:
	LPD3DXFONT			m_pD3DXFont	;
	CLcSprite*			m_pLcSprite	;
	CLcInput*			m_pInput	;

public:
	CLcTile*			m_pTile		;
	CLcChar*			m_pChar		;

public:
	CMain();
    virtual HRESULT Init();
    virtual HRESULT Destroy();

	virtual HRESULT Restore();
    virtual HRESULT Invalidate();

	virtual HRESULT FrameMove();
	virtual HRESULT Render();

	virtual LRESULT	MsgProc(HWND,UINT,WPARAM,LPARAM);
};

extern	CMain*	g_pApp;
#define	GMAIN	g_pApp

#endif