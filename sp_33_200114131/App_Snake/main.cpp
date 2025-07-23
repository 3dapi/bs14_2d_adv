// This is a earthworm game.
//
// Author: 박세연(청강대학교: parkse83@hotmail.com)
// Date  : 2006-06-16
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"



int g_GameState = GM_START;
int g_nFont1;
int g_Stage = 0;
int g_Next_Stage = 0;
int g_Level = 0;
int g_ToTal_Grade = 0;
int g_KeyInput;
int g_nCount = 1;
tgFOOD FOOD[MAX_Tail];
int g_ItemCount = 0;
DWORD g_EndTime;
DWORD g_BigenTime;
UserInput g_Input;
int g_Speed;
tgScreen ScreenSrc[Screen_MAX];
tgLINE LINE[MAX_Tail];


#ifndef WINMAIN_BASE_APP
int main()
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	printf("Play Snake Game!\n\n");

	//배경색을 바꾼다.
	SpLib_SetClearColor(0xFFc8c8c8);
	SpLib_CreateWin(100, 100, 640, 480, "- Snake Game", false);
	g_nFont1	= SpLib_FontCreate("굴림", 15, 0);

	// 그림을 프로그램에 로딩
	ScreenSrc[0].nTx = SpLib_TextureLoad("Texture/Header1.bmp",0xFFFF00FF);
	ScreenSrc[0].iImgW = SpLib_TextureWidth(ScreenSrc[0].nTx);
	ScreenSrc[0].iImgH = SpLib_TextureHeight(ScreenSrc[0].nTx);

	ScreenSrc[1].nTx = SpLib_TextureLoad("Texture/Tail.bmp",0xFFFF00FF);
	ScreenSrc[1].iImgW = SpLib_TextureWidth(ScreenSrc[1].nTx);
	ScreenSrc[1].iImgH = SpLib_TextureHeight(ScreenSrc[1].nTx);

	ScreenSrc[2].nTx = SpLib_TextureLoad("Texture/BG1.bmp",0xFFFF00FF);
	ScreenSrc[2].iImgW = SpLib_TextureWidth(ScreenSrc[2].nTx);
	ScreenSrc[2].iImgH = SpLib_TextureHeight(ScreenSrc[2].nTx);

	ScreenSrc[3].nTx = SpLib_TextureLoad("Texture/BG2.bmp");
	ScreenSrc[3].iImgW = SpLib_TextureWidth(ScreenSrc[3].nTx);
	ScreenSrc[3].iImgH = SpLib_TextureHeight(ScreenSrc[3].nTx);

	ScreenSrc[4].nTx = SpLib_TextureLoad("Texture/rol.bmp");
	ScreenSrc[4].iImgW = SpLib_TextureWidth(ScreenSrc[4].nTx);
	ScreenSrc[4].iImgH = SpLib_TextureHeight(ScreenSrc[4].nTx);

	ScreenSrc[5].nTx = SpLib_TextureLoad("Texture/Food1.bmp",0xFFFF00FF);
	ScreenSrc[5].iImgW = SpLib_TextureWidth(ScreenSrc[5].nTx);
	ScreenSrc[5].iImgH = SpLib_TextureHeight(ScreenSrc[5].nTx);

	ScreenSrc[6].nTx = SpLib_TextureLoad("Texture/item1.bmp",0xFFFF00FF);
	ScreenSrc[6].iImgW = SpLib_TextureWidth(ScreenSrc[6].nTx);
	ScreenSrc[6].iImgH = SpLib_TextureHeight(ScreenSrc[6].nTx);

	ScreenSrc[7].nTx = SpLib_TextureLoad("Texture/item2.bmp",0xFFFF00FF);
	ScreenSrc[7].iImgW = SpLib_TextureWidth(ScreenSrc[7].nTx);
	ScreenSrc[7].iImgH = SpLib_TextureHeight(ScreenSrc[7].nTx);

	ScreenSrc[8].nTx = SpLib_TextureLoad("Texture/bg3.bmp");
	ScreenSrc[8].iImgW = SpLib_TextureWidth(ScreenSrc[8].nTx);
	ScreenSrc[8].iImgH = SpLib_TextureHeight(ScreenSrc[8].nTx);

	ScreenSrc[9].nTx = SpLib_TextureLoad("Texture/bg4.bmp");
	ScreenSrc[9].iImgW = SpLib_TextureWidth(ScreenSrc[9].nTx);
	ScreenSrc[9].iImgH = SpLib_TextureHeight(ScreenSrc[9].nTx);

	ScreenSrc[10].nTx = SpLib_TextureLoad("Texture/side1.bmp");
	ScreenSrc[10].iImgW = SpLib_TextureWidth(ScreenSrc[10].nTx);
	ScreenSrc[10].iImgH = SpLib_TextureHeight(ScreenSrc[10].nTx);

	// 화면에 출력하기 위해서 함수를 연결한다.
	SpLib_SetFrameMove(FrameMove);
	SpLib_SetRender(Render);

	g_BigenTime = timeGetTime();
	g_EndTime = g_BigenTime;

	SpLib_SetStateShow(false);// State 보여주는 것을 감춘다.


	SpLib_Run();



//	SpLib_TextureRelease(nTx);

	SpLib_DestroyWin();


	return 0;
}

int FrameMove()
{
	memcpy(g_Input.pKeyOld,   g_Input.pKeyNew,	sizeof g_Input.pKeyOld);
	memset(g_Input.pKeyNew,					0,	sizeof g_Input.pKeyNew);
	memcpy(g_Input.pKeyNew, SpLib_GetKeyboard(),sizeof g_Input.pKeyNew);

	switch(g_GameState)
	{
	case GM_START:
		IntroFrameMove();
		break;
	case GM_PLAY:
		PlayFrameMove();
		break;
	case GM_END:
		EndFrameMove();
		break;
	}

	return 0;
}

int Render()
{
	switch(g_GameState)
	{
	case GM_START:
		IntroRender();
		break;
	case GM_PLAY:
		PlayRender();
		break;
	case GM_END:
		EndRender();
		break;
	}

	return 0;
}

