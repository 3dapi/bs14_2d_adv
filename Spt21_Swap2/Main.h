// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
public:
	HWND				m_hImgSc	;
	CLcSwpWn			m_SpW		;		// Swap Chain Window

protected:
	LPDIRECT3DTEXTURE9	m_pTx1		;		// Dx texture

public:
	CMain();
    virtual HRESULT Init();
    virtual HRESULT Destroy();
	virtual HRESULT FrameMove();
	virtual HRESULT Render();

	virtual LRESULT	MsgProc(HWND,UINT,WPARAM,LPARAM);
	static INT_PTR CALLBACK WndImgSrc(HWND, UINT, WPARAM, LPARAM);
};

extern	CMain*	g_pApp;
#define	GMAIN	g_pApp

#endif