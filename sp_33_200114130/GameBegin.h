// Interface for the CGameBegin class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _GAMEBEGIN_H_
#define _GAMEBEGIN_H_


class CGameBegin : public IGameStage
{
protected:
	LPDIRECT3DTEXTURE9	m_pTxMain;

	INT					i, j;


public:
	CGameBegin();
	virtual ~CGameBegin();

	virtual	INT		Init();
	virtual	void	Destroy();

	virtual	INT		FrameMove();
	virtual	void	Render();
};

#endif