//Copyright (c) 2020
//Hamamatsu Mirai Sougou Senmongakkou GameCreate Y.Suzuki
//All rights reserved.
//�}�`�̃\�[�X�t�@�C��

//########## �w�b�_�[�t�@�C���ǂݍ��� ##########
#include "Game.h"
#include "Shape.h"
#include <math.h>
using namespace std;

//########## �֐� ##########

/// <summary>
/// �l�p�̈�̓����蔻����`�F�b�N����
/// </summary>
/// <param name="a">�̈�A</param>
/// <param name="b">�̈�B</param>
/// <returns>�������Ă���Ȃ�TRUE</returns>
BOOL CheckCollRectToRect(RECT a, RECT b) 
{
	if (a.left<b.right && a.top<b.bottom
		&& a.right>b.left && a.bottom>b.top)
	{
		return TRUE;	//�������Ă���
	}

	return FALSE;		//�������Ă��Ȃ�
}

/// <summary>
/// �_�Ǝl�p�̓����蔻��
/// </summary>
/// <param name="pt">I_POINT�^�̓_</param>
/// <param name="r">��`�̈�</param>
/// <returns>�������Ă���Ȃ�TRUE</returns>
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
/// �_�Ɖ~�̓����蔻��
/// </summary>
/// <param name="pt">I_POINT�^�̓_</param>
/// <param name="c">�~�̈�</param>
/// <returns>�������Ă���Ȃ�TRUE</returns>
BOOL CheckColliPointToCircle(I_POINT pt, CIRCLE c) 
{
	float bm = pt.x - c.center.x;		//�O�p�`�̒�ӂ̒��������߂�
	float ht = pt.y - c.center.y;		//�O�p�`�̍����̒��������߂�
	float sq = sqrtf(bm * bm + ht * ht);//�O�p�`�̎Εӂ̒��������߂�(sqrt=������)

	//�Εӂ̒������A�~�̔��a�����Z���Ȃ�΁A�~�̒��ɂ���
	return (sq <= c.radius) ? TRUE : FALSE;
}

/// <summary>
/// �~�Ɖ~�̓����蔻��
/// </summary>
/// <param name="a">�~�̈�1</param>
/// <param name="b">�~�̈�2</param>
/// <returns>�������Ă���Ȃ�TRUE</returns>
BOOL CheckCollCircleToCircle(CIRCLE a, CIRCLE b) 
{
	float bm = a.center.x - b.center.x;		//�O�p�`�̒�ӂ̒��������߂�
	float ht = a.center.y - b.center.y;		//�O�p�`�̍����̒��������߂�
	float sq = sqrtf(bm * bm + ht * ht);//�O�p�`�̎Εӂ̒��������߂�(sqrt=������)

	//�Εӂ̒������A�~�̔��a�����Z���Ȃ�΁A�~�̒��ɂ���
	return (sq <= a.radius + b.radius) ? TRUE : FALSE;
}

/// <summary>
/// RECT�^���ꎞ�I�ɓn��
/// </summary>
/// <param name="left">��</param>
/// <param name="top">��</param>
/// <param name="right">�E</param>
/// <param name="bottom">��</param>
/// <returns>RECT�^</returns>
RECT GetRect(int left, int top, int right, int bottom) 
{
	//�ꎞ�I��RECT�^�̕ϐ�������ĕԂ�
	RECT rect = { left,top,right,bottom };
	return rect;
}

/// <summary>
/// RECT�𗘗p���Ďl�p��`��
/// </summary>
/// <param name="r">RECT�\����</param>
/// <param name="color">�`��F</param>
/// <param name="b">�h��Ԃ��Ȃ�TRUE</param>
VOID DrawRect(RECT r, unsigned int color, bool b) 
{
	//���������ɕ`��
	DrawBox(r.left, r.top, r.right, r.bottom, color, b);
	return;
}

/// <summary>
/// �l�p��`��(��,�h��Ԃ��Ȃ�)
/// </summary>
/// <param name="r">RECT�\����</param>
VOID DrawRect(RECT r) 
{
	//���������ɕ`��
	DrawBox(r.left, r.top, r.right, r.bottom, GetColor(255,255,255), FALSE);
	return;
}

/// <summary>
/// CIRCLE�𗘗p���ĉ~��`��
/// </summary>
/// <param name="c">CIRCLE�\����</param>
/// <param name="color">�`��F</param>
/// <param name="b">�h��Ԃ��Ȃ�TRUE</param>
/// <param name="thick">���̑���</param>
VOID DrawCircle(CIRCLE c, unsigned int color, bool b, int thick) 
{
	//���������ɕ`��
	DrawCircle(c.center.x, c.center.y, c.radius, color, b, thick);
	return;
}

/// <summary>
/// �~��`��(��,�h��Ԃ��Ȃ�,��1)
/// </summary>
/// <param name="c">CIRCLE�\����</param>
VOID DrawCircle(CIRCLE c) 
{
	//���������ɕ`��
	DrawCircle(c.center.x, c.center.y, c.radius, 
		GetColor(255,255,255), FALSE, 1);
	return;
}


