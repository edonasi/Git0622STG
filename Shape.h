#pragma once
//�}�`�̃w�b�_�t�@�C��
//########## �w�b�_�[�t�@�C���ǂݍ��� ##########
#include "Game.h"

//########## �\���� ##########
//int�^��POINT�\����
struct I_POINT
{	
	//���W��������
	int x = - 1;
	int y = -1;
};

//�~�̍\����
struct CIRCLE
{
	I_POINT center;	//���S�_
	int radius;		//���a
};

BOOL CheckCollRectToRect(RECT a, RECT b);			//�l�p�̈�̓����蔻����`�F�b�N����
BOOL CheckColliPointToRect(I_POINT pt, RECT r);		//�_�Ǝl�p�̓����蔻��
BOOL CheckColliPointToCircle(I_POINT pt, CIRCLE c);	//�_�Ɖ~�̓����蔻��
BOOL CheckCollRectToCircle(RECT r, CIRCLE c);
BOOL CheckCollCircleToCircle(CIRCLE a, CIRCLE b);	//�~�Ɖ~�̓����蔻��

RECT GetRect(int left, int top, int right, int bottom);	//RECT�^���ꎞ�I�ɓn��
VOID DrawRect(RECT r, unsigned int color, bool b);		//RECT�𗘗p���Ďl�p��`��
VOID DrawRect(RECT r);									//�l�p��`��(��,�h��Ԃ��Ȃ�)
VOID DrawCircle(CIRCLE c, unsigned int color, bool b, int thick = 1);	//CIRCLE�𗘗p���ĉ~��`��
VOID DrawCircle(CIRCLE c);								//�~��`��(��,�h��Ԃ��Ȃ�,��1)



