#include "MenuLayer.h"

MenuLayer::MenuLayer() :menu(RESTART)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 파일 불러오기
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites/menu_sprites.plist");
}

bool MenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// 배경 컬러 레이어 생성
	createBackground();

	// 메뉴 생성
	createMenu();

	// 레이어 숨기기
	setVisible(false);

    return true;
}

void MenuLayer::createBackground()
{
	auto layer = LayerColor::create(Color4B(0, 0, 0, 200));
	this->addChild(layer, -1);
}

void MenuLayer::createMenu()
{
	// 재시작 메뉴 생성
	auto restart = Sprite::createWithSpriteFrameName("restart.png");
	restart->setPosition(origin.x + visibleSize.width * 0.25, origin.y + visibleSize.height * 0.5);
	this->addChild(restart, OFF);

	auto restartOn = Sprite::createWithSpriteFrameName("restart_on.png");
	restartOn->setPosition(restart->getPosition());
	restartOn->setOpacity(255);
	this->addChild(restartOn, ON);
	on[RESTART] = restartOn;

	// 곡 선택 메뉴 생성
	auto songselect = Sprite::createWithSpriteFrameName("songselect.png");
	songselect->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5);
	this->addChild(songselect, OFF);

	auto songselectOn = Sprite::createWithSpriteFrameName("songselect_on.png");
	songselectOn->setPosition(songselect->getPosition());
	songselectOn->setOpacity(0);
	this->addChild(songselectOn, ON);
	on[SONGSELECT] = songselectOn;

	// 메인 화면 메뉴 생성
	auto home = Sprite::createWithSpriteFrameName("home.png");
	home->setPosition(origin.x + visibleSize.width * 0.75, origin.y + visibleSize.height * 0.5);
	this->addChild(home, OFF);

	auto homeOn = Sprite::createWithSpriteFrameName("home_on.png");
	homeOn->setPosition(home->getPosition());
	homeOn->setOpacity(0);
	this->addChild(homeOn, ON);
	on[HOME] = homeOn;
}

void MenuLayer::setLayerVisible()
{
	if (this->isVisible())
	{
		// 레이어 숨기기
		this->setVisible(false);
	}
	else
	{
		// 레이어 보이기
		this->setVisible(true);
	}
}

void MenuLayer::setMenu(bool _left)
{
	// 이전 메뉴 저장
	int pastMenu = menu;

	// 메뉴 설정
	if (_left)
	{
		if (menu > RESTART)
		{
			menu--;
		}
		else
		{
			menu = HOME;
		}
	}
	else
	{
		if (menu < HOME)
		{
			menu++;
		}
		else
		{
			menu = RESTART;
		}
	}

	// 메뉴 커서 설정
	setMenuCursor(pastMenu);
}

void MenuLayer::setMenuCursor(int _pastMenu)
{
	// 페이드 아웃
	auto fadeOut = FadeOut::create(0.3);
	on[_pastMenu]->runAction(fadeOut);

	// 페이드 인
	auto fadeIn = FadeIn::create(0.3);
	on[menu]->runAction(fadeIn);
}