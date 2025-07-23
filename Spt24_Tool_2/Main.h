// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
public:
	CLcSprite*			m_pLcSprite	;
	CLcInput*			m_pInput	;

	LPD3DXFONT			m_pD3DXFont	;
	LPD3DXFONT			m_pFntIdx	;

	HWND				m_hWrk		;
	HWND				m_hWrk1		;

	HWND				m_hWrkTb	;											// Base Work Tab
	HWND				m_hImgSc	;

	HIMAGELIST			m_IL		;											// Image List for Tab and List control
	INT					m_nTab		;

	CLcSwpWn			m_SpWnd		;											// Swap Chain Window

public:
	CLcTexture*			m_pTx		;

	RECT*				m_prcSc		;
	INT					m_iImgX		;
	INT					m_iImgY		;
	INT					m_iW		;

	INT					m_bIdx		;
	D3DXVECTOR2			m_vcScI		;
	D3DXVECTOR2			m_vcScM		;
	INT					m_nImgSc	;											// Source Index
	
	RECT				m_rcDs		;

	INT					m_nImgDs	;											// Dest Index
	RECT				m_rcMkSc	;
	CLcTile*			m_pTile		;

public:
	CMain();
    virtual HRESULT Init();
    virtual HRESULT Destroy();

	virtual HRESULT FrameMove();
	virtual HRESULT Render();

	virtual LRESULT	MsgProc(HWND,UINT,WPARAM,LPARAM);

	INT				UpdateSc();

	static INT_PTR CALLBACK WndWrk	(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK WndWrk1	(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK WndImgSc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK WndCrt	(HWND, UINT, WPARAM, LPARAM);
};

extern	CMain*	g_pApp;
#define	GMAIN	g_pApp

#endif