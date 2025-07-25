﻿

#pragma warning( disable : 4996)


#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <D3D9.h>

#include "DXUtil.h"
#include "D3DUtil.h"
#include "D3DEnumeration.h"
#include "D3DSettings.h"
#include "D3DApp.h"
#include "resource.h"

#include "SpLib.h"


extern HINSTANCE		m_hInst;
extern HWND				m_hWnd;

extern HACCEL			m_hAccel;

extern int				m_dScnX;
extern int				m_dScnY;

extern DWORD			m_dScnW;
extern DWORD			m_dScnH;

extern CD3DApplication*	g_pD3DApp;
extern TCHAR			m_sCls[512];

extern DWORD			m_dWinStyle;
extern BOOL				m_bStartFull;
extern BOOL				m_bShowCursor;

extern LPDIRECT3D9			m_pD3D;
extern LPDIRECT3DDEVICE9	m_pd3dDevice;
extern D3DSURFACE_DESC		m_d3dsdBackBuffer;
extern LPD3DXSPRITE			m_pd3dSprite;
extern D3DCAPS9				m_d3dCaps;


CD3DApplication::CD3DApplication()
{
	g_pD3DApp           = this;

	m_pD3D              = NULL;
	m_pd3dDevice        = NULL;
	m_hWnd              = NULL;
	m_hWndFocus         = NULL;
	m_hMenu             = NULL;
	m_bWindowed         = TRUE;
	m_bActive           = FALSE;
	m_bDeviceLost       = FALSE;
	m_bMinimized        = FALSE;
	m_bMaximized        = FALSE;
	m_bIgnoreSizeChange = FALSE;
	m_bDeviceObjectsInited = FALSE;
	m_bDeviceObjectsRestored = FALSE;
	m_dwCreateFlags     = 0;
	m_bLoadingApp               = TRUE;

	m_bFrameMoving      = TRUE;
	m_bSingleStep       = FALSE;
	m_fTime             = 0.0f;
	m_fElapsedTime      = 0.0f;
	m_fFPS              = 0.0f;
	m_strDeviceStats[0] = _T('\0');
	m_strFrameStats[0]  = _T('\0');

	m_d3dEnumeration.AppUsesDepthBuffer   = TRUE;
	m_d3dEnumeration.AppMinStencilBits	= 8;
	m_d3dEnumeration.AppMinDepthBits	= 24;


	Pause( TRUE ); // Pause until we're ready to render

	// When m_bClipCursorWhenFullscreen is TRUE, the cursor is limited to
	// the device window when the app goes fullscreen.  This prevents users
	// from accidentally clicking outside the app window on a multimon system.
	// This flag is turned off by default for debug builds, since it makes
	// multimon debugging difficult.
#if defined(_DEBUG) || defined(DEBUG)
	m_bClipCursorWhenFullscreen = FALSE;
#else
	m_bClipCursorWhenFullscreen = TRUE;
#endif
}




//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );
}



//-----------------------------------------------------------------------------
// Name: ConfirmDeviceHelper()
// Desc: Static function used by D3DEnumeration
//-----------------------------------------------------------------------------
BOOL CD3DApplication::ConfirmDeviceHelper( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType,
										  D3DFORMAT backBufferFormat )
{
	DWORD dwBehavior;

	if (vertexProcessingType == SOFTWARE_VP)
		dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else if (vertexProcessingType == MIXED_VP)
		dwBehavior = D3DCREATE_MIXED_VERTEXPROCESSING;
	else if (vertexProcessingType == HARDWARE_VP)
		dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else if (vertexProcessingType == PURE_HARDWARE_VP)
		dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	else
		dwBehavior = 0; // TODO: throw exception

	return SUCCEEDED( g_pD3DApp->ConfirmDevice( pCaps, dwBehavior, backBufferFormat ) );
}




//-----------------------------------------------------------------------------
// Name: Create()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Create( HINSTANCE hInstance )
{
	HRESULT hr;

	// Create the Direct3D object
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pD3D == NULL )
		return DisplayErrorMsg( D3DAPPERR_NODIRECT3D, MSGERR_APPMUSTEXIT );

	// Build a list of Direct3D adapters, modes and devices. The
	// ConfirmDevice() callback is used to confirm that only devices that
	// meet the app's requirements are considered.
	m_d3dEnumeration.SetD3D( m_pD3D );
	m_d3dEnumeration.ConfirmDeviceCallback = ConfirmDeviceHelper;
	if( FAILED( hr = m_d3dEnumeration.Enumerate() ) )
	{
		SAFE_RELEASE( m_pD3D );
		return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
	}

	// Unless a substitute hWnd has been specified, create a window to
	// render into
	if( m_hWnd == NULL)
	{
		// Register the windows class
		WNDCLASS wndClass = { 0, WndProc, 0, 0, hInstance,
			LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
			LoadCursor( NULL, IDC_ARROW ),
			(HBRUSH)GetStockObject(WHITE_BRUSH),
			NULL, m_sCls };
		RegisterClass( &wndClass );


		// Set the window's initial width
		RECT rc;
        SetRect( &rc, 0, 0, m_dScnW, m_dScnH );
        AdjustWindowRect( &rc, m_dWinStyle, FALSE );

		int iScnSysW = ::GetSystemMetrics(SM_CXSCREEN);
		int iScnSysH = ::GetSystemMetrics(SM_CYSCREEN);


		// Create the render window
		m_hWnd = CreateWindow( m_sCls, m_sCls, m_dWinStyle
			,	m_dScnX
			,	m_dScnY
			,	(rc.right-rc.left)
			,	(rc.bottom-rc.top)
			, 0
			, LoadMenu( hInstance, MAKEINTRESOURCE(IDR_MENU) ),
			hInstance, 0 );
	}

	// The focus window can be a specified to be a different window than the
	// device window.  If not, use the device window as the focus window.
	if( m_hWndFocus == NULL )
		m_hWndFocus = m_hWnd;

	// Save window properties
	m_dWinStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	GetWindowRect( m_hWnd, &m_rcWindowBounds );
	GetClientRect( m_hWnd, &m_rcWindowClient );

	if( FAILED( hr = ChooseInitialD3DSettings() ) )
	{
		SAFE_RELEASE( m_pD3D );
		return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
	}

	// Initialize the application timer
	DXUtil_Timer( TIMER_START );

	// Initialize the app's custom scene stuff
	if( FAILED( hr = OneTimeSceneInit() ) )
	{
		SAFE_RELEASE( m_pD3D );
		return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
	}

	// Initialize the 3D environment for the app
	if( FAILED( hr = Initialize3DEnvironment() ) )
	{
		SAFE_RELEASE( m_pD3D );
		return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
	}

	// The app is ready to go
	Pause( FALSE );

	return 0;
}




//-----------------------------------------------------------------------------
// Name: FindBestWindowedMode()
// Desc: Sets up m_d3dSettings with best available windowed mode, subject to
//       the bRequireHAL and bRequireREF constraints.  Returns FALSE if no such
//       mode can be found.
//-----------------------------------------------------------------------------
BOOL CD3DApplication::FindBestWindowedMode( BOOL bRequireHAL, BOOL bRequireREF )
{
	// Get display mode of primary adapter (which is assumed to be where the window
	// will appear)
	D3DDISPLAYMODE primaryDesktopDisplayMode;
	m_pD3D->GetAdapterDisplayMode(0, &primaryDesktopDisplayMode);

	D3DAdapterInfo* pBestAdapterInfo = NULL;
	D3DDeviceInfo* pBestDeviceInfo = NULL;
	D3DDeviceCombo* pBestDeviceCombo = NULL;

	for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
	{
		D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
		for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
		{
			D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
			if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
				continue;
			if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
				continue;
			for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
			{
				D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
				BOOL bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
				if (!pDeviceCombo->IsWindowed)
					continue;
				if (pDeviceCombo->AdapterFormat != primaryDesktopDisplayMode.Format)
					continue;
				// If we haven't found a compatible DeviceCombo yet, or if this set
				// is better (because it's a HAL, and/or because formats match better),
				// save it
				if( pBestDeviceCombo == NULL ||
					pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceCombo->DevType == D3DDEVTYPE_HAL ||
					pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
				{
					pBestAdapterInfo = pAdapterInfo;
					pBestDeviceInfo = pDeviceInfo;
					pBestDeviceCombo = pDeviceCombo;
					if( pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
					{
						// This windowed device combo looks great -- take it
						goto EndWindowedDeviceComboSearch;
					}
					// Otherwise keep looking for a better windowed device combo
				}
			}
		}
	}
EndWindowedDeviceComboSearch:
	if (pBestDeviceCombo == NULL )
		return FALSE;

	m_d3dSettings.pWindowed_AdapterInfo = pBestAdapterInfo;
	m_d3dSettings.pWindowed_DeviceInfo = pBestDeviceInfo;
	m_d3dSettings.pWindowed_DeviceCombo = pBestDeviceCombo;
	m_d3dSettings.IsWindowed = TRUE;
	m_d3dSettings.Windowed_DisplayMode = primaryDesktopDisplayMode;
	m_d3dSettings.Windowed_Width = m_rcWindowClient.right - m_rcWindowClient.left;
	m_d3dSettings.Windowed_Height = m_rcWindowClient.bottom - m_rcWindowClient.top;
	if (m_d3dEnumeration.AppUsesDepthBuffer)
		m_d3dSettings.Windowed_DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
	m_d3dSettings.Windowed_MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
	m_d3dSettings.Windowed_MultisampleQuality = 0;
	m_d3dSettings.Windowed_VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
	m_d3dSettings.Windowed_PresentInterval = *(UINT*)pBestDeviceCombo->pPresentIntervalList->GetPtr(0);
	return TRUE;
}




//-----------------------------------------------------------------------------
// Name: FindBestFullscreenMode()
// Desc: Sets up m_d3dSettings with best available fullscreen mode, subject to
//       the bRequireHAL and bRequireREF constraints.  Returns FALSE if no such
//       mode can be found.
//-----------------------------------------------------------------------------
BOOL CD3DApplication::FindBestFullscreenMode( BOOL bRequireHAL, BOOL bRequireREF )
{
	// For fullscreen, default to first HAL DeviceCombo that supports the current desktop
	// display mode, or any display mode if HAL is not compatible with the desktop mode, or
	// non-HAL if no HAL is available
	D3DDISPLAYMODE adapterDesktopDisplayMode;
	D3DDISPLAYMODE bestAdapterDesktopDisplayMode;
	D3DDISPLAYMODE bestDisplayMode;
	bestAdapterDesktopDisplayMode.Width = 0;
	bestAdapterDesktopDisplayMode.Height = 0;
	bestAdapterDesktopDisplayMode.Format = D3DFMT_UNKNOWN;
	bestAdapterDesktopDisplayMode.RefreshRate = 0;

	D3DAdapterInfo* pBestAdapterInfo = NULL;
	D3DDeviceInfo* pBestDeviceInfo = NULL;
	D3DDeviceCombo* pBestDeviceCombo = NULL;

	for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
	{
		D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
		m_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &adapterDesktopDisplayMode );
		for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
		{
			D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
			if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
				continue;
			if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
				continue;
			for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
			{
				D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
				BOOL bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
				BOOL bAdapterMatchesDesktop = (pDeviceCombo->AdapterFormat == adapterDesktopDisplayMode.Format);
				if (pDeviceCombo->IsWindowed)
					continue;
				// If we haven't found a compatible set yet, or if this set
				// is better (because it's a HAL, and/or because formats match better),
				// save it
				if (pBestDeviceCombo == NULL ||
					pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceInfo->DevType == D3DDEVTYPE_HAL ||
					pDeviceCombo->DevType == D3DDEVTYPE_HAL && pBestDeviceCombo->AdapterFormat != adapterDesktopDisplayMode.Format && bAdapterMatchesDesktop ||
					pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB )
				{
					bestAdapterDesktopDisplayMode = adapterDesktopDisplayMode;
					pBestAdapterInfo = pAdapterInfo;
					pBestDeviceInfo = pDeviceInfo;
					pBestDeviceCombo = pDeviceCombo;
					if (pDeviceInfo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB)
					{
						// This fullscreen device combo looks great -- take it
						goto EndFullscreenDeviceComboSearch;
					}
					// Otherwise keep looking for a better fullscreen device combo
				}
			}
		}
	}
EndFullscreenDeviceComboSearch:
	if (pBestDeviceCombo == NULL)
		return FALSE;

	// Need to find a display mode on the best adapter that uses pBestDeviceCombo->AdapterFormat
	// and is as close to bestAdapterDesktopDisplayMode's res as possible
	bestDisplayMode.Width = 0;
	bestDisplayMode.Height = 0;
	bestDisplayMode.Format = D3DFMT_UNKNOWN;
	bestDisplayMode.RefreshRate = 0;
	for( UINT idm = 0; idm < pBestAdapterInfo->pDisplayModeList->Count(); idm++ )
	{
		D3DDISPLAYMODE* pdm = (D3DDISPLAYMODE*)pBestAdapterInfo->pDisplayModeList->GetPtr(idm);
		if( pdm->Format != pBestDeviceCombo->AdapterFormat )
			continue;
		if( pdm->Width == m_dScnW &&
			pdm->Height == m_dScnH &&
			pdm->RefreshRate == bestAdapterDesktopDisplayMode.RefreshRate )
		{
			// found a perfect match, so stop
			bestDisplayMode = *pdm;
			break;
		}
		else if( pdm->Width == m_dScnW &&
			pdm->Height == m_dScnH &&
			pdm->RefreshRate > bestDisplayMode.RefreshRate )
		{
			// refresh rate doesn't match, but width/height match, so keep this
			// and keep looking
			bestDisplayMode = *pdm;
		}
		else if( pdm->Width == bestAdapterDesktopDisplayMode.Width )
		{
			// width matches, so keep this and keep looking
			bestDisplayMode = *pdm;
		}
		else if( bestDisplayMode.Width == 0 )
		{
			// we don't have anything better yet, so keep this and keep looking
			bestDisplayMode = *pdm;
		}
	}

	m_d3dSettings.pFullscreen_AdapterInfo = pBestAdapterInfo;
	m_d3dSettings.pFullscreen_DeviceInfo = pBestDeviceInfo;
	m_d3dSettings.pFullscreen_DeviceCombo = pBestDeviceCombo;
	m_d3dSettings.IsWindowed = FALSE;
	m_d3dSettings.Fullscreen_DisplayMode = bestDisplayMode;
	if (m_d3dEnumeration.AppUsesDepthBuffer)
		m_d3dSettings.Fullscreen_DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
	m_d3dSettings.Fullscreen_MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
	m_d3dSettings.Fullscreen_MultisampleQuality = 0;
	m_d3dSettings.Fullscreen_VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
	m_d3dSettings.Fullscreen_PresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
	return TRUE;
}




//-----------------------------------------------------------------------------
// Name: ChooseInitialD3DSettings()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ChooseInitialD3DSettings()
{
	BOOL bFoundFullscreen = FindBestFullscreenMode( FALSE, FALSE );
	BOOL bFoundWindowed = FindBestWindowedMode( FALSE, FALSE );

	if( m_bStartFull && bFoundFullscreen )
		m_d3dSettings.IsWindowed = FALSE;
	if( !bFoundWindowed && bFoundFullscreen )
		m_d3dSettings.IsWindowed = FALSE;

	if( !bFoundFullscreen && !bFoundWindowed )
		return D3DAPPERR_NOCOMPATIBLEDEVICES;

	return 0;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message handling function.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_PAINT:
		// Handle paint messages when the app is paused
		if( m_pd3dDevice && !m_bActive && m_bWindowed &&
			m_bDeviceObjectsInited && m_bDeviceObjectsRestored )
		{
			Render();
			m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
		}
		break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		break;

	case WM_ENTERSIZEMOVE:
		// Halt frame movement while the app is sizing or moving
		Pause( TRUE );
		break;

	case WM_SIZE:
		// Pick up possible changes to window style due to maximize, etc.
		if( m_bWindowed && m_hWnd != NULL )
			m_dWinStyle = GetWindowLong( m_hWnd, GWL_STYLE );

		if( SIZE_MINIMIZED == wParam )
		{
			if( m_bClipCursorWhenFullscreen && !m_bWindowed )
				ClipCursor( NULL );
			Pause( TRUE ); // Pause while we're minimized
			m_bMinimized = TRUE;
			m_bMaximized = FALSE;
		}
		else if( SIZE_MAXIMIZED == wParam )
		{
			if( m_bMinimized )
				Pause( FALSE ); // Unpause since we're no longer minimized
			m_bMinimized = FALSE;
			m_bMaximized = TRUE;
			HandlePossibleSizeChange();
		}
		else if( SIZE_RESTORED == wParam )
		{
			if( m_bMaximized )
			{
				m_bMaximized = FALSE;
				HandlePossibleSizeChange();
			}
			else if( m_bMinimized)
			{
				Pause( FALSE ); // Unpause since we're no longer minimized
				m_bMinimized = FALSE;
				HandlePossibleSizeChange();
			}
			else
			{
				// If we're neither maximized nor minimized, the window size
				// is changing by the user dragging the window edges.  In this
				// case, we don't reset the device yet -- we wait until the
				// user stops dragging, and a WM_EXITSIZEMOVE message comes.
			}
		}
		break;

	case WM_EXITSIZEMOVE:
		Pause( FALSE );
		HandlePossibleSizeChange();
		break;

	case WM_SETCURSOR:
		// Turn off Windows cursor in fullscreen mode
		if( m_bActive && !m_bWindowed )
		{
			SetCursor( NULL );
			if( m_bShowCursor)
				m_pd3dDevice->ShowCursor( TRUE );
			return TRUE; // prevent Windows from setting cursor to window class cursor
		}
		break;

	case WM_MOUSEMOVE:
		if( m_bActive && m_pd3dDevice != NULL )
		{
			POINT ptCursor;
			GetCursorPos( &ptCursor );
			if( !m_bWindowed )
				ScreenToClient( m_hWnd, &ptCursor );
			m_pd3dDevice->SetCursorPosition( ptCursor.x, ptCursor.y, 0 );
		}
		break;

	case WM_ENTERMENULOOP:
		// Pause the app when menus are displayed
		Pause(TRUE);
		break;

	case WM_EXITMENULOOP:
		Pause(FALSE);
		break;

	case WM_NCHITTEST:
		// Prevent the user from selecting the menu in fullscreen mode
		if( !m_bWindowed )
			return HTCLIENT;
		break;

	case WM_POWERBROADCAST:
		switch( wParam )
		{
#ifndef PBT_APMQUERYSUSPEND
#define PBT_APMQUERYSUSPEND 0x0000
#endif
		case PBT_APMQUERYSUSPEND:
			// At this point, the app should save any data for open
			// network connections, files, etc., and prepare to go into
			// a suspended mode.
			return TRUE;

#ifndef PBT_APMRESUMESUSPEND
#define PBT_APMRESUMESUSPEND 0x0007
#endif
	case PBT_APMRESUMESUSPEND:
		// At this point, the app should recover any data, network
		// connections, files, etc., and resume running from when
		// the app was suspended.
		return TRUE;
	}
	break;

	case WM_SYSCOMMAND:
		// Prevent moving/sizing and power loss in fullscreen mode
		switch( wParam )
		{
		case SC_MOVE:
			break;
		case SC_SIZE:
			break;
		case SC_MAXIMIZE:
			// Toggle the fullscreen/window mode
				Pause( TRUE );
				if( FAILED( ToggleFullscreen() ) )
					DisplayErrorMsg( D3DAPPERR_RESETFAILED, MSGERR_APPMUSTEXIT );
				Pause( FALSE );
				break;

		case SC_KEYMENU:
		case SC_MONITORPOWER:
			if( FALSE == m_bWindowed )
				return 0;
			break;
		}
		break;

		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDM_CHANGEDEVICE:
				// Prompt the user to select a new device or mode
				Pause(TRUE);
				UserSelectNewDevice();
				Pause(FALSE);
				return 0;

			case IDM_TOGGLEFULLSCREEN:
				// Toggle the fullscreen/window mode
				Pause( TRUE );
				if( FAILED( ToggleFullscreen() ) )
					DisplayErrorMsg( D3DAPPERR_RESETFAILED, MSGERR_APPMUSTEXIT );
				Pause( FALSE );
				return 0;

			case IDM_EXIT:
				// Recieved key/menu command to exit app
				SendMessage( hWnd, WM_CLOSE, 0, 0 );
				return 0;
			}
			break;

			case WM_CLOSE:
				Cleanup3DEnvironment();
				SAFE_RELEASE( m_pD3D );
				FinalCleanup();
				HMENU hMenu;
				hMenu = GetMenu(hWnd);
				if( hMenu != NULL )
					DestroyMenu( hMenu );
				DestroyWindow( hWnd );
				PostQuitMessage(0);
				m_hWnd = NULL;
				return 0;
}

return DefWindowProc( hWnd, uMsg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: HandlePossibleSizeChange()
// Desc: Reset the device if the client area size has changed.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::HandlePossibleSizeChange()
{
	HRESULT hr = S_OK;
	RECT rcClientOld;
	rcClientOld = m_rcWindowClient;

	if( m_bIgnoreSizeChange )
		return 0;

	// Update window properties
	GetWindowRect( m_hWnd, &m_rcWindowBounds );
	GetClientRect( m_hWnd, &m_rcWindowClient );

	if( rcClientOld.right - rcClientOld.left !=
		m_rcWindowClient.right - m_rcWindowClient.left ||
		rcClientOld.bottom - rcClientOld.top !=
		m_rcWindowClient.bottom - m_rcWindowClient.top)
	{
		// A new window size will require a new backbuffer
		// size, so the 3D structures must be changed accordingly.
		Pause( TRUE );

		m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
		m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

		if( m_pd3dDevice != NULL )
		{
			// Reset the 3D environment
			if( FAILED( hr = Reset3DEnvironment() ) )
			{
				if( hr != D3DERR_OUTOFVIDEOMEMORY )
					hr = D3DAPPERR_RESETFAILED;
				DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
			}
		}
		Pause( FALSE );
	}
	return hr;
}




//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Initialize3DEnvironment()
{
	HRESULT hr;

	D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
	D3DDeviceInfo* pDeviceInfo = m_d3dSettings.PDeviceInfo();

	m_bWindowed = m_d3dSettings.IsWindowed;

	// Prepare window for possible windowed/fullscreen change
	AdjustWindowForChange();

	// Set up the presentation parameters
	BuildPresentParamsFromSettings();

	if( pDeviceInfo->Caps.PrimitiveMiscCaps & D3DPMISCCAPS_NULLREFERENCE )
	{
		// Warn user about null ref device that can't render anything
		DisplayErrorMsg( D3DAPPERR_NULLREFDEVICE, 0 );
	}

	DWORD behaviorFlags;
	if (m_d3dSettings.GetVertexProcessingType() == SOFTWARE_VP)
		behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else if (m_d3dSettings.GetVertexProcessingType() == MIXED_VP)
		behaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
	else if (m_d3dSettings.GetVertexProcessingType() == HARDWARE_VP)
		behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else if (m_d3dSettings.GetVertexProcessingType() == PURE_HARDWARE_VP)
		behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	else
		behaviorFlags = 0; // TODO: throw exception

	// Create the device
	hr = m_pD3D->CreateDevice( m_d3dSettings.AdapterOrdinal(), pDeviceInfo->DevType,
		m_hWndFocus, behaviorFlags, &m_d3dpp,
		&m_pd3dDevice );

	if( SUCCEEDED(hr) )
	{
		// When moving from fullscreen to windowed mode, it is important to
		// adjust the window size after recreating the device rather than
		// beforehand to ensure that you get the window size you want.  For
		// example, when switching from 640x480 fullscreen to windowed with
		// a 1000x600 window on a 1024x768 desktop, it is impossible to set
		// the window size to 1000x600 until after the display mode has
		// changed to 1024x768, because windows cannot be larger than the
		// desktop.
		if( m_bWindowed )
		{
			SetWindowPos( m_hWnd, HWND_NOTOPMOST,
				m_rcWindowBounds.left, m_rcWindowBounds.top,
				( m_rcWindowBounds.right - m_rcWindowBounds.left ),
				( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
				SWP_SHOWWINDOW );
		}

		// Store device Caps
		m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );
		m_dwCreateFlags = behaviorFlags;

		// Store device description
		if( pDeviceInfo->DevType == D3DDEVTYPE_REF )
			lstrcpy( m_strDeviceStats, TEXT("REF") );
		else if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
			lstrcpy( m_strDeviceStats, TEXT("HAL") );
		else if( pDeviceInfo->DevType == D3DDEVTYPE_SW )
			lstrcpy( m_strDeviceStats, TEXT("SW") );

		if( behaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING &&
			behaviorFlags & D3DCREATE_PUREDEVICE )
		{
			if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
				lstrcat( m_strDeviceStats, TEXT(" (pure hw vp)") );
			else
				lstrcat( m_strDeviceStats, TEXT(" (simulated pure hw vp)") );
		}
		else if( behaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
		{
			if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
				lstrcat( m_strDeviceStats, TEXT(" (hw vp)") );
			else
				lstrcat( m_strDeviceStats, TEXT(" (simulated hw vp)") );
		}
		else if( behaviorFlags & D3DCREATE_MIXED_VERTEXPROCESSING )
		{
			if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
				lstrcat( m_strDeviceStats, TEXT(" (mixed vp)") );
			else
				lstrcat( m_strDeviceStats, TEXT(" (simulated mixed vp)") );
		}
		else if( behaviorFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
		{
			lstrcat( m_strDeviceStats, TEXT(" (sw vp)") );
		}

		if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
		{
			// Be sure not to overflow m_strDeviceStats when appending the adapter
			// description, since it can be long.  Note that the adapter description
			// is initially CHAR and must be converted to TCHAR.
			lstrcat( m_strDeviceStats, TEXT(": ") );
			const int cchDesc = sizeof(pAdapterInfo->AdapterIdentifier.Description);
			TCHAR szDescription[cchDesc];
			DXUtil_ConvertAnsiStringToGenericCch( szDescription,
				pAdapterInfo->AdapterIdentifier.Description, cchDesc );
			int maxAppend = sizeof(m_strDeviceStats) / sizeof(TCHAR) -
				lstrlen( m_strDeviceStats ) - 1;
			_tcsncat( m_strDeviceStats, szDescription, maxAppend );
		}

		// Store render target surface desc
		LPDIRECT3DSURFACE9 pBackBuffer = NULL;
		m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
		pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
		pBackBuffer->Release();

		// Set up the fullscreen cursor
		if( m_bShowCursor && !m_bWindowed )
		{
			HCURSOR hCursor;
#ifdef _WIN64
			hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
			hCursor = (HCURSOR)ULongToHandle( GetClassLong( m_hWnd, GCL_HCURSOR ) );
#endif
			D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, TRUE );
			m_pd3dDevice->ShowCursor( TRUE );
		}

		// Confine cursor to fullscreen window
		if( m_bClipCursorWhenFullscreen )
		{
			if (!m_bWindowed )
			{
				RECT rcWindow;
				GetWindowRect( m_hWnd, &rcWindow );
				ClipCursor( &rcWindow );
			}
			else
			{
				ClipCursor( NULL );
			}
		}


		// DX의 스프라이트는 디바이스가 생성된 후에 만들어야 한다.
		if(FAILED(D3DXCreateSprite(m_pd3dDevice, &m_pd3dSprite)))
		{
			return -1;
		}

		// Initialize the app's device-dependent objects
		hr = Init();
		if( FAILED(hr) )
		{
			Destroy();
		}
		else
		{
			m_bDeviceObjectsInited = TRUE;
			hr = Restore();

			if( FAILED(hr) )
			{
				Invalid();
			}
			else
			{
				m_bDeviceObjectsRestored = TRUE;
				return 0;
			}
		}

		// Cleanup before we try again
		Cleanup3DEnvironment();
}

// If that failed, fall back to the reference rasterizer
if( hr != D3DAPPERR_MEDIANOTFOUND &&
   hr != HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) &&
   pDeviceInfo->DevType == D3DDEVTYPE_HAL )
{
	if (FindBestWindowedMode(FALSE, TRUE))
	{
		m_bWindowed = TRUE;
		AdjustWindowForChange();
		// Make sure main window isn't topmost, so error message is visible
		SetWindowPos( m_hWnd, HWND_NOTOPMOST,
			m_rcWindowBounds.left, m_rcWindowBounds.top,
			( m_rcWindowBounds.right - m_rcWindowBounds.left ),
			( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
			SWP_SHOWWINDOW );

		// Let the user know we are switching from HAL to the reference rasterizer
		DisplayErrorMsg( hr, MSGWARN_SWITCHEDTOREF );

		hr = Initialize3DEnvironment();
	}
}
return hr;
}




//-----------------------------------------------------------------------------
// Name: BuildPresentParamsFromSettings()
// Desc:
//-----------------------------------------------------------------------------
void CD3DApplication::BuildPresentParamsFromSettings()
{
	m_d3dpp.Windowed               = m_d3dSettings.IsWindowed;
	m_d3dpp.BackBufferCount        = 1;
	m_d3dpp.MultiSampleType        = m_d3dSettings.MultisampleType();
	m_d3dpp.MultiSampleQuality     = m_d3dSettings.MultisampleQuality();
	m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.EnableAutoDepthStencil = m_d3dEnumeration.AppUsesDepthBuffer;
	m_d3dpp.hDeviceWindow          = m_hWnd;
	if( m_d3dEnumeration.AppUsesDepthBuffer )
	{
		m_d3dpp.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		m_d3dpp.AutoDepthStencilFormat = m_d3dSettings.DepthStencilBufferFormat();
	}
	else
	{
		m_d3dpp.Flags              = 0;
	}

	if( m_bWindowed )
	{
		m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
		m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
		m_d3dpp.BackBufferFormat = m_d3dSettings.PDeviceCombo()->BackBufferFormat;
		m_d3dpp.FullScreen_RefreshRateInHz = 0;
		m_d3dpp.PresentationInterval = m_d3dSettings.PresentInterval();
	}
	else
	{
		m_d3dpp.BackBufferWidth  = m_d3dSettings.DisplayMode().Width;
		m_d3dpp.BackBufferHeight = m_d3dSettings.DisplayMode().Height;
		m_d3dpp.BackBufferFormat = m_d3dSettings.PDeviceCombo()->BackBufferFormat;
		m_d3dpp.FullScreen_RefreshRateInHz = m_d3dSettings.Fullscreen_DisplayMode.RefreshRate;
		m_d3dpp.PresentationInterval = m_d3dSettings.PresentInterval();
	}
}




//-----------------------------------------------------------------------------
// Name: Reset3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Reset3DEnvironment()
{
	HRESULT hr;

	// Release all vidmem objects
	if( m_bDeviceObjectsRestored )
	{
		m_bDeviceObjectsRestored = FALSE;
		Invalid();
	}

	if(m_pd3dSprite)
		m_pd3dSprite->OnLostDevice();

	// Reset the device
	if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) )
		return hr;

	if(m_pd3dSprite)
		m_pd3dSprite->OnResetDevice();

	// Store render target surface desc
	LPDIRECT3DSURFACE9 pBackBuffer;
	m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
	pBackBuffer->Release();

	// Set up the fullscreen cursor
	if( m_bShowCursor && !m_bWindowed )
	{
		HCURSOR hCursor;
#ifdef _WIN64
		hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
		hCursor = (HCURSOR)ULongToHandle( GetClassLong( m_hWnd, GCL_HCURSOR ) );
#endif
		D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, TRUE );
		m_pd3dDevice->ShowCursor( TRUE );
	}

	// Confine cursor to fullscreen window
	if( m_bClipCursorWhenFullscreen )
	{
		if (!m_bWindowed )
		{
			RECT rcWindow;
			GetWindowRect( m_hWnd, &rcWindow );
			ClipCursor( &rcWindow );
		}
		else
		{
			ClipCursor( NULL );
		}
	}

	// Initialize the app's device-dependent objects
	hr = Restore();
	if( FAILED(hr) )
	{
		Invalid();
		return hr;
	}
	m_bDeviceObjectsRestored = TRUE;

	// If the app is paused, trigger the rendering of the current frame
	if( FALSE == m_bFrameMoving )
	{
		m_bSingleStep = TRUE;
		DXUtil_Timer( TIMER_START );
		DXUtil_Timer( TIMER_STOP );
	}

	return 0;
}




//-----------------------------------------------------------------------------
// Name: ToggleFullScreen()
// Desc: Called when user toggles between fullscreen mode and windowed mode
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ToggleFullscreen()
{
	HRESULT hr;
	int AdapterOrdinalOld = m_d3dSettings.AdapterOrdinal();
	D3DDEVTYPE DevTypeOld = m_d3dSettings.DevType();

	Pause( TRUE );
	m_bIgnoreSizeChange = TRUE;

	// Toggle the windowed state
	m_bWindowed = !m_bWindowed;
	m_d3dSettings.IsWindowed = m_bWindowed;

	// Prepare window for windowed/fullscreen change
	AdjustWindowForChange();

	// If AdapterOrdinal and DevType are the same, we can just do a Reset().
	// If they've changed, we need to do a complete device teardown/rebuild.
	if (m_d3dSettings.AdapterOrdinal() == AdapterOrdinalOld &&
		m_d3dSettings.DevType() == DevTypeOld)
	{
		// Reset the 3D device
		BuildPresentParamsFromSettings();
		hr = Reset3DEnvironment();
	}
	else
	{
		Cleanup3DEnvironment();
		hr = Initialize3DEnvironment();
	}
	if( FAILED( hr ) )
	{
		if( hr != D3DERR_OUTOFVIDEOMEMORY )
			hr = D3DAPPERR_RESETFAILED;
		m_bIgnoreSizeChange = FALSE;
		if( !m_bWindowed )
		{
			// Restore window type to windowed mode
			m_bWindowed = !m_bWindowed;
			m_d3dSettings.IsWindowed = m_bWindowed;
			AdjustWindowForChange();
			SetWindowPos( m_hWnd, HWND_NOTOPMOST,
				m_rcWindowBounds.left, m_rcWindowBounds.top,
				( m_rcWindowBounds.right - m_rcWindowBounds.left ),
				( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
				SWP_SHOWWINDOW );
		}
		return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
	}

	m_bIgnoreSizeChange = FALSE;

	// When moving from fullscreen to windowed mode, it is important to
	// adjust the window size after resetting the device rather than
	// beforehand to ensure that you get the window size you want.  For
	// example, when switching from 640x480 fullscreen to windowed with
	// a 1000x600 window on a 1024x768 desktop, it is impossible to set
	// the window size to 1000x600 until after the display mode has
	// changed to 1024x768, because windows cannot be larger than the
	// desktop.
	if( m_bWindowed )
	{
		SetWindowPos( m_hWnd, HWND_NOTOPMOST,
			m_rcWindowBounds.left, m_rcWindowBounds.top,
			( m_rcWindowBounds.right - m_rcWindowBounds.left ),
			( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
			SWP_SHOWWINDOW );
	}

	Pause( FALSE );
	return 0;
}




//-----------------------------------------------------------------------------
// Name: ForceWindowed()
// Desc: Switch to a windowed mode, even if that means picking a new device
//       and/or adapter
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ForceWindowed()
{
	HRESULT hr;

	if( m_bWindowed )
		return 0;

	if( !FindBestWindowedMode(FALSE, FALSE) )
	{
		return E_FAIL;
	}
	m_bWindowed = TRUE;

	// Now destroy the current 3D device objects, then reinitialize

	Pause( TRUE );

	// Release all scene objects that will be re-created for the new device
	Cleanup3DEnvironment();

	// Create the new device
	if( FAILED(hr = Initialize3DEnvironment() ) )
		return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

	Pause( FALSE );
	return 0;
}



HRESULT CD3DApplication::AdjustWindowForChange()
{
	if( m_bWindowed )
	{
		// Set windowed-mode style
		SetWindowLong( m_hWnd, GWL_STYLE, m_dWinStyle );
		if( m_hMenu != NULL )
		{
			SetMenu( m_hWnd, m_hMenu );
			m_hMenu = NULL;
		}
	}
	else
	{
		// Set fullscreen-mode style
		SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
		if( m_hMenu == NULL )
		{
			m_hMenu = GetMenu( m_hWnd );
			SetMenu( m_hWnd, NULL );
		}
	}
	return 0;
}




HRESULT CD3DApplication::UserSelectNewDevice()
{

	return 0;
}




INT CD3DApplication::Run()
{
	// Load keyboard accelerators
	m_hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );


	// Now we're ready to recieve and process Windows messages.
	BOOL bGotMsg;
	MSG  msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message  )
	{
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		if( m_bActive )
			bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
		else
			bGotMsg = ( GetMessage( &msg, NULL, 0U, 0U ) != 0 );

		if( bGotMsg )
		{
			// Translate and dispatch the message
			if( m_hAccel == NULL || m_hWnd == NULL || 0 == TranslateAccelerator( m_hWnd, m_hAccel, &msg ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			if( m_bDeviceLost )
			{
				// Yield some CPU time to other processes
				Sleep( 100 ); // 100 milliseconds
			}
			// Render a frame during idle time (no messages are waiting)
			if( m_bActive )
			{
				if( FAILED( Render3DEnvironment() ) )
					break;
			}
		}
	}

	return (INT)msg.wParam;
}





HRESULT CD3DApplication::Render3DEnvironment()
{
	HRESULT hr;

	if( m_bDeviceLost )
	{
		// Test the cooperative level to see if it's okay to render
		if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
		{
			// If the device was lost, do not render until we get it back
			if( D3DERR_DEVICELOST == hr )
				return 0;

			// Check if the device needs to be reset.
			if( D3DERR_DEVICENOTRESET == hr )
			{
				// If we are windowed, read the desktop mode and use the same format for
				// the back buffer
				if( m_bWindowed )
				{
					D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
					m_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &m_d3dSettings.Windowed_DisplayMode );
					m_d3dpp.BackBufferFormat = m_d3dSettings.Windowed_DisplayMode.Format;
				}

				if( FAILED( hr = Reset3DEnvironment() ) )
					return hr;
			}
			return hr;
		}
		m_bDeviceLost = FALSE;
	}

	// Get the app's time, in seconds. Skip rendering if no time elapsed
	FLOAT fAppTime        = DXUtil_Timer( TIMER_GETAPPTIME );
	FLOAT fElapsedAppTime = DXUtil_Timer( TIMER_GETELAPSEDTIME );
	if( ( 0.0f == fElapsedAppTime ) && m_bFrameMoving )
		return 0;

	// FrameMove (animate) the scene
	if( m_bFrameMoving || m_bSingleStep )
	{
		// Store the time for the app
		m_fTime        = fAppTime;
		m_fElapsedTime = fElapsedAppTime;

		// Frame move the scene
		if( FAILED( hr = FrameMove() ) )
			return hr;

		m_bSingleStep = FALSE;
	}

	// Render the scene as normal
	if( FAILED( hr = Render() ) )
		return hr;

	UpdateStats();

	// Show the frame on the primary surface.
	hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	if( D3DERR_DEVICELOST == hr )
		m_bDeviceLost = TRUE;

	return 0;
}




//-----------------------------------------------------------------------------
// Name: UpdateStats()
// Desc:
//-----------------------------------------------------------------------------
void CD3DApplication::UpdateStats()
{
	// Keep track of the frame count
	static FLOAT fLastTime = 0.0f;
	static DWORD dwFrames  = 0;
	FLOAT fTime = DXUtil_Timer( TIMER_GETABSOLUTETIME );
	++dwFrames;

	// Update the scene stats once per second
	if( dwFrames> 4 )
	{
		m_fFPS    = dwFrames / (fTime - fLastTime);
		fLastTime = fTime;
		dwFrames  = 0;

		TCHAR strFmt[100];
		D3DFORMAT fmtAdapter = m_d3dSettings.DisplayMode().Format;
		if( fmtAdapter == m_d3dsdBackBuffer.Format )
		{
			lstrcpyn( strFmt, D3DUtil_D3DFormatToString( fmtAdapter, FALSE ), 100 );
		}
		else
		{
			_sntprintf( strFmt, 100, TEXT("backbuf %s, adapter %s"),
				D3DUtil_D3DFormatToString( m_d3dsdBackBuffer.Format, FALSE ),
				D3DUtil_D3DFormatToString( fmtAdapter, FALSE ) );
		}
		strFmt[99] = TEXT('\0');

		TCHAR strDepthFmt[100];
		if( m_d3dEnumeration.AppUsesDepthBuffer )
		{
			_sntprintf( strDepthFmt, 100, TEXT(" (%s)"),
				D3DUtil_D3DFormatToString( m_d3dSettings.DepthStencilBufferFormat(), FALSE ) );
			strDepthFmt[99] = TEXT('\0');
		}
		else
		{
			// No depth buffer
			strDepthFmt[0] = TEXT('\0');
		}

		TCHAR* pstrMultiSample;
		switch( m_d3dSettings.MultisampleType() )
		{
		case D3DMULTISAMPLE_NONMASKABLE:  pstrMultiSample = TEXT(" (Nonmaskable Multisample)"); break;
		case D3DMULTISAMPLE_2_SAMPLES:  pstrMultiSample = TEXT(" (2x Multisample)"); break;
		case D3DMULTISAMPLE_3_SAMPLES:  pstrMultiSample = TEXT(" (3x Multisample)"); break;
		case D3DMULTISAMPLE_4_SAMPLES:  pstrMultiSample = TEXT(" (4x Multisample)"); break;
		case D3DMULTISAMPLE_5_SAMPLES:  pstrMultiSample = TEXT(" (5x Multisample)"); break;
		case D3DMULTISAMPLE_6_SAMPLES:  pstrMultiSample = TEXT(" (6x Multisample)"); break;
		case D3DMULTISAMPLE_7_SAMPLES:  pstrMultiSample = TEXT(" (7x Multisample)"); break;
		case D3DMULTISAMPLE_8_SAMPLES:  pstrMultiSample = TEXT(" (8x Multisample)"); break;
		case D3DMULTISAMPLE_9_SAMPLES:  pstrMultiSample = TEXT(" (9x Multisample)"); break;
		case D3DMULTISAMPLE_10_SAMPLES: pstrMultiSample = TEXT(" (10x Multisample)"); break;
		case D3DMULTISAMPLE_11_SAMPLES: pstrMultiSample = TEXT(" (11x Multisample)"); break;
		case D3DMULTISAMPLE_12_SAMPLES: pstrMultiSample = TEXT(" (12x Multisample)"); break;
		case D3DMULTISAMPLE_13_SAMPLES: pstrMultiSample = TEXT(" (13x Multisample)"); break;
		case D3DMULTISAMPLE_14_SAMPLES: pstrMultiSample = TEXT(" (14x Multisample)"); break;
		case D3DMULTISAMPLE_15_SAMPLES: pstrMultiSample = TEXT(" (15x Multisample)"); break;
		case D3DMULTISAMPLE_16_SAMPLES: pstrMultiSample = TEXT(" (16x Multisample)"); break;
		default:                        pstrMultiSample = TEXT(""); break;
		}

		const int cchMaxFrameStats = sizeof(m_strFrameStats) / sizeof(TCHAR);
		_sntprintf( m_strFrameStats, cchMaxFrameStats, _T("(%dx%d), %s%s%s %.f fps"),
			m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height,
			strFmt, strDepthFmt, pstrMultiSample, m_fFPS);

		m_strFrameStats[cchMaxFrameStats - 1] = TEXT('\0');
	}
}




//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
void CD3DApplication::Pause( BOOL bPause )
{
	static DWORD dwAppPausedCount = 0;

	dwAppPausedCount += ( bPause ? +1 : -1 );
	m_bActive         = ( dwAppPausedCount ? FALSE : TRUE );

	// Handle the first pause request (of many, nestable pause requests)
	if( bPause && ( 1 == dwAppPausedCount ) )
	{
		// Stop the scene from animating
		if( m_bFrameMoving )
			DXUtil_Timer( TIMER_STOP );
	}

	if( 0 == dwAppPausedCount )
	{
		// Restart the timers
		if( m_bFrameMoving )
			DXUtil_Timer( TIMER_START );
	}
}




//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
void CD3DApplication::Cleanup3DEnvironment()
{
	if( m_pd3dDevice != NULL )
	{
		if( m_bDeviceObjectsRestored )
		{
			m_bDeviceObjectsRestored = FALSE;
			Invalid();
		}
		if( m_bDeviceObjectsInited )
		{
			m_bDeviceObjectsInited = FALSE;
			Destroy();
		}

		SAFE_RELEASE(	m_pd3dSprite	);
		SAFE_RELEASE( m_pd3dDevice		);
	}
}




//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::DisplayErrorMsg( HRESULT hr, DWORD dwType )
{
	static BOOL s_bFatalErrorReported = FALSE;
	TCHAR strMsg[512];

	// If a fatal error message has already been reported, the app
	// is already shutting down, so don't show more error messages.
	if( s_bFatalErrorReported )
		return hr;

	switch( hr )
	{
	case D3DAPPERR_NODIRECT3D:
		_tcscpy( strMsg, _T("Could not initialize Direct3D. You may\n")
			_T("want to check that the latest version of\n")
			_T("DirectX is correctly installed on your\n")
			_T("system.  Also make sure that this program\n")
			_T("was compiled with header files that match\n")
			_T("the installed DirectX DLLs.") );
		break;

	case D3DAPPERR_NOCOMPATIBLEDEVICES:
		_tcscpy( strMsg, _T("Could not find any compatible Direct3D\n")
			_T("devices.") );
		break;

	case D3DAPPERR_NOWINDOWABLEDEVICES:
		_tcscpy( strMsg, _T("This sample cannot run in a desktop\n")
			_T("window with the current display settings.\n")
			_T("Please change your desktop settings to a\n")
			_T("16- or 32-bit display mode and re-run this\n")
			_T("sample.") );
		break;

	case D3DAPPERR_NOHARDWAREDEVICE:
		_tcscpy( strMsg, _T("No hardware-accelerated Direct3D devices\n")
			_T("were found.") );
		break;

	case D3DAPPERR_HALNOTCOMPATIBLE:
		_tcscpy( strMsg, _T("This sample requires functionality that is\n")
			_T("not available on your Direct3D hardware\n")
			_T("accelerator.") );
		break;

	case D3DAPPERR_NOWINDOWEDHAL:
		_tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
			_T("render into a window.\n")
			_T("Press F2 while the app is running to see a\n")
			_T("list of available devices and modes.") );
		break;

	case D3DAPPERR_NODESKTOPHAL:
		_tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
			_T("render into a window with the current\n")
			_T("desktop display settings.\n")
			_T("Press F2 while the app is running to see a\n")
			_T("list of available devices and modes.") );
		break;

	case D3DAPPERR_NOHALTHISMODE:
		_tcscpy( strMsg, _T("This sample requires functionality that is\n")
			_T("not available on your Direct3D hardware\n")
			_T("accelerator with the current desktop display\n")
			_T("settings.\n")
			_T("Press F2 while the app is running to see a\n")
			_T("list of available devices and modes.") );
		break;

	case D3DAPPERR_MEDIANOTFOUND:
	case ERROR_FILE_NOT_FOUND:
		_tcscpy( strMsg, _T("Could not load required media." ) );
		break;

	case D3DAPPERR_RESETFAILED:
		_tcscpy( strMsg, _T("Could not reset the Direct3D device." ) );
		break;

	case D3DAPPERR_NONZEROREFCOUNT:
		_tcscpy( strMsg, _T("A D3D object has a non-zero reference\n")
			_T("count (meaning things were not properly\n")
			_T("cleaned up).") );
		break;

	case D3DAPPERR_NULLREFDEVICE:
		_tcscpy( strMsg, _T("Warning: Nothing will be rendered.\n")
			_T("The reference rendering device was selected, but your\n")
			_T("computer only has a reduced-functionality reference device\n")
			_T("installed.  Install the DirectX SDK to get the full\n")
			_T("reference device.\n") );
		break;

	case E_OUTOFMEMORY:
		_tcscpy( strMsg, _T("Not enough memory.") );
		break;

	case D3DERR_OUTOFVIDEOMEMORY:
		_tcscpy( strMsg, _T("Not enough video memory.") );
		break;

	default:
		_tcscpy( strMsg, _T("Generic application error. Enable\n")
			_T("debug output for detailed information.") );
	}

	if( MSGERR_APPMUSTEXIT == dwType )
	{
		s_bFatalErrorReported = TRUE;
		_tcscat( strMsg, _T("\n\nThis sample will now exit.") );
		MessageBox( NULL, strMsg, m_sCls, MB_ICONERROR|MB_OK );

		// Close the window, which shuts down the app
		if( m_hWnd )
			SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
	}
	else
	{
		if( MSGWARN_SWITCHEDTOREF == dwType )
			_tcscat( strMsg, _T("\n\nSwitching to the reference rasterizer,\n")
			_T("a software device that implements the entire\n")
			_T("Direct3D feature set, but runs very slowly.") );
		MessageBox( NULL, strMsg, m_sCls, MB_ICONWARNING|MB_OK );
	}

	return hr;
}



HRESULT CD3DApplication::OneTimeSceneInit()
{
	SendMessage( m_hWnd, WM_PAINT, 0, 0 );

	m_bLoadingApp = FALSE;

	return 0;
}



HRESULT CD3DApplication::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format )
{
	UNREFERENCED_PARAMETER( Format );
	UNREFERENCED_PARAMETER( dwBehavior );
	UNREFERENCED_PARAMETER( pCaps );

	BOOL bCapsAcceptable;

	bCapsAcceptable = TRUE;

	if( bCapsAcceptable )
		return 0;
	else
		return E_FAIL;
}







HRESULT CD3DApplication::FinalCleanup()
{
	return 0;
}