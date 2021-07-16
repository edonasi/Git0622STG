#pragma once
//図形のヘッダファイル
//########## ヘッダーファイル読み込み ##########
#include "Game.h"

//########## 構造体 ##########
//int型のPOINT構造体
struct I_POINT
{	
	//座標を初期化
	int x = - 1;
	int y = -1;
};

//円の構造体
struct CIRCLE
{
	I_POINT center;	//中心点
	int radius;		//半径
};

BOOL CheckCollRectToRect(RECT a, RECT b);			//四角領域の当たり判定をチェックする
BOOL CheckColliPointToRect(I_POINT pt, RECT r);		//点と四角の当たり判定
BOOL CheckColliPointToCircle(I_POINT pt, CIRCLE c);	//点と円の当たり判定
BOOL CheckCollRectToCircle(RECT r, CIRCLE c);
BOOL CheckCollCircleToCircle(CIRCLE a, CIRCLE b);	//円と円の当たり判定

RECT GetRect(int left, int top, int right, int bottom);	//RECT型を一時的に渡す
VOID DrawRect(RECT r, unsigned int color, bool b);		//RECTを利用して四角を描画
VOID DrawRect(RECT r);									//四角を描画(黒,塗りつぶさない)
VOID DrawCircle(CIRCLE c, unsigned int color, bool b, int thick = 1);	//CIRCLEを利用して円を描画
VOID DrawCircle(CIRCLE c);								//円を描画(黒,塗りつぶさない,線1)



