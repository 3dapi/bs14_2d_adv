
#ifndef D3DAPP_H
#define D3DAPP_H


enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESETFAILED         0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d



class CD3DApplication
{
protected:
	CD3DEnumeration   m_d3dEnumeration;
	CD3DSettings      m_d3dSettings;

	// Internal variables for the state of the app
	BOOL              m_bWindowed;
	BOOL              m_bActive;
	BOOL              m_bDeviceLost;
	BOOL              m_bMinimized;
	BOOL              m_bMaximized;
	BOOL              m_bIgnoreSizeChange;
	BOOL              m_bDeviceObjectsInited;
	BOOL              m_bDeviceObjectsRestored;
	BOOL              m_bLoadingApp;          // TRUE, if the app is loading

	// Internal variables used for timing
	BOOL              m_bFrameMoving;
	BOOL              m_bSingleStep;

	// Internal error handling function
	HRESULT DisplayErrorMsg( HRESULT hr, DWORD dwType );

	// Internal functions to manage and render the 3D scene
	static BOOL ConfirmDeviceHelper(D3DCAPS9* , VertexProcessingType, D3DFORMAT);
	void    BuildPresentParamsFromSettings();
	BOOL    FindBestWindowedMode( BOOL bRequireHAL, BOOL bRequireREF );
	BOOL    FindBestFullscreenMode( BOOL bRequireHAL, BOOL bRequireREF );
	HRESULT ChooseInitialD3DSettings();
	HRESULT Initialize3DEnvironment();
	HRESULT HandlePossibleSizeChange();
	HRESULT Reset3DEnvironment();
	HRESULT ToggleFullscreen();
	HRESULT ForceWindowed();
	HRESULT UserSelectNewDevice();

	HRESULT Render3DEnvironment();
	virtual HRESULT AdjustWindowForChange();
	virtual void UpdateStats();

public:
	void    Cleanup3DEnvironment();


protected:
	// Main objects used for creating and rendering the 3D scene
	D3DPRESENT_PARAMETERS m_d3dpp;         // Parameters for CreateDevice/Reset
	HWND              m_hWndFocus;         // The D3D focus window (usually same as m_hWnd)
	HMENU             m_hMenu;             // App menu bar (stored here when fullscreen)

	DWORD             m_dwCreateFlags;     // Indicate sw or hw vertex processing

	RECT              m_rcWindowBounds;    // Saved window bounds for mode switches
	RECT              m_rcWindowClient;    // Saved client area size for mode switches

	// Variables for timing
	FLOAT             m_fTime;             // Current time in seconds
	FLOAT             m_fElapsedTime;      // Time elapsed since last frame
	FLOAT             m_fFPS;              // Instanteous frame rate
	TCHAR             m_strDeviceStats[90];// String to hold D3D device stats
	TCHAR             m_strFrameStats[90]; // String to hold frame stats


	BOOL              m_bClipCursorWhenFullscreen; // Whether to limit cursor pos when fullscreen

	// Overridable functions for the 3D scene created by the app
	virtual HRESULT ConfirmDevice(D3DCAPS9*,DWORD,D3DFORMAT);

public:
	virtual HRESULT OneTimeSceneInit();
	virtual HRESULT FinalCleanup();

	virtual HRESULT Init()						{ return 0; }
	virtual HRESULT Destroy()					{ return 0; }

	virtual HRESULT Restore()					{ return 0; }
	virtual HRESULT Invalid()					{ return 0; }

	virtual HRESULT FrameMove()					{ return 0; }
	virtual HRESULT Render()					{ return 0; }

public:
	// Functions to create, run, pause, and clean up the application
	virtual HRESULT Create( HINSTANCE);
	virtual INT     Run();
	virtual LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);
	virtual void    Pause( BOOL bPause );

	CD3DApplication();
};




#endif