//マウス処理のソースファイル

//########## ヘッダーファイル読み込み ##########
#include "Game.h"
#include "Mouse.h"
#include "Fps.h"

//########## グローバル変数 ##########

//マウス入力を取得
MOUSE mouse;

//########## 関数 ##########

//マウスの入力情報を更新する
VOID MouseUpdate(VOID)
{
//マウスの以前の情報を取得
	mouse.oldPoint = mouse.point;

	//マウスの以前のボタン入力を取得
	mouse.oldButton[MOUSE_INPUT_LEFT] = mouse.button[MOUSE_INPUT_LEFT];
	mouse.oldButton[MOUSE_INPUT_MIDDLE] = mouse.button[MOUSE_INPUT_MIDDLE];
	mouse.oldButton[MOUSE_INPUT_RIGHT] = mouse.button[MOUSE_INPUT_RIGHT];

	//マウスの座標を取得
	GetMousePoint(&mouse.point.x, &mouse.point.y);

	//マウスの座標が画面外の場合は、画面内に収める
	if (mouse.point.x < 0) { mouse.point.x = 0; }
	if (mouse.point.x > GAME_WIDTH) { mouse.point.x = GAME_WIDTH; }
	if (mouse.point.y < 0) { mouse.point.y = 0; }
	if (mouse.point.y > GAME_HEIGHT) { mouse.point.y = GAME_HEIGHT; }

	//マウスの推しているボタンを取得
	mouse.inputValue = GetMouseInput();

	//マウスコードに対応したボタンが押されているかチェック
	MyMouseCheack(&mouse, MOUSE_INPUT_LEFT);	//左
	MyMouseCheack(&mouse, MOUSE_INPUT_MIDDLE);	//中
	MyMouseCheack(&mouse, MOUSE_INPUT_RIGHT);	//右
	
	//ホイールの回転量を取得
	mouse.wheelValue = GetMouseWheelRotVol();

	return;
}

/// <summary>
/// マウスコードに対応したボタンが押されているかチェック
/// </summary>
/// <param name="mouse">判定するマウスポインタ</param>
/// <param name="MOUSE_INPUT_">マウスコード</param>
void MyMouseCheack(MOUSE* mouse,int MOUSE_INPUT_) 
{
	//押しているとき
	if ((mouse->inputValue &  MOUSE_INPUT_) ==  MOUSE_INPUT_)
	{
		mouse->button[ MOUSE_INPUT_]++;
	}
	//押していないとき
	if ((mouse->inputValue &  MOUSE_INPUT_) !=  MOUSE_INPUT_)
	{
		mouse->button[ MOUSE_INPUT_] = 0;
	}
}

/// <summary>
/// ボタンを押しているか
/// </summary>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <returns>ボタンを押しているならTRUE</returns>
BOOL MouseDown(int MOUSE_INPUT_) 
{
	//TRUEならボタンが押されている
	return (mouse.button[MOUSE_INPUT_] != 0) ? TRUE : FALSE;
}

/// <summary>
/// ボタンを押し上げたか
/// </summary>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <returns>ボタンを押しあげているならTRUE</returns>
BOOL MouseUp(int MOUSE_INPUT_) 
{
	if (mouse.oldButton[MOUSE_INPUT_] >= 1	//直前は押していて
		&& mouse.button[MOUSE_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//押し上げている
	}
	else 
	{
		return FALSE;	//押し上げていない
	}
}

/// <summary>
/// ボタンを押し続けているか
/// </summary>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <param name="milliTime">ボタンを押し続けている時間(ミリ秒)</param>
/// <returns>押し続けていたらTRUE</returns>
BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime) 
{
	float millSec = 1000.0f;	//1秒は1000ミリ秒

	//押し続ける時間=秒数×FPS値
	//例）60FPSのゲームで、1秒間押し続けるなら、1秒×60FPS
	int updateTime = (milliTime / millSec) * fps.Value;

	//押し続けていたらTRUE
	return (mouse.button[MOUSE_INPUT_] > updateTime) ? TRUE : FALSE;
}	

/// <summary>
/// マウスをクリックしたか
/// </summary>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <returns>クリックしたらTRUE</returns>
BOOL MouseChick(int MOUSE_INPUT_) 
{
	if (mouse.oldButton[MOUSE_INPUT_] >= 1	//直前は押していて
		&& mouse.button[MOUSE_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//押し上げている
	}
	else
	{
		return FALSE;	//押し上げていない
	}
}

//マウスの情報を描画する
VOID MouseDraw(VOID) 
{
	if (GAME_DEBUG == TRUE) 
	{
	//マウスの座標を描画
		DrawFormatString(
			0, GAME_HEIGHT - 40,
			GetColor(255, 255, 255), "MOUSE[X:%4d/Y:%4d]",
			mouse.point.x, mouse.point.y
		);
	}
	return;
}

/// <summary>
/// 位置を指定してマウスの情報を描画する
/// </summary>
/// <param name="x">x座標</param>
/// <param name="y">y座標</param>
/// <returns></returns>
VOID MouseDraw(int x, int y) 
{
	if (GAME_DEBUG == TRUE)
	{
		//マウスの座標を描画
		DrawFormatString(
			x, y,
			GetColor(255, 255, 255), "MOUSE[X:%4d/Y:%4d]",
			mouse.point.x, mouse.point.y
		);
	}
	return;
}


/// <summary>
/// マウスが矩形領域をクリックしたか
/// </summary>
/// <param name="rect">矩形領域</param>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <returns>クリックしたらTRUE</returns>
BOOL MouseRectChick(RECT rect, int MOUSE_INPUT_) 
{
	//点と四角の当たり判定
	if (CheckColliPointToRect(mouse.point,rect) == TRUE) 
	{
		//マウスをクリックしているか
		return MouseChick(MOUSE_INPUT_);
	}

	return FALSE;	//クリックしていない
}

/// <summary>
/// マウスが円領域をクリックしたか
/// </summary>
/// <param name="circle">円領域</param>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <returns>クリックしたらTRUE</returns>
BOOL MouseCircleChick(CIRCLE circle, int MOUSE_INPUT_) 
{
	//点と円の当たり判定
	if (CheckColliPointToCircle(mouse.point, circle) == TRUE) 
	{
		//マウスをクリックしているか
		return MouseChick(MOUSE_INPUT_);
	}

	return FALSE;	//クリックしていない
}