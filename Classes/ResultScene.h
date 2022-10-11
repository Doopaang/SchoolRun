#ifndef __SONGSELECT_SCENE_H__
#define __SONGSLECET_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

const std::string UTF8(const std::string& string);

class ResultScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(ResultScene);

	// 키보드 누름 함수
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;

private:
	Size visibleSize;
	Vec2 origin;

	// 커서 변수
	Sprite* songselectSelect;
	Sprite* restartSelect;

	// 노래 이름 변수
	std::string name;

	// 들어온 시간 변수
	float score;

	// 넘어진 횟수 변수
	int fall;

	// 최대 콤보 변수
	int combo;

	// 메뉴 변수
	int menu;

	// 배경 화면 생성 함수
	void createBackground();

	// 메뉴 생성 함수
	void createMenu();

	// 결과 생성 함수
	void createResult();

	// 곡 정보 생성 함수
	void createInfo();

	// 클리어 여부 생성 함수
	void createClear();

	// 화면 전환 함수
	void changeScene();

	// 커서 설정 함수
	void setCursor();

	// 데이터 저장 함수
	void saveData();

	// 유저 데이터 저장 함수
	void saveUserData();

	// 이미지 너비 상수
	const int IMAGE_LENGTH = 310;

	// 메뉴 상수
	enum MENU
	{
		RESTART, SONGSELECT
	};

	// Z오더 상수
	enum ZORDER
	{
		IMAGE, BACKGROUND, OBJECT, OBJECT_UP
	};

public:
	// 생성자
	ResultScene();
};

#endif
