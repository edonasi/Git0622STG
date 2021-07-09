#pragma once

//ヘッダ読み込み
#include "Game.h"	//ゲーム全体のヘッダファイル

//マクロ定義
#define GAME_FPS 60			//FPS標準
#define GAME_FPS_MAX 240	//FPS最大値
#define GAME_FPS_MIN 60 	//FPS最大値

//FPS構造体
struct FPS
{
	BOOL IsInitFlag = FALSE;	//最初の測定開始フラグ
	LONGLONG StartTime = 0;		//開始開始時刻
	LONGLONG NowTime = 0;		//現在の時刻
	LONGLONG OldTime = 0;		//以前の時刻

	int Value = GAME_FPS;		//FPSの値

	float DeltaTime = 0.000001;	//経過時間
	int Count = 1;				//現在のフレーム数
	float DrawValue = 0.0f;		//計算結果を描画
	int SampleRote = GAME_FPS;	//平均を取るサンプル値
};

//グローバル変数の外部参照
extern FPS fps;	//FPSの管理

//プロトタイプ宣言
VOID FpsUpdate(VOID);	// Fps値を測定し、値を更新する
VOID FpsDraw(int draw_x, int draw_y, BOOL isBlackColor);	//デバック用の平均FPS値を描画する
VOID FpsWait();			// FPSで処理を待つ