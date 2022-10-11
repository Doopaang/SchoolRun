#include "MainScene.h"
#include "ManualScene.h"
#include "SongselectScene.h"
#include "AudioEngine.h"

MainScene::MainScene() : menu(MENU::START), can(TRUE)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 파일 불러오기
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites/main_sprites.plist");

	// 배경 불러오기
	AudioEngine::preload("./sound/bgm_main.mp3");

	// 배경음 출력
	AudioEngine::play2d("./sound/bgm_main.mp3", true);

	// 효과음 불러오기
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

	// 바탕 화면 생성
	createBackground();

	// 버튼 생성
	createButton();
	
	// 키보드 이벤트 리스너
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

	// 화면 전환
	changeScene();

	// 스프라이트 지우기
	this->removeChild((Sprite*)sender);

	// 가능
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
	// 시작 메뉴 생성
	auto startNormal = Sprite::createWithSpriteFrameName("start_normal.png");
	startNormal->setPosition(Vec2(origin.x + visibleSize.width * 0.5, button_height * 4));
	this->addChild(startNormal);

	// 시작 선택 생성
	auto startSelect = Sprite::createWithSpriteFrameName("start_select.png");
	startSelect->setPosition(startNormal->getPosition());
	startSelect->setVisible(true);
	this->addChild(startSelect);

	// 시작 선택 저장
	menus[START] = startSelect;

	// 도움말 메뉴 생성
	auto manualNormal = Sprite::createWithSpriteFrameName("manual_normal.png");
	manualNormal->setPosition(Vec2(origin.x + visibleSize.width * 0.5, button_height * 2.5));
	this->addChild(manualNormal);

	// 도움말 선택 생성
	auto manualSelect = Sprite::createWithSpriteFrameName("manual_select.png");
	manualSelect->setPosition(manualNormal->getPosition());
	manualSelect->setVisible(false);
	this->addChild(manualSelect);

	// 도움말 선택 저장
	menus[MANUAL] = manualSelect;

	// 종료 메뉴 생성
	auto exitNormal = Sprite::createWithSpriteFrameName("exit_normal.png");
	exitNormal->setPosition(Vec2(origin.x + visibleSize.width * 0.5, button_height * 1));
	this->addChild(exitNormal);
	
	// 종료 선택 생성
	auto exitSelect = Sprite::createWithSpriteFrameName("exit_select.png");
	exitSelect->setPosition(exitNormal->getPosition());
	exitSelect->setVisible(false);
	this->addChild(exitSelect);

	// 종료 선택 저장
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
	// 메뉴 선택 숨기기
	menus[menu]->setVisible(false);

	// 메뉴 설정
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

	// 메뉴 선택 보이기
	menus[menu]->setVisible(true);
}