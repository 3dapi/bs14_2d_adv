#ifndef _STAGE_H_
#define _STAGE_H_



struct Stage
{
	int		stageInfo[12][12];	// 맵
	int		PlayerPosX;			// 플레이어위치x
	int		PlayerPosY;			// 플레이어위치y
};




#define BACKGROUND	0	// 배경
#define BLOCK		1	// 블럭
#define LANE		2	// 이동통로
#define BALL		3	// 볼
#define FINISH		4	// 목적지



int GetTotalState();

#endif