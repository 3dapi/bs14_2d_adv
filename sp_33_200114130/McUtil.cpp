// Implementation of the Utilities.
//
////////////////////////////////////////////////////////////////////////////////

#include "_StdAfx.h"


INT	McUtil_TextureLoad(TCHAR* sFile, LPDIRECT3DTEXTURE9& pTx, DWORD dc)
{
	if(FAILED(D3DXCreateTextureFromFileEx(
		GDEVICE
		, sFile
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, D3DX_FILTER_NONE
		, D3DX_FILTER_NONE
		, dc
		, NULL
		, NULL
		, &pTx
		)) )
	{
		MessageBox(0, "Create Texture Failed", "Err", MB_ICONEXCLAMATION);
		pTx = NULL;
		return -1;
	}

	return 1;
}
