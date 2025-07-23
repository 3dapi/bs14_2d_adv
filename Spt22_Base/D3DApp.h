// Interface for the CD3DApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _D3DApp_H_
#define _D3DApp_H_


class CD3DApplication
{
protected:
	HINSTANCE		m_hInst			;
	HWND			m_hWnd			;
	HMENU			m_hMenu			;

	DWORD			m_dWinStyle		;
	RECT			m_rcWinBounds	;
	RECT			m_rcWinClient	;

	char			m_sCls[128]		;
	DWORD			m_dScnW			;
	DWORD			m_dScnH			;

public:
	CD3DApplication();
	virtual ~CD3DApplication();

	HRESULT			Create( HINSTANCE);
	INT				Run();
	virtual LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);

	virtual HRESULT Init()				{ return S_OK; }
	virtual HRESULT Destroy()			{ return S_OK; }

public:
	HINSTANCE			GetInstance(){	return m_hInst		;	}
	HWND				GetHwnd()	{	return m_hWnd		;	}
	DWORD				GetScnW()	{	return m_dScnW		;	}
	DWORD				GetScnH()	{	return m_dScnH		;	}
};

#endif