// Interface for the CLcTexture class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LCTEXTURE_H_
#define _LCTEXTURE_H_


class CLcTexture
{
protected:
	LPDIRECT3DDEVICE9	m_pDev	;
	
	D3DXIMAGE_INFO		m_ImgInf;
	D3DSURFACE_DESC		m_SrfDsc;
	LPDIRECT3DTEXTURE9	m_pTx	;
	DWORD				m_dColor;
	D3DLOCKED_RECT		m_LockRect;

public:
	CLcTexture();
	virtual ~CLcTexture();

	virtual INT		Create(LPDIRECT3DDEVICE9 pDev, char* sFile, DWORD	dRscID=0, DWORD dFormat=0);
	virtual INT		Create(LPDIRECT3DDEVICE9 pDev, INT nWidth, INT nHeight, INT nMipLevel=1, DWORD dFormat=0, DWORD dUsage =0);
	virtual void	Destroy();

public:
	INT		GetImageWidth();
	INT		GetImageHeight();
	void	GetImageRect(RECT* pRc);
	RECT	GetImageRect();

	INT		GetSurfaceWidth();
	INT		GetSurfaceHeight();
	DWORD	GetSurfaceFormat();

	LPDIRECT3DTEXTURE9	GetTexture();

	void*	LockRect();
	void	UnlockRect();
};

#endif