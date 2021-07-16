//Copyright (c) 2020
//Hamamatsu Mirai Sougou Senmongakkou GameCreate Y.Suzuki
//All rights reserved.
//図形のソースファイル

//########## ヘッダーファイル読み込み ##########
#include "Game.h"
#include "Shape.h"
#include <math.h>
using namespace std;

//########## 関数 ##########

/// <summary>
/// 四角領域の当たり判定をチェックする
/// </summary>
/// <param name="a">領域A</param>
/// <param name="b">領域B</param>
/// <returns>当たっているならTRUE</returns>
BOOL CheckCollRectToRect(RECT a, RECT b) 
{
	if (a.left<b.right && a.top<b.bottom
		&& a.right>b.left && a.bottom>b.top)
	{
		return TRUE;	//当たっている
	}

	return FALSE;		//当たっていない
}

/// <summary>
/// 点と四角の当たり判定
/// </summary>
/// <param name="pt">I_POINT型の点</param>
/// <param name="r">矩形領域</param>
/// <returns>当たっているならTRUE</returns>
BOOL CheckColliPointToRect(I_POINT pt, RECT r) 
{
	if (pt.x > r.left && pt.x<r.right
		&& pt.y>r.top && pt.y < r.bottom)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CheckCollRectToCircle(RECT r, CIRCLE c) 
{
	if (c.center.x + c.radius > r.left
		&& c.center.x - c.radius<r.right
		&& c.center.y + c.radius>r.top
		&& c.center.y - c.radius < r.bottom)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// 点と円の当たり判定
/// </summary>
/// <param name="pt">I_POINT型の点</param>
/// <param name="c">円領域</param>
/// <returns>当たっているならTRUE</returns>
BOOL CheckColliPointToCircle(I_POINT pt, CIRCLE c) 
{
	float bm = pt.x - c.center.x;		//三角形の底辺の長さを求める
	float ht = pt.y - c.center.y;		//三角形の高さの長さを求める
	float sq = sqrtf(bm * bm + ht * ht);//三角形の斜辺の長さを求める(sqrt=平方根)

	//斜辺の長さが、円の半径よりも短いならば、円の中にいる
	return (sq <= c.radius) ? TRUE : FALSE;
}

/// <summary>
/// 円と円の当たり判定
/// </summary>
/// <param name="a">円領域1</param>
/// <param name="b">円領域2</param>
/// <returns>当たっているならTRUE</returns>
BOOL CheckCollCircleToCircle(CIRCLE a, CIRCLE b) 
{
	float bm = a.center.x - b.center.x;		//三角形の底辺の長さを求める
	float ht = a.center.y - b.center.y;		//三角形の高さの長さを求める
	float sq = sqrtf(bm * bm + ht * ht);//三角形の斜辺の長さを求める(sqrt=平方根)

	//斜辺の長さが、円の半径よりも短いならば、円の中にいる
	return (sq <= a.radius + b.radius) ? TRUE : FALSE;
}

/// <summary>
/// RECT型を一時的に渡す
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
/// <returns>RECT型</returns>
RECT GetRect(int left, int top, int right, int bottom) 
{
	//一時的にRECT型の変数を作って返す
	RECT rect = { left,top,right,bottom };
	return rect;
}

/// <summary>
/// RECTを利用して四角を描画
/// </summary>
/// <param name="r">RECT構造体</param>
/// <param name="color">描画色</param>
/// <param name="b">塗りつぶしならTRUE</param>
VOID DrawRect(RECT r, unsigned int color, bool b) 
{
	//引数を元に描画
	DrawBox(r.left, r.top, r.right, r.bottom, color, b);
	return;
}

/// <summary>
/// 四角を描画(黒,塗りつぶさない)
/// </summary>
/// <param name="r">RECT構造体</param>
VOID DrawRect(RECT r) 
{
	//引数を元に描画
	DrawBox(r.left, r.top, r.right, r.bottom, GetColor(255,255,255), FALSE);
	return;
}

/// <summary>
/// CIRCLEを利用して円を描画
/// </summary>
/// <param name="c">CIRCLE構造体</param>
/// <param name="color">描画色</param>
/// <param name="b">塗りつぶしならTRUE</param>
/// <param name="thick">線の太さ</param>
VOID DrawCircle(CIRCLE c, unsigned int color, bool b, int thick) 
{
	//引数を元に描画
	DrawCircle(c.center.x, c.center.y, c.radius, color, b, thick);
	return;
}

/// <summary>
/// 円を描画(黒,塗りつぶさない,線1)
/// </summary>
/// <param name="c">CIRCLE構造体</param>
VOID DrawCircle(CIRCLE c) 
{
	//引数を元に描画
	DrawCircle(c.center.x, c.center.y, c.radius, 
		GetColor(255,255,255), FALSE, 1);
	return;
}


