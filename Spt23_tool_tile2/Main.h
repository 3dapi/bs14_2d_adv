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
	CLcTexture*			m_pTxTile	;
	CLcSwpWn			m_SpWnd		;											// Swap Chain Window
	HWND				m_hwWrk		;
	HWND				m_hwWrk1	;

	HWND				m_hwWrkTb	;											// Base Work Tab
	HWND				m_hwImgSc	;

	HIMAGELIST			m_IL		;											// Image List for Tab and List control
	INT					m_nTab		;

public:
	CMain();
    virtual HRESULT Init();
    virtual HRESULT Destroy();

	virtual HRESULT FrameMove();
	virtual HRESULT Render();

	virtual LRESULT	MsgProc(HWND,UINT,WPARAM,LPARAM);

	static INT_PTR CALLBACK WndWrk	(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK WndWrk1	(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK WndImgSrc(HWND, UINT, WPARAM, LPARAM);
};


extern	CMain*	g_pApp;
#define	GMAIN	g_pApp


#endif

