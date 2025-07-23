// Interface for the CLcSwpWn class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcSwpWn_H_
#define _LcSwpWn_H_

class CLcSwpWn								// Swap chain window
{
protected:
	LPDIRECT3DDEVICE9		m_pDev;

	HWND					m_hSwW;			// Window Handle
	LPDIRECT3DSWAPCHAIN9	m_pSwS;			// Swap chain
	LPDIRECT3DSURFACE9		m_pSwC;			// Back Buffer(Color Buffer) Surface
	LPDIRECT3DSURFACE9		m_pSwD;			// Depth and Stencil Buffer Surface	

	LPDIRECT3DSURFACE9		m_pDevC;		// Device Color Buffer
	LPDIRECT3DSURFACE9		m_pDevD;		// Device Depth Stencil Surface


public:
	CLcSwpWn();
	~CLcSwpWn();

	INT		Create(HWND hWnd, LPDIRECT3DDEVICE9	pDev);
	void	Destroy();

	INT		Begin();
	INT		End();

	INT		OnResetDevice();
	INT		OnLostDevice();
};

#endif