#ifndef __CHARACTER_INFO_H__
#define __CHARACTER_INFO_H__

#include "cocos2d.h"

USING_NS_CC;

class Character
{
public:
	// ĳ���� ���� ����
	int state;

	// �׼� �Լ�
	Sprite* action(float* _fps, const float _frameSpeed, int _state);

	// ���� �˸� �Լ�
	void jumpArim(bool _downPress, const float _frameSpeed);

	// �����̵� �˸� �Լ�
	void slideArim(bool _downPress, const float _frameSpeed);

	// �Ͼ �˸� �Լ�
	void standArim();

	// �׸��� �ð��� �帥��
	void resumeCharacter();

	// ������ �ʱ�ȭ �Լ�
	void resetFrame();

	// ���� �Լ�
	bool endRun(const int _frameSpeed, int _BPM);

	// �׼� �ӵ� ���� �Լ�
	void setActionSpeed(const float _frameSpeed, int _BPM);

	// �׸��� ��ȯ �Լ�
	Sprite* getShadow(float _fps, const float _frameSpeed, int _state);

	// ĳ���� ���� ���
	enum STATE
	{
		DEFAULT, S_ING, S_ED, J_SINGLE, J_DOUBLE, F_ING, F_ED
	};

private:
	Size visibleSize;
	Vec2 origin;

	// ĳ���� ��������Ʈ ����
	Sprite* character;

	// �׸��� ��������Ʈ ����
	Sprite* shadow;

	// �ִϸ��̼� ������ ����
	int frame;

	// ���� ���� ����
	bool yoyo;

	// ���ƿ��� �׼� ����
	MoveTo* comeback;

	// ������ �׼� ����
	Sequence* jumping;

	// �޸��� �ִϸ��̼� �Լ�
	void animationRun(const int _FRAMES);

	// �����̵� �ִϸ��̼� �Լ�
	void animationSlide(const int _FRAMES, bool _ing);

	// ���� �ִϸ��̼� �Լ�
	void animationJump(const int _FRAMES);

	// �Ѿ���/�Ͼ �ִϸ��̼� �Լ�
	void animationFallAndStand(const int _FRAMES);

	// �ִϸ��̼� ���� �Լ�
	void animation(Sprite* _target, const int _FRAME_START);

	// �׼� �ʱ�ȭ �Լ�
	void resetAction();

	// ���� ���� �ݹ� �Լ�
	void jumpEndCallback();

	// ������ ���� ���
	const int RUN_FRAME_START = 1;
	const int RUN_FRAMES = 6;
	const int JUMP_FRAME_START = 7;
	const int JUMP_FRAMES = 5;
	const int SLIDE_FRAME_START = 12;
	const int SLIDE_FRAMES = 2;
	const int SLIDED_FRAME_START = 14;
	const int SLIDED_FRAMES = 1;
	const int FALL_FRAME_START = 16;
	const int FALL_FRAMES = 4;
	const int STAND_FRAME_START = 20;
	const int STAND_FRAMES = 2;

	// ���� �ð� ���
	const float JUMP_TIME = 0.9f;

	// ���� ���ӵ� ���
	const float EASE = 4.0f;

	// ���� ���� ���
	const int JUMP_HEIGHT = 200;

	// FPS ���� ���
	const int RUN_FPS = 10;
	const int JUMP_FPS = 7;
	const int SLIDE_FPS = 8;
	const int SLIDED_FPS = 8;
	const int FALL_FPS = 9;

	// �׸��� ���
	const int IS_SHADOW = 100;

public:
	// ������
	Character(Vec2 _position);

	// �Ҹ���
	~Character();
};

#endif
