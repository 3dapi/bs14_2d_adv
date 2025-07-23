// Interface for the Utilities.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCUTIL_H_
#define _MCUTIL_H_

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


#define ONE_RADtoDEG	57.2957795130823208767981548f
#define ONE_DEGtoRAD	0.01745329251994329576923690f
#define MCMATH_PI		3.14159265358979323846264338f
#define DEG90toRAD		1.57079632679489661923132163f
#define RADtoDEG(p)		( (p)*ONE_RADtoDEG)
#define DEGtoRAD(p)		( (p)*ONE_DEGtoRAD)





#define SAFE_NEWINIT(p, CLASSTYPE)												\
{																				\
	if(NULL == (p))																\
	{																			\
		p = new CLASSTYPE;														\
																				\
		if(!(p))																\
		{																		\
			return -1;															\
		}																		\
																				\
		if(FAILED((p)->Init()))													\
		{																		\
			delete p;															\
			p = NULL;															\
			return -1;															\
		}																		\
	}																			\
}

#define SAFE_FRAMEMOVE(p)														\
{																				\
	if(p)																		\
	{																			\
		if(FAILED(	(p)->FrameMove()))											\
			return -1;															\
	}																			\
}

#define SAFE_DESTROY(p)		{	if(p)	(p)->Destroy();			}
#define SAFE_INVALIDATE(p)	{	if(p)	(p)->Invalidate();		}
#define SAFE_RENDER(p)		{	if(p)	(p)->Render();			}



struct McTexture
{
	TCHAR				sF[256];		// File Name
	LPDIRECT3DTEXTURE9	pTx;			// Texture point
	D3DXIMAGE_INFO		Inf;			// Image Info
	DWORD				dC;				// Color Key
	RECT				rcA[6][6];
	RECT				prcA[32];
	INT					sell;

	McTexture()
	{
		pTx	= NULL;			// Texture point
	}

	McTexture(CHAR* _sF)
	{
		strcpy( sF, _sF);
	}

	McTexture(CHAR* _sF, INT _sell, DWORD _dC=0xFFFF00FF)
	{
		sprintf(sF, "Texture/%s", _sF);
		sell = _sell;
		dC = _dC;
	}

	void operator=(McTexture& rhs)
	{
		strcpy(sF, rhs.sF);
		pTx = rhs.pTx;
		dC	= rhs.dC;
		memcpy(&Inf, &rhs.Inf, sizeof Inf);
	}

	RECT GetImgRect()
	{
		RECT rc;

		rc.left =0;
		rc.top=0;
		rc.right = Inf.Width;
		rc.bottom= Inf.Height;
		return rc;
	}

	RECT GetImgRect(INT iTxNo)
	{
		RECT rc;
		int iNo, iWid, iHig, ihi=0;

		iNo = iTxNo;
		if(sell == 0)
		{
			rc.left = 0;
			rc.top	= 0;
			rc.right = Inf.Width;
			rc.bottom= Inf.Height;
		}

		else
		{
			iWid = iNo * (Inf.Width/sell);
			while((UINT)iWid >= Inf.Width)
			{
				iWid = iWid - Inf.Width;
				ihi++;
			}
			iHig = (Inf.Height/sell) * ihi;

			rc.left = iWid;
			rc.top	= iHig;
			rc.right = iWid+(Inf.Width/sell);
			rc.bottom= iHig+(Inf.Height/sell);

		}

		return rc;
	}
};



INT	McUtil_TextureLoad(TCHAR* sFile, LPDIRECT3DTEXTURE9& pTx, DWORD dc= 0x00FFFFFF);


#endif