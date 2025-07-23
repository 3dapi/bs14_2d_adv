// Implementation of the CGameEnd class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CGameEnd::CGameEnd()
{
}

CGameEnd::~CGameEnd()
{
	Destroy();
}


INT CGameEnd::Init()
{
	McUtil_TextureLoad("Texture/END.bmp", m_pTxEND, D3DCOLOR_ARGB(1,1,1,1));
	
	return 1;
}


void CGameEnd::Destroy()
{
	SAFE_RELEASE( m_pTxEND );
}


INT CGameEnd::FrameMove()
{
	if(GINPUT->KeyState(VK_RETURN))
	{
		PostQuitMessage(0);
		return 0;
	}

	return 1;
}


void CGameEnd::Render()
{
	RECT rtMain = {0, 0, 504, 504};
	GSPRITE->Draw(m_pTxEND, &rtMain, NULL, &D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1,1,1,1));
}