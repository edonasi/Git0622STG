//�}�E�X�����̃\�[�X�t�@�C��

//########## �w�b�_�[�t�@�C���ǂݍ��� ##########
#include "Game.h"
#include "Mouse.h"
#include "Fps.h"

//########## �O���[�o���ϐ� ##########

//�}�E�X���͂��擾
MOUSE mouse;

//########## �֐� ##########

//�}�E�X�̓��͏����X�V����
VOID MouseUpdate(VOID)
{
//�}�E�X�̈ȑO�̏����擾
	mouse.oldPoint = mouse.point;

	//�}�E�X�̈ȑO�̃{�^�����͂��擾
	mouse.oldButton[MOUSE_INPUT_LEFT] = mouse.button[MOUSE_INPUT_LEFT];
	mouse.oldButton[MOUSE_INPUT_MIDDLE] = mouse.button[MOUSE_INPUT_MIDDLE];
	mouse.oldButton[MOUSE_INPUT_RIGHT] = mouse.button[MOUSE_INPUT_RIGHT];

	//�}�E�X�̍��W���擾
	GetMousePoint(&mouse.point.x, &mouse.point.y);

	//�}�E�X�̍��W����ʊO�̏ꍇ�́A��ʓ��Ɏ��߂�
	if (mouse.point.x < 0) { mouse.point.x = 0; }
	if (mouse.point.x > GAME_WIDTH) { mouse.point.x = GAME_WIDTH; }
	if (mouse.point.y < 0) { mouse.point.y = 0; }
	if (mouse.point.y > GAME_HEIGHT) { mouse.point.y = GAME_HEIGHT; }

	//�}�E�X�̐����Ă���{�^�����擾
	mouse.inputValue = GetMouseInput();

	//�}�E�X�R�[�h�ɑΉ������{�^����������Ă��邩�`�F�b�N
	MyMouseCheack(&mouse, MOUSE_INPUT_LEFT);	//��
	MyMouseCheack(&mouse, MOUSE_INPUT_MIDDLE);	//��
	MyMouseCheack(&mouse, MOUSE_INPUT_RIGHT);	//�E
	
	//�z�C�[���̉�]�ʂ��擾
	mouse.wheelValue = GetMouseWheelRotVol();

	return;
}

/// <summary>
/// �}�E�X�R�[�h�ɑΉ������{�^����������Ă��邩�`�F�b�N
/// </summary>
/// <param name="mouse">���肷��}�E�X�|�C���^</param>
/// <param name="MOUSE_INPUT_">�}�E�X�R�[�h</param>
void MyMouseCheack(MOUSE* mouse,int MOUSE_INPUT_) 
{
	//�����Ă���Ƃ�
	if ((mouse->inputValue &  MOUSE_INPUT_) ==  MOUSE_INPUT_)
	{
		mouse->button[ MOUSE_INPUT_]++;
	}
	//�����Ă��Ȃ��Ƃ�
	if ((mouse->inputValue &  MOUSE_INPUT_) !=  MOUSE_INPUT_)
	{
		mouse->button[ MOUSE_INPUT_] = 0;
	}
}

/// <summary>
/// �{�^���������Ă��邩
/// </summary>
/// <param name="MOUSE_INPUT_">�}�E�X�R�[�h</param>
/// <returns>�{�^���������Ă���Ȃ�TRUE</returns>
BOOL MouseDown(int MOUSE_INPUT_) 
{
	//TRUE�Ȃ�{�^����������Ă���
	return (mouse.button[MOUSE_INPUT_] != 0) ? TRUE : FALSE;
}

/// <summary>
/// �{�^���������グ����
/// </summary>
/// <param name="MOUSE_INPUT_">�}�E�X�R�[�h</param>
/// <returns>�{�^�������������Ă���Ȃ�TRUE</returns>
BOOL MouseUp(int MOUSE_INPUT_) 
{
	if (mouse.oldButton[MOUSE_INPUT_] >= 1	//���O�͉����Ă���
		&& mouse.button[MOUSE_INPUT_] == 0)	//���͉����Ă��Ȃ��Ƃ�
	{
		return TRUE;	//�����グ�Ă���
	}
	else 
	{
		return FALSE;	//�����グ�Ă��Ȃ�
	}
}

/// <summary>
/// �{�^�������������Ă��邩
/// </summary>
/// <param name="MOUSE_INPUT_">�}�E�X�R�[�h</param>
/// <param name="milliTime">�{�^�������������Ă��鎞��(�~���b)</param>
/// <returns>���������Ă�����TRUE</returns>
BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime) 
{
	float millSec = 1000.0f;	//1�b��1000�~���b

	//���������鎞��=�b���~FPS�l
	//��j60FPS�̃Q�[���ŁA1�b�ԉ���������Ȃ�A1�b�~60FPS
	int updateTime = (milliTime / millSec) * fps.Value;

	//���������Ă�����TRUE
	return (mouse.button[MOUSE_INPUT_] > updateTime) ? TRUE : FALSE;
}	

/// <summary>
/// �}�E�X���N���b�N������
/// </summary>
/// <param name="MOUSE_INPUT_">�}�E�X�R�[�h</param>
/// <returns>�N���b�N������TRUE</returns>
BOOL MouseChick(int MOUSE_INPUT_) 
{
	if (mouse.oldButton[MOUSE_INPUT_] >= 1	//���O�͉����Ă���
		&& mouse.button[MOUSE_INPUT_] == 0)	//���͉����Ă��Ȃ��Ƃ�
	{
		return TRUE;	//�����グ�Ă���
	}
	else
	{
		return FALSE;	//�����グ�Ă��Ȃ�
	}
}

//�}�E�X�̏���`�悷��
VOID MouseDraw(VOID) 
{
	if (GAME_DEBUG == TRUE) 
	{
	//�}�E�X�̍��W��`��
		DrawFormatString(
			0, GAME_HEIGHT - 40,
			GetColor(255, 255, 255), "MOUSE[X:%4d/Y:%4d]",
			mouse.point.x, mouse.point.y
		);
	}
	return;
}

/// <summary>
/// �ʒu���w�肵�ă}�E�X�̏���`�悷��
/// </summary>
/// <param name="x">x���W</param>
/// <param name="y">y���W</param>
/// <returns></returns>
VOID MouseDraw(int x, int y) 
{
	if (GAME_DEBUG == TRUE)
	{
		//�}�E�X�̍��W��`��
		DrawFormatString(
			x, y,
			GetColor(255, 255, 255), "MOUSE[X:%4d/Y:%4d]",
			mouse.point.x, mouse.point.y
		);
	}
	return;
}


/// <summary>
/// �}�E�X����`�̈���N���b�N������
/// </summary>
/// <param name="rect">��`�̈�</param>
/// <param name="MOUSE_INPUT_">�}�E�X�R�[�h</param>
/// <returns>�N���b�N������TRUE</returns>
BOOL MouseRectChick(RECT rect, int MOUSE_INPUT_) 
{
	//�_�Ǝl�p�̓����蔻��
	if (CheckColliPointToRect(mouse.point,rect) == TRUE) 
	{
		//�}�E�X���N���b�N���Ă��邩
		return MouseChick(MOUSE_INPUT_);
	}

	return FALSE;	//�N���b�N���Ă��Ȃ�
}

/// <summary>
/// �}�E�X���~�̈���N���b�N������
/// </summary>
/// <param name="circle">�~�̈�</param>
/// <param name="MOUSE_INPUT_">�}�E�X�R�[�h</param>
/// <returns>�N���b�N������TRUE</returns>
BOOL MouseCircleChick(CIRCLE circle, int MOUSE_INPUT_) 
{
	//�_�Ɖ~�̓����蔻��
	if (CheckColliPointToCircle(mouse.point, circle) == TRUE) 
	{
		//�}�E�X���N���b�N���Ă��邩
		return MouseChick(MOUSE_INPUT_);
	}

	return FALSE;	//�N���b�N���Ă��Ȃ�
}