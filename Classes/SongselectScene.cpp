#include "SongselectScene.h"
#include "MainScene.h"
#include "Song.h"
#include "LoadingScene.h"
#include "win32_kLanguage.h"
#include "AudioEngine.h"

SongselectScene::SongselectScene() :menu(1), cursor(1), offset(1), image(nullptr)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// �뷡 ����
	AudioEngine::stopAll();
}

Scene* SongselectScene::createScene()
{
	auto scene = Scene::create();

	auto layer = SongselectScene::create();
	scene->addChild(layer);

	return scene;
}

bool SongselectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// �뷡 ����Ʈ �Ҵ�
	song = new Song;

	// ���� ȭ�� ����
	createBackground();

	// ����Ʈ ���̾� ����
	createLayer();
	
	// ����Ʈ ����
	createList();

	// Ŀ�� ����
	createCursor();

	// �� ���� ���̾� ����
	createInfoLayer();

	// �� ���� �� ����
	createInfoLabel();

	// ���� ���� ���̾� ����
	createUserLayer();

	// ���� ���� �� ����
	createUserLabel();

	// �� �̹��� ����
	createImage();

	// �뷡 ���� �ҷ�����
	loadSongInfo();

	// Ű���� �̺�Ʈ ������
	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(SongselectScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(SongselectScene::onKeyReleased, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void SongselectScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	Song::List* temp = song->searchList(menu);

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		// ������ ����
		setOffset(TRUE);

		// �޴� ����
		if (setMenu(TRUE))
		{
			// �뷡 ���� �ҷ�����
			loadSongInfo();

			// �� �̹��� ����
			createImage();
		}

		// Ŀ�� ����
		setCursor(TRUE);

		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		// ������ ����
		setOffset(FALSE);

		// �޴� ����
		if (setMenu(FALSE))
		{
			// �뷡 ���� �ҷ�����
			loadSongInfo();

			// �� �̹��� ����
			createImage();
		}

		// Ŀ�� ����
		setCursor(FALSE);

		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		// �뷡 ����
		AudioEngine::stop(audioID);

		// �뷡 �̸� ����
		UserDefault::getInstance()->setStringForKey("SONGNAME", temp->name);

		// BPM ����
		UserDefault::getInstance()->setIntegerForKey("BPM", temp->BPM);

		// �� ���� ����
		UserDefault::getInstance()->setFloatForKey("DURATION", temp->duration);

		// �ΰ������� �̵�
		Director::getInstance()->replaceScene(LoadingScene::createScene());
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		// �뷡 ����
		AudioEngine::stop(audioID);

		// ����ȭ������ �̵�
		Director::getInstance()->replaceScene(MainScene::createScene());
		break;
	}
}

void SongselectScene::removeListCallback(Ref* sender)
{
	CCLOG("removeListCallback");

	// ����
	this->removeChild((Node*)sender);
}

void SongselectScene::createBackground()
{
	auto background = Sprite::create(StringUtils::format("./img/background/songselect_background.png"));
	background->setPosition(Vec2(origin.x + visibleSize.width * 0.5,
		origin.y + visibleSize.height - background->getContentSize().height * 0.5));
	this->addChild(background, -1);
}

void SongselectScene::createLayer()
{
	// ���̾� ����
	listLayer = Layer::create();
	listLayer->setContentSize(Size(SCROLL_WIDTH, SCROLL_HEIGHT));
	listLayer->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.2));
	listLayer->setRotation(-25);
	this->addChild(listLayer);
}

void SongselectScene::createList()
{
	Song::List* temp = song->head;

	listMax = song->tail->index < LIST_MAX ? song->tail->index : LIST_MAX;

	for (int count = NULL; count < listMax; count++)
	{
		auto label = Label::createWithTTF(UTF8(StringUtils::format(" %s", temp->name.c_str())), "./font/HMFMPYUN.TTF", 36, Size(SCROLL_WIDTH, SONG_HEIGHT), TextHAlignment::LEFT, TextVAlignment::CENTER);
		label->setColor(Color3B::BLACK);
		label->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
		label->setPosition(Point(0.0, SCROLL_HEIGHT - SONG_HEIGHT * count));
		listLayer->addChild(label);

		// ���� ���
		temp->label = label;

		// ���� ����Ʈ�� �̵�
		temp = temp->next;
	}
}

void SongselectScene::createCursor()
{
	cursorSprite = Sprite::create("./img/sprites/songselect_selector.png");
	cursorSprite->setAnchorPoint(Point::ANCHOR_TOP_LEFT + Point(0.1f, 0));
	cursorSprite->setPosition(Point(0.0, SCROLL_HEIGHT));
	listLayer->addChild(cursorSprite);
}

void SongselectScene::createInfoLayer()
{
	infoLayer = Layer::create();
	infoLayer->setContentSize(Size(POST_LENGTH, POST_LENGTH));
	infoLayer->setAnchorPoint(Point::ZERO);
	infoLayer->setPosition(Vec2(origin.x + visibleSize.width * 0.28, origin.y + visibleSize.height * 0.01));
	infoLayer->setRotation(-6);
	this->addChild(infoLayer);
}

void SongselectScene::createInfoLabel()
{
	infoLabel = Label::createWithTTF("", "./font/HMFMPYUN.TTF", 24, Size(POST_LENGTH, POST_LENGTH), TextHAlignment::LEFT, TextVAlignment::CENTER);
	infoLabel->setAnchorPoint(Point::ZERO);
	infoLabel->setPosition(Point::ZERO);
	infoLabel->setColor(Color3B::BLACK);
	infoLayer->addChild(infoLabel);
}

void SongselectScene::createUserLayer()
{
	userLayer = Layer::create();
	userLayer->setContentSize(Size(POST_LENGTH, POST_LENGTH));
	userLayer->setAnchorPoint(Point::ZERO);
	userLayer->setPosition(Vec2(origin.x + visibleSize.width * 0.02, origin.y + visibleSize.height * 0.15));
	userLayer->setRotation(14);
	this->addChild(userLayer);
}

void SongselectScene::createUserLabel()
{
	userLabel = Label::createWithTTF("", "./font/HMFMPYUN.TTF", 24, Size(POST_LENGTH, POST_LENGTH), TextHAlignment::LEFT, TextVAlignment::CENTER);
	userLabel->setAnchorPoint(Point::ZERO);
	userLabel->setPosition(Point::ZERO);
	userLabel->setColor(Color3B::BLACK);
	userLayer->addChild(userLabel);
}

void SongselectScene::createImage()
{
	if (image)
	{
		// �̹��� ����
		this->removeChild(image);
	}

	Song::List* temp = song->searchList(menu);

	// �̹��� ���
	image = Sprite::create(StringUtils::format("./song/%s/%s.png", temp->name.c_str(), temp->name.c_str()));
	if (!image)
	{
		// �� �̹��� ����
		image = Sprite::create("./img/sprites/songselect_empty.png");
	}
	image->setAnchorPoint(Point::ANCHOR_MIDDLE);
	image->setPosition(Vec2(origin.x + visibleSize.width * 0.214f, origin.y + visibleSize.height * 0.724f));
	if (image->getContentSize().width < image->getContentSize().height)
	{
		image->setScale((float)IMAGE_LENGTH / (float)image->getContentSize().width);
	}
	else
	{
		image->setScale((float)IMAGE_LENGTH / (float)image->getContentSize().height);
	}
	this->addChild(image, -2);
}

void SongselectScene::setOffset(bool _isUp)
{
	// �ø� ��
	if (_isUp)
	{
		if (cursor == 1 &&
			offset > 1)
		{
			Song::List* temp = song->searchList(offset + LIST_MAX - 1);

			// �� �б�
			for (int count = 1; count <= LIST_MAX + 1; count++)
			{
				// ���� ���� ��
				if (count == 1)
				{
					// ���� �׼� ����
					temp->label->runAction(getRemoveAction());
				}
				// ���� ���� ��
				else if (count == LIST_MAX + 1)
				{
					// �� ����
					auto label = Label::createWithTTF(UTF8(StringUtils::format(" %s", temp->name.c_str())), "./font/HMFMPYUN.TTF", 36, Size(SCROLL_WIDTH, SONG_HEIGHT), TextHAlignment::LEFT, TextVAlignment::CENTER);
					label->setColor(Color3B::BLACK);
					label->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
					label->setPosition(Point(0.0, SCROLL_HEIGHT + SONG_HEIGHT));
					label->setOpacity(0);
					label->setScale(0.5);
					listLayer->addChild(label);

					// ���� ���
					temp->label = label;

					// ���� �׼� ����
					temp->label->runAction(getCreateAction());
				}

				// ������ �׼� ����
				temp->label->runAction(getPushDownAction());

				// ���� ����Ʈ�� �̵�
				temp = temp->prev;
			}

			// ������ ����
			offset--;
		}
	}
	// ���� ��
	else
	{
		if (cursor == LIST_MAX &&
			offset + LIST_MAX - 1 < song->tail->index)
		{
			Song::List* temp = song->searchList(offset);

			// �� �б�
			for (int count = 1; count <= LIST_MAX + 1; count++)
			{
				// ���� ���� ��
				if (count == 1)
				{
					// ���� �׼� ����
					temp->label->runAction(getRemoveAction());
				}
				// ���� ���� ��
				else if (count == LIST_MAX + 1)
				{
					// �� ����
					auto label = Label::createWithTTF(UTF8(StringUtils::format(" %s", temp->name.c_str())), "./font/HMFMPYUN.TTF", 36, Size(SCROLL_WIDTH, SONG_HEIGHT), TextHAlignment::LEFT, TextVAlignment::CENTER);
					label->setColor(Color3B::BLACK);
					label->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
					label->setPosition(Point(0.0, SCROLL_HEIGHT - SONG_HEIGHT * (LIST_MAX)));
					label->setOpacity(0);
					label->setScale(0.5);
					listLayer->addChild(label);

					// ���� ���
					temp->label = label;

					// ���� �׼� ����
					temp->label->runAction(getCreateAction());
				}

				// �ø��� �׼� ����
				temp->label->runAction(getPushUpAction());

				// ���� ����Ʈ�� �̵�
				temp = temp->next;
			}

			// ������ ����
			offset++;
		}
	}
}

bool SongselectScene::setMenu(bool _isUp)
{
	// �ø� ��
	if (_isUp)
	{
		if (menu > 1)
		{
			// �޴� ����
			menu--;

			return TRUE;
		}
		return FALSE;
	}
	// ���� ��
	else
	{
		if (menu < song->tail->index)
		{
			// �޴� ����
			menu++;

			return TRUE;
		}
		return FALSE;
	}
}

void SongselectScene::setCursor(bool _isUp)
{
	// �ø� ��
	if (_isUp)
	{
		if (cursor > 1)
		{
			// Ŀ�� ����
			cursor--;

			// Ŀ�� ��ġ ����
			cursorSprite->setPositionY(cursorSprite->getPositionY() + SONG_HEIGHT);
		}
	}
	// ���� ��
	else
	{
		if (cursor < listMax)
		{
			// Ŀ�� ����
			cursor++;

			// Ŀ�� ��ġ ����
			cursorSprite->setPositionY(cursorSprite->getPositionY() - SONG_HEIGHT);
		}
	}
}

void SongselectScene::loadSongInfo()
{
	// �뷡 ����
	AudioEngine::stop(audioID);

	Song::List* temp = song->searchList(menu);

	// �뷡 ���
	audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", temp->name.c_str(), temp->name.c_str()).c_str(), true);

	// �� ����
	setInfoLabel();
	setUserLabel();
}

void SongselectScene::setInfoLabel()
{
	Song::List* temp = song->searchList(menu);

	infoLabel->setString(StringUtils::format(" �� ���� : %d:%2d\n\n BPM : %d\n\n ���̵� : %d", (int)temp->duration / 60, (int)temp->duration % 60, temp->BPM, temp->level));
}

void SongselectScene::setUserLabel()
{
	Song::List* temp = song->searchList(menu);

	if (temp->score)
	{
		userLabel->setString(StringUtils::format(" �ִ� �޺� : %d\n\n ���� �ð� : %d:%2d\n\n �Ѿ��� Ƚ�� : %d", temp->combo, (int)temp->score / 60, (int)temp->score % 60, temp->fall));
	}
	else
	{
		userLabel->setString(" �ִ� �޺� :\n\n ���� �ð� :\n\n �Ѿ��� Ƚ�� :");
	}
}

MoveBy* SongselectScene::getPushDownAction()
{
	auto listPushDown = MoveBy::create(SCROLL_SPEED, Vec2(0.0, -SONG_HEIGHT));
	return listPushDown;
}

MoveBy* SongselectScene::getPushUpAction()
{
	auto listPushUp = MoveBy::create(SCROLL_SPEED, Vec2(0.0, SONG_HEIGHT));
	return listPushUp;
}

Sequence* SongselectScene::getRemoveAction()
{
	auto action_1 = ScaleTo::create(SCROLL_SPEED, 0.5);
	auto action_2 = FadeOut::create(SCROLL_SPEED);
	auto action_3 = Spawn::create(action_1, action_2, NULL);
	auto action_4 = CallFuncN::create(CC_CALLBACK_1(SongselectScene::removeListCallback, this));
	auto listRemove = Sequence::create(action_3, action_4, NULL);
	return listRemove;
}

Sequence* SongselectScene::getCreateAction()
{
	//auto action_1 = DelayTime::create(SCROLL_SPEED * 0.5);
	auto action_2 = ScaleTo::create(SCROLL_SPEED, 1.0);
	auto action_3 = FadeIn::create(SCROLL_SPEED);
	auto action_4 = Spawn::create(action_2, action_3, NULL);
	auto listCreate = Sequence::create(/*action_1, */action_4, NULL);
	return listCreate;
}