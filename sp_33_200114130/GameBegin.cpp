// Implementation of the CGameBegin class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CGameBegin::CGameBegin()
{
	m_pTxMain	= NULL;
}

CGameBegin::~CGameBegin()
{
	Destroy();
}


INT CGameBegin::Init()
{
	McUtil_TextureLoad("Texture/main.bmp", m_pTxMain, D3DCOLOR_ARGB(1,1,1,1));
		
	return 1;
}


void CGameBegin::Destroy()
{
	SAFE_RELEASE( m_pTxMain		);
}


INT CGameBegin::FrameMove()
{
	if(GINPUT->KeyState(VK_RETURN))
	{
		GMAIN->SetGameStateCurrent(IGameStage::GMST_PLAY);
		return 0;
	}

	RECT rcStart = {150, 172, 350, 204};
	if(GINPUT->IsInRect(rcStart))
	{
		if(GINPUT->GetMouseSt(0))
			i = 9;
		if(GINPUT->GetMouseSt(1))
		{
			if(i == 9)
			{
				GMAIN->SetGameStateCurrent(IGameStage::GMST_PLAY);
				return 0;
			}
			i = 0;
		}
		else
			return 0;		
	}
	if(!(GINPUT->IsInRect(rcStart)))
		i = 0;

	RECT rcExit = {200, 285, 295, 305};
	if(GINPUT->IsInRect(rcExit))
	{
		if(GINPUT->GetMouseSt(0))
			j = 9;
		if(GINPUT->GetMouseSt(1))
		{
			if(j == 9)
			{
				if(MessageBox(GHWND, "Quit?", "Quit", MB_YESNO)==IDYES)
				{
					PostQuitMessage(0);
					return 0;
				}				
			}
			j = 0;
		}
		else
			return 0;		
	}
	if(!(GINPUT->IsInRect(rcExit)))
		j = 0;

	return 1;
}


void CGameBegin::Render()
{
	GSPRITE->Draw(m_pTxMain, NULL, NULL, &D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1,1,1,1));
}