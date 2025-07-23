// Interface for the Utilities.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcUtil_H_
#define _LcUtil_H_

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


#define SAFE_FRMMOV(p)			{ if(p)	{if(FAILED((p)->FrameMove()))return -1;	}}
#define SAFE_RENDER(p)			{ if(p)	{ (p)->Render();			} }


#define SAFE_DESTROY_WINDOW(p)	{	if(p){	DestroyWindow(p);	p=NULL;	}	}

#define SAFE_NEWINIT(p, CLASSNAME)								\
{																\
	if(NULL == p)												\
	{															\
		p = new CLASSNAME;										\
																\
		if(FAILED( (p)->Init()))								\
		{														\
			delete (p);											\
			(p)	= NULL;											\
																\
			return -1;											\
		}														\
	}															\
}

#define SAFE_NEWCREATE1(p, CLASSNAME, v1)						\
{																\
	if(NULL == p)												\
	{															\
		p = new CLASSNAME;										\
																\
		if(FAILED( (p)->Create(v1)))							\
		{														\
			delete (p);											\
			(p)	= NULL;											\
																\
			return -1;											\
		}														\
	}															\
}


#define SAFE_NEWCREATE2(p, CLASSNAME, v1, v2)					\
{																\
	if(NULL == p)												\
	{															\
		p = new CLASSNAME;										\
																\
		if(FAILED( (p)->Create(v1, v2)))						\
		{														\
			delete (p);											\
			(p)	= NULL;											\
																\
			return -1;											\
		}														\
	}															\
}


#define SAFE_NEWCREATE3(p, CLASSNAME, v1, v2, v3)				\
{																\
	if(NULL == p)												\
	{															\
		p = new CLASSNAME;										\
																\
		if(FAILED( (p)->Create(v1, v2, v3)))					\
		{														\
			delete (p);											\
			(p)	= NULL;											\
																\
			return -1;											\
		}														\
	}															\
}



#define SAFE_DESTROY_WINDOW(p)	{	if(p){	DestroyWindow(p);	p=NULL;	}	}


INT	LcUtil_TextureLoad(LPDIRECT3DDEVICE9 pDev
					, char* sFile
					, LPDIRECT3DTEXTURE9* pTx
					, D3DXIMAGE_INFO *pInf=NULL
					, DWORD dColorKey= 0x00FFFFFF
					, DWORD dFilter = D3DX_FILTER_NONE);


void LcUtil_SetWindowTitle(const char *format, ...);


#endif


