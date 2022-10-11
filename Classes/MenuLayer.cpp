#include "MenuLayer.h"

MenuLayer::MenuLayer() :menu(RESTART)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ���� �ҷ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites/menu_sprites.plist");
}

bool MenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// ��� �÷� ���̾� ����
	createBackground();

	// �޴� ����
	createMenu();

	// ���̾� �����
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
	// ����� �޴� ����
	auto restart = Sprite::createWithSpriteFrameName("restart.png");
	restart->setPosition(origin.x + visibleSize.width * 0.25, origin.y + visibleSize.height * 0.5);
	this->addChild(restart, OFF);

	auto restartOn = Sprite::createWithSpriteFrameName("restart_on.png");
	restartOn->setPosition(restart->getPosition());
	restartOn->setOpacity(255);
	this->addChild(restartOn, ON);
	on[RESTART] = restartOn;

	// �� ���� �޴� ����
	auto songselect = Sprite::createWithSpriteFrameName("songselect.png");
	songselect->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5);
	this->addChild(songselect, OFF);

	auto songselectOn = Sprite::createWithSpriteFrameName("songselect_on.png");
	songselectOn->setPosition(songselect->getPosition());
	songselectOn->setOpacity(0);
	this->addChild(songselectOn, ON);
	on[SONGSELECT] = songselectOn;

	// ���� ȭ�� �޴� ����
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
		// ���̾� �����
		this->setVisible(false);
	}
	else
	{
		// ���̾� ���̱�
		this->setVisible(true);
	}
}

void MenuLayer::setMenu(bool _left)
{
	// ���� �޴� ����
	int pastMenu = menu;

	// �޴� ����
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

	// �޴� Ŀ�� ����
	setMenuCursor(pastMenu);
}

void MenuLayer::setMenuCursor(int _pastMenu)
{
	// ���̵� �ƿ�
	auto fadeOut = FadeOut::create(0.3);
	on[_pastMenu]->runAction(fadeOut);

	// ���̵� ��
	auto fadeIn = FadeIn::create(0.3);
	on[menu]->runAction(fadeIn);
}