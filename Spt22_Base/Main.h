// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

#define SAFE_DESTROY_WINDOW(p)	{if(p)DestroyWindow(p);	}


INT_PTR CALLBACK WndWrk		(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK WndWrk1	(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK WndImgSrc	(HWND, UINT, WPARAM, LPARAM);


class CMain : public CD3DApplication
{
public:
	HWND		m_hwWrk;
	HWND		m_hwWrk1;

	HWND		m_hwWrkTb;														// Base Work Tab
	HWND		m_hwImgSc;

	HIMAGELIST	m_IL;															// Image List for Tab and List control
	INT			m_nTab;

public:
	CMain();
    
	virtual HRESULT Init();
    virtual HRESULT Destroy();

	virtual LRESULT	MsgProc(HWND,UINT,WPARAM,LPARAM);
};

extern	CMain*	g_pApp;
#define	GMAIN	g_pApp

#endif