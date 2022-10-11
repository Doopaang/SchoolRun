#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class MenuLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(MenuLayer);
	
	// 메뉴 변수
	int menu;

	// 레이어 활성화 설정 함수
	void setLayerVisible();

	// 메뉴 설정 함수
	void setMenu(bool _left);

	// 메뉴 상수
	enum MENU
	{
		RESTART, SONGSELECT, HOME
	};

private:
	Size visibleSize;
	Vec2 origin;

	// 스프라이트 관리 변수
	Sprite* on[HOME + 1];

	// 배경 컬러 레이어 생성 함수
	void createBackground();
	
	// 메뉴 생성 함수
	void createMenu();

	// 메뉴 커서 설정 함수
	void setMenuCursor(int _pastMenu);

	// z오더 상수
	enum ZORDER
	{
		OFF, ON
	};

public:
	// 생성자
	MenuLayer();
};

#endif
