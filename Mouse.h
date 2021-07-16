#pragma once
//マウス処理のヘッダファイル
//########## ヘッダーファイル読み込み ##########
#include "Game.h"
#include "Shape.h"

//########## マクロ定義 ##########
#define MOUSE_BUTTON_CODE 129

//########## 構造体 ##########
struct MOUSE 
{
	int inputValue = 0;	//GetMouseInputの値
	int wheelValue = 0;	//マウスホイールの回転量(奥＋　手前－)
	I_POINT point;
	I_POINT oldPoint;
	int oldButton[MOUSE_BUTTON_CODE];
	int button[MOUSE_BUTTON_CODE];
};

//########## 外部のグローバル変数 ##########
extern MOUSE mouse;	//マウス入力を取得

//########## プロトタイプ宣言 ##########
VOID MouseUpdate(VOID);								//マウスの入力情報を更新する
void MyMouseCheack(MOUSE* mouse, int MOUSE_INPUT_);	//マウスコードに対応したボタンが押されているかチェック
BOOL MouseDown(int MOUSE_INPUT_);					//ボタンを押しているか
BOOL MouseUp(int MOUSE_INPUT_);						//ボタンを押し上げたか
BOOL MouseDownKeep(int MOUSE_INPUT_,int milliTime);	//ボタンを押し続けているか
BOOL MouseChick(int MOUSE_INPUT_);					//マウスをクリックしたか
VOID MouseDraw(VOID);								//マウスの情報を描画する
VOID MouseDraw(int x,int y);						//位置を指定してマウスの情報を描画する
BOOL MouseRectChick(RECT rect,int MOUSE_INPUT_);	//マウスが矩形領域をクリックしたか
BOOL MouseCircleChick(CIRCLE circle,int MOUSE_INPUT_);//マウスが円領域をクリックしたか
