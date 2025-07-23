// Implementation of the CLcSprite class.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "LcSprite.h"


CLcSprite::CLcSprite()
{
	m_pDxSprite	= NULL;
}

CLcSprite::~CLcSprite()
{

}

INT CLcSprite::Create(LPD3DXSPRITE pSprite)
{
	m_pDxSprite = pSprite;

	return 0;
}

INT CLcSprite::Begin(DWORD Flags)
{
	return m_pDxSprite->Begin(Flags);
}
INT CLcSprite::End()
{
	return m_pDxSprite->End();
}


INT CLcSprite::Draw(LPDIRECT3DTEXTURE9 pTex
					, const RECT* pSrcRect
					, const D3DXVECTOR2* pScaling		// Scaling
					, const D3DXVECTOR2* pCenter		// Rotation Center
					, FLOAT	fAngle						// Degree.
					, const D3DXVECTOR2* pPosition		// Translation
					, D3DCOLOR Color
					)
{
	

	D3DXMATRIX mtW;
	D3DXMatrixIdentity(&mtW);

	FLOAT			fTheta = D3DXToRadian(fAngle);
	D3DXMATRIX		mtScl;		// Scaling Matrix
	D3DXMATRIX		mtRot;		// Rotation Matrix
	D3DXMATRIX		mtTrn;		// Translation Matrix
	D3DXMATRIX		mtRct;		// Rotation Center Matrix
	D3DXMATRIX		mtRctI;		// Rotation Center Matrix Inverse

	D3DXMatrixIdentity(&mtScl);
	D3DXMatrixIdentity(&mtRct);
	D3DXMatrixIdentity(&mtRot);
	D3DXMatrixIdentity(&mtTrn);
	D3DXMatrixIdentity(&mtRctI);

	if(pScaling)
		D3DXMatrixScaling(&mtScl, pScaling->x, pScaling->y, 1);

	
	D3DXMatrixRotationZ(&mtRot, -fTheta);

	
	if(pCenter)
		D3DXMatrixTranslation(&mtRct, pCenter->x, pCenter->y, 0);


	//	D3DXMatrixInverse(&mtRctI, NULL, &mtRct);
	if(pCenter)
		D3DXMatrixTranslation(&mtRctI, -pCenter->x, -pCenter->y, 0);


	if(pPosition)
	D3DXMatrixTranslation(&mtTrn, pPosition->x, pPosition->y, 0);

	
	mtW =  mtScl * mtRctI * mtRot * mtRct * mtTrn;

	
	m_pDxSprite->SetTransform(&mtW);
	m_pDxSprite->Draw(pTex, pSrcRect, NULL, NULL, Color);

	D3DXMatrixIdentity(&mtW);
	m_pDxSprite->SetTransform(&mtW);

	return 0;
}




LPDIRECT3DDEVICE9 CLcSprite::GetDevice()
{
	HRESULT hr=0;
	LPDIRECT3DDEVICE9	pDev=NULL;
	
	hr = m_pDxSprite->GetDevice(&pDev);
	pDev->Release();

	return pDev;
}