#ifndef __MANUAL_SCENE_H__
#define __MANUAL_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

// 페이지 상수
enum PAGE
{
	HOW2PLAY, NOTES_1, NOTES_2, NOTES_3, NOTES_4, NOTES_5, CREDITS
};

class Character;

class ManualScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(ManualScene);

	// 키보드 누름 함수
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)override;

private:
	Size visibleSize;
	Vec2 origin;

	// 타이틀 라벨 변수
	Label* titleLabel;

	// 페이지 라벨 변수
	Label* pageLabel;

	// 스페이스 바 변수
	MenuItemSprite* spaceBar;

	// ESC 변수
	MenuItemSprite* esc;

	// 왼쪽 방향키 변수
	MenuItemSprite* left;

	// 위쪽 방향키 변수
	MenuItemSprite* up;

	// 오른쪽 방향키 변수
	MenuItemSprite* right;

	// 아래쪽 방향키 변수
	MenuItemSprite* down;

	// 캐릭터 변수
	Character* character;

	// 캐릭터 스프라이트 변수
	Sprite* characterSprite;

	// 그림자 스프라이트 변수
	Sprite* shadowSprite;

	// 페이지 관리 변수
	Layer* pages[CREDITS + 1];

	// 페이지 변수
	int page;

	// 왼쪽 누름 변수
	bool leftPress;

	// 오른쪽 누름 변수
	bool rightPress;

	// 아래쪽 누름 변수
	bool downPress;

	// fps 변수
	float fps;

	// 프레임 속도 변수
	float frameSpeed;

	// 업데이트 함수
	void update(float delta);

	// 바탕 화면 생성 함수
	void createBackground();

	// 타이틀 라벨 생성 함수
	void createTitleLabel();

	// 페이지 라벨 생성 함수
	void createPageLabel();

	// 인터페이스 버튼 생성 함수
	void createInterfaceButton();

	// 키 설명 레이어 생성 함수
	void createHow2Play();

	// 노트 설명 1 레이어 생성 함수
	void createNote1();

	// 노트 설명 2 레이어 생성 함수
	void createNote2();

	// 노트 설명 3 레이어 생성 함수
	void createNote3();

	// 노트 설명 4 레이어 생성 함수
	void createNote4();

	// 노트 설명 마지막 레이어 생성 함수
	void createNote5();

	// 크레딧 레이어 생성 함수
	void createCredits();

	// 타이틀 라벨 내용 반환 함수
	char* getTitleLabel();

	// 페이지 설정 함수
	void setPage();

	// 속도 설정 함수
	void setSpeed();

	// 기본 속도 상수
	const float DEFAULT = 1.0f;

	// 느린 속도 상수
	const float SLOW = 1.5f;

	// 빠른 속도 상수
	const float FAST = 0.8f;

	// BPM 상수
	const int BPM = 2;

public:
	// 생성자
	ManualScene();

	// 소멸자
	~ManualScene();
};

#endif
