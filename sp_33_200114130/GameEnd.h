// Interface for the CGameEnd class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _GAMEEND_H_
#define _GAMEEND_H_


class CGameEnd : public IGameStage
{
protected:
	LPDIRECT3DTEXTURE9	m_pTxEND;
	

public:
	CGameEnd();
	virtual ~CGameEnd();

	virtual	INT		Init();
	virtual	void	Destroy();

	virtual	INT		FrameMove();
	virtual	void	Render();
};

#endif