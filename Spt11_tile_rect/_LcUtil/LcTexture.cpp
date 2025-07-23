// LcTexture.cpp: implementation of the CLcTexture class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#include "LcTexture.h"


CLcTexture::CLcTexture()
{
	m_pDev	= NULL;
	m_pTx	= NULL;

	memset(&m_ImgInf, 0, sizeof m_ImgInf);

	m_dColor	= 0;
}

CLcTexture::~CLcTexture()
{
	Destroy();
}


void CLcTexture::Destroy()
{
	if(m_pTx)
	{
		m_pTx->Release();
		m_pTx= NULL;
	}
}


INT CLcTexture::Create(LPDIRECT3DDEVICE9 pDev, char* sFile, DWORD	dRsc, DWORD dFormat)
{
	INT hr=0;

	m_pDev	= pDev;

	m_dColor	= 0x00FFFFFF;

	if(sFile && strlen(sFile)>6)
	{
		m_dColor	= dRsc;

		hr = D3DXCreateTextureFromFileEx(
			m_pDev
			, sFile
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, 1
			, 0
			, (D3DFORMAT)dFormat
			, D3DPOOL_MANAGED
			, D3DX_FILTER_NONE
			, D3DX_FILTER_NONE
			, m_dColor
			, &m_ImgInf
			, NULL
			, &m_pTx
			);

		if(FAILED(hr))
		{
			m_pTx = NULL;
			MessageBox( GetActiveWindow()
						, "Create Texture Failed"
						, "Err"
						, MB_ICONEXCLAMATION
						);
			return -1;
		}
	}
	
	else if(dRsc)
	{
		HINSTANCE	hInst	= GetModuleHandle(NULL);
		HRSRC		hrsc	= FindResource(hInst, MAKEINTRESOURCE( dRsc ), "png");
		HGLOBAL		hglobal = LoadResource(hInst, hrsc);
		DWORD		dwSize	= SizeofResource(hInst,hrsc);
		void*		pMemory = LockResource(hglobal);

		hr = D3DXCreateTextureFromFileInMemoryEx(
			m_pDev
			, pMemory
			, dwSize
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, 1
			, 0
			, D3DFMT_UNKNOWN
			, D3DPOOL_MANAGED
			, D3DX_FILTER_NONE
			, D3DX_FILTER_NONE
			, m_dColor
			, &m_ImgInf
			, NULL
			, &m_pTx
			);

		if(FAILED(hr))
		{
			m_pTx = NULL;
			MessageBox( GetActiveWindow()
						, "Create Texture Failed"
						, "Err"
						, MB_ICONEXCLAMATION
						);
			return -1;
		}

		UnlockResource(hglobal);
		FreeResource(hglobal);
	}


	hr = m_pTx->GetLevelDesc(0, &m_SrfDsc);
	hr = m_pTx->LockRect(0, &m_LockRect, NULL, 0);
	hr = m_pTx->UnlockRect(0);

	return 0;
}



INT CLcTexture::Create(LPDIRECT3DDEVICE9 pDev, INT nWidth, INT nHeight, INT nMipLevel, DWORD dFormat, DWORD dUsage)
{
	INT hr = 0;

	m_pDev	= pDev;

	hr = D3DXCreateTexture(
		m_pDev
		, nWidth
		, nHeight
		, nMipLevel
		, dUsage
		, (D3DFORMAT)dFormat
		, D3DPOOL_MANAGED
		, &m_pTx
		);

	if(FAILED(hr))
	{
		m_pTx = NULL;
		MessageBox( GetActiveWindow()
					, "Create Texture Failed"
					, "Err"
					, MB_ICONEXCLAMATION
					);
		return -1;
	}

	m_ImgInf.Width		= nWidth;
    m_ImgInf.Height		= nHeight;
    m_ImgInf.Depth		= 0;
    m_ImgInf.MipLevels	= nMipLevel;
    m_ImgInf.Format		= (D3DFORMAT)dFormat;
    m_ImgInf.ResourceType = D3DRTYPE_TEXTURE;


	hr = m_pTx->GetLevelDesc(0, &m_SrfDsc);
	hr = m_pTx->LockRect(0, &m_LockRect, NULL, 0);
	hr = m_pTx->UnlockRect(0);
	
	return 0;
}


INT CLcTexture::GetImageWidth()
{
	return m_ImgInf.Width;
}


INT CLcTexture::GetImageHeight()
{
	return m_ImgInf.Height;
}

void CLcTexture::GetImageRect(RECT* pRc)
{
	pRc->left	= 0;
	pRc->top	= 0;
	pRc->right	= m_ImgInf.Width;
	pRc->bottom	= m_ImgInf.Height;
}

RECT CLcTexture::GetImageRect()
{
	RECT rc={0,0, m_ImgInf.Width, m_ImgInf.Height};
	return rc;
}


LPDIRECT3DTEXTURE9 CLcTexture::GetTexture()
{
	return m_pTx;
}


INT CLcTexture::GetSurfaceWidth()
{
	return m_SrfDsc.Width;
}

INT CLcTexture::GetSurfaceHeight()
{
	return m_SrfDsc.Height;
}


DWORD CLcTexture::GetSurfaceFormat()
{
	return m_SrfDsc.Format;
}



void*  CLcTexture::LockRect()
{
	m_pTx->LockRect(0, &m_LockRect, NULL, 0);
	
	return m_LockRect.pBits;
}

void  CLcTexture::UnlockRect()
{
	m_pTx->UnlockRect(0);
}
