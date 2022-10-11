#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

const std::string UTF8(const std::string& string);

// 메뉴 종류 상수
enum MENU
{
	START, MANUAL, EXIT
};

class MainScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(MainScene);

	// 가능 변수
	bool can;

	// 키보드 누름 함수
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;

	// 화면 전환 함수
	void changeScene();

private:
	Size visibleSize;
	Vec2 origin;

	// 메뉴 관리 변수
	Sprite* menus[EXIT + 1];

	// 메뉴 상태 변수
	int menu;

	// 애니메이션 콜백 함수
	void animationCallback(Ref* sender);

	// 페인트1 콜백 함수
	void playPaint1Callback();

	// 페인트2 콜백 함수
	void playPaint2Callback();

	// 배경 화면 생성 함수
	void createBackground();

	// 버튼 생성 함수
	void createButton();

	// 애니메이션 실행 함수
	void createAnimation();

	// 메뉴 설정 함수
	void setMenu(bool _isUp);

	// 버튼 높이 상수 70
	const int button_height = 70;

public:
	// 생성자
	MainScene();
};

#endif
