// Implementation of the Utilities.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


INT	LcUtil_TextureLoad(LPDIRECT3DDEVICE9 pDev
					   , char* sFile
					   , LPDIRECT3DTEXTURE9* pTx
					   , D3DXIMAGE_INFO *pInf
					   , DWORD dColorKey
					   , DWORD dFilter)
{
	UINT MipLevels	= D3DX_DEFAULT;

	if(dFilter < D3DX_FILTER_LINEAR)
		MipLevels	= 1;

	if(FAILED(D3DXCreateTextureFromFileEx(
		pDev
		, sFile
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, MipLevels
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, dFilter
		, dFilter
		, dColorKey
		, pInf
		, NULL
		, pTx
		)) )
	{
		pTx = NULL;
		return -1;
	}

	return 0;
}
