#include "Game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "Fps.h"		//FPS�����̃w�b�_�t�@�C��
#include "Keyboard.h"	//�L�[�{�[�h�̏����̃w�b�_�t�@�C��
#include "Mouse.h"
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>

struct IMAGE_COM
{
	int x;				//x�̈ʒu
	int y;				//y�̈ʒu
	int startX;
	int startY;
	int width;			//�摜�̕�
	int height;			//�摜�̍���
	BOOL isDraw = FALSE;//�摜���`��ł��邩
};

//�����蔻��̂Ȃ��摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h��(�Ǘ��ԍ�)
	char path[255];		//�摜�̏ꏊ(�p�X)
	int x;				//x�̈ʒu
	int y;				//y�̈ʒu
	int width;			//�摜�̕�
	int height;			//�摜�̍���
	int speed = 1;		//�ړ����x

	BOOL isDraw = FALSE;//�摜���`��ł��邩
};

//�L�����N�^�̍\����
struct CHARACTER
{
	IMAGE img;			//�摜�\����

	int speed = 1;		//�ړ����x

	RECT coll;			//��`�����蔻��̈�
};

struct MOVIE
{
	int handle = -1;	//����n���h��
	char path[255];		//����p�X
	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����
	int volume = 255;	//�{�����[��(�ŏ�)0�`255(�ő�)
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int volume = -1;	//�{�����[��
	int playType = -1;
};

const int SHOT_ARRAY_MAX = 4;	//�e�z��̍ő吔
//�e�̍\����
struct SHOT
{
	int handle[SHOT_ARRAY_MAX];	//�摜�̃n���h��
	char path[255];				//�摜�̃p�X

	int divX;	//������(��)
	int divY;	//������(�c)
	int divMax;	//��������

	int animeCnt = 0;		//�A�j���[�V�����J�E���^�[
	int animeCntMax = 0;	//�A�j���[�V�����J�E���^�[�ő�l

	int nowIndex = 0;		//���݂̉摜�̗v�f��

	int speed;

	float radius;	//���a
	float degree;	//�p�x

	IMAGE_COM imageCom;		//�摜�̋��ʍ���

	RECT coll;
	CIRCLE circle;
};

//�V�[�����Ǘ�����ϐ�
//���݂̃Q�[���̃V�[��
GAME_SCENE GameScene;
//�O��̃Q�[���̃V�[��
GAME_SCENE GameSceneOld;
//���̃Q�[���̃V�[��
GAME_SCENE GameSceneNext;


//��ʂ̐؂�ւ�
//�t�F�[�h�A�E�g���Ă��邩
BOOL isFadeOut = FALSE;
//�t�F�[�h�C�����Ă��邩
BOOL isFadeIn = FALSE;

//�؂�ւ��~���b
int fadeTimeMill = 2000;
//�~���b���t���[���b�ɕϊ�
int fadeTimeMax = fadeTimeMill / 1000 * 60;

int fadeCnt = 0;

//�t�F�[�h�A�E�g
//�����l
int fadeOutCntInit = 0;
//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCnt = fadeOutCntInit;
//�t�F�[�h�A�E�g�̃J�E���^MAX
int fadeOutCntMax = fadeTimeMax;

//�t�F�[�h�C��
//�����l
int fadeInCntInit = fadeTimeMax;
//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCnt = fadeInCntInit;
//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;

//titleLogo�̏㉺
int titleLogoCnt = 0;
const int titleLogoCntMax = 120;
BOOL isTitleEnterBrink = FALSE;

//�e�̍\����
struct SHOT shotMoto;	//��
const int SHOT_MAX = 36;		//�e�r�o�ő吔
struct SHOT shotUse[SHOT_MAX];	//���ۂɎg��

//�e�̔��˃J�E���^
int shotIntervalCnt = 0;
int shotIntervalCntMax = 10;

//�v���C���[
CHARACTER player;

//�G
const int ENEMY_KIND = 7;
const int ENEMY_MAX = 10;
CHARACTER enemyMoto[ENEMY_KIND];
CHARACTER enemyUse[ENEMY_MAX];
char enemyPath[ENEMY_KIND][255] = {
	{".\\Images\\teki_yellow.png"},
	{".\\Images\\teki_red.png"},
	{".\\Images\\teki_purple.png"},
	{".\\Images\\teki_mizu.png"},
	{".\\Images\\teki_green.png"},
	{".\\Images\\teki_gray.png"},
	{".\\Images\\teki_blue.png"}
};
//�G���o�Ă���C���^�[�o��
int enemyInterval = 0;
int enemyIntervalMax = 128;

//�w�i�摜
const int BACK_IMG_MAX = 2;
IMAGE backImg[BACK_IMG_MAX];

//�X�R�A
int score = 0;

//�����摜�̃n���h��
const int EXPROSION_X_MAX = 8;
const int EXPROSION_Y_MAX = 2;
const int EXPROSION_ARRAY_MAX = EXPROSION_X_MAX * EXPROSION_Y_MAX;
int exprosion[EXPROSION_ARRAY_MAX];
int exprosionIndex = 0;
int exprosionCnt = 0;
int exprosionCntMax = 4;

//�Q�[���S�̂̏�����
BOOL GameLoad();
//�摜�̓ǂݍ���
BOOL CharacterLoad(CHARACTER* chara, const char* path);
BOOL ImageLoad(IMAGE* chara, const char* path);
BOOL ImageLoadDivMem(int* handle, const char* path, int arrayMax, int divX, int divY);
//���y�ǂݍ���
BOOL AudioLoad(AUDIO* audio, const char* path, int playType, int volume);
//
VOID GameInit();
//��Alt+Shift+���h���b�O=��`�I��
// //�v���g�^�C�v�錾
//��ʂ�؂�ւ��鏈��
VOID ChangeScene(GAME_SCENE scene);
//�^�C�g�����
VOID Title();
//�^�C�g����� ����
VOID TitleProc();
//�^�C�g����� �`��
VOID TitleDraw();
//�e�̕`��
VOID DrawShot(SHOT* shot);
//�v���C���
VOID Play();
//�v���C��� ����
VOID PlayProc();
//�e���΂�
VOID Shot(SHOT* shot, float deg);
//�v���C��� �`��
VOID PlayDraw();
//�G���h���
VOID End();
//�G���h��� ����
VOID EndProc();
//�G���h��� �`��
VOID EndDraw();
//�؂�ւ����
VOID Change();
//�؂�ւ���� ����
VOID ChangeProc();
//�؂�ւ���� �`��
VOID ChangeDraw();
//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTER* chara);
VOID CollUpdate(SHOT* shot);
VOID CollUpdate(CHARACTER* chara, int addLeft, int addTop, int addRight, int addBottom);
VOID CollCircleUpdate(SHOT* shot);
//�����蔻��(Enter)
BOOL CollStay(CHARACTER chara1, CHARACTER chara2);
BOOL CollStay(RECT rect1, RECT rect2);

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Log.txt���o�͂��Ȃ�(GitHub�ŊǗ����邽�߁A���ݕK�v�Ȃ�)
	SetOutApplicationLogValidFlag(FALSE);	
	//�E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);
	//�Q�[���^�C�g���̐ݒ�
	SetMainWindowText(GAME_TITLE);
	//�Q�[���E�B���h�E�𑜓x�ݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	//�E�B���h�E�̃T�C�Y��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);
	//�w�i�F�̐ݒ�
	SetBackgroundColor(BACK_GROUND_COLOR);
	//�Q�[����ICON��ݒ�
	SetWindowIconID(GAME_ICON_ID);
	//�E�B���h�E�o�[�̏��
	SetWindowStyleMode(GAME_WINDOW_BAR);
	//�E�B���h�E�̐���������L���ɂ���(�f�B�X�v���C�̔\�͂𐧌����āA�t���[�����[�g(fps)�����ɕۂ�)=FALSE�̓t���[�����[�g���Ǘ����Ȃ���΂Ȃ�Ȃ�
	SetWaitVSyncFlag(TRUE);
	//��A�N�e�B�u�ł����s����
	SetAlwaysRunFlag(TRUE);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);
	
	//�ŏ��̃V�[���̓^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���ǂݍ���
	if (!GameLoad()) 
	{
	//�f�[�^�̓ǂݍ��݂Ɏ��s������
		DxLib_End();	//DX���C�u�����̏I��
		return -1;
	}

	//�Q�[���̏�����
	GameInit();
	
	while (1)
	{		
		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }
		//�L�[���͏�Ԃ��X�V����
		AllKeyUpdate();

		//FPS�l�̍X�V��AllKeyUpdate�Ȃǂ̈�A�̏������I����ɏ���
		FpsUpdate();

		//�}�E�X�̓��͏����X�V����
		MouseUpdate();
		
		//���b�Z�[�W���󂯎�葱����A�~�ȂǂŃE�B���h�E�������Ƃ�
		if (ProcessMessage() != 0) 
		{
			//�������[�v�𔲂���
			break;
		}


		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE)) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE) 
		{
			GameSceneOld = GameScene;
		}

		//��switch tab��enum��� tab��switch_on�̒���enum�̕ϐ������� enter
		switch (GameScene)
		{
			//�^�C�g�����
		case GAME_SCENE_TITLE:
			Title();
			break;

			//�v���C���
		case GAME_SCENE_PLAY:
			Play();
			break;

			//�G���h���
		case GAME_SCENE_END:
			End();
			break;

			//�؂�ւ����
		case GAME_SCENE_CHANGE:
			Change();
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (GameSceneOld != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene!=GAME_SCENE_CHANGE) 
			{
				GameSceneNext = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		

		//FPS�l��`��
		FpsDraw(0,GAME_HEIGHT-16,TRUE);

		//FPS�l��҂�
		FpsWait();

		//�_�u���o�b�t�@�����O�����摜��`��
		ScreenFlip();
	}

	//�ǂݍ��񂾉摜�����
	for (int i = 0; i < SHOT_ARRAY_MAX; i++) 
	{
		DeleteGraph(shotMoto.handle[i]);
	}
	DeleteGraph(player.img.handle);
	DeleteGraph(backImg[0].handle);
	DeleteGraph(backImg[1].handle);
	for (int i = 0; i < ENEMY_KIND; i++)
	{
		DeleteGraph(enemyMoto[i].img.handle);
	}
	
	// �c�w���C�u�����g�p�̏I����������(return 0�Ń\�t�g���I������)
	DxLib_End();				

	// �\�t�g�̏I��
	return 0;				 
}

const int VOLUME_MAX = 255;
const int BGM_LOOP = DX_PLAYTYPE_LOOP;

/// <summary>
/// �Q�[���f�[�^�̓ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE, �ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad() 
{
	//�e�̕�������ݒ�
	shotMoto.divX = 4;
	shotMoto.divY = 1;

	//�e�摜�̓ǂݍ���
	strcpyDx(shotMoto.path, ".\\Images\\Image\\dia_pink.png");

	if (ImageLoadDivMem(&shotMoto.handle[0], shotMoto.path, shotMoto.divX*shotMoto.divY, shotMoto.divX, shotMoto.divY)
		==FALSE)
	{
		return FALSE;
	}

	//���ƍ������擾
	GetGraphSize(shotMoto.handle[0], &shotMoto.imageCom.width, &shotMoto.imageCom.height);

	//�ʒu�̐ݒ�
	shotMoto.imageCom.x = GAME_WIDTH / 2 - shotMoto.imageCom.width/2;	//��������
	shotMoto.imageCom.y = GAME_HEIGHT/2 - shotMoto.imageCom.height/2;	//��ʉ�
	
	//���x
	shotMoto.speed = 5;

	//�A�j���[�V������ς���X�s�[�h
	shotMoto.animeCntMax = 30;

	//�����蔻��̍X�V
	//CollUpdate(&shotMoto);
	CollCircleUpdate(&shotMoto);

	//�摜��\�����Ȃ�
	shotMoto.imageCom.isDraw = FALSE;

	//���ׂĂ̋��ɏ����R�s�[
	for (int i = 0; i < SHOT_MAX; i++) 
	{
		shotUse[i] = shotMoto;
	}

	//�v���C���[�摜��ǂݍ���
	if (ImageLoad(&player.img, ".\\Images\\Image\\player.png")
		== FALSE)
	{
		return FALSE;
	}
	player.img.x = GAME_WIDTH / 2 - player.img.width/2;
	player.img.y = GAME_HEIGHT / 2 - player.img.height/2;
	CollUpdate(&player,20,10,-20,-10);
	player.img.isDraw = TRUE;
	player.speed = 5;


	//�G�摜��ǂݍ���
	for (int i = 0; i < ENEMY_KIND; i++)
	{
		if (ImageLoad(&enemyMoto[i].img, enemyPath[i])
			== FALSE)
		{
			return FALSE;
		}
		enemyMoto[i].img.x = GAME_WIDTH / 2 - enemyMoto[i].img.width / 2;
		enemyMoto[i].img.y = -enemyMoto[i].img.height;
		CollUpdate(&enemyMoto[i]);
		enemyMoto[i].img.isDraw = FALSE;
		enemyMoto[i].speed = 1;
	}

	//�w�i�摜��ǂݍ���
	if (ImageLoad(&backImg[0], ".\\Images\\hoshi_rev.jpg")
		== FALSE)
	{
		return FALSE;
	}

	if (ImageLoad(&backImg[1], ".\\Images\\hoshi_rev2.jpg")
		== FALSE)
	{
		return FALSE;
	}

	//�w�i�摜�̏�����
	for (int i = 0; i < BACK_IMG_MAX; i++)
	{
		backImg[i].x = 0;
		backImg[i].isDraw = TRUE;
		backImg[i].speed = 1;
	}
	backImg[0].y = -backImg[0].height;
	backImg[1].y = 0;

	//�X�R�A�̏�����
	score = 0;


	//�����摜�̓ǂݍ���
	const char* exprosionPath = ".\\Images\\Image\\baku1.png";
	if (ImageLoadDivMem(&exprosion[0], exprosionPath,
		EXPROSION_ARRAY_MAX, EXPROSION_X_MAX, EXPROSION_Y_MAX)
		== FALSE) 
	{
		return FALSE;
	}

	return TRUE;
}

//�L�����N�^�[�̓ǂݍ���
BOOL CharacterLoad(CHARACTER* chara, const char* path)
{
	//�摜�̓ǂݍ���
	strcpyDx(chara->img.path, path);
	chara->img.handle = LoadGraph(chara->img.path);

	if (chara->img.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			chara->img.path,
			"�摜�ǂݍ��݃G���[",
			MB_OK
		);

		return FALSE;
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(chara->img.handle, &chara->img.width, &chara->img.height);

	return TRUE;
}

//�摜�̓ǂݍ���
BOOL ImageLoad(IMAGE* chara, const char* path)
{
	//�摜�̓ǂݍ���
	strcpyDx(chara->path, path);
	chara->handle = LoadGraph(chara->path);

	if (chara->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			chara->path,
			"�摜�ǂݍ��݃G���[",
			MB_OK
		);

		return FALSE;
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(chara->handle, &chara->width, &chara->height);

	return TRUE;
}

/// <summary>
/// �摜�𕪊����ă������ɓǂݍ���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="divX">�������鎞�̉��̐�</param>
/// <param name="divY">�������鎞�̏c�̐�</param>
/// <returns></returns>
BOOL ImageLoadDivMem(int* handle, const char* path,int arrayMax, int divX, int divY) 
{
	//�e�����摜��ǂݍ���
	int isShotLoad = -1;	//�摜��ǂݍ��߂���

	//�����摜�̑傫���擾�p
	int shotImageHandle = LoadGraph(path);

	//�摜�̓ǂݍ��݃G���[����
	if (shotImageHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,			//�{��
			"�e�摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	int shotWidth = -1;
	int shotHeight = -1;
	GetGraphSize(shotImageHandle, &shotWidth, &shotHeight);

	isShotLoad = LoadDivGraph(
		path,	//�摜�̃p�X
		arrayMax,				//��������
		divX, divY,	//���c�̕���
		shotWidth / divX, shotHeight / divY,
		handle
	);



	if (isShotLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,			//�{��
			"�e�����摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;
	}

	//�e�̉摜�̑傫���̖����͈ȏ�Ȃ̂ō폜
	DeleteGraph(shotImageHandle);

	return TRUE;
}

//���y�̓ǂݍ���
BOOL AudioLoad(AUDIO* audio,const char* path, int playType, int volume)
{
	//���y�̓ǂݍ���
	strcpyDx(audio->path, path);
	audio->handle = LoadSoundMem(audio->path);

	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),
			audio->path,
			"���y�ǂݍ��݃G���[",
			MB_OK
		);

		return FALSE;
	}

	audio->playType = playType;
	audio->volume = volume;

	return TRUE;
}

/// <summary>
/// �Q�[���f�[�^�̏�����
/// </summary>
VOID GameInit() 
{
	//�w�i�摜�̏�����
	for (int i = 0; i < BACK_IMG_MAX; i++)
	{
		backImg[i].x = 0;
		backImg[i].isDraw = TRUE;
		backImg[i].speed = 1;
	}
	backImg[0].y = -backImg[0].height;
	backImg[1].y = 0;

	//�X�R�A�̏�����
	score = 0;
}

/// <summary>
/// �V�[����؂�ւ���
/// </summary>
/// <param name="scene">�؂�ւ���̃V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;				//��ʐ؂�ւ�
	isFadeIn = FALSE;				//�t�F�[�h�C���ɂ��Ȃ�
	isFadeOut = TRUE;				//�t�F�[�h�A�E�g����	
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title() 
{
	//����
	TitleProc();
	//�`��
	TitleDraw();
}

//����
VOID TitleProc() 
{
	//�X�y�[�X�������ꂽ�Ƃ��ɋ��𔭎˂���
	

	//�Q�[����ʂɐ؂�ւ��
	if (KeyClick(KEY_INPUT_RETURN)) 
	{
		//���̃V�[���̏������������ōs���Ɗy
		GameInit();	//�Q�[���̏�����

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	return;
}

//�`��
VOID TitleDraw() 
{
	

	//�����̕`��
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
/// �e�̕`��
/// </summary>
/// <param name="shot">�e�̍\����</param>
VOID DrawShot(SHOT* shot) 
{
	if (shot->imageCom.isDraw) 
	{
		//�e�̕`��
		DrawGraph(shot->imageCom.x, shot->imageCom.y, shot->handle[shot->nowIndex], TRUE);

		if (shot->animeCnt < shot->animeCntMax)
		{
			shot->animeCnt++;
		}
		else
		{
			shot->animeCnt = 0;

			//�e�̓Y�������e�̕������̍ő�����������Ƃ�
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

//�v���C���
VOID Play() 
{
	//����
	PlayProc();
	//�`��
	PlayDraw();

	
}

//����
VOID PlayProc() 
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		return;
	}

	//�}�E�X���\���ɂ���
	SetMouseDispFlag(FALSE);
	//�}�E�X�̈ʒu�Ƀv���C���[��u��
	player.img.x = mouse.point.x - player.img.width / 2;
	player.img.y = mouse.point.y - player.img.height / 2;

	//�v���C���[�̓����蔻��̍X�V
	CollUpdate(&player, 20, 10, -20, -10);
	
	//�E�N���b�N�ŋ��𔭎˂���
	if (MouseDown(MOUSE_INPUT_LEFT) == TRUE && shotIntervalCnt == 0)
	{
		//�e�𔭎˂���
		for (int i = 0; i < SHOT_MAX; i++)
		{
			if (shotUse[i].imageCom.isDraw == FALSE) 
			{
				//�e�𔭎�
				Shot(&shotUse[i], 270.0f);

				break;
			}
		}

		//--�O�����ɔ���--��
		////�e�𔭎˂���
		//for (int i = 0; i < SHOT_MAX; i++)
		//{
		//	if (shotUse[i].imageCom.isDraw == FALSE) 
		//	{
		//		//�e�𔭎�
		//		Shot(&shotUse[i], 270.0f);

		//		break;
		//	}
		//}

		////�e�𔭎˂���
		//for (int i = 0; i < SHOT_MAX; i++)
		//{
		//	if (shotUse[i].imageCom.isDraw == FALSE)
		//	{
		//		//�e�𔭎�
		//		Shot(&shotUse[i], 240.0f);

		//		break;
		//	}
		//}

		////�e�𔭎˂���
		//for (int i = 0; i < SHOT_MAX; i++)
		//{
		//	if (shotUse[i].imageCom.isDraw == FALSE)
		//	{
		//		//�e�𔭎�
		//		Shot(&shotUse[i], 300.0f);

		//		break;
		//	}
		//}
		//--�O�����ɔ���--��


		////���ˏ�ɔ���
		//for (int deg = 0; deg < 180; deg += 10) {
		//	for (int i = 0; i<SHOT_MAX; i++) {
		//		if (shotUse[i].imageCom.isDraw == FALSE) {
		//			Shot(&shotUse[i], deg);

		//			break;
		//		}
		//	}
		//}
	}

	//�e�̔��ˑ҂�
	if (shotIntervalCnt != 0 && shotIntervalCnt < shotIntervalCntMax)
	{
		shotIntervalCnt++;
	}
	else
	{
		shotIntervalCnt = 0;
	}

	//�e���΂�
	for (int i = 0; i < SHOT_MAX; i++)
	{
		if (shotUse[i].imageCom.isDraw == TRUE)
		{
			//�e�̈ʒu���C��
			//���S�ʒu�{��΂��p�x����΂��������v�Z������
			shotUse[i].imageCom.x = shotUse[i].imageCom.startX+cos(shotUse[i].degree * DX_PI / 180.0f) * shotUse[i].radius;
			shotUse[i].imageCom.y = shotUse[i].imageCom.startY+sin(shotUse[i].degree * DX_PI / 180.0f) * shotUse[i].radius;

			//���̏�����ǉ�����ƒe����]����
			//shotUse[i].degree--;

			//���a�𑫂�
			shotUse[i].radius += shotUse[i].speed;

			//�e�̓����蔻����X�V
			//CollUpdate(&shotUse[i]);
			CollCircleUpdate(&shotUse[i]);

			//��ʊO�ɏo����`�悵�Ȃ�
			if (shotUse[i].imageCom.y + shotUse[i].imageCom.height<0
				|| shotUse[i].imageCom.y>GAME_HEIGHT
				|| shotUse[i].imageCom.x + shotUse[i].imageCom.width<0
				|| shotUse[i].imageCom.x>GAME_WIDTH)
			{
				shotUse[i].imageCom.isDraw = FALSE;
			}
		}
	}
	
	//�G�𐶐�����
	if (enemyInterval < enemyIntervalMax)
	{
		enemyInterval++;
	}
	else 
	{
		enemyInterval = 0;

		//�G�𐶐�����
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemyUse[i].img.isDraw == FALSE)
			{
				if (score < 1000)
				{
					enemyUse[i] = enemyMoto[0];
				}
				else if (score<2000) 
				{
					enemyUse[i] = enemyMoto[1];
				}
				else 
				{
					enemyUse[i] = enemyMoto[GetRand(ENEMY_KIND - 1)];
				}

				int div = 10;
				enemyUse[i].img.x = GetRand(div-1) * GAME_WIDTH / 4;
				enemyUse[i].img.y = -enemyUse[i].img.height;
				enemyUse[i].img.isDraw = TRUE;	//�`�悷��
				break;
			}
		}
	}
	
	
	//�G�̏���
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemyUse[i].img.isDraw == TRUE) 
		{
			//�ړ�������
			enemyUse[i].img.y += enemyUse[i].img.speed;

			//��ʊO�ɏo����`�悵�Ȃ�
			if (enemyUse[i].img.y > GAME_HEIGHT)
			{
				enemyUse[i].img.isDraw = FALSE;
				return;
			}

			//�����蔻��̍X�V
			CollUpdate(&enemyUse[i]);

			for (int si = 0; si < SHOT_MAX; si++)
			{
				if (shotUse[si].imageCom.isDraw == TRUE) 
				{
					//�����蔻��
					if (CheckCollRectToCircle(enemyUse[i].coll, shotUse[si].circle) == TRUE)
					{
						enemyUse[i].img.isDraw = FALSE;
						shotUse[si].imageCom.isDraw = FALSE;
						score += 100;
					}
				}
			}
			
		}
	}
	return;
}

/// <summary>
/// �e���΂�
/// </summary>
/// <param name="shot">�e�̍\����</param>
/// <param name="deg">�p�x</param>
VOID Shot(SHOT *shot,float deg) 
{
	shot->imageCom.isDraw = TRUE;

	//�e�̈ʒu�����߂�
	shot->imageCom.startX = player.img.x + player.img.width / 2 - shot->imageCom.width / 2;
	shot->imageCom.startY = player.img.y;

	shot->imageCom.x = player.img.x + player.img.width / 2 - shot->imageCom.width / 2;
	shot->imageCom.y = player.img.y;

	//�e�̊p�x�����߂�
	shot->degree = deg;
	shot->radius = 0.0f;

	//�e�̓����蔻����X�V
	//CollUpdate(shot);
	CollCircleUpdate(shot);

	shotIntervalCnt++;
}

//�`��
VOID PlayDraw() 
{
	//�w�i�̕`��
	for (int i = 0; i < BACK_IMG_MAX; i++) 
	{
		//�w�i�̕`��
		DrawGraph(backImg[i].x, backImg[i].y, backImg[i].handle, TRUE);

		//�w�i�摜����ʊO�ɏo���Ƃ���Ɉړ�
		if (backImg[i].y > GAME_HEIGHT)
		{
			backImg[i].y = -backImg[i].height;
		}

		backImg[i].y += backImg[i].speed;
	}

	//�v���C���[�̕`��
	if (player.img.isDraw) 
	{
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		
		//�����蔻��̕`��
		if (GAME_DEBUG)
		{
			DrawBox(
				player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE
			);
		}
	}

	//�G�̕`��
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemyUse[i].img.isDraw)
		{
			DrawGraph(enemyUse[i].img.x, enemyUse[i].img.y, enemyUse[i].img.handle, TRUE);
		}

		//�����蔻��̕`��
		if (GAME_DEBUG)
		{
			if (enemyUse[i].img.isDraw)
			{
				DrawBox(
					enemyUse[i].coll.left, enemyUse[i].coll.top, enemyUse[i].coll.right, enemyUse[i].coll.bottom,
					GetColor(255, 0, 0), FALSE
				);
			}
		}
	}

	//�e���o��
	for (int i = 0; i < SHOT_MAX; i++)
	{
		if (shotUse[i].imageCom.isDraw)
		{
			DrawShot(&shotUse[i]);
		}

		//�����蔻��̕`��
		if (GAME_DEBUG)
		{
			if (shotUse[i].imageCom.isDraw)
			{
				/*DrawBox(
					shotUse[i].coll.left, shotUse[i].coll.top, shotUse[i].coll.right, shotUse[i].coll.bottom,
					GetColor(255, 0, 0), FALSE
				);*/
				DrawCircle(shotUse[i].circle);
			}
		}
	}

	if (GAME_DEBUG)
	{
		const char* string = "Play";
		DrawString(0, 50, string, GetColor(0, 0, 0));

		//�X�R�A�̕`��
		int old = GetFontSize();	//�ȑO�̃t�H���g�̃T�C�Y���擾
		SetFontSize(40);
		DrawFormatString(0, 100, GetColor(255, 255, 255), "SCORE : %05d", score);
		SetFontSize(old);

		//�}�E�X�̈ʒu��`�悷��
		MouseDraw();
	}


	return;
}

//�G���h���
VOID End() 
{
	//����
	EndProc();
	//�`��
	EndDraw();
}

//����
VOID EndProc()
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	return;
}

//�`��
VOID EndDraw()
{
	if (GAME_DEBUG)
	{
		const char* string = "End";
		DrawString(0, 50, string, GetColor(0, 0, 0));
	}


	return;
}

//�؂�ւ����
VOID Change() 
{
	if (GAME_DEBUG)
	{
		DrawString(0, 100, "�ǂݍ���", GetColor(0, 0, 0));
	}

	//����
	ChangeProc();
	//�`��
	ChangeDraw();
}

//����
VOID ChangeProc()
{
	//�t�F�[�h�C��
	if (isFadeIn) 
	{
		if (fadeInCnt > fadeInCntMax) 
		{
			//�J�E���^�����炷
			fadeInCnt--;
		}
		else //�t�F�[�h�C���������I�������Ƃ�
		{
			//�J�E���^��������
			fadeInCnt = fadeInCntInit;
			isFadeIn = FALSE;
		}
	}

	//�t�F�[�h�A�E�g
	if (isFadeOut) 
	{
		if (fadeOutCnt < fadeOutCntMax) 
		{
		//�J�E���^�𑝂₷
			fadeOutCnt++;
		}
		else 
		{
		//�J�E���^������
			fadeOutCnt = fadeOutCntInit;;
			isFadeOut = FALSE;
		}
	}

	//�؂�ւ������I����
	if (isFadeIn == FALSE && isFadeOut == FALSE) 
	{
	//�t�F�[�h�C�����Ă��Ȃ��A���t�F�[�h�A�E�g�����Ă��Ȃ�
		//���̃V�[���ɐ؂�ւ�
		GameScene = GameSceneNext;
		//�ȑO�̃V�[���̍X�V
		GameSceneOld = GameScene;
	}

	return;
}

//�`��
VOID ChangeDraw()
{
	switch (GameSceneOld)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C�@��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h�@��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (isFadeIn) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (isFadeOut) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);
	
	//�l�p��`��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (GAME_DEBUG)
	{
		DrawString(0, 0, "�؂�ւ�", GetColor(0, 0, 0));
	}

	return;
}



/// <summary>
/// �����蔻��̗̈�X�V
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
/// �����蔻��̗̈�X�V
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
/// �\����SHOT�̉~�̈擖���蔻����X�V
/// </summary>
/// <param name="shot">SHOT�\����</param>
VOID CollCircleUpdate(SHOT* shot) 
{
	//��`�̔����̒��������߂�
	int wh = shot->imageCom.width / 2;
	int hh = shot->imageCom.height / 2;

	//���S�_�����߂�
	shot->circle.center.x = shot->imageCom.x + wh;
	shot->circle.center.y = shot->imageCom.y + hh;

	//���a�����߂�
	shot->circle.radius = sqrtf(wh * wh + hh * hh);

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V(�l���̃I�[�o�[���[�h)
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

//�����蔻��(Enter)
BOOL CollStay(CHARACTER chara1, CHARACTER chara2)
{
	//�`�悳��Ă��Ȃ��Ȃ�s��Ȃ�
	if (chara1.img.isDraw && chara2.img.isDraw)
	{
		//�����蔻��
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

//�����蔻��(Enter)
BOOL CollStay(RECT rect1, RECT rect2)
{

	//�����蔻��
	if (
		rect1.left<rect2.right
		&& rect1.right>rect2.left
		&& rect1.top<rect2.bottom
		&& rect1.bottom>rect2.top
		)
	{
		return TRUE;
	}



	return FALSE;
}