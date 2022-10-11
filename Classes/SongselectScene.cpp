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

	// 노래 종료
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

	// 노래 리스트 할당
	song = new Song;

	// 바탕 화면 생성
	createBackground();

	// 리스트 레이어 생성
	createLayer();
	
	// 리스트 생성
	createList();

	// 커서 생성
	createCursor();

	// 곡 정보 레이어 생성
	createInfoLayer();

	// 곡 정보 라벨 생성
	createInfoLabel();

	// 유저 정보 레이어 생성
	createUserLayer();

	// 유저 정보 라벨 생성
	createUserLabel();

	// 곡 이미지 생성
	createImage();

	// 노래 정보 불러오기
	loadSongInfo();

	// 키보드 이벤트 리스너
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
		// 오프셋 설정
		setOffset(TRUE);

		// 메뉴 설정
		if (setMenu(TRUE))
		{
			// 노래 정보 불러오기
			loadSongInfo();

			// 곡 이미지 설정
			createImage();
		}

		// 커서 설정
		setCursor(TRUE);

		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		// 오프셋 설정
		setOffset(FALSE);

		// 메뉴 설정
		if (setMenu(FALSE))
		{
			// 노래 정보 불러오기
			loadSongInfo();

			// 곡 이미지 설정
			createImage();
		}

		// 커서 설정
		setCursor(FALSE);

		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		// 노래 종료
		AudioEngine::stop(audioID);

		// 노래 이름 설정
		UserDefault::getInstance()->setStringForKey("SONGNAME", temp->name);

		// BPM 설정
		UserDefault::getInstance()->setIntegerForKey("BPM", temp->BPM);

		// 곡 길이 설정
		UserDefault::getInstance()->setFloatForKey("DURATION", temp->duration);

		// 인게임으로 이동
		Director::getInstance()->replaceScene(LoadingScene::createScene());
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		// 노래 종료
		AudioEngine::stop(audioID);

		// 메인화면으로 이동
		Director::getInstance()->replaceScene(MainScene::createScene());
		break;
	}
}

void SongselectScene::removeListCallback(Ref* sender)
{
	CCLOG("removeListCallback");

	// 삭제
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
	// 레이어 생성
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

		// 정보 등록
		temp->label = label;

		// 다음 리스트로 이동
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
		// 이미지 삭제
		this->removeChild(image);
	}

	Song::List* temp = song->searchList(menu);

	// 이미지 출력
	image = Sprite::create(StringUtils::format("./song/%s/%s.png", temp->name.c_str(), temp->name.c_str()));
	if (!image)
	{
		// 빈 이미지 생성
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
	// 올릴 때
	if (_isUp)
	{
		if (cursor == 1 &&
			offset > 1)
		{
			Song::List* temp = song->searchList(offset + LIST_MAX - 1);

			// 라벨 밀기
			for (int count = 1; count <= LIST_MAX + 1; count++)
			{
				// 없앨 라벨일 때
				if (count == 1)
				{
					// 삭제 액션 실행
					temp->label->runAction(getRemoveAction());
				}
				// 만들 라벨일 때
				else if (count == LIST_MAX + 1)
				{
					// 라벨 생성
					auto label = Label::createWithTTF(UTF8(StringUtils::format(" %s", temp->name.c_str())), "./font/HMFMPYUN.TTF", 36, Size(SCROLL_WIDTH, SONG_HEIGHT), TextHAlignment::LEFT, TextVAlignment::CENTER);
					label->setColor(Color3B::BLACK);
					label->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
					label->setPosition(Point(0.0, SCROLL_HEIGHT + SONG_HEIGHT));
					label->setOpacity(0);
					label->setScale(0.5);
					listLayer->addChild(label);

					// 정보 등록
					temp->label = label;

					// 생성 액션 실행
					temp->label->runAction(getCreateAction());
				}

				// 내리기 액션 실행
				temp->label->runAction(getPushDownAction());

				// 이전 리스트로 이동
				temp = temp->prev;
			}

			// 오프셋 감소
			offset--;
		}
	}
	// 내릴 때
	else
	{
		if (cursor == LIST_MAX &&
			offset + LIST_MAX - 1 < song->tail->index)
		{
			Song::List* temp = song->searchList(offset);

			// 라벨 밀기
			for (int count = 1; count <= LIST_MAX + 1; count++)
			{
				// 없앨 라벨일 때
				if (count == 1)
				{
					// 삭제 액션 실행
					temp->label->runAction(getRemoveAction());
				}
				// 만들 라벨일 때
				else if (count == LIST_MAX + 1)
				{
					// 라벨 생성
					auto label = Label::createWithTTF(UTF8(StringUtils::format(" %s", temp->name.c_str())), "./font/HMFMPYUN.TTF", 36, Size(SCROLL_WIDTH, SONG_HEIGHT), TextHAlignment::LEFT, TextVAlignment::CENTER);
					label->setColor(Color3B::BLACK);
					label->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
					label->setPosition(Point(0.0, SCROLL_HEIGHT - SONG_HEIGHT * (LIST_MAX)));
					label->setOpacity(0);
					label->setScale(0.5);
					listLayer->addChild(label);

					// 정보 등록
					temp->label = label;

					// 생성 액션 실행
					temp->label->runAction(getCreateAction());
				}

				// 올리기 액션 실행
				temp->label->runAction(getPushUpAction());

				// 다음 리스트로 이동
				temp = temp->next;
			}

			// 오프셋 증가
			offset++;
		}
	}
}

bool SongselectScene::setMenu(bool _isUp)
{
	// 올릴 때
	if (_isUp)
	{
		if (menu > 1)
		{
			// 메뉴 감소
			menu--;

			return TRUE;
		}
		return FALSE;
	}
	// 내릴 때
	else
	{
		if (menu < song->tail->index)
		{
			// 메뉴 증가
			menu++;

			return TRUE;
		}
		return FALSE;
	}
}

void SongselectScene::setCursor(bool _isUp)
{
	// 올릴 때
	if (_isUp)
	{
		if (cursor > 1)
		{
			// 커서 감소
			cursor--;

			// 커서 위치 설정
			cursorSprite->setPositionY(cursorSprite->getPositionY() + SONG_HEIGHT);
		}
	}
	// 내릴 때
	else
	{
		if (cursor < listMax)
		{
			// 커서 증가
			cursor++;

			// 커서 위치 설정
			cursorSprite->setPositionY(cursorSprite->getPositionY() - SONG_HEIGHT);
		}
	}
}

void SongselectScene::loadSongInfo()
{
	// 노래 중지
	AudioEngine::stop(audioID);

	Song::List* temp = song->searchList(menu);

	// 노래 재생
	audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", temp->name.c_str(), temp->name.c_str()).c_str(), true);

	// 라벨 설정
	setInfoLabel();
	setUserLabel();
}

void SongselectScene::setInfoLabel()
{
	Song::List* temp = song->searchList(menu);

	infoLabel->setString(StringUtils::format(" 곡 길이 : %d:%2d\n\n BPM : %d\n\n 난이도 : %d", (int)temp->duration / 60, (int)temp->duration % 60, temp->BPM, temp->level));
}

void SongselectScene::setUserLabel()
{
	Song::List* temp = song->searchList(menu);

	if (temp->score)
	{
		userLabel->setString(StringUtils::format(" 최대 콤보 : %d\n\n 들어온 시간 : %d:%2d\n\n 넘어진 횟수 : %d", temp->combo, (int)temp->score / 60, (int)temp->score % 60, temp->fall));
	}
	else
	{
		userLabel->setString(" 최대 콤보 :\n\n 들어온 시간 :\n\n 넘어진 횟수 :");
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