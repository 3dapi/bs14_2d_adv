// Implementation of the CMcInput class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMcInput*	g_pInput;


CMcInput::CMcInput()
{
	g_pInput = this;
}

CMcInput::~CMcInput()
{

}


INT CMcInput::Init()
{
	memset(KeyCur, 0, sizeof(KeyCur));
	memset(KeyOld, 0, sizeof(KeyOld));
	
	memset(&MousePosCur, 0, sizeof (MousePosCur));
	memset(&MousePosOld, 0, sizeof(MousePosOld));
	
	memset(&MsStCur, 0, sizeof(MsStCur));
	memset(&MsStOld, 0, sizeof(MsStOld));
	
	return 1;
}

INT CMcInput::FrameMove()
{
	memcpy(KeyOld		,       KeyCur, sizeof(KeyCur)	  );
	memset(KeyCur		, 0, sizeof(KeyCur));
	
	memcpy(&MousePosOld	, &MousePosCur, sizeof(MousePosCur));
	memcpy(&MsStOld		, &MsStCur	  , sizeof(MsStCur));
	
	UpdateGeneral();
	
	return 1;
}


void CMcInput::UpdateGeneral()
{
	POINT mouse;

	// 1. Update Keyboard
	::GetKeyboardState(KeyCur);

	// 2. Update Mouse
	::GetCursorPos(&mouse);
	::ScreenToClient(GHWND, &mouse );

	MousePosCur.x = FLOAT(mouse.x);
	MousePosCur.y = FLOAT(mouse.y);
}


void CMcInput::AddZ(INT	d)
{
	MousePosCur.z += FLOAT(d);
}

void CMcInput::SetMouseSt(INT nBtn, bool bSt)
{
	memset(MsStCur, 0, sizeof(MsStCur));
	MsStCur.m[nBtn]  = bSt;
}


bool CMcInput::KeyDown(BYTE cKey)
{
	return (KeyCur[cKey] & 0x80 && KeyOld[cKey] != KeyCur[cKey]);
}

bool CMcInput::KeyState(BYTE cKey)
{
	return (KeyCur[cKey] & 0x80 && true);
}


bool CMcInput::GetMouseSt(INT nBtn)
{
	return MsStCur.m[nBtn];
}


D3DXVECTOR3	CMcInput::GetMousePos()
{
	return MousePosCur;
}


bool CMcInput::IsInRect(RECT rc)
{
	return (MousePosCur.x > rc.left  &&
			MousePosCur.x < rc.right &&
			MousePosCur.y > rc.top   &&
			MousePosCur.y < rc.bottom);
}