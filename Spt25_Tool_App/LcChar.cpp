// Implementation of the CLcChar class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CLcChar::CLcChar()
{
	m_pLcSprite	= NULL;
	m_pTex		= NULL;

	m_iY = 0;
	m_iX = 0;

	m_vcP = D3DXVECTOR2(210, 350);

	m_fSp = 2.f;
	m_vcS = D3DXVECTOR2(0.7f, 0.7f);

	m_nAni	= 0;
}

CLcChar::~CLcChar()
{
	Destroy();
}

void CLcChar::Destroy()
{
	SAFE_DELETE(	m_pTex	);
}


INT CLcChar::Create(CLcSprite* pSprite, char* sTex)
{
	m_pLcSprite	= pSprite;

	SAFE_NEWCREATE3(m_pTex, CLcTexture, pSprite->GetDevice(), sTex, 0xFF000000);


	INT	nImgW	= m_pTex->GetImageWidth();
	INT	nImgH	= m_pTex->GetImageHeight();


	for(int j=0; j<9; ++j)
	{
		for(int i=0; i<7;++i)
		{
			long x0 = nImgW/7 * i;
			long y0 = nImgH/9 * j;

			long x1 = nImgW/7 * (i+1);
			long y1 = nImgH/9 * (j+1);
			
			SetRect(&m_rcA[j][i], x0, y0, x1, y1);
		}
	}

	m_dBgn =timeGetTime();
	
	return 0;
}
	


INT CLcChar::FrameMove()
{
	INT	bKey[4]={0};

	if(GINPUT->KeyState(VK_UP))
		bKey[0] = 1;

	if(GINPUT->KeyState(VK_DOWN))
		bKey[1] = 1;

	if(GINPUT->KeyState(VK_LEFT))
		bKey[2] = 1;

	if(GINPUT->KeyState(VK_RIGHT))
		bKey[3] = 1;


	m_nAni = 0;
	
	if(bKey[0] && !bKey[1] && !bKey[2] && !bKey[3])
	{
		m_iY = 0;
		m_vcP.y -=m_fSp;

		m_nAni = 1;
	}

	if(bKey[0] && !bKey[1] && !bKey[2] && bKey[3])
	{
		m_iY = 1;
		m_vcP.y -=m_fSp*0.707f;
		m_vcP.x +=m_fSp*0.707f;

		m_nAni = 1;
	}

	if(!bKey[0] && !bKey[1] && !bKey[2] && bKey[3])
	{
		m_iY = 2;
		m_vcP.x +=m_fSp;
		m_nAni = 1;
	}

	if(!bKey[0] && bKey[1] && !bKey[2] && bKey[3])
	{
		m_iY = 3;
		m_vcP.y +=m_fSp*0.707f;
		m_vcP.x +=m_fSp*0.707f;
		m_nAni = 1;
	}

	if(!bKey[0] && bKey[1] && !bKey[2] && !bKey[3])
	{
		m_iY = 4;
		m_vcP.y +=m_fSp;
		m_nAni = 1;
	}

	if(!bKey[0] && bKey[1] && bKey[2] && !bKey[3])
	{
		m_iY = 5;
		m_vcP.y +=m_fSp*0.707f;
		m_vcP.x -=m_fSp*0.707f;
		m_nAni = 1;
	}

	if(!bKey[0] && !bKey[1] && bKey[2] && !bKey[3])
	{
		m_iY = 6;
		m_vcP.x -=m_fSp;
		m_nAni = 1;
	}

	if(bKey[0] && !bKey[1] && bKey[2] && !bKey[3])
	{
		m_iY = 7;
		m_vcP.y -=m_fSp*0.707f;
		m_vcP.x -=m_fSp*0.707f;
		m_nAni = 1;
	}
	

	if(GINPUT->KeyState(VK_LCONTROL))
		m_nAni = 5;


	if(!m_nAni)
		m_iX = 0;
	else
	{
		DWORD	dEnd = timeGetTime();
	
		if(dEnd>= (m_dBgn+66))
		{
			m_dBgn = dEnd;
			++m_iX;

			if(1 == m_nAni)
			{
				if(m_iX>=4)
					m_iX = 1;
			}

			else if(5 == m_nAni)
			{
				if(m_iX>=6)
					m_iX = 2;
			}
		}
	}
	
	return 0;
}


void CLcChar::Render()
{
	m_pLcSprite->Begin();

	m_pLcSprite->Draw(m_pTex->GetTexture()
						, &m_rcA[m_iY][m_iX],&m_vcS
						, NULL
						, 0
						, &m_vcP
						, D3DXCOLOR(1,1,1,1)
						);

	m_pLcSprite->End();
}