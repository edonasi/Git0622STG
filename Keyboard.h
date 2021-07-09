#pragma once

//�w�b�_�t�@�C���̓ǂݍ���
#include "Game.h"

//�擾����L�[�̎��
#define KEY_KIND_MAX 256

//�L�[�{�[�h�\����
struct KEYBOARD
{
	//���͏�Ԃ��Ǘ�
	char tmpKeyState[KEY_KIND_MAX];
	//���ׂẴL�[�̓��͎��Ԃ��Ǘ�
	int allKeyState[KEY_KIND_MAX];
	//�ȑO�̂��ׂẴL�[�̓��͎��Ԃ��Ǘ�
	int oldKeyState[KEY_KIND_MAX];
};

//�O���̃O���[�o���ϐ�(�錾���Ă���̂ł͂Ȃ�)
extern KEYBOARD keyboard;

//----- �v���g�^�C�v�錾 ------
//���ׂẴL�[�̓��͏�Ԃ��擾����
VOID AllKeyUpdate(VOID);
//����̃L�[�������Ă��邩
BOOL KeyDown(int keyInput);
//����̃L�[���������u��
BOOL Key1Down(int keyInput);
//����̃L�[���グ�Ă��邩
BOOL KeyUp(int keyInput);
BOOL KeyUpping(int keyInput);
//����̃L�[���N���b�N������
BOOL KeyClick(int keyInput);
//����̃L�[���w��̃~���b�����������Ă��邩
BOOL KeyDownKeep(int keyInput, int millSec);