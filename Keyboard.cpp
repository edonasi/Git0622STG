#include "DxLib.h"
//キーボードのソースファイル
#include "Keyboard.h"

//グローバル変数
KEYBOARD keyboard;

//関数

/// <summary>
/// キー入力状態を更新する
/// </summary>
VOID AllKeyUpdate(VOID) 
{
	//直前のキー入力を取っておく
	for (int i = 0; i < KEY_KIND_MAX; i++) 
	{
		keyboard.oldKeyState[i] = keyboard.allKeyState[i];
	}

	//すべてのキーの入力状態を得る
	GetHitKeyStateAll(keyboard.tmpKeyState);

	//押されているキーの時間を更新する
	for (int i = 0; i < KEY_KIND_MAX; i++) 
	{
	//キーコードを押しているとき
		if (keyboard.tmpKeyState[i] != 0)
		{
			//押している時間を加算
			keyboard.allKeyState[i]++;
		}
		else 
		{
			//押している時間を0に戻す
			keyboard.allKeyState[i] = 0;
		}
	}

	return;
}

/// <summary>
/// キーを押しているかキーコードで判断する
/// </summary>
/// <param name="keyInput">キーコード</param>
/// <returns>押しているならTRUE</returns>
BOOL KeyDown(int keyInput)
{
	if (keyboard.allKeyState[keyInput] != 0) 
	{
		//押した
		return TRUE;
	}
	else 
	{
		//押してない
		return FALSE;
	}
}

/// <summary>
/// キーを押した瞬間かキーコードで判断する
/// </summary>
/// <param name="keyInput">キーコード</param>
/// <returns>押しているならTRUE</returns>
BOOL Key1Down(int keyInput)
{
	if (keyboard.allKeyState[keyInput] == 1)
	{
		//押した
		return TRUE;
	}
	else
	{
		//押してない
		return FALSE;
	}
}

/// <summary>
/// キーを上げているかキーコードで判断する
/// </summary>
/// <param name="keyInput">キーコード</param>
/// <returns>上げているならTRUE</returns>
BOOL KeyUp(int keyInput)
{
	//以前は押した
	if (keyboard.oldKeyState[keyInput] != 0		
		//現在は押してない
		&& keyboard.allKeyState[keyInput] == 0)	
	{
		//上げた
		return TRUE;
	}
	else
	{
		//上げていない
		return FALSE;
	}
}

BOOL KeyUpping(int keyInput) 
{
	//以前は押した
	if (keyboard.allKeyState[keyInput] ==0)
	{
		//上げた
		return TRUE;
	}
	else
	{
		//上げていない
		return FALSE;
	}
}

/// <summary>
/// キーをクリックしたか
/// </summary>
/// <param name="keyInput">キーコード</param>
/// <returns>クリックしたらTRUETRUE</returns>
BOOL KeyClick(int keyInput)
{
	//以前は押した
	if (keyboard.oldKeyState[keyInput] != 0		
		//現在は押してない
		&& keyboard.allKeyState[keyInput] == 0)	
	{
		//クリックした
		return TRUE;
	}
	else
	{
		//クリックしてない
		return FALSE;
	}
}

/// <summary>
/// キーを押し続けているか
/// </summary>
/// <param name="keyInput">キーコード</param>
/// <param name="milliTime">キーコードを押し続けているミリ秒</param>
/// <returns></returns>
BOOL KeyDownKeep(int keyInput, int milliTime)
{
	//1秒 = 1000ミリ秒
	float milliSec = 1000.0f;

	//押し続けている時間はミリ秒数×FPS値
	//例：1500ミリ秒押す / 1000ミリ = 1.5秒×60FPS = 90(keyTime)
	int updateTime = (milliTime / milliSec) * 60;

	if (keyboard.allKeyState[keyInput] > updateTime)
	{
		//押し続けている
		return TRUE;
	}
	else 
	{
		//押し続けていない
		return FALSE;
	}
}