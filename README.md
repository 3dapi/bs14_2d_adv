

# ◈2D 게임 지형과 툴 프로그래밍◈

◈2D 게임 지형과 툴 프로그래밍◈


지형 툴(Map Tool)이 없이 게임을 얼마든지 만들 수 있지만 많은 스테이지를 요구하거나


넓은 지형을 요구하는 게임을 툴이 없이 만들기란 대단히 어렵습니다. 지형 툴을 만드는


것은 손으로 작성한 것보다 표현력이 떨어지지만 광활한 지형을 만들기 위해 꼭 필요하며


각각의 장면에서 연출되는 게임의 기획적인 요소를 적절히 배치하는 데도 꼭 필요합니다.


지형 툴은 프로그래머가 아닌 게임 기획자를 위한 소프트웨어 입니다. 즉 기획의 내용을


가장 충실히 반영할 수 있는 툴을 만들어야 하며 이러한 툴이 가장 좋은 툴이라 할 수


있습니다.




종종 프로그래머들이 MS 워드나 Adobe의 포토샵(Photoshop) 같이 두루 사용하는 범용 툴을


만들기 위해 노력하기도 하는데 이러한 것은 현실이 뒷받침 되지 않아 생각을 달리 하는


것이 좋습니다. 그러한 것은 게임마다 장르마다 기획이 다른고 가장 중요한 개발 기간이


넉넉하지 않기 때문입니다.




게임프로그램에서는 크게 지형 툴, 캐릭터 툴, 이펙트 툴 세가지로 나누어서 개발하고,


종종 UI 툴, 데이터 익스포트 툴 등을 만들기도 하는데 이중에서 지형 툴이 가장 난이도가


높고 개발 기간이 가장 길며, 게임의 색깔을 결정 짓는 툴이 됩니다.




지형 툴의 구성은 화면 연출부분과 게임의 이벤트 구성 두 부분으로 나눌 수 있습니다. 화면


연출은 지형의 구성, 오브젝트 배치, 효과음 설정, 지형 이펙트 등이 포함이 되고, 이벤트


부분은 게임의 기획적인 요소로 캐릭터의 움직임 속도, 충돌, 특정영역에서의 이벤트 등이


포함이 되어야 합니다.




이 강의에서는 이벤트 부분을 제외한 화면 연출 부분에서 타일을 이용한 지형 구성만


다루겠습니다. 타일은 반복 이미지의 최소 패턴이라 할 수 있습니다. 가장 많이 사용되는


타일은 모바일 게임 “그녀의 기사단”의 배경 지형처럼 x 축과 축에 평행한 정사각형 타일과


블리자드 사의 스타크래프트 와 같이 3차원 직교 투영(Orthographic Projection)의 한 종류인


Isometric Projection(등각, 등축 투영)을 2차원에서 표현한 등각 타일(Isometric Tile)


두 종류가 있습니다.




정사각형 직교 타일은 초보자도 쉽게 구현하기 쉽고, 3D로 전환 하기도 쉽습니다. 이 타일을


3D 패키지 게임에서도 볼 수 있는데 블리자드 사의 WarIII를 들 수 있습니다. 배경은 3차원


이지만 이 지형에 표현된 그래픽은 2차원 타일과 동일한 형식으로 표현되어 있습니다.




등각 타일은 2D에서 3D 배경 효과를 발휘할 수 있지만 그래픽, 프로그램 양쪽 모두 난이도가


높은 반면 표현력 하나는 2D 지형에서는 최고라 할 수 있습니다.


이 강의에서는 타일링(Tiling)의 원리를 설명하고 타일링 중에서 정사각형에 의한 직교


타일을 툴로 만드는 과정을 설명하겠습니다.














## 1. 타일링(Tiling) 원리

1. 타일링(Tiling) 원리


### 1. 축에 평행한 정사각형 직교 타일

1. 축에 평행한 정사각형 직교 타일


앞서 타일은 최소화 패턴이라 했는데 화면(Screen)의 축과 평행한 직교 타일의 패턴은 다음


그림과 같이 정리 됩니다.






![tile001.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tile001.png)

![tile002.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tile002.png)



![tile003.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tile003.png)

![tile004.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tile004.png)







직교 타일은 회전을 고려하지 않는다면 위의 그림처럼 최소 19개의 조각 이미지가 필요합니다.


DirectX는 이미지를 텍스처로 변환할 때 2의 승수로 맞춥니다. 따라서 게임에서 사용하는


이미지는 메모리의 낭비를 막기 위해 대부분 가로의 폭을 2의 승수로 정합니다.




위에서 보여지는 패턴에서 14, 15, 16, 17, 18, 19 패턴은 자주 사용되지 않기 때문에 대부분


총 13개의 패턴과 5번 패턴에 대한 확장 이미지 5-2, 5-3, 5-4 를 만들어서 다음과 같이 타일


이미지를 구성합니다.






![map_rect001.bmp](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/map_rect001.bmp)







타일 배치 방법을 알기 위해 간단하게 다음과 같은 2종류의 타일을 이용해서 지형을


구성해 봅시다.






![tile005.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tile005.png)









위의 그림에서 붉은 색 글씨는 타일의 인덱스를 나타내기 위한 가상의 표시입니다.


이 타일을 사용하는 지형을 만든다면 먼저 각각의 타일에 대한 정보를 저장하기 위한


구조체를 다음과 같이 설정합니다.


```

	struct LcTile
	{
		INT		nIdx	;		// Tile Index
		RECT		rc	;		// Tile Image Rect
		D3DXVECTOR2	p	;		// Tile Position
	};

```

struct LcTile
	{
		INT		nIdx	;		// Tile Index
		RECT		rc	;		// Tile Image Rect
		D3DXVECTOR2	p	;		// Tile Position
	};


다음으로 타일을 이용한 가상의 지형을 다음과 같은 형식으로 구성한다고 가정해봅시다.


```

	Texture		Texture/map_rect002.bmp
	TileWidth	32
	TileMaxX	6
	TileMaxY	3
	TileNumX	14
	TileNumY	9
	   7   7   7   7   7   7   7   7   7   7   7   7   7   7
	   7   3   4   4   5   7   7   7   7   7   7   7   7   7
	   7   9  10  10  11   7   7   7   7   7   7   7   7   7
	   7   9  10  10  12   4   4   5   7   7   3   4   5   7
	   7   9  10  10  10  10  10  11   7   7   8  10  11   7
	   7   9  10  10   0   1   2  11   7   7   8  10  11   7
	   7   9  10  10  11   7   8  11   7   7   8  10  12   4
	   7   9  10  10  11   7   8  12   4   4  14  10  10  10
	   7  15  16  16  17   7   8  10  10  10  10  10  10  10

```

Texture		Texture/map_rect002.bmp
	TileWidth	32
	TileMaxX	6
	TileMaxY	3
	TileNumX	14
	TileNumY	9
	   7   7   7   7   7   7   7   7   7   7   7   7   7   7
	   7   3   4   4   5   7   7   7   7   7   7   7   7   7
	   7   9  10  10  11   7   7   7   7   7   7   7   7   7
	   7   9  10  10  12   4   4   5   7   7   3   4   5   7
	   7   9  10  10  10  10  10  11   7   7   8  10  11   7
	   7   9  10  10   0   1   2  11   7   7   8  10  11   7
	   7   9  10  10  11   7   8  11   7   7   8  10  12   4
	   7   9  10  10  11   7   8  12   4   4  14  10  10  10
	   7  15  16  16  17   7   8  10  10  10  10  10  10  10


위에서 Texture는 텍스처 파일의 이름을 나타냅니다. TileWidth는 타일의 폭으로 32 픽셀로


지정되어 있습니다. TileMaxX와 TileMaxY는 이미지에 대해서 타일의 최대 개수를 나타내고


있는데 위의 그림과 비교해서 보면 X축으로는 6개 Y축으로는 3개로 구성되어 있습니다.


TileNumX과 TileNumY는 위의 타일 이미지를 가지고 실제로 구성한 지형에서 X축과 Y축에


대한 최대 타일 수입니다. 나머지 숫자들은 타일의 인덱스를 가리킵니다.




이것을 파일로 작성했다면 fscanf()와 같은 함수를 사용해서 먼저 다음과 같이 파일에서 읽습니다


```

	LcTile**	m_pTile		;	// Tiles
	…
	fscanf(fp,"%*s %s", m_sBmp);
	fscanf(fp,"%*s %d", &m_nTileWidth);
	fscanf(fp,"%*s %d", &m_nTileMaxX);
	fscanf(fp,"%*s %d", &m_nTileMaxY);
	fscanf(fp,"%*s %d", &m_nTileNumX);
	fscanf(fp,"%*s %d", &m_nTileNumY);
	…
	for(y=0; y< m_nTileNumY; ++y)
	{
		m_pTile[y] = new CLcTile::LcTile[m_nTileNumX];

		for(x=0; x< m_nTileNumX; ++x)
			fscanf(fp,"%d", &m_pTile[y][x].nIdx);
	}

```

LcTile**	m_pTile		;	// Tiles
	…
	fscanf(fp,"%*s %s", m_sBmp);
	fscanf(fp,"%*s %d", &m_nTileWidth);
	fscanf(fp,"%*s %d", &m_nTileMaxX);
	fscanf(fp,"%*s %d", &m_nTileMaxY);
	fscanf(fp,"%*s %d", &m_nTileNumX);
	fscanf(fp,"%*s %d", &m_nTileNumY);
	…
	for(y=0; y< m_nTileNumY; ++y)
	{
		m_pTile[y] = new CLcTile::LcTile[m_nTileNumX];

		for(x=0; x< m_nTileNumX; ++x)
			fscanf(fp,"%d", &m_pTile[y][x].nIdx);
	}


다음으로 각각의 타일에 대해서 타일 인덱스를 가지고 이미지 영역과 위치를 설정합니다.


```

	for(y=0; y< m_nTileNumY; ++y)
	{
		for(x=0; x< m_nTileNumX; ++x)
		{
			CLcTile::LcTile*	pTile = &m_pTile[y][x];
			INT	nX	= pTile->nIdx%m_nTileMaxX;
			INT	nY	= pTile->nIdx/m_nTileMaxX;

			pTile->p.x	= FLOAT(x*m_nTileWidth);
			pTile->p.y	= FLOAT(y*m_nTileWidth);

			pTile->rc.left	= nX*m_nTileWidth;
			pTile->rc.top	= nY*m_nTileWidth;
			pTile->rc.right	= nX*m_nTileWidth+m_nTileWidth;
			pTile->rc.bottom= nY*m_nTileWidth+m_nTileWidth;
		}
	}

```

for(y=0; y< m_nTileNumY; ++y)
	{
		for(x=0; x< m_nTileNumX; ++x)
		{
			CLcTile::LcTile*	pTile = &m_pTile[y][x];
			INT	nX	= pTile->nIdx%m_nTileMaxX;
			INT	nY	= pTile->nIdx/m_nTileMaxX;

			pTile->p.x	= FLOAT(x*m_nTileWidth);
			pTile->p.y	= FLOAT(y*m_nTileWidth);

			pTile->rc.left	= nX*m_nTileWidth;
			pTile->rc.top	= nY*m_nTileWidth;
			pTile->rc.right	= nX*m_nTileWidth+m_nTileWidth;
			pTile->rc.bottom= nY*m_nTileWidth+m_nTileWidth;
		}
	}


각각의 타일에 대한 정보를 다 채웠다면 화면에 출력하기만 하면 됩니다. 화면에 출력할


때에는 지형의 위치가 이동할 수 있으므로 지형의 시작 위치에 타일의 위치를 더해서


출력을 합니다.


```

	for(y=iBgnY; y< m_nTileNumY; ++y)
	{
		for(x=iBgnX; x< m_nTileNumX; ++x)
		{
			CLcTile::LcTile* pTile = &m_pTile[y][x];
			D3DXVECTOR2	vcP(0,0);

			vcP	= m_vcP + pTile->p;
			m_pLcSprite->Draw(m_pTxMap, &pTile->rc, NULL, NULL, 0
					, &vcP, D3DXCOLOR(1,1,1,1));
			}
		}
	}

```

for(y=iBgnY; y< m_nTileNumY; ++y)
	{
		for(x=iBgnX; x< m_nTileNumX; ++x)
		{
			CLcTile::LcTile* pTile = &m_pTile[y][x];
			D3DXVECTOR2	vcP(0,0);

			vcP	= m_vcP + pTile->p;
			m_pLcSprite->Draw(m_pTxMap, &pTile->rc, NULL, NULL, 0
					, &vcP, D3DXCOLOR(1,1,1,1));
			}
		}
	}




![tile006.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tile006.png)





전체 코드는 다음 예제를 살펴 보기 바랍니다.




[Spt11_tile_rect.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt11_tile_rect.zip)
Spt11_tile_rect.zip














## 2. 지형 툴(Map Tool) 제작

2. 지형 툴(Map Tool) 제작


지형 툴은 위의 직교 타일이나 등각 타일을 구성하고, 이동, 배경 사운드, 기타 게임에서


필요한 지형의 성질 등을 설정하는 프로그램으로 주요 사용자가 게임 기획자라는 전제하에


제작을 해야 합니다.




툴에서 구현해야 할 요구 사항을 큰 틀에서 정리해 보면 다음과 같습니다.
```

	1. 지형의 전체 크기를 설정한다.
	2. 지형의 렌더링에 대한 데이터를 등을 설정한다.
	3. 게임의 기획에서 지형에 대한 속성을 적용할 수 있도록 한다.
	4. 작업물을 저장할 수 있고, 저장된 작업물을 불러와서 수정할 수 있도록 한다.

```

1. 지형의 전체 크기를 설정한다.
	2. 지형의 렌더링에 대한 데이터를 등을 설정한다.
	3. 게임의 기획에서 지형에 대한 속성을 적용할 수 있도록 한다.
	4. 작업물을 저장할 수 있고, 저장된 작업물을 불러와서 수정할 수 있도록 한다.


이 강좌에서는 직교 지형의 렌더링과 작업물의 저장, 불러오기만 구현해 보기만 하겠습니다.


지형 속성은 위의 타일 구조체를 변경해서 사용하면 되기 때문에 여러분들이 직접 구현해


보기 바랍니다.












### 1. Swap Chain

1. Swap Chain


위의 툴에 대한 요구사항을 구현하기 위해서 먼저 프로그램 인터페이스를 구성해야 하는데


지형 파일을 bmp, png, jpg, tga 등 DirectX에서 지원이 되는 모든 이미지 파일을 사용할


계획입니다.


그런데 이들의 파일 형식은 bmp처럼 쉽게 구성되어 있는 것도 있지만 png, jpg처럼 파일의


구성이 복잡한 것도 있습니다. png, jpg 파일을 다룰 수 있는 라이브러리를 구해서 사용해도


되지만 너무 손이 많이 가는 작업이라 Direct3D를 사용하는 것이 편리합니다.




하나의 화면을 분할해 사용하는 경우라면 뷰 포트(View port)를 나누어서 사용합니다.


그런데 툴과 같은 프로그램은 다중 윈도우를 사용하는 경우가 많습니다. 이럴 때 여러


윈도우에 장면을 연출하려고 할 때 각각의 IDirect3DDevice9 객체들을 생성해서 사용해야


하는데 이런 경우 디바이스 객체들 모두를 생성하는 것과 정보 교환에 대해서 막대한


오버헤드에 대한 비용이 듭니다.




이렇게 다중 윈도우에 대해서 DirectX는 IDirect3DDevice9 객체들을 각각 생성하지 않고


Swap Chain을 이용해서 오버헤드를 줄이는 방법을 제공합니다.




Swap Chain을 이용하기 위해서는 다음과 같이 윈도우 핸들, Swap Chain 객체, Swap Chain


색상 버퍼(옵션), Swap Chain 깊이 버퍼(옵션), 4개의 객체가 필요합니다.


```

	HWND			m_hSwW;		// Window Handle
	LPDIRECT3DSWAPCHAIN9	m_pSwS;		// Swap chain
	LPDIRECT3DSURFACE9	m_pSwC;		// Back Buffer(Color Buffer) Surface
	LPDIRECT3DSURFACE9	m_pSwD;		// Depth and Stencil Buffer Surface

```

HWND			m_hSwW;		// Window Handle
	LPDIRECT3DSWAPCHAIN9	m_pSwS;		// Swap chain
	LPDIRECT3DSURFACE9	m_pSwC;		// Back Buffer(Color Buffer) Surface
	LPDIRECT3DSURFACE9	m_pSwD;		// Depth and Stencil Buffer Surface


Swap Chain은 메인 디바이스와 거의 동일한 정보를 가지고 만들어야 하므로 다음과 같은


순서를 통해서 생성합니다.


```

	1. 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼의 속성을 가져온다.
	2. 대상 윈도우 핸들을 얻는다.
	3. Present Parameter 속성을 설정한다. 이 때 후면 버퍼의 너비와 높이가 0이면 뷰
		포트가 자동으로 대상 윈도우 영역으로 설정 된다.
	4. Swap Chain을 생성 한다.
	5. Swap Chain으로 생성된 후면 버퍼의 색상 버퍼를 가져온다.
	6. Swap Chain을 위한 깊이, 스텐실 버퍼를 생성한다.

```

1. 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼의 속성을 가져온다.
	2. 대상 윈도우 핸들을 얻는다.
	3. Present Parameter 속성을 설정한다. 이 때 후면 버퍼의 너비와 높이가 0이면 뷰
		포트가 자동으로 대상 윈도우 영역으로 설정 된다.
	4. Swap Chain을 생성 한다.
	5. Swap Chain으로 생성된 후면 버퍼의 색상 버퍼를 가져온다.
	6. Swap Chain을 위한 깊이, 스텐실 버퍼를 생성한다.


이에 대한 코드의 예는 다음과 같습니다.


```

	// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼의 속성을 가져온다.
	D3DSURFACE_DESC		dscC;
	D3DSURFACE_DESC		dscD;
	LPDIRECT3DSURFACE9	pDevC=NULL;
	LPDIRECT3DSURFACE9	pDevD=NULL;

	m_pd3dDevice->GetRenderTarget(0, &pDevC);
	pDevC->GetDesc(&dscC);
	pDevC->Release();

	m_pd3dDevice->GetDepthStencilSurface(&pDevD);
	pDevD->GetDesc(&dscD);
	pDevD->Release();

	// 대상 윈도우 핸들을 얻는다.
	m_hSwW = (HWND)GetDlgItem(m_hImgSc, IDC_TX);

	// Present Parameter 속성을 설정한다.
	D3DPRESENT_PARAMETERS	d3dParam;
	memset(&d3dParam, 0, sizeof(D3DPRESENT_PARAMETERS));

	d3dParam.SwapEffect		= D3DSWAPEFFECT_DISCARD;
	d3dParam.Windowed		= TRUE;
	d3dParam.BackBufferWidth	= 0;
	d3dParam.BackBufferHeight	= 0;
	d3dParam.BackBufferFormat	= dscC.Format;
	d3dParam.AutoDepthStencilFormat	= dscD.Format;
	d3dParam.hDeviceWindow		= m_hSwW;

	// Swap Chain을 생성 한다.
	if( FAILED(m_pd3dDevice->CreateAdditionalSwapChain(&d3dParam, &m_pSwS)))
		return -1;

	// Swap Chain으로 생성된 후면 백퍼의 색상 버퍼를 가져온다.
	if( FAILED(m_pSwS->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pSwC)))
		return -1;

	// Swap Chain을 위한 깊이, 스텐실 버퍼를 생성한다.
	if ( FAILED(m_pd3dDevice->CreateDepthStencilSurface(
				d3dParam.BackBufferWidth
				, d3dParam.BackBufferHeight
				, d3dParam.AutoDepthStencilFormat
				, D3DMULTISAMPLE_NONE, 0, 0
				, &m_pSwD, NULL)))
		return -1;

```

// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼의 속성을 가져온다.
	D3DSURFACE_DESC		dscC;
	D3DSURFACE_DESC		dscD;
	LPDIRECT3DSURFACE9	pDevC=NULL;
	LPDIRECT3DSURFACE9	pDevD=NULL;

	m_pd3dDevice->GetRenderTarget(0, &pDevC);
	pDevC->GetDesc(&dscC);
	pDevC->Release();

	m_pd3dDevice->GetDepthStencilSurface(&pDevD);
	pDevD->GetDesc(&dscD);
	pDevD->Release();

	// 대상 윈도우 핸들을 얻는다.
	m_hSwW = (HWND)GetDlgItem(m_hImgSc, IDC_TX);

	// Present Parameter 속성을 설정한다.
	D3DPRESENT_PARAMETERS	d3dParam;
	memset(&d3dParam, 0, sizeof(D3DPRESENT_PARAMETERS));

	d3dParam.SwapEffect		= D3DSWAPEFFECT_DISCARD;
	d3dParam.Windowed		= TRUE;
	d3dParam.BackBufferWidth	= 0;
	d3dParam.BackBufferHeight	= 0;
	d3dParam.BackBufferFormat	= dscC.Format;
	d3dParam.AutoDepthStencilFormat	= dscD.Format;
	d3dParam.hDeviceWindow		= m_hSwW;

	// Swap Chain을 생성 한다.
	if( FAILED(m_pd3dDevice->CreateAdditionalSwapChain(&d3dParam, &m_pSwS)))
		return -1;

	// Swap Chain으로 생성된 후면 백퍼의 색상 버퍼를 가져온다.
	if( FAILED(m_pSwS->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pSwC)))
		return -1;

	// Swap Chain을 위한 깊이, 스텐실 버퍼를 생성한다.
	if ( FAILED(m_pd3dDevice->CreateDepthStencilSurface(
				d3dParam.BackBufferWidth
				, d3dParam.BackBufferHeight
				, d3dParam.AutoDepthStencilFormat
				, D3DMULTISAMPLE_NONE, 0, 0
				, &m_pSwD, NULL)))
		return -1;




장면 연출은 다음의 코드와 같이 메인 디바이스의 색상 버퍼와 깊이 버퍼를 Swap Chain의


색상 버퍼와 깊이 버퍼로 바꾸어서 진행합니다.


```

	// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼를 가져온다.
	LPDIRECT3DSURFACE9	pDevC=NULL;
	LPDIRECT3DSURFACE9	pDevD=NULL;

	m_pd3dDevice->GetRenderTarget(0, &pDevC);
	m_pd3dDevice->GetDepthStencilSurface(&pDevD);

	// Swap Chain의 색상버퍼와 깊이 버퍼로 변경한다.
	m_pd3dDevice->SetRenderTarget(0, m_pSwC);
	m_pd3dDevice->SetDepthStencilSurface(m_pSwD);

	// 장면을 연출한다.
	m_pd3dDevice->BeginScene();
	…
	m_pd3dDevice->EndScene();

	// Swap Chain에 저장된 데이터를 화면에 출력한다.
	m_pSwS->Present(0, 0, 0, 0, 0);

	// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼로 변경한다.
	m_pd3dDevice->SetRenderTarget(0, pDevC);
	m_pd3dDevice->SetDepthStencilSurface(pDevD);
	pDevC->Release();
	pDevD->Release();

```

// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼를 가져온다.
	LPDIRECT3DSURFACE9	pDevC=NULL;
	LPDIRECT3DSURFACE9	pDevD=NULL;

	m_pd3dDevice->GetRenderTarget(0, &pDevC);
	m_pd3dDevice->GetDepthStencilSurface(&pDevD);

	// Swap Chain의 색상버퍼와 깊이 버퍼로 변경한다.
	m_pd3dDevice->SetRenderTarget(0, m_pSwC);
	m_pd3dDevice->SetDepthStencilSurface(m_pSwD);

	// 장면을 연출한다.
	m_pd3dDevice->BeginScene();
	…
	m_pd3dDevice->EndScene();

	// Swap Chain에 저장된 데이터를 화면에 출력한다.
	m_pSwS->Present(0, 0, 0, 0, 0);

	// 메인 윈도우 후면 버퍼의 색상 버퍼와 깊이 버퍼로 변경한다.
	m_pd3dDevice->SetRenderTarget(0, pDevC);
	m_pd3dDevice->SetDepthStencilSurface(pDevD);
	pDevC->Release();
	pDevD->Release();




![tile007.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tile007.png)





전체 코드는 다음 예제를 살펴 보기 바랍니다.




[Spt21_Swap1.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt21_Swap1.zip)
Spt21_Swap1.zip






만약 Swap Chain을 자주 사용하는 경우라면 클래스로 만들어서 사용하는 것이 좋습니다.


다음은 위의 코드를 클래스로 구현한 예제입니다.




[Spt21_Swap2.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt21_Swap2.zip)
Spt21_Swap2.zip














### 2. Interface 구성

2. Interface 구성


지형 툴 프로그램을 위해서 다음과 같이 화면을 구성합니다.




![tool001.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tool001.png)





이 강의에서는 위의 화면을 구현하기 위해서 MFC를 사용하지 않고 순전히 윈도우 API만을


가지고 툴을 구성하겠습니다.


우측 화면은 작업한 지형을 출력합니다. 좌측 상단 화면은 지형에 물체들을 배치할 수 있도록


오브젝트 리스트를 보여줄 것인데 이 강의에서는 구현하지 않겠습니다. 좌측 하단의 윈도우는


된 work윈도우를 띄우고, 메인 윈도우 하단에는 원본이미지를 출력하도록 구성합니다.




작업에 대한 모든 윈도우는 Modaless 입니다. Modaless 윈도우를 만드는 방법은 다음과


같습니다.


```

	INT_PTR CALLBACK WndWrk(HWND, UINT, WPARAM, LPARAM);
	m_hwWrk = CreateDialog( GetModuleHandle(NULL)
	, MAKEINTRESOURCE(IDD_WORK)
	, m_hWnd
	, WndWrk);

```

INT_PTR CALLBACK WndWrk(HWND, UINT, WPARAM, LPARAM);
	m_hwWrk = CreateDialog( GetModuleHandle(NULL)
	, MAKEINTRESOURCE(IDD_WORK)
	, m_hWnd
	, WndWrk);


WndWrk() 함수는 탭 컨트롤를 다루기 위한 메시지 프로시저(Message Procedure)함수 입니다.


자식 윈도우들은 다음 코드와 같이 윈도우 다이얼로그(Window Dialog)가 만들어지는


WM_INITDIALOG 메시지가 발생할 때 붙입니다.


또한 탭을 선택하는 이벤트가 발생하면 자식 윈도우를 선택해야 하는데 이것은 WM_NOTIFY


이벤트에서 처리합니다.


```

	INT_PTR CALLBACK WndWrk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	…
		case WM_INITDIALOG:
		{
			…
			tie.mask=TCIF_TEXT | TCIF_IMAGE;
			tie.pszText="Work 1";
			tie.iImage=0;
			TabCtrl_InsertItem(GMAIN->m_hwWrkTb,0,&tie);
			…
			// 자식 윈도우 붙이기
			GMAIN->m_hwWrk1 = CreateDialog( GetModuleHandle(NULL)
							,MAKEINTRESOURCE(IDD_WORK1)
							, hWnd
							,WndWrk1);
				…
			}

		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case TCN_SELCHANGE:
				// 자식 윈도우 선택
				…
	break;
	…

```

INT_PTR CALLBACK WndWrk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	…
		case WM_INITDIALOG:
		{
			…
			tie.mask=TCIF_TEXT | TCIF_IMAGE;
			tie.pszText="Work 1";
			tie.iImage=0;
			TabCtrl_InsertItem(GMAIN->m_hwWrkTb,0,&tie);
			…
			// 자식 윈도우 붙이기
			GMAIN->m_hwWrk1 = CreateDialog( GetModuleHandle(NULL)
							,MAKEINTRESOURCE(IDD_WORK1)
							, hWnd
							,WndWrk1);
				…
			}

		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case TCN_SELCHANGE:
				// 자식 윈도우 선택
				…
	break;
	…


전체 코드는 다음 예제를 살펴 보기 바랍니다.




[Spt22_Base.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt22_Base.zip)
Spt22_Base.zip






이렇게 툴 프로그램의 화면 인터페이스를 간단히 만들어 놓고 나서 여기에 DirectX의


스프라이트(ID3DXSprite) 객체를 붙입니다.






![tool002.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tool002.png)







DirectX 스프라이트 적용은 2D 게임 기초 과정에서 강의했기 때문에 여기서는 넘어가겠


습니다. 전체 코드는 다음 예제를 참고하기 바랍니다.




[Spt22_Sprite.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt22_Sprite.zip)
Spt22_Sprite.zip






스프라이트를 적용했다면 다음 단계로 이전에 연습했던 타일을 적용합니다. 적용하는데


그리 큰 어려움이 없으므로 자세한 내용을 생략하겠습니다.






![tool003.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tool003.png)







[Spt23_tool_tile1.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt23_tool_tile1.zip)
Spt23_tool_tile1.zip






인터페이스의 마지막 단계로 Swap Chain을 적용합니다. 지형을 적용한 게임의 윈도우가


800X600 이라는 가정하에 메인 윈도우의 크기는 1024X768에서 800X600 크기로 조정합니다.


앞으로 Swap Chain은 다른 툴 프로그램에서도 자주 사용될 수 있으므로 CLcSwpWn 클래스로


구현해 놓고 이 클래스의 객체를 사용합니다. (Spt21_Swap2
[.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/.zip)
.zip
예제 참조) 스프라이트는


이미지가 회전할 수 있다는 가정하에 2D 그래픽 프로그래밍에서 ID3DXSprite 객체를


Wrapping 한 CLcSprite 클래스를 사용합니다.


텍스처 LPDIRECT3DDEVICE9 객체와 이미지 정보 D3DXIMAGE_INFO 구조체를 포함하는


CLcTexture 클래스를 사용합니다.




이들을 사용한 CMain 클래스의 멤버 변수들을 살펴보면 다음과 같습니다.


```

	class CMain : public CD3DApp
	…
		CLcSprite*	m_pLcSprite	;
		CLcInput*	m_pInput	;

		CLcTile*	m_pTile		;

		CLcTexture*	m_pTxTile	;
		CLcSwpWn	m_SpWnd		;
	 …

```

class CMain : public CD3DApp
	…
		CLcSprite*	m_pLcSprite	;
		CLcInput*	m_pInput	;

		CLcTile*	m_pTile		;

		CLcTexture*	m_pTxTile	;
		CLcSwpWn	m_SpWnd		;
	 …


![tool004.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tool004.png)





[Spt23_tool_tile2.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt23_tool_tile2.zip)
Spt23_tool_tile2.zip














### 3. Tool 제작

3. Tool 제작


윈도우 인터페이스, 타일 알고리듬, 각종 유틸리티 클래스들을 갖추었으므로 본격적으로


툴 프로그램을 진행해 봅시다.


Texture/tilemap.bmp 파일과 지형 파일인 Data/mapindex.txt 파일을 실행해 보면 다음


그림과 같이 지형과 지형 인덱스를 화면에 출력합니다.






![tool005.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tool005.png)





[Spt24_Tool_1.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt24_Tool_1.zip)
Spt24_Tool_1.zip






키보드의 Ct기 키를 누르고 마우스 왼쪽 버튼을 누르면 브러쉬(Brush) 기능이 동작하도록


다음과같이 코드를 추가합니다.


```

	INT CLcTile::FrameMove()
	…
		if(GINPUT->BtnPress(0))
		{
			int nIdx = GMAIN->m_nImgSc;
			D3DXVECTOR3 vcP = GINPUT->GetMousePos();
	…
			if(GINPUT->KeyState(VK_LCONTROL)
				 && iX>=0 && iX< m_nTileNumX
				 && iY>=0 && iY< m_nTileNumY)
			{
				m_vcDsM.x = iX * 32.f;
				m_vcDsM.y = iY * 32.f;

				m_nImgDs	= iX + GMAIN->m_iImgX * iY;

				m_pTile[iY][iX].nIdx = nIdx;
				SetRect(&GMAIN->m_prcSc[m_nImgDs], iX*GMAIN->m_iW
					, iY*GMAIN->m_iW, (iX+1)*GMAIN->m_iW, (iY+1)*GMAIN->m_iW);

				…
			}
		…

```

INT CLcTile::FrameMove()
	…
		if(GINPUT->BtnPress(0))
		{
			int nIdx = GMAIN->m_nImgSc;
			D3DXVECTOR3 vcP = GINPUT->GetMousePos();
	…
			if(GINPUT->KeyState(VK_LCONTROL)
				 && iX>=0 && iX< m_nTileNumX
				 && iY>=0 && iY< m_nTileNumY)
			{
				m_vcDsM.x = iX * 32.f;
				m_vcDsM.y = iY * 32.f;

				m_nImgDs	= iX + GMAIN->m_iImgX * iY;

				m_pTile[iY][iX].nIdx = nIdx;
				SetRect(&GMAIN->m_prcSc[m_nImgDs], iX*GMAIN->m_iW
					, iY*GMAIN->m_iW, (iX+1)*GMAIN->m_iW, (iY+1)*GMAIN->m_iW);

				…
			}
		…


![tool006.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tool006.png)





[Spt24_Tool_2.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt24_Tool_2.zip)
Spt24_Tool_2.zip






2D 게임에 대한 지형 툴 만들기에 대한 기본적인 내용은 모두 끝났습니다. 남아있는 것은


오브젝트 배치, 타일의 속성, 기타 툴의 편리성 등이 있습니다.


타일의 속성의 경우 다음 구조체처럼 unsigned long int 형 데이터를 추가하고 이를 툴에서


수정할 수 있도록 합니다.


```

	struct LcTile
	{
		…
		DWORD		dAtt;

		LcTile()
		{
			…
			dAtt	= 0;
		}
	};

```

struct LcTile
	{
		…
		DWORD		dAtt;

		LcTile()
		{
			…
			dAtt	= 0;
		}
	};


또한 텍스트 파일을 이진(Binary) 파일로 변경을 해서 불러오기와 저장을 빠르게 진행하도록


해야 합니다. 이진 파일은 타일과 같은 구조체가 있다면 다음 코드처럼 읽고 쓰기가


텍스트보다 훨씬 편리합니다.


```

	…
	fread(m_sBmp, sizeof(char), sizeof(m_sBmp), fp);
	fread(&m_nTileWidth,sizeof(int), 1, fp);
	fread(&m_nTileMaxX, sizeof(int), 1, fp);
	fread(&m_nTileMaxY, sizeof(int), 1, fp);
	fread(&m_nTileNumX, sizeof(int), 1, fp);
	fread(&m_nTileNumY, sizeof(int), 1, fp);

	m_pTile = new CLcTile::LcTile*[m_nTileNumY];

	for(y=0; y< m_nTileNumY; ++y)
	{
		m_pTile[y] = new CLcTile::LcTile[m_nTileNumX];
		fread(m_pTile[y], sizeof(CLcTile::LcTile), m_nTileNumX, fp);
	}
	…

```

…
	fread(m_sBmp, sizeof(char), sizeof(m_sBmp), fp);
	fread(&m_nTileWidth,sizeof(int), 1, fp);
	fread(&m_nTileMaxX, sizeof(int), 1, fp);
	fread(&m_nTileMaxY, sizeof(int), 1, fp);
	fread(&m_nTileNumX, sizeof(int), 1, fp);
	fread(&m_nTileNumY, sizeof(int), 1, fp);

	m_pTile = new CLcTile::LcTile*[m_nTileNumY];

	for(y=0; y< m_nTileNumY; ++y)
	{
		m_pTile[y] = new CLcTile::LcTile[m_nTileNumX];
		fread(m_pTile[y], sizeof(CLcTile::LcTile), m_nTileNumX, fp);
	}
	…


다음 예제는 위의 타일 속성과 이진 파일 부분만 적용해서 클라이언트 프로그램에 적용한 예제입니다.






![tool007.png](https://github.com/3dapi/bs14_2d_adv/raw/master/img_src/tool007.png)





[Spt25_Tool_App.zip](https://github.com/3dapi/bs14_2d_adv/raw/master/Spt25_Tool_App.zip)
Spt25_Tool_App.zip



