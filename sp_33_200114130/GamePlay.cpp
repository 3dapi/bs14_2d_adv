// Implementation of the CGamePlay class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"
#include "Stage.h"



extern Stage	g_Stage[];

CGamePlay::CGamePlay()
{
	m_pTxMap		= NULL;
	m_pTxBlock		= NULL;
	m_pTxPlayer		= NULL;
	m_pTxBall		= NULL;
	m_pTxFinish		= NULL;

	m_pStCur		= NULL;
	m_pStDst		= NULL;

	m_nSt			= 0;
	m_nStTot		= GetTotalState() -1;
}


CGamePlay::~CGamePlay()
{
	SAFE_DELETE(	m_pStCur	);
	SAFE_DELETE(	m_pStDst	);

	Destroy();
}


INT CGamePlay::Init()
{
	McUtil_TextureLoad("Texture/background.bmp", m_pTxMap, D3DCOLOR_ARGB(1,1,1,1));		// 블럭
	McUtil_TextureLoad("Texture/block.bmp", m_pTxBlock, D3DCOLOR_ARGB(1,1,1,1));		// 블럭
	McUtil_TextureLoad("Texture/player.bmp", m_pTxPlayer, D3DCOLOR_ARGB(1,1,1,1));		// 플레이어
	McUtil_TextureLoad("Texture/ball.bmp", m_pTxBall, D3DCOLOR_ARGB(1,1,1,1));			// 볼
	McUtil_TextureLoad("Texture/finish0.bmp", m_pTxFinish, D3DCOLOR_ARGB(1,1,1,1));		// 목적지

	m_pStCur	= new Stage;
	m_pStDst	= new Stage;
	
	SetupStage();
	
	return 1;
}


void CGamePlay::Destroy()
{
	SAFE_RELEASE( m_pTxMap		 );
	SAFE_RELEASE( m_pTxBlock	 );
	SAFE_RELEASE( m_pTxPlayer	 );
	SAFE_RELEASE( m_pTxBall		 );
	SAFE_RELEASE( m_pTxFinish	 );
}


INT CGamePlay::FrameMove()
{
	// 목적지의 갯수와 비교
	if(m_nDstTot == m_nDstCur)
	{
		m_nSt += 1;

		if(m_nSt == 7)
			GMAIN->SetGameStateCurrent(IGameStage::GMST_END);

		SetupStage();		

		return 1;
	}

	m_nDstCur = 0;

	// 목적지에 볼이 있는 지 확인
	for(int i=0 ; i<12 ; i++)
	{
		for(int j=0 ; j<12 ; j++)
		{
			if( FINISH == m_pStDst->stageInfo[i][j])
			{
				if(BALL == m_pStCur->stageInfo[i][j])
				{
					++m_nDstCur;
				}
			}
		}
	}

	// 이동
	if(GINPUT->KeyDown(VK_UP))
	{
		if(Crash(0) == TRUE)
			m_pStCur->PlayerPosY = m_pStCur->PlayerPosY - 1;
	}
	if(GINPUT->KeyDown(VK_DOWN))
	{
		if(Crash(1) == TRUE)
			m_pStCur->PlayerPosY = m_pStCur->PlayerPosY + 1;
	}
	if(GINPUT->KeyDown(VK_LEFT))
	{
		if(Crash(2) == TRUE)
			m_pStCur->PlayerPosX = m_pStCur->PlayerPosX - 1;
	}
	if(GINPUT->KeyDown(VK_RIGHT))
	{
		if(Crash(3) == TRUE)
			m_pStCur->PlayerPosX = m_pStCur->PlayerPosX + 1;
	}

	// Rest
	if(GINPUT->KeyDown('R'))
	{
		SetupStage();
	}

	return 1;
}


void CGamePlay::Render()
{
	RECT	rtTile	= {0, 0, 42, 42};	// 타일 크기
	
	for(int i=0 ; i<12 ; i++)
	{
		for(int j=0 ; j<12 ; j++)
		{
			if(m_pStCur->stageInfo[i][j] == BACKGROUND)
				GSPRITE->Draw(m_pTxMap, &rtTile, NULL, &D3DXVECTOR3(float(42*j), float(42*i), 0), D3DXCOLOR(1,1,1,1));

			else if(m_pStCur->stageInfo[i][j] == BLOCK)

				GSPRITE->Draw(m_pTxBlock, &rtTile, NULL, &D3DXVECTOR3(float(42*j), float(42*i), 0), D3DXCOLOR(1,1,1,1));
			
			else if(m_pStCur->stageInfo[i][j] == BALL)
				GSPRITE->Draw(m_pTxBall, &rtTile, NULL, &D3DXVECTOR3(float(42*j), float(42*i), 0), D3DXCOLOR(1,1,1,1));

			else if(m_pStCur->stageInfo[i][j] == LANE)
			{
				if(m_pStDst->stageInfo[i][j] == FINISH)
					GSPRITE->Draw(m_pTxFinish, &rtTile, NULL, &D3DXVECTOR3(float(42*j), float(42*i), 0), D3DXCOLOR(1,1,1,1));

				else
					GSPRITE->Draw(m_pTxMap, &rtTile, NULL, &D3DXVECTOR3(float(42*j), float(42*i), 0), D3DXCOLOR(1,1,1,1));
			}
			
			
			else if(m_pStCur->stageInfo[i][j] == FINISH)
				GSPRITE->Draw(m_pTxFinish, &rtTile, NULL, &D3DXVECTOR3(float(42*j), float(42*i), 0), D3DXCOLOR(1,1,1,1));
		}
	}
	
	GSPRITE->Draw(m_pTxPlayer, &rtTile, NULL, &D3DXVECTOR3(float(42*m_pStCur->PlayerPosX), float(42*m_pStCur->PlayerPosY), 0), D3DXCOLOR(1,1,1,1));
}



BOOL CGamePlay::Crash(INT move)
{	
	// up ↑
	if( move == 0 )
	{
		// 이동할 곳에 블럭이 있다면 FALSE
		if( m_pStCur->stageInfo[m_pStCur->PlayerPosY-1][m_pStCur->PlayerPosX] == BLOCK )
			return FALSE;
		else
		{
			// 이동할 곳에 볼이 있다면
			if( m_pStCur->stageInfo[m_pStCur->PlayerPosY-1][m_pStCur->PlayerPosX] == BALL )
			{
				// 볼 너머에 블럭 or 볼이 있다면 FALSE
				if( m_pStCur->stageInfo[m_pStCur->PlayerPosY-2][m_pStCur->PlayerPosX] == BLOCK ||
					m_pStCur->stageInfo[m_pStCur->PlayerPosY-2][m_pStCur->PlayerPosX] == BALL )				
					return FALSE;
				// 볼 너머에 아무것도 없다면 볼이동
				else
				{
					m_pStCur->stageInfo[m_pStCur->PlayerPosY-2][m_pStCur->PlayerPosX] = BALL;
					m_pStCur->stageInfo[m_pStCur->PlayerPosY-1][m_pStCur->PlayerPosX] = LANE;
				}
			}
		}
	}	

	// down ↓
	if( move == 1 )
	{
		if( m_pStCur->stageInfo[m_pStCur->PlayerPosY+1][m_pStCur->PlayerPosX] == BLOCK )
			return FALSE;
		else
		{
			if( m_pStCur->stageInfo[m_pStCur->PlayerPosY+1][m_pStCur->PlayerPosX] == BALL )
			{
				if( m_pStCur->stageInfo[m_pStCur->PlayerPosY+2][m_pStCur->PlayerPosX] == BLOCK ||
					m_pStCur->stageInfo[m_pStCur->PlayerPosY+2][m_pStCur->PlayerPosX] == BALL )
					return FALSE;
				else
				{
					m_pStCur->stageInfo[m_pStCur->PlayerPosY+2][m_pStCur->PlayerPosX] = BALL;
					m_pStCur->stageInfo[m_pStCur->PlayerPosY+1][m_pStCur->PlayerPosX] = LANE;
				}
			}
		}
	}
	

	// left ←
	if( move == 2 )
	{
		if( m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX-1] == BLOCK )
			return FALSE;
		else
		{
			if( m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX-1] == BALL )
			{
				if( m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX-2] == BLOCK ||
					m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX-2] == BALL )
					return FALSE;
				else
				{
					m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX-2] = BALL;
					m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX-1] = LANE;
				}
			}
		}
	}

	// right →
	if( move == 3 )
	{
		if( m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX+1] == BLOCK )
			return FALSE;
		else
		{
			if( m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX+1] == BALL )
			{
				if( m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX+2] == BLOCK ||
					m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX+2] == BALL )
					return FALSE;
				else
				{
					m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX+2] = BALL;
					m_pStCur->stageInfo[m_pStCur->PlayerPosY][m_pStCur->PlayerPosX+1] = LANE;
				}
			}
		}
	}

	return TRUE;
}



void CGamePlay::SetupStage()
{
	int i=0, j=0;

	m_nDstCur	= 0;
	m_nDstTot	= 0;
	memcpy(m_pStCur, &g_Stage[m_nSt], sizeof(Stage));
	memcpy(m_pStDst, &g_Stage[m_nSt], sizeof(Stage));


	for(i=0 ; i<12 ; i++)
	{
		for(j=0 ; j<12 ; j++)
		{
			if( 34 == m_pStCur->stageInfo[i][j])
			{
				m_pStCur->stageInfo[i][j] = BALL;
				m_pStDst->stageInfo[i][j] = FINISH;
			}
		}
	}


	for(i=0 ; i<12 ; i++)
	{
		for(j=0 ; j<12 ; j++)
		{
			if( FINISH == m_pStDst->stageInfo[i][j])
				++m_nDstTot;
		}
	}
}

