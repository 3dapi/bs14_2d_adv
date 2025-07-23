//
////////////////////////////////////////////////////////////////////////////////


#pragma warning( disable : 4996)


#define STRICT
#define _WIN32_WINNT 0x0400

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DxErr.lib")

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")

#include <vector>

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>


#include "DXUtil.h"
#include "DSUtil.h"

#include "D3DEnumeration.h"
#include "D3DSettings.h"
#include "D3DApp.h"
#include "D3DUtil.h"
#include "resource.h"

#include "Main.h"

#include "SpLib.h"


int			m_nKeyTx=0;
int			m_nKeySnd=0;

struct McTexture
{
	int					nKey;
	char				sFile[256];
	LPDIRECT3DTEXTURE9	pTx;
	DIMG				pImg;

	McTexture()
	{
		memset(sFile, 0, sizeof(sFile));
		pTx	= NULL;

		nKey = ++m_nKeyTx;
	}

	~McTexture()
	{
		SAFE_RELEASE(	pTx	);
	}
};


struct McSound
{
	int			nKey;
	char		sFile[256];
	CSound*		pSnd;

	McSound()
	{
		memset(sFile, 0, sizeof(sFile));
		pSnd = NULL;

		nKey = ++m_nKeySnd;
	}

	~McSound()
	{
		SAFE_DELETE(	pSnd	);
	}
};


typedef std::vector<McTexture* >	lsPDTX;
typedef lsPDTX::iterator			itPDTX;


typedef std::vector<ID3DXFont* >	lsDxFont;
typedef lsDxFont::iterator			itDxFont;


typedef std::vector<McSound* >		lsMcSound;
typedef	lsMcSound::iterator			itMcSound;




HINSTANCE			m_hInst = NULL		;
HWND				m_hWnd				;

HACCEL				m_hAccel			;

TCHAR				m_sCls[512]			;

int					m_dScnX				;
int					m_dScnY				;

DWORD				m_dScnW				;
DWORD				m_dScnH				;

DWORD				m_dWinStyle		= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_VISIBLE;
DWORD				m_dColor		= 0xFF006699;

BOOL				m_bStartFull	= FALSE;		// Whether to start up the app in fullscreen mode
BOOL				m_bShowCursor	= TRUE;			// Whether to show cursor when fullscreen

BOOL				m_bShowState	=TRUE;


LPDIRECT3D9			m_pD3D				;			// The main D3D object
LPDIRECT3DDEVICE9	m_pd3dDevice		;			// The D3D rendering device
LPD3DXSPRITE		m_pd3dSprite		;			// 2D Sprite

D3DSURFACE_DESC		m_d3dsdBackBuffer	;			// Surface desc of the backbuffer
D3DCAPS9			m_d3dCaps			;			// Caps for the device
BYTE				m_KeyCur[256]		;

int					m_mouseX			;
int					m_mouseY			;
int					m_mouseZ			;
int					m_mouseEvent		;

DWORD				m_dTimeBgn=0;
DWORD				m_dTimeElapsed=0;


lsPDTX				m_vTx				;			// Texture
lsDxFont			m_vFont				;			// Font


CSoundManager*		m_pSndMn			;			// DirectSound manager class
lsMcSound			m_vSound			;


CD3DApplication*	g_pD3DApp = NULL;
CMain*				g_pApp	  = NULL;





int (*SpLib_FrameMove2D)();
int (*SpLib_Render2D)();


int (*SpLib_Keyboard)(BYTE* key);
int (*SpLib_Mouse)(int x, int y, int z, int _event);



int SpLib_DefaultFrameMove()
{
	//printf("SpLib_Default FrameMove\n");
	return 0;
}

int SpLib_DefaultRender()
{
	//printf("SpLib_Default Render\n");
	return 0;
}

int SpLib_DefaultKeyboard(BYTE* key)
{
	//printf("SpLib_Default Keyboard\n");
	return 0;
}

int SpLib_DefaultMouse(int x, int y, int z, int _event)
{
	//printf("SpLib_Default Mouse\n");
	return 0;
}



void SpLib_SetFrameMove(int (*_FrameMove)() )
{
	SpLib_FrameMove2D = _FrameMove;
}


void SpLib_SetRender(int (*_Render)() )
{
	SpLib_Render2D = _Render;
}



void SpLib_SetKeyboard(int (*_Keyboard)(BYTE* key) )
{
	SpLib_Keyboard	= _Keyboard;
}

void SpLib_SetMouse(int (*_Mouse)(int x, int y, int z, int _event) )
{
	SpLib_Mouse = _Mouse;
}




BYTE*	SpLib_GetKeyboard()					{	return m_KeyCur;			}
int		SpLib_GetMouseX()					{	return m_mouseX;			}
int		SpLib_GetMouseY()					{	return m_mouseY;			}
int		SpLib_GetMouseZ()					{	return m_mouseZ;			}

int		SpLib_GetMouseEvent(int nMouse)
{
	if(0 == nMouse)			return m_KeyCur[VK_LBUTTON];
	else if(1 == nMouse)	return m_KeyCur[VK_RBUTTON];
	else if(2 == nMouse)	return m_KeyCur[VK_MBUTTON];
	return 0;
}

DWORD	SpLib_ElapsedTime()					{	return m_dTimeElapsed;		}


DWORD	SpLib_GetWindowStyle()				{	return m_dWinStyle;			}
HWND	SpLib_GetHwnd()						{	return m_hWnd;				}
int		SpLib_GetScnW()						{	return m_dScnW;				}
int		SpLib_GetScnH()						{	return m_dScnH;				}


void	SpLib_SetWindowStyle(DWORD dSty)	{	m_dWinStyle		= dSty;		}
void	SpLib_SetStateShow(BOOL _bShow)		{	m_bShowState	= _bShow;	}
void	SpLib_SetCursorShow(BOOL _bShow)	{	m_bShowCursor	= _bShow;	}
void	SpLib_SetClearColor(DWORD dC)		{	m_dColor = dC;				}
DWORD	SpLib_GetClearColor()				{	return m_dColor;			}


void	SpLib_ChangeWindow()
{
	SendMessage(m_hWnd, WM_COMMAND, IDM_TOGGLEFULLSCREEN, 0);
}


int		SpLib_FontCreate(char* sName, LONG iH, BYTE iItalic)
{
	ID3DXFont*	pD3DXFont;
	D3DXFONT_DESC hFont =
	{
		iH, 0
		, FW_NORMAL, 1, iItalic
		, HANGEUL_CHARSET
		, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE
		, "Arial"
	};

	strcpy(hFont.FaceName, sName);

    if( FAILED(D3DXCreateFontIndirect( m_pd3dDevice, &hFont, &pD3DXFont ) ) )
        return -1;

	m_vFont.push_back(	pD3DXFont);

	return (m_vFont.size()-1);
}



int 	SpLib_FontDrawText(int nIdx
						   , LONG lLeft
						   , LONG lTop
						   , LONG lRight
						   , LONG lBottom
						   , DWORD fontColor
						   , const char *format, ...)
{
	int iSize = m_vFont.size();

	if(nIdx<0 || nIdx>=iSize)
		return -1;


	va_list ap;
	char s[1024];

	if (format == NULL)
		return -1;

	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);

	if (s == NULL)
		return -2;

	ID3DXFont*	pD3DXFont = m_vFont[nIdx];

    RECT rc;
	rc.left		= lLeft;
	rc.top		= lTop;
    rc.right	= lRight+20;
	rc.bottom	= lBottom;

    return pD3DXFont->DrawText(NULL, s, -1, &rc, 0, fontColor );
}








int	SpLib_TextureLoad(char* sFileName, DWORD dc)
{
	PDTX	pTx	= NULL;
	DIMG	pImg;

	if(FAILED(D3DXCreateTextureFromFileEx(
		m_pd3dDevice
		, sFileName
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, D3DX_FILTER_NONE
		, D3DX_FILTER_NONE
		, dc
		, &pImg
		, NULL
		, &pTx
		)) )
	{
		printf("Create Texture Failed: %s\n", sFileName);
		pTx = NULL;
		return -1;
	}

	McTexture*	p = new McTexture;

	p->pTx = pTx;
	strcpy(p->sFile, sFileName);
	memcpy(&p->pImg, &pImg, sizeof(DIMG));

	m_vTx.push_back(p);

	// Key를 돌려 준다.
	return p->nKey;
}


int SpLib_TextureRelease(int _nKey)
{
	int iSize = m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;


	lsPDTX::iterator	itTx;

	itTx = m_vTx.begin() + nIdx;
	SAFE_DELETE(	m_vTx[nIdx]	);
	m_vTx.erase(itTx);

	iSize = m_vTx.size();

	return iSize;
}

int SpLib_TextureWidth(int _nKey)
{
	int iSize = m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;

	return (int)m_vTx[nIdx]->pImg.Width;
}


int SpLib_TextureHeight(int _nKey)
{
	int iSize = m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;

	return (int)m_vTx[nIdx]->pImg.Height;
}






int SpLib_SoundLoad(char* sFileName)
{
	CSound*	pSnd = NULL;

	if(FAILED(m_pSndMn->Create( &pSnd, sFileName, 0, GUID_NULL, 1 )))
		return -1;

	if(NULL ==pSnd)
		return -1;

	McSound* p = new McSound;

	p->pSnd = pSnd;
	strcpy(p->sFile, sFileName);

	m_vSound.push_back(p);

	// Key를 돌려 준다.
	return p->nKey;
}


int SpLib_SoundRelease(int _nKey)
{
	int iSize = m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return -1;


	lsMcSound::iterator	itSound;

	itSound = m_vSound.begin() + nIdx;
	SAFE_DELETE( m_vSound[nIdx]		);
	m_vSound.erase(itSound);

	iSize = m_vSound.size();

	return iSize;
}




void	SpLib_SoundPlay(int _nKey)
{
	int iSize = m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return;

	m_vSound[nIdx]->pSnd->Play();
}

void	SpLib_SoundStop(int _nKey)
{
	int iSize = m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return;

	m_vSound[nIdx]->pSnd->Stop();
}

void	SpLib_SoundReset(int _nKey)
{
	int iSize = m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return;

	m_vSound[nIdx]->pSnd->Reset();
}

BOOL	SpLib_SoundIsPlaying(int _nKey)
{
	int iSize = m_vSound.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vSound[i]->nKey == _nKey)
		{
			nIdx = i;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
		return 0;

	return m_vSound[nIdx]->pSnd->IsSoundPlaying();
}



void SpLib_SetWindowTitle(const char *format, ...)
{
	va_list ap;
	char s[512];

	if (format == NULL) return;

	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);

	if (s == NULL)	return;

	SetWindowText(m_hWnd, s);
}



void SpLib_Draw2D(int _nKey, RECT* pSrcRect, VEC2* pTranslation, VEC2* pScaling, VEC2* pRot, FLOAT fAngle, DWORD dC)
{
	int iSize = m_vTx.size();
	int	nIdx=-1;

	for(int i=0; i<iSize; ++i)
	{
		if(m_vTx[i]->nKey == _nKey)
		{
			nIdx = i;
			break;
		}
	}

	if(nIdx<0 || nIdx>=iSize)
	{
		return;
	}

	PDTX		pTx =  m_vTx[nIdx]->pTx;
	D3DXMATRIX	mtW;
	D3DXMATRIX	mtS;
	D3DXMATRIX	mtR;
	D3DXMATRIX	mtRt;
	D3DXMATRIX	mtRti;
	D3DXMATRIX	mtT;

	D3DXMatrixIdentity(&mtS);
	D3DXMatrixIdentity(&mtR);
	D3DXMatrixIdentity(&mtRt);
	D3DXMatrixIdentity(&mtRti);
	D3DXMatrixIdentity(&mtT);

	if(pTranslation)
		D3DXMatrixTranslation(&mtT, pTranslation->x, pTranslation->y, 0);

	if(0.0F != fAngle)
	{
		D3DXMatrixRotationZ(&mtR, fAngle);

		if(pRot)
		{
			D3DXMatrixTranslation(&mtRti, -pRot->x, -pRot->y, 0);
			D3DXMatrixTranslation(&mtRt, pRot->x, pRot->y, 0);
		}
	}

	if(pScaling)
		D3DXMatrixScaling(&mtS, pScaling->x, pScaling->y, 1);

	mtW	= mtS * mtRti * mtR * mtRt * mtT;

	m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pd3dSprite->SetTransform(&mtW);
	m_pd3dSprite->Draw(pTx, pSrcRect, NULL, NULL, dC);

	D3DXMatrixIdentity(&mtW);
	m_pd3dSprite->SetTransform(&mtW);

	m_pd3dSprite->End();
}



int SpLib_CreateWin(int x, int y, int width, int height, char* sName, BOOL bFull)
{
	SpLib_SetFrameMove( SpLib_DefaultFrameMove);
	SpLib_SetRender( SpLib_DefaultRender );
	SpLib_SetKeyboard( SpLib_DefaultKeyboard );
	SpLib_SetMouse( SpLib_DefaultMouse );

	strcpy(m_sCls, sName);

	m_dScnX = x;
	m_dScnY = y;
	m_dScnW = width;
	m_dScnH = height;

	m_bStartFull = bFull;


	m_hInst =(HINSTANCE)GetModuleHandle(NULL);
	g_pApp  = new CMain;

	InitCommonControls();

	if( FAILED( g_pApp->Create( m_hInst ) ) )
		return -1;


	m_pSndMn = new CSoundManager;


	if( FAILED(m_pSndMn->Initialize( m_hWnd, DSSCL_PRIORITY ) ) )
	{
		MessageBox( m_hWnd, "Sound Manager Initialize Failed", "Err", MB_ICONEXCLAMATION);
		return -1;
	}

	if( FAILED( m_pSndMn->SetPrimaryBufferFormat( 2, 22050, 16 ) ) )
	{
		MessageBox( m_hWnd, "SetPrimaryBufferFormat Failed", "Err", MB_ICONEXCLAMATION);
		return -1;
	}

	m_dTimeBgn = timeGetTime();

	return 0;
}



int	SpLib_Run()
{
	return g_pApp->Run();
}



void SpLib_DestroyWin()
{
	int i=0;
	int iSize = m_vFont.size();

	for(i=0; i<iSize; ++i)
	{
		SAFE_RELEASE(	m_vFont[i]	);
	}

	m_vFont.clear();


	iSize = m_vTx.size();

	for(i=0; i<iSize; ++i)
	{
		SAFE_DELETE(	m_vTx[i]	);
	}

	m_vTx.clear();


	iSize = m_vSound.size();

	for(i=0; i<iSize; ++i)
	{
		SAFE_DELETE(	m_vSound[i]	);
	}

	m_vSound.clear();


	SAFE_DELETE(	m_pSndMn	);

	g_pApp->Cleanup3DEnvironment();
	SAFE_RELEASE( m_pD3D );
	g_pApp->FinalCleanup();


	if( m_hAccel!= NULL )
	{
		DestroyAcceleratorTable( m_hAccel );
		m_hAccel = NULL;
	}

	if(m_hWnd)
	{
		SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
	}



	SAFE_DELETE(	g_pApp	);

	m_hWnd = NULL;
}










CMain::CMain()
{
	m_pD3DXFont			= NULL;
}


HRESULT CMain::Init()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, m_dColor, 1.0f, 0L );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	m_pd3dDevice->EndScene();
	m_pd3dDevice->Present( 0, 0, 0, 0);

	return 0;
}


HRESULT CMain::Destroy()
{
	return 0;
}


HRESULT CMain::Restore()
{
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);

	D3DXFONT_DESC hFont =
	{
		14, 0
		, FW_BOLD, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE
		, "굴림체"
	};

	D3DXCreateFontIndirect(m_pd3dDevice, &hFont, &m_pD3DXFont);


	int iSize= m_vFont.size();

	for(int i=0; i<iSize; ++i)
	{
		m_vFont[i]->OnResetDevice();
	}

	return 0;
}




HRESULT CMain::Invalid()
{
	SAFE_RELEASE( m_pD3DXFont );

	int iSize= m_vFont.size();

	for(int i=0; i<iSize; ++i)
	{
		m_vFont[i]->OnLostDevice();
	}

	return 0;
}



HRESULT CMain::FrameMove()
{
	m_dTimeElapsed = timeGetTime() - m_dTimeBgn;

	POINT mouse;

	memset(m_KeyCur, 0, sizeof(m_KeyCur));

	::GetKeyboardState(m_KeyCur);


	for(int i=0; i<256; ++i)
		m_KeyCur[i] = (m_KeyCur[i] & 0x80);

	::GetCursorPos(&mouse);
	::ScreenToClient(m_hWnd, &mouse );

	m_mouseX = mouse.x;
	m_mouseY = mouse.y;

	SpLib_Keyboard(m_KeyCur);
	SpLib_Mouse(m_mouseX, m_mouseY, m_mouseZ, 0);

	return SpLib_FrameMove2D();
}




HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, m_dColor, 1.0f, 0L );

	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


	if( FAILED( SpLib_Render2D() ) )
	{
		m_pd3dDevice->EndScene();
		m_pd3dDevice->Present( 0, 0, 0, 0);
		return 0;
	};


	INT nNextLine = 10;
	D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
    TCHAR szMsg[MAX_PATH] = TEXT("");
    RECT rct;
    ZeroMemory( &rct, sizeof(rct) );

    rct.left   = 2;
    rct.right  = m_d3dsdBackBuffer.Width - 20;
	rct.top = nNextLine; rct.bottom = rct.top + 20;


	sprintf( szMsg, "%s %s", m_strDeviceStats, m_strFrameStats );

	if(m_bShowState)
		m_pD3DXFont->DrawText(NULL,  szMsg, -1, &rct, 0, fontColor );

	m_pd3dDevice->EndScene();

	return 0;
}




LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	WPARAM wHi = HIWORD(wParam);
	WPARAM wLo = LOWORD(wParam);

	switch( msg )
	{
		case WM_PAINT:
		{
			if( m_bLoadingApp )
			{
				// Draw on the window tell the user that the app is loading
				// TODO: change as needed
				HDC hDC = GetDC( hWnd );
				TCHAR strMsg[MAX_PATH];
				wsprintf( strMsg, TEXT("Loading... Please wait") );
				RECT rct;
				GetClientRect( hWnd, &rct );
				DrawText( hDC, strMsg, -1, &rct, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
				ReleaseDC( hWnd, hDC );
			}
			break;
		}

		case WM_MOUSEWHEEL:
		{
			m_mouseZ += short( wHi);

			break;
		}

	}

	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}