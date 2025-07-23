// Interface for the CD3DApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _D3DApp_H_
#define _D3DApp_H_


class CD3DApplication
{
protected:
	HINSTANCE				m_hInst			;
	HWND					m_hWnd			;
	HMENU					m_hMenu			;

	DWORD					m_dWinStyle		;
	RECT					m_rcWinBounds	;
	RECT					m_rcWinClient	;

	char					m_sCls[128]		;
	DWORD					m_dScnW			;
	DWORD					m_dScnH			;

	D3DPRESENT_PARAMETERS	m_d3dParam		;
	LPDIRECT3D9             m_pD3D			;
	LPDIRECT3DDEVICE9       m_pd3dDevice	;
	LPD3DXSPRITE			m_pd3dSprite	;

	FLOAT					m_fFps			;
	BOOL					m_bDestroyed	;

public:
	CD3DApplication();
	virtual ~CD3DApplication();

	HRESULT			Create( HINSTANCE);
	INT				Run();
	void			Cleanup();
	virtual LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);
	static	LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM);

	virtual HRESULT Init()				{ return 0; }
	virtual HRESULT Destroy()			{ return 0; }
	virtual HRESULT FrameMove()			{ return 0; }
	virtual HRESULT Render()			{ return 0; }

protected:
	void			UpdateFrame();

public:
	HINSTANCE			GetInstance(){	return m_hInst		;	}
	HWND				GetHwnd()	{	return m_hWnd		;	}
	DWORD				GetScnW()	{	return m_dScnW		;	}
	DWORD				GetScnH()	{	return m_dScnH		;	}

	LPDIRECT3DDEVICE9	GetDevice()	{	return m_pd3dDevice	;	}
	LPD3DXSPRITE		GetSprite()	{	return m_pd3dSprite	;	}
};

#endif