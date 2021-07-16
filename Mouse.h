#pragma once
//�}�E�X�����̃w�b�_�t�@�C��
//########## �w�b�_�[�t�@�C���ǂݍ��� ##########
#include "Game.h"
#include "Shape.h"

//########## �}�N����` ##########
#define MOUSE_BUTTON_CODE 129

//########## �\���� ##########
struct MOUSE 
{
	int inputValue = 0;	//GetMouseInput�̒l
	int wheelValue = 0;	//�}�E�X�z�C�[���̉�]��(���{�@��O�|)
	I_POINT point;
	I_POINT oldPoint;
	int oldButton[MOUSE_BUTTON_CODE];
	int button[MOUSE_BUTTON_CODE];
};

//########## �O���̃O���[�o���ϐ� ##########
extern MOUSE mouse;	//�}�E�X���͂��擾

//########## �v���g�^�C�v�錾 ##########
VOID MouseUpdate(VOID);								//�}�E�X�̓��͏����X�V����
void MyMouseCheack(MOUSE* mouse, int MOUSE_INPUT_);	//�}�E�X�R�[�h�ɑΉ������{�^����������Ă��邩�`�F�b�N
BOOL MouseDown(int MOUSE_INPUT_);					//�{�^���������Ă��邩
BOOL MouseUp(int MOUSE_INPUT_);						//�{�^���������グ����
BOOL MouseDownKeep(int MOUSE_INPUT_,int milliTime);	//�{�^�������������Ă��邩
BOOL MouseChick(int MOUSE_INPUT_);					//�}�E�X���N���b�N������
VOID MouseDraw(VOID);								//�}�E�X�̏���`�悷��
VOID MouseDraw(int x,int y);						//�ʒu���w�肵�ă}�E�X�̏���`�悷��
BOOL MouseRectChick(RECT rect,int MOUSE_INPUT_);	//�}�E�X����`�̈���N���b�N������
BOOL MouseCircleChick(CIRCLE circle,int MOUSE_INPUT_);//�}�E�X���~�̈���N���b�N������
