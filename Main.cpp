#include "Game.h"		//ゲーム全体のヘッダファイル
#include "Fps.h"		//FPS処理のヘッダファイル
#include "Keyboard.h"	//キーボードの処理のヘッダファイル
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>

struct IMAGE_COM
{
	int x;				//xの位置
	int y;				//yの位置
	int startX;
	int startY;
	int width;			//画像の幅
	int height;			//画像の高さ
	BOOL isDraw = FALSE;//画像が描画できるか
};

//当たり判定のない画像の構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル(管理番号)
	char path[255];		//画像の場所(パス)
	int x;				//xの位置
	int y;				//yの位置
	int width;			//画像の幅
	int height;			//画像の高さ
	int speed = 1;		//移動速度

	BOOL isDraw = FALSE;//画像が描画できるか
};

//キャラクタの構造体
struct CHARACTER
{
	IMAGE img;			//画像構造体

	int speed = 1;		//移動速度

	RECT coll;			//矩形当たり判定領域
};

struct MOVIE
{
	int handle = -1;	//動画ハンドル
	char path[255];		//動画パス
	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ
	int volume = 255;	//ボリューム(最小)0～255(最大)
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int volume = -1;	//ボリューム
	int playType = -1;
};

const int SHOT_ARRAY_MAX = 4;	//弾配列の最大数
//弾の構造体
struct SHOT
{
	int handle[SHOT_ARRAY_MAX];	//画像のハンドル
	char path[255];				//画像のパス

	int divX;	//分割数(横)
	int divY;	//分割数(縦)
	int divMax;	//分割総数

	int animeCnt = 0;		//アニメーションカウンター
	int animeCntMax = 0;	//アニメーションカウンター最大値

	int nowIndex = 0;		//現在の画像の要素数

	int speed;

	float radius;	//半径
	float degree;	//角度

	IMAGE_COM imageCom;		//画像の共通項目

	RECT coll;
};

//シーンを管理する変数
//現在のゲームのシーン
GAME_SCENE GameScene;
//前回のゲームのシーン
GAME_SCENE GameSceneOld;
//次のゲームのシーン
GAME_SCENE GameSceneNext;


//画面の切り替え
//フェードアウトしているか
BOOL isFadeOut = FALSE;
//フェードインしているか
BOOL isFadeIn = FALSE;

//切り替えミリ秒
int fadeTimeMill = 2000;
//ミリ秒をフレーム秒に変換
int fadeTimeMax = fadeTimeMill / 1000 * 60;

int fadeCnt = 0;

//フェードアウト
//初期値
int fadeOutCntInit = 0;
//フェードアウトのカウンタ
int fadeOutCnt = fadeOutCntInit;
//フェードアウトのカウンタMAX
int fadeOutCntMax = fadeTimeMax;

//フェードイン
//初期値
int fadeInCntInit = fadeTimeMax;
//フェードアウトのカウンタ
int fadeInCnt = fadeInCntInit;
//フェードアウトのカウンタ
int fadeInCntMax = fadeTimeMax;

//titleLogoの上下
int titleLogoCnt = 0;
const int titleLogoCntMax = 120;
BOOL isTitleEnterBrink = FALSE;

//弾の構造体
struct SHOT shotMoto;	//元
const int SHOT_MAX = 36;		//弾排出最大数
struct SHOT shotUse[SHOT_MAX];	//実際に使う

//弾の発射カウンタ
int shotIntervalCnt = 0;
int shotIntervalCntMax = 10;

//プレイヤー
CHARACTER player;

//爆発画像のハンドル
const int EXPROSION_X_MAX = 8;
const int EXPROSION_Y_MAX = 2;
const int EXPROSION_ARRAY_MAX = EXPROSION_X_MAX * EXPROSION_Y_MAX;
int exprosion[EXPROSION_ARRAY_MAX];
int exprosionIndex = 0;
int exprosionCnt = 0;
int exprosionCntMax = 4;

//ゲーム全体の初期化
BOOL GameLoad();
//画像の読み込み
BOOL CharacterLoad(CHARACTER* chara, const char* path);
BOOL ImageLoad(IMAGE* chara, const char* path);
BOOL ImageLoadDivMem(int* handle, const char* path, int arrayMax, int divX, int divY);
//音楽読み込み
BOOL AudioLoad(AUDIO* audio, const char* path, int playType, int volume);
//
VOID GameInit();
//※Alt+Shift+左ドラッグ=矩形選択
// //プロトタイプ宣言
//画面を切り替える処理
VOID ChangeScene(GAME_SCENE scene);
//タイトル画面
VOID Title();
//タイトル画面 処理
VOID TitleProc();
//タイトル画面 描画
VOID TitleDraw();
//弾の描画
VOID DrawShot(SHOT* shot);
//プレイ画面
VOID Play();
//プレイ画面 処理
VOID PlayProc();
//弾を飛ばす
VOID Shot(SHOT* shot, float deg);
//プレイ画面 描画
VOID PlayDraw();
//エンド画面
VOID End();
//エンド画面 処理
VOID EndProc();
//エンド画面 描画
VOID EndDraw();
//切り替え画面
VOID Change();
//切り替え画面 処理
VOID ChangeProc();
//切り替え画面 描画
VOID ChangeDraw();
//当たり判定の領域を更新
VOID CollUpdate(CHARACTER* chara);
VOID CollUpdate(SHOT* shot);
VOID CollUpdate(CHARACTER* chara, int addLeft, int addTop, int addRight, int addBottom);
//当たり判定(Enter)
BOOL CollStay(CHARACTER chara1, CHARACTER chara2);

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Log.txtを出力しない(GitHubで管理するため、現在必要ない)
	SetOutApplicationLogValidFlag(FALSE);	
	//ウィンドウモードに設定
	ChangeWindowMode(TRUE);
	//ゲームタイトルの設定
	SetMainWindowText(GAME_TITLE);
	//ゲームウィンドウ解像度設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	//ウィンドウのサイズを設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);
	//背景色の設定
	SetBackgroundColor(BACK_GROUND_COLOR);
	//ゲームのICONを設定
	SetWindowIconID(GAME_ICON_ID);
	//ウィンドウバーの状態
	SetWindowStyleMode(GAME_WINDOW_BAR);
	//ウィンドウの垂直同期を有効にする(ディスプレイの能力を制限して、フレームレート(fps)を一定に保つ)=FALSEはフレームレートを管理しなければならない
	SetWaitVSyncFlag(TRUE);
	//非アクティブでも実行する
	SetAlwaysRunFlag(TRUE);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);
	
	//最初のシーンはタイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//ゲーム読み込み
	if (!GameLoad()) 
	{
	//データの読み込みに失敗した時
		DxLib_End();	//DXライブラリの終了
		return -1;
	}

	//ゲームの初期化
	GameInit();
	
	while (1)
	{		
		//画面を消去する
		if (ClearDrawScreen() != 0) { break; }
		//キー入力状態を更新する
		AllKeyUpdate();

		//FPS値の更新※AllKeyUpdateなどの一連の処理が終了後に書く
		FpsUpdate();
		
		//メッセージを受け取り続ける、×などでウィンドウが閉じたとき
		if (ProcessMessage() != 0) 
		{
			//無限ループを抜ける
			break;
		}


		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE)) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE) 
		{
			GameSceneOld = GameScene;
		}

		//※switch tab→enum候補 tab→switch_onの中にenumの変数が入る enter
		switch (GameScene)
		{
			//タイトル画面
		case GAME_SCENE_TITLE:
			Title();
			break;

			//プレイ画面
		case GAME_SCENE_PLAY:
			Play();
			break;

			//エンド画面
		case GAME_SCENE_END:
			End();
			break;

			//切り替え画面
		case GAME_SCENE_CHANGE:
			Change();
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (GameSceneOld != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene!=GAME_SCENE_CHANGE) 
			{
				GameSceneNext = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		

		//FPS値を描画
		FpsDraw(0,GAME_HEIGHT-16,TRUE);

		//FPS値を待つ
		FpsWait();

		//ダブルバッファリングした画像を描画
		ScreenFlip();
	}

	//読み込んだ画像を解放
	for (int i = 0; i < SHOT_ARRAY_MAX; i++) 
	{
		DeleteGraph(shotMoto.handle[i]);
	}

	// ＤＸライブラリ使用の終了処理準備(return 0でソフトが終了する)
	DxLib_End();				

	// ソフトの終了
	return 0;				 
}

const int VOLUME_MAX = 255;
const int BGM_LOOP = DX_PLAYTYPE_LOOP;

/// <summary>
/// ゲームデータの読み込み
/// </summary>
/// <returns>読み込めたらTRUE, 読み込めなかったらFALSE</returns>
BOOL GameLoad() 
{
	//弾の分割数を設定
	shotMoto.divX = 4;
	shotMoto.divY = 1;

	//弾画像の読み込み
	strcpyDx(shotMoto.path, ".\\Images\\Image\\dia_pink.png");

	if (ImageLoadDivMem(&shotMoto.handle[0], shotMoto.path, shotMoto.divX*shotMoto.divY, shotMoto.divX, shotMoto.divY)
		==FALSE)
	{
		return FALSE;
	}

	//幅と高さを取得
	GetGraphSize(shotMoto.handle[0], &shotMoto.imageCom.width, &shotMoto.imageCom.height);

	//位置の設定
	shotMoto.imageCom.x = GAME_WIDTH / 2 - shotMoto.imageCom.width/2;	//中央揃え
	shotMoto.imageCom.y = GAME_HEIGHT/2 - shotMoto.imageCom.height/2;	//画面下
	
	//速度
	shotMoto.speed = 5;

	//アニメーションを変えるスピード
	shotMoto.animeCntMax = 30;

	//当たり判定の更新
	CollUpdate(&shotMoto);

	//画像を表示しない
	shotMoto.imageCom.isDraw = FALSE;

	//すべての球に情報をコピー
	for (int i = 0; i < SHOT_MAX; i++) 
	{
		shotUse[i] = shotMoto;
	}

	//プレイヤー画像を読み込み
	if (ImageLoad(&player.img, ".\\Images\\Image\\player.png")
		== FALSE)
	{
		return FALSE;
	}
	player.img.x = GAME_WIDTH / 2 - player.img.width/2;
	player.img.y = GAME_HEIGHT / 2 - player.img.height/2;
	CollUpdate(&player,20,10,-20,-10);
	player.img.isDraw = TRUE;
	player.speed = 1;

	//爆発画像の読み込み
	const char* exprosionPath = ".\\Images\\Image\\baku1.png";
	if (ImageLoadDivMem(&exprosion[0], exprosionPath,
		EXPROSION_ARRAY_MAX, EXPROSION_X_MAX, EXPROSION_Y_MAX)
		== FALSE) 
	{
		return FALSE;
	}

	return TRUE;
}

//キャラクターの読み込み
BOOL CharacterLoad(CHARACTER* chara, const char* path)
{
	//画像の読み込み
	strcpyDx(chara->img.path, path);
	chara->img.handle = LoadGraph(chara->img.path);

	if (chara->img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			chara->img.path,
			"画像読み込みエラー",
			MB_OK
		);

		return FALSE;
	}

	//画像の幅と高さを取得
	GetGraphSize(chara->img.handle, &chara->img.width, &chara->img.height);

	return TRUE;
}

//画像の読み込み
BOOL ImageLoad(IMAGE* chara, const char* path)
{
	//画像の読み込み
	strcpyDx(chara->path, path);
	chara->handle = LoadGraph(chara->path);

	if (chara->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			chara->path,
			"画像読み込みエラー",
			MB_OK
		);

		return FALSE;
	}

	//画像の幅と高さを取得
	GetGraphSize(chara->handle, &chara->width, &chara->height);

	return TRUE;
}

/// <summary>
/// 画像を分割してメモリに読み込み
/// </summary>
/// <param name="handle">ハンドル配列の先頭アドレス</param>
/// <param name="path">画像のパス</param>
/// <param name="divX">分割する時の横の数</param>
/// <param name="divY">分割する時の縦の数</param>
/// <returns></returns>
BOOL ImageLoadDivMem(int* handle, const char* path,int arrayMax, int divX, int divY) 
{
	//弾分割画像を読み込む
	int isShotLoad = -1;	//画像を読み込めたか

	//分割画像の大きさ取得用
	int shotImageHandle = LoadGraph(path);

	//画像の読み込みエラー判定
	if (shotImageHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,			//本文
			"弾画像読み込みエラー",	//タイトル
			MB_OK					//ボタン
		);

		return FALSE;	//読み込み失敗
	}

	//画像の幅と高さを取得
	int shotWidth = -1;
	int shotHeight = -1;
	GetGraphSize(shotImageHandle, &shotWidth, &shotHeight);

	isShotLoad = LoadDivGraph(
		path,	//画像のパス
		arrayMax,				//分割総数
		divX, divY,	//横縦の分割
		shotWidth / divX, shotHeight / divY,
		handle
	);



	if (isShotLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウィンドウハンドル
			path,			//本文
			"弾分割画像読み込みエラー",	//タイトル
			MB_OK					//ボタン
		);

		return FALSE;
	}

	//弾の画像の大きさの役割は以上なので削除
	DeleteGraph(shotImageHandle);

	return TRUE;
}

//音楽の読み込み
BOOL AudioLoad(AUDIO* audio,const char* path, int playType, int volume)
{
	//音楽の読み込み
	strcpyDx(audio->path, path);
	audio->handle = LoadSoundMem(audio->path);

	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			audio->path,
			"音楽読み込みエラー",
			MB_OK
		);

		return FALSE;
	}

	audio->playType = playType;
	audio->volume = volume;

	return TRUE;
}

/// <summary>
/// ゲームデータの初期化
/// </summary>
VOID GameInit() 
{
	
}

/// <summary>
/// シーンを切り替える
/// </summary>
/// <param name="scene">切り替え先のシーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;				//画面切り替え
	isFadeIn = FALSE;				//フェードインにしない
	isFadeOut = TRUE;				//フェードアウトする	
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title() 
{
	//処理
	TitleProc();
	//描画
	TitleDraw();
}

//処理
VOID TitleProc() 
{
	//スペースが押されたときに球を発射する
	

	//ゲーム画面に切り替わる
	if (KeyClick(KEY_INPUT_RETURN)) 
	{
		//次のシーンの初期化をここで行うと楽
		GameInit();	//ゲームの初期化

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	return;
}

//描画
VOID TitleDraw() 
{
	

	//爆発の描画
	DrawGraph(100, 100, exprosion[exprosionIndex], TRUE);

	if (exprosionCnt < exprosionCntMax)
	{
		exprosionCnt++;
	}
	else 
	{
		exprosionCnt = 0;

		if (exprosionIndex < EXPROSION_ARRAY_MAX - 1) 
		{
			exprosionIndex++;
		}
		else 
		{
			exprosionIndex = 0;
		}
	}
	

	return;
}

/// <summary>
/// 弾の描画
/// </summary>
/// <param name="shot">弾の構造体</param>
VOID DrawShot(SHOT* shot) 
{
	if (shot->imageCom.isDraw) 
	{
		//弾の描画
		DrawGraph(shot->imageCom.x, shot->imageCom.y, shot->handle[shot->nowIndex], TRUE);

		if (shot->animeCnt < shot->animeCntMax)
		{
			shot->animeCnt++;
		}
		else
		{
			shot->animeCnt = 0;

			//弾の添え字が弾の分割数の最大よりも小さいとき
			if (shot->nowIndex < SHOT_ARRAY_MAX - 1)
			{
				shot->nowIndex++;
			}
			else
			{
				shot->nowIndex = 0;
			}
		}
	}
}

//プレイ画面
VOID Play() 
{
	//処理
	PlayProc();
	//描画
	PlayDraw();

	
}

//処理
VOID PlayProc() 
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_END);

		return;
	}

	//プレイヤーを操作する
	//左
	if (KeyDown(KEY_INPUT_LEFT)==TRUE) 
	{
		if (player.img.x - player.speed >= 0) 
		{
			player.img.x -= player.speed;
		}
	}

	//右
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		if (player.img.x + player.speed <= GAME_WIDTH)
		{
			player.img.x += player.speed;
		}
	}

	//上
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		if (player.img.y - player.speed >= 0)
		{
			player.img.y -= player.speed;
		}
	}

	//下
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		if (player.img.y + player.speed <= GAME_HEIGHT)
		{
			player.img.y += player.speed;
		}
	}

	//プレイヤーの当たり判定の更新
	CollUpdate(&player, 20, 10, -20, -10);

	if (KeyDown(KEY_INPUT_SPACE)&& shotIntervalCnt == 0)
	{
		//--三方向に発射--↓
		////弾を発射する
		//for (int i = 0; i < SHOT_MAX; i++)
		//{
		//	if (shotUse[i].imageCom.isDraw == FALSE) 
		//	{
		//		//弾を発射
		//		Shot(&shotUse[i], 270.0f);

		//		break;
		//	}
		//}

		////弾を発射する
		//for (int i = 0; i < SHOT_MAX; i++)
		//{
		//	if (shotUse[i].imageCom.isDraw == FALSE)
		//	{
		//		//弾を発射
		//		Shot(&shotUse[i], 240.0f);

		//		break;
		//	}
		//}

		////弾を発射する
		//for (int i = 0; i < SHOT_MAX; i++)
		//{
		//	if (shotUse[i].imageCom.isDraw == FALSE)
		//	{
		//		//弾を発射
		//		Shot(&shotUse[i], 300.0f);

		//		break;
		//	}
		//}
		//--三方向に発射--↑

		//放射状に発射
		for (int deg = 0; deg < 180; deg += 10) {
			for (int i = 0; i<SHOT_MAX; i++) {
				if (shotUse[i].imageCom.isDraw == FALSE) {
					Shot(&shotUse[i], deg);

					break;
				}
			}
		}
	}

	//弾の発射待ち
	if (shotIntervalCnt != 0 && shotIntervalCnt < shotIntervalCntMax)
	{
		shotIntervalCnt++;
	}
	else
	{
		shotIntervalCnt = 0;
	}

	//弾を飛ばす
	for (int i = 0; i < SHOT_MAX; i++)
	{
		if (shotUse[i].imageCom.isDraw == TRUE)
		{
			//弾の位置を修正
			//中心位置＋飛ばす角度→飛ばす距離を計算＊距離
			shotUse[i].imageCom.x = shotUse[i].imageCom.startX+cos(shotUse[i].degree * DX_PI / 180.0f) * shotUse[i].radius;
			shotUse[i].imageCom.y = shotUse[i].imageCom.startY+sin(shotUse[i].degree * DX_PI / 180.0f) * shotUse[i].radius;

			//この処理を追加すると弾が回転する
			//shotUse[i].degree--;

			//半径を足す
			shotUse[i].radius += shotUse[i].speed;

			//弾の当たり判定を更新
			CollUpdate(&shotUse[i]);

			//画面外に出たら描画しない
			if (shotUse[i].imageCom.y + shotUse[i].imageCom.height<0
				|| shotUse[i].imageCom.y>GAME_HEIGHT
				|| shotUse[i].imageCom.x + shotUse[i].imageCom.width<0
				|| shotUse[i].imageCom.x>GAME_WIDTH)
			{
				shotUse[i].imageCom.isDraw = FALSE;
			}
		}
	}
	
	
	
	return;
}

/// <summary>
/// 弾を飛ばす
/// </summary>
/// <param name="shot">弾の構造体</param>
/// <param name="deg">角度</param>
VOID Shot(SHOT *shot,float deg) 
{
	shot->imageCom.isDraw = TRUE;

	//弾の位置を決める
	shot->imageCom.startX = player.img.x + player.img.width / 2 - shot->imageCom.width / 2;
	shot->imageCom.startY = player.img.y;

	shot->imageCom.x = player.img.x + player.img.width / 2 - shot->imageCom.width / 2;
	shot->imageCom.y = player.img.y;

	//弾の角度を決める
	shot->degree = deg;
	shot->radius = 0.0f;

	//弾の当たり判定を更新
	CollUpdate(shot);

	shotIntervalCnt++;
}

//描画
VOID PlayDraw() 
{
	//プレイヤーの描画
	if (player.img.isDraw) 
	{
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		
		//当たり判定の描画
		if (GAME_DEBUG)
		{
			DrawBox(
				player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE
			);
		}
	}

	//弾を出す
	for (int i = 0; i < SHOT_MAX; i++)
	{
		if (shotUse[i].imageCom.isDraw)
		{
			DrawShot(&shotUse[i]);
		}

		//当たり判定の描画
		if (GAME_DEBUG)
		{
			DrawBox(
				shotUse[i].coll.left, shotUse[i].coll.top, shotUse[i].coll.right, shotUse[i].coll.bottom,
				GetColor(255, 0, 0), FALSE
			);
		}
	}

	if (GAME_DEBUG)
	{
		const char* string = "Play";
		DrawString(0, 50, string, GetColor(0, 0, 0));
	}


	return;
}

//エンド画面
VOID End() 
{
	//処理
	EndProc();
	//描画
	EndDraw();
}

//処理
VOID EndProc()
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	return;
}

//描画
VOID EndDraw()
{
	if (GAME_DEBUG)
	{
		const char* string = "End";
		DrawString(0, 50, string, GetColor(0, 0, 0));
	}


	return;
}

//切り替え画面
VOID Change() 
{
	if (GAME_DEBUG)
	{
		DrawString(0, 100, "読み込み", GetColor(0, 0, 0));
	}

	//処理
	ChangeProc();
	//描画
	ChangeDraw();
}

//処理
VOID ChangeProc()
{
	//フェードイン
	if (isFadeIn) 
	{
		if (fadeInCnt > fadeInCntMax) 
		{
			//カウンタを減らす
			fadeInCnt--;
		}
		else //フェードイン処理が終了したとき
		{
			//カウンタを初期化
			fadeInCnt = fadeInCntInit;
			isFadeIn = FALSE;
		}
	}

	//フェードアウト
	if (isFadeOut) 
	{
		if (fadeOutCnt < fadeOutCntMax) 
		{
		//カウンタを増やす
			fadeOutCnt++;
		}
		else 
		{
		//カウンタ初期化
			fadeOutCnt = fadeOutCntInit;;
			isFadeOut = FALSE;
		}
	}

	//切り替え処理終了か
	if (isFadeIn == FALSE && isFadeOut == FALSE) 
	{
	//フェードインしていない、かつフェードアウトもしていない
		//次のシーンに切り替え
		GameScene = GameSceneNext;
		//以前のシーンの更新
		GameSceneOld = GameScene;
	}

	return;
}

//描画
VOID ChangeDraw()
{
	switch (GameSceneOld)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ　画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド　画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (isFadeIn) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (isFadeOut) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);
	
	//四角を描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (GAME_DEBUG)
	{
		DrawString(0, 0, "切り替え", GetColor(0, 0, 0));
	}

	return;
}



/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="coll"></param>
VOID CollUpdate(CHARACTER* chara) 
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="imageCom.coll"></param>
VOID CollUpdate(SHOT* shot)
{
	shot->coll.left = shot->imageCom.x;
	shot->coll.top = shot->imageCom.y;
	shot->coll.right = shot->imageCom.x + shot->imageCom.width;
	shot->coll.bottom = shot->imageCom.y + shot->imageCom.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新(四方のオーバーロード)
/// </summary>
/// <param name="chara"></param>
/// <param name="addLeft"></param>
/// <param name="addTop"></param>
/// <param name="addRight"></param>
/// <param name="addBottom"></param>
VOID CollUpdate(CHARACTER* chara, int addLeft, int addTop, int addRight, int addBottom)
{
	chara->coll.left = chara->img.x + addLeft;
	chara->coll.top = chara->img.y + addTop;
	chara->coll.right = chara->img.x + chara->img.width + addRight;
	chara->coll.bottom = chara->img.y + chara->img.height + addBottom;

	return;
}

//当たり判定(Enter)
BOOL CollStay(CHARACTER chara1, CHARACTER chara2)
{
	//描画されていないなら行わない
	if (chara1.img.isDraw && chara2.img.isDraw)
	{
		//当たり判定
		if (
			chara1.coll.left<chara2.coll.right
			&& chara1.coll.right>chara2.coll.left
			&& chara1.coll.top<chara2.coll.bottom
			&& chara1.coll.bottom>chara2.coll.top
			)
		{
			return TRUE;
		}
	}
	

	return FALSE;
}