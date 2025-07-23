// Interface for the CGamePlay class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_


class CGamePlay : public IGameStage
{
protected:
	LPDIRECT3DTEXTURE9		m_pTxMap;		// 맵
	LPDIRECT3DTEXTURE9		m_pTxBlock;		// 블럭
	LPDIRECT3DTEXTURE9		m_pTxPlayer;	// 플레이어
	LPDIRECT3DTEXTURE9		m_pTxBall;		// 볼
	LPDIRECT3DTEXTURE9		m_pTxFinish;	// 목적지

	INT						m_nSt;			// 스테이지
	INT						m_nStTot;		// 토탈 스테이지
	Stage*					m_pStCur;		// 현재 상태
	Stage*					m_pStDst;		// 원본 상태
	INT						m_nDstTot;		// 목적지의 갯수
	INT						m_nDstCur;		// 현재 목적니에 채운 갯수
	

public:
	CGamePlay();
	virtual ~CGamePlay();

	virtual	INT		Init();
	virtual	void	Destroy();

	virtual	INT		FrameMove();
	virtual	void	Render();

public:
	void			SetupStage();
	BOOL			Crash(INT move);
};

#endif