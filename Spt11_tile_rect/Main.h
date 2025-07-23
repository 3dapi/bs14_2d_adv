// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

class CMain : public CD3DApplication
{
public:
	CLcSprite*			m_pLcSprite	;
	LPD3DXFONT			m_pD3DXFont	;
	CLcInput*			m_pInput	;

	CLcTile*			m_pTile		;

public:
	CMain();
    virtual HRESULT Init();
    virtual HRESULT Destroy();

	virtual HRESULT FrameMove();
	virtual HRESULT Render();

	virtual LRESULT	MsgProc(HWND,UINT,WPARAM,LPARAM);
};


extern	CMain*	g_pApp;
#define	GMAIN	g_pApp


#endif

