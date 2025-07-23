// Interface for the IGameStage class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _IGAMESTAGE_H_
#define _IGAMESTAGE_H_


class IGameStage
{
public:
	enum EGameStage
	{
		GMST_BEGIN	=0x0,
		GMST_PLAY		,
		GMST_END		,
		GMST_TOT		,
	};


public:
	IGameStage(){};
	virtual ~IGameStage(){};

	virtual	INT		Init()=0;
	virtual	void	Destroy()=0;

	virtual	INT		FrameMove()=0;
	virtual	void	Render()=0;

};

#endif