#include "MainScene.h"
#include "ManualScene.h"
#include "SongselectScene.h"
#include "AudioEngine.h"

MainScene::MainScene() : menu(MENU::START), can(TRUE)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ���� �ҷ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites/main_sprites.plist");

	// ��� �ҷ�����
	AudioEngine::preload("./sound/bgm_main.mp3");

	// ����� ���
	AudioEngine::play2d("./sound/bgm_main.mp3", true);

	// ȿ���� �ҷ�����
	AudioEngine::preload("./sound/fx_paint_1.mp3");
	AudioEngine::preload("./sound/fx_paint_2.mp3");
	AudioEngine::preload("./sound/fx_book.mp3");
	AudioEngine::preload("./sound/fx_fall.mp3");
	AudioEngine::preload("./sound/fx_stamp.mp3");
}

Scene* MainScene::createScene()
{
	auto scene = Scene::create();

	auto layer = MainScene::create();
	scene->addChild(layer);

	return scene;
}

bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// ���� ȭ�� ����
	createBackground();

	// ��ư ����
	createButton();
	
	// Ű���� �̺�Ʈ ������
	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (can)
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			setMenu(TRUE);
			break;

		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			setMenu(FALSE);
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			createAnimation();
			can = FALSE;
			break;

		case EventKeyboard::KeyCode::KEY_ESCAPE:
			exit(0);
		}
	}
}

void MainScene::animationCallback(Ref* sender)
{
	CCLOG("animationCallback");

	// ȭ�� ��ȯ
	changeScene();

	// ��������Ʈ �����
	this->removeChild((Sprite*)sender);

	// ����
	can = TRUE;
}

void MainScene::playPaint1Callback()
{
	CCLOG("playPaint1Callback");

	AudioEngine::play2d("./sound/fx_paint_1.mp3");
}

void MainScene::playPaint2Callback()
{
	CCLOG("playPaint2Callback");

	AudioEngine::play2d("./sound/fx_paint_2.mp3");
}

void MainScene::createBackground()
{
	auto background = Sprite::create(StringUtils::format("./img/background/main_background.png"));

	background->setPosition(Vec2(origin.x + visibleSize.width * 0.5,
		origin.y + visibleSize.height - background->getContentSize().height * 0.5));

	this->addChild(background, -1);
}

void MainScene::createButton()
{
	// ���� �޴� ����
	auto startNormal = Sprite::createWithSpriteFrameName("start_normal.png");
	startNormal->setPosition(Vec2(origin.x + visibleSize.width * 0.5, button_height * 4));
	this->addChild(startNormal);

	// ���� ���� ����
	auto startSelect = Sprite::createWithSpriteFrameName("start_select.png");
	startSelect->setPosition(startNormal->getPosition());
	startSelect->setVisible(true);
	this->addChild(startSelect);

	// ���� ���� ����
	menus[START] = startSelect;

	// ���� �޴� ����
	auto manualNormal = Sprite::createWithSpriteFrameName("manual_normal.png");
	manualNormal->setPosition(Vec2(origin.x + visibleSize.width * 0.5, button_height * 2.5));
	this->addChild(manualNormal);

	// ���� ���� ����
	auto manualSelect = Sprite::createWithSpriteFrameName("manual_select.png");
	manualSelect->setPosition(manualNormal->getPosition());
	manualSelect->setVisible(false);
	this->addChild(manualSelect);

	// ���� ���� ����
	menus[MANUAL] = manualSelect;

	// ���� �޴� ����
	auto exitNormal = Sprite::createWithSpriteFrameName("exit_normal.png");
	exitNormal->setPosition(Vec2(origin.x + visibleSize.width * 0.5, button_height * 1));
	this->addChild(exitNormal);
	
	// ���� ���� ����
	auto exitSelect = Sprite::createWithSpriteFrameName("exit_select.png");
	exitSelect->setPosition(exitNormal->getPosition());
	exitSelect->setVisible(false);
	this->addChild(exitSelect);

	// ���� ���� ����
	menus[EXIT] = exitSelect;
}

void MainScene::createAnimation()
{
	auto select = Sprite::createWithSpriteFrameName("select_1.png");
	select->setPosition(menus[menu]->getPosition());
	this->addChild(select);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	for (int count = 1; count <= 3; count++)
	{
		auto frame = SpriteFrameCache::getInstance()->spriteFrameByName(StringUtils::format("select_%d.png", count));
		animation->addSpriteFrame(frame);
	}

	auto sound1 = CallFunc::create(CC_CALLBACK_0(MainScene::playPaint1Callback, this));
	auto delay = DelayTime::create(0.1f);
	auto sound2 = CallFunc::create(CC_CALLBACK_0(MainScene::playPaint2Callback, this));
	auto sound = Sequence::create(sound1, delay, sound2, delay, sound1, NULL);
	auto animate = Animate::create(animation);
	auto callback = CallFuncN::create(CC_CALLBACK_1(MainScene::animationCallback, this));
	auto paint = Sequence::create(animate, delay, callback, NULL);
	auto action = Spawn::create(paint, sound, NULL);
	select->runAction(action);
}

void MainScene::changeScene()
{
	switch (menu)
	{
	case MENU::START:
		AudioEngine::stopAll();
		Director::getInstance()->replaceScene(SongselectScene::createScene());
		break;

	case MENU::MANUAL:
		AudioEngine::play2d("./sound/fx_page.mp3");
		Director::getInstance()->pushScene(ManualScene::createScene());
		break;

	case MENU::EXIT:
		AudioEngine::stopAll();
		exit(0);
	}
}
void MainScene::setMenu(bool _isUp)
{
	// �޴� ���� �����
	menus[menu]->setVisible(false);

	// �޴� ����
	if (_isUp)
	{
		if (menu > MENU::START)
		{
			menu--;
		}
		else
		{
			menu = MENU::EXIT;
		}
	}
	else
	{
		if (menu < MENU::EXIT)
		{
			menu++;
		}
		else
		{
			menu = MENU::START;
		}
	}

	// �޴� ���� ���̱�
	menus[menu]->setVisible(true);
}