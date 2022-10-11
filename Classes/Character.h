#ifndef __CHARACTER_INFO_H__
#define __CHARACTER_INFO_H__

#include "cocos2d.h"

USING_NS_CC;

class Character
{
public:
	// 캐릭터 상태 변수
	int state;

	// 액션 함수
	Sprite* action(float* _fps, const float _frameSpeed, int _state);

	// 점프 알림 함수
	void jumpArim(bool _downPress, const float _frameSpeed);

	// 슬라이드 알림 함수
	void slideArim(bool _downPress, const float _frameSpeed);

	// 일어남 알림 함수
	void standArim();

	// 그리고 시간은 흐른다
	void resumeCharacter();

	// 프레임 초기화 함수
	void resetFrame();

	// 엔딩 함수
	bool endRun(const int _frameSpeed, int _BPM);

	// 액션 속도 설정 함수
	void setActionSpeed(const float _frameSpeed, int _BPM);

	// 그림자 반환 함수
	Sprite* getShadow(float _fps, const float _frameSpeed, int _state);

	// 캐릭터 상태 상수
	enum STATE
	{
		DEFAULT, S_ING, S_ED, J_SINGLE, J_DOUBLE, F_ING, F_ED
	};

private:
	Size visibleSize;
	Vec2 origin;

	// 캐릭터 스프라이트 변수
	Sprite* character;

	// 그림자 스프라이트 변수
	Sprite* shadow;

	// 애니메이션 프레임 변수
	int frame;

	// 점프 상태 변수
	bool yoyo;

	// 돌아오는 액션 변수
	MoveTo* comeback;

	// 시퀀스 액션 변수
	Sequence* jumping;

	// 달리기 애니메이션 함수
	void animationRun(const int _FRAMES);

	// 슬라이드 애니메이션 함수
	void animationSlide(const int _FRAMES, bool _ing);

	// 점프 애니메이션 함수
	void animationJump(const int _FRAMES);

	// 넘어짐/일어남 애니메이션 함수
	void animationFallAndStand(const int _FRAMES);

	// 애니메이션 실행 함수
	void animation(Sprite* _target, const int _FRAME_START);

	// 액션 초기화 함수
	void resetAction();

	// 점프 종료 콜백 함수
	void jumpEndCallback();

	// 프레임 관련 상수
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

	// 점프 시간 상수
	const float JUMP_TIME = 0.9f;

	// 점프 가속도 상수
	const float EASE = 4.0f;

	// 점프 높이 상수
	const int JUMP_HEIGHT = 200;

	// FPS 관련 상수
	const int RUN_FPS = 10;
	const int JUMP_FPS = 7;
	const int SLIDE_FPS = 8;
	const int SLIDED_FPS = 8;
	const int FALL_FPS = 9;

	// 그림자 상수
	const int IS_SHADOW = 100;

public:
	// 생성자
	Character(Vec2 _position);

	// 소멸자
	~Character();
};

#endif
