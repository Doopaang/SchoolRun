#include "ResultScene.h"
#include "InGame.h"
#include "SongselectScene.h"
#include "Note.h"
#include "AudioEngine.h"

ResultScene::ResultScene() :menu(RESTART)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ���� �ҷ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites/result_sprites.plist");

	// �뷡 �̸� �ҷ�����
	name = UserDefault::getInstance()->getStringForKey("SONGNAME");

	// �ִ� �޺� �ҷ�����
	combo = UserDefault::getInstance()->getIntegerForKey("COMBO");

	// ���� �ð� �ҷ�����
	score = UserDefault::getInstance()->getFloatForKey("SCORE");

	// �Ѿ��� Ƚ�� �ҷ�����
	fall = UserDefault::getInstance()->getIntegerForKey("FALL");
}

Scene* ResultScene::createScene()
{
	auto scene = Scene::create();

	auto layer = ResultScene::create();
	scene->addChild(layer);

	return scene;
}

bool ResultScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// ��� ȭ�� ����
	createBackground();

	// �޴� ����
	createMenu();

	// ��� ����
	createResult();

	// �� ���� ����
	createInfo();

	// Ŭ���� ���� ����
	createClear();

	// Ű���� �̺�Ʈ ������
	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(ResultScene::onKeyPressed, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void ResultScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (menu == MENU::RESTART)
		{
			menu = MENU::SONGSELECT;
		}
		else
		{
			menu = MENU::RESTART;
		}
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (menu == MENU::SONGSELECT)
		{
			menu = MENU::RESTART;
		}
		else
		{
			menu = MENU::SONGSELECT;
		}
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		// ������ ����
		saveData();

		// ȭ�� ��ȯ
		ResultScene::changeScene();
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		// ������ ����
		saveData();

		// ȭ�� ��ȯ
		Director::getInstance()->replaceScene(SongselectScene::createScene());
		break;
	}
	// Ŀ�� ����
	setCursor();
}

void ResultScene::createBackground()
{
	auto background = Sprite::create("./img/background/result_background.png");
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	this->addChild(background, BACKGROUND);
}

void ResultScene::createMenu()
{
	// ����� �⺻ ��������Ʈ ����
	auto restartNormal = Sprite::createWithSpriteFrameName("restart_normal.png");
	restartNormal->setPosition(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.22);
	this->addChild(restartNormal, OBJECT);

	// ����� ���� ��������Ʈ ����
	restartSelect = Sprite::createWithSpriteFrameName("restart_select.png");
	restartSelect->setPosition(restartNormal->getPosition());
	this->addChild(restartSelect, OBJECT_UP);
	restartSelect->setVisible(true);

	// ���� �⺻ ��������Ʈ ����
	auto songselectNormal = Sprite::createWithSpriteFrameName("songselect_normal.png");
	songselectNormal->setPosition(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.22);
	this->addChild(songselectNormal, OBJECT);

	// ���� ���� ��������Ʈ ����
	songselectSelect = Sprite::createWithSpriteFrameName("songselect_select.png");
	songselectSelect->setPosition(songselectNormal->getPosition());
	this->addChild(songselectSelect, OBJECT_UP);
	songselectSelect->setVisible(false);
}

void ResultScene::createResult()
{
	// �ִ� �޺� ���
	auto comboLabel = Label::createWithTTF(StringUtils::format("%d", combo).c_str(), "./font/THEPlaying.TTF", 68, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	comboLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	comboLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.85, origin.y + visibleSize.height * 0.82));
	comboLabel->setColor(Color3B::WHITE);
	this->addChild(comboLabel, OBJECT);

	// ���� �ð� ���
	auto scoreLabel = Label::createWithTTF(StringUtils::format("%d:%2d", (int)score / 60, (int)score % 60).c_str(), "./font/THEPlaying.TTF", 68, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	scoreLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	scoreLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.85, origin.y + visibleSize.height * 0.7));
	scoreLabel->setColor(Color3B::WHITE);
	this->addChild(scoreLabel, OBJECT);

	// �Ѿ��� Ƚ�� ���
	auto fallLabel = Label::createWithTTF(StringUtils::format("%d", fall).c_str(), "./font/THEPlaying.TTF", 68, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	fallLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	fallLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.85, origin.y + visibleSize.height * 0.57));
	fallLabel->setColor(Color3B::WHITE);
	this->addChild(fallLabel, OBJECT);
}

void ResultScene::createInfo()
{
	// �̹��� ���
	auto image = Sprite::create(StringUtils::format("./song/%s/%s.png", name.c_str(), name.c_str()));
	if (!image)
	{
		// �� �̹��� ����
		image = Sprite::create("./img/sprites/songselect_empty.png");
	}
	image->setAnchorPoint(Point::ANCHOR_MIDDLE);
	image->setPosition(Vec2(origin.x + visibleSize.width * 0.25 - 5, origin.y + visibleSize.height * 0.7 - 5));
	if (image->getContentSize().width < image->getContentSize().height)
	{
		image->setScale((float)IMAGE_LENGTH / (float)image->getContentSize().width);
	}
	else
	{
		image->setScale((float)IMAGE_LENGTH / (float)image->getContentSize().height);
	}
	this->addChild(image, IMAGE);

	// ���� ���
	auto nameLabel = Label::createWithTTF(StringUtils::format("%s", name.c_str()).c_str(), "./font/THEPlaying.TTF", 68, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	nameLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	nameLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.25 - 5, origin.y + visibleSize.height * 0.4));
	nameLabel->setColor(Color3B::WHITE);
	this->addChild(nameLabel, OBJECT);
}

void ResultScene::createClear()
{
	bool clear = UserDefault::getInstance()->getBoolForKey("CLEAR");

	auto stamp = Sprite::createWithSpriteFrameName(clear ? "complete.png" : "fail.png");
	stamp->setAnchorPoint(Point::ANCHOR_MIDDLE);
	stamp->setPosition(Vec2(origin.x + visibleSize.width * 0.9, origin.y + visibleSize.height * 0.45));
	stamp->setScale(5);
	this->addChild(stamp, OBJECT_UP);

	auto action = ScaleTo::create(0.3f, 1.0f);
	stamp->runAction(action);

	AudioEngine::play2d("./sound/fx_stamp.mp3");
}

void ResultScene::changeScene()
{
	switch (menu)
	{
	case RESTART:
		Director::getInstance()->replaceScene(InGame::createScene());
		break;

	case SONGSELECT:
		Note* note = (Note*)UserDefault::getInstance()->getIntegerForKey("NOTE");
		delete note;
		Director::getInstance()->replaceScene(SongselectScene::createScene());
		break;
	}
}

void ResultScene::setCursor()
{
	switch (menu)
	{
	case RESTART:
		restartSelect->setVisible(true);
		songselectSelect->setVisible(false);
		break;

	case SONGSELECT:
		restartSelect->setVisible(false);
		songselectSelect->setVisible(true);
		break;
	}
}

void ResultScene::saveData()
{
	// ���� ����
	FILE* file = fopen(StringUtils::format("./data/%s", name.c_str(), name.c_str()).c_str(), "r");

	// ���� ���� Ȯ��
	int comboPast;
	float scorePast;
	int fallPast;
	if (fscanf(file, "%d", &comboPast) == -1 ||
		fscanf(file, "%f", &scorePast) == -1 ||
		fscanf(file, "%d", &fallPast) == -1)
	{
		saveUserData();
	}
	// ���� ��
	else if (score < scorePast)
	{
		saveUserData();
	}
	else if (score == scorePast)
	{
		if (fall < fallPast)
		{
			saveUserData();
		}
		else if (fall == fallPast)
		{
			if (combo < comboPast)
			{
				saveUserData();
			}
		}
	}

	// ���� �ݱ�
	fclose(file);
}

void ResultScene::saveUserData()
{
	// ���� ����
	FILE* fp = fopen(StringUtils::format("./data/%s", name.c_str(), name.c_str()).c_str(), "w");

	// ���� ����
	fprintf(fp, "%d\n%f\n%d", combo, score, fall);

	// ���� �ݱ�
	fclose(fp);
}