// Implementation of the CLcSwpWn class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3d9.h>

#include "LcSwpWn.h"


CLcSwpWn::CLcSwpWn()
{
	m_pDev	= NULL;
	
	m_hSwW	= NULL;
	m_pSwS	= NULL;
	m_pSwC	= NULL;
	m_pSwD	= NULL;	

	m_pDevC	= NULL;
	m_pDevD	= NULL;
}

CLcSwpWn::~CLcSwpWn()
{
	Destroy();
}

void CLcSwpWn::Destroy()
{
	if(m_pSwS)	{	m_pSwS->Release();	m_pSwS = NULL;	}
	if(m_pSwC)	{	m_pSwC->Release();	m_pSwC = NULL;	}
	if(m_pSwD)	{	m_pSwD->Release();	m_pSwD = NULL;	}

	if(m_pDevC)	{	m_pDevC->Release();	m_pDevC = NULL;	}
	if(m_pDevD)	{	m_pDevD->Release();	m_pDevD = NULL;	}
}


INT CLcSwpWn::Create(HWND hWnd, LPDIRECT3DDEVICE9 pDev)
{
	m_hSwW	= hWnd;
	m_pDev	= pDev;

	return OnResetDevice();
}

INT CLcSwpWn::Begin()
{
	HRESULT hr=0;
	hr = m_pDev->SetRenderTarget(0, m_pSwC);
	hr = m_pDev->SetDepthStencilSurface(m_pSwD);

	return hr;
}


INT	CLcSwpWn::End()
{
	HRESULT hr=0;
	m_pSwS->Present(0, 0, 0, 0, 0);

	m_pDev->SetRenderTarget(0, m_pDevC);
	m_pDev->SetDepthStencilSurface(m_pDevD);

	return hr;
}



INT CLcSwpWn::OnResetDevice()
{
	if(m_pSwS)
		return 0;

	// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼의 속성을 가져온다.
	D3DSURFACE_DESC		dscC;
	D3DSURFACE_DESC		dscD;

	m_pDev->GetRenderTarget(0, &m_pDevC);
	m_pDev->GetDepthStencilSurface(&m_pDevD);

	m_pDevC->GetDesc(&dscC);
	m_pDevD->GetDesc(&dscD);
	

	// Present Parameter 속성을 설정한다.
	// BackBuffer Width, Height가 0이면 뷰포트가 자동으로
	// 윈도우 영역만큼 설정 된다.
	
	D3DPRESENT_PARAMETERS	d3dParam;
	memset(&d3dParam, 0, sizeof(D3DPRESENT_PARAMETERS));
	
	d3dParam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dParam.Windowed				= TRUE;
	d3dParam.BackBufferWidth		= 0;
	d3dParam.BackBufferHeight		= 0;
	d3dParam.BackBufferFormat		= dscC.Format;
	d3dParam.AutoDepthStencilFormat	= dscD.Format;
	d3dParam.hDeviceWindow			= m_hSwW;

	// Swap Chain을 생성 한다.
	if( FAILED(m_pDev->CreateAdditionalSwapChain(&d3dParam, &m_pSwS)))
		return -1;

	// Swap Chain으로 생성된 후면 버퍼의 색상 버퍼를 가져온다.
	if( FAILED(m_pSwS->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pSwC)))
		return -1;

	// Swap Chain을 위한 깊이, 스텐실 버퍼를 생성한다.
	if ( FAILED(m_pDev->CreateDepthStencilSurface(
								d3dParam.BackBufferWidth
								, d3dParam.BackBufferHeight
								, d3dParam.AutoDepthStencilFormat
								, D3DMULTISAMPLE_NONE, 0, 0
								, &m_pSwD
								, NULL)))
		return -1;


	return 0;
}


INT	CLcSwpWn::OnLostDevice()
{
	Destroy();
	return 0;
}
