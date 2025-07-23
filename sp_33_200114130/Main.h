// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication  
{
protected:
	CMcInput*		m_pInput					;
	LPD3DXFONT		m_pD3DXFont					;

	IGameStage::EGameStage	m_GameStCur			;								// Current State
	IGameStage::EGameStage	m_GameStOld			;								// Old State
	IGameStage*		m_pGameSt[IGameStage::GMST_TOT]	;							// Game Stages

public:
	CMain();
	virtual ~CMain();


	virtual	HRESULT	Init();														// Game Data 생성
	virtual	HRESULT	Destroy();													// Game Data 소멸

	virtual	HRESULT	FrameMove();												// Game Data 갱신
	virtual	HRESULT	Render();													// Game Data 그리기

	virtual LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

	void	SetGameStateCurrent(INT nSt)
	{
		m_GameStCur = (IGameStage::EGameStage)nSt;
	}

	INT		GetGameStateCurrent()
	{
		return m_GameStCur;
	}
};

#define GMAIN	g_pApp
extern	CMain*	g_pApp;

#endif