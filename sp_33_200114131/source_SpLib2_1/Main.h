
#ifndef _MAIN_H_
#define _MAIN_H_


class CMain: public CD3DApplication
{
protected:
	ID3DXFont*              m_pD3DXFont;            // D3DX font

public:
	CMain();

    virtual HRESULT Init();
	virtual HRESULT Destroy();

    virtual HRESULT Restore();
    virtual HRESULT Invalid();

	virtual HRESULT FrameMove();
    virtual HRESULT Render();

    virtual LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);
};


extern CMain*	g_pApp;

#endif