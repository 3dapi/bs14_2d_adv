// Interface for the CMcInput class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCINPUT_H_
#define _MCINPUT_H_

class CMcInput
{
protected:
	struct McMouseSt															// Mouse State
	{
		union
		{
			struct
			{
				bool	bBtnLDn, bBtnLUp, bBtnLDc;								// 0 Left Button Down, 1 Left Button Up, 2 Left Button Doublic Click
				bool	bBtnRDn, bBtnRUp, bBtnRDc;								// 3 Right Button Down, 4 Right Button Up, 5 Right Button Doublic Click
				bool	bBtnMDn, bBtnMUp, bBtnMDc;								// 6 Wheel Button Down, 7 Wheel Button Up, 8 Wheel Button Doublic
			};

			bool m[9];
		};

		operator bool*()		{	return (bool *) &bBtnLDn;				}

	};

	BYTE			KeyCur[256], KeyOld[256];
	
	D3DXVECTOR3		MousePosCur;												// Z is Wheel Mouse
	D3DXVECTOR3		MousePosOld;
	McMouseSt		MsStCur;
	McMouseSt		MsStOld;

public:
	CMcInput();
	~CMcInput();

	INT		Init();
	INT		FrameMove();

	void	UpdateGeneral();

	void		AddZ(INT d);

public:
	bool		KeyDown(BYTE cKey);
	bool		KeyState(BYTE cKey);
	bool		GetMouseSt(INT nM);
	void		SetMouseSt(INT nBtn, bool bSt);

	D3DXVECTOR3	GetMousePos();

	bool		IsInRect(RECT rc);

};

#define GINPUT		g_pInput
extern CMcInput*	g_pInput;

#endif
