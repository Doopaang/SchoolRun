#include "InGame.h"
#include "Character.h"
#include "Note.h"
#include "AudioEngine.h"
#include "MenuLayer.h"
#include "SongselectScene.h"
#include "MainScene.h"
#include "ResultScene.h"

MenuLayer* menuLayer;

InGame::InGame() :leftPress(FALSE), rightPress(FALSE), downPress(FALSE), fps(NULL), time(NULL), speed(DEFAULT), frameSpeed(DEFAULT), state(PLAYING), stateTime(NULL), stateBackground(B_DEFAULT), score(NULL), fall(NULL), combo(NULL), comboMax(NULL), background_(NULL)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 파일 불러오기
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites/ingame_sprites.plist");

	// 노래 이름 설정
	name = UserDefault::getInstance()->getStringForKey("SONGNAME");

	// BPM 설정
	BPM = UserDefault::getInstance()->getIntegerForKey("BPM");

	// 노래 길이 설정
	duration = UserDefault::getInstance()->getFloatForKey("DURATION");
}

InGame::~InGame()
{
	// 할당 해제
	delete character;

	// 노래 정지
	AudioEngine::stop(audioID);

	// 노래 삭제
	AudioEngine::uncache(StringUtils::format("./song/%s/%s.mp3", name.c_str(), name.c_str()));
}

Scene* InGame::createScene()
{
    auto scene = Scene::create();
    
	// 인게임 레이어 생성
    auto layer = InGame::create();
    scene->addChild(layer);

	// 메뉴 레이어 생성
	menuLayer = MenuLayer::create();
	scene->addChild(menuLayer);

    return scene;
}

bool InGame::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	// 노트 할당
	note = (Note*)UserDefault::getInstance()->getIntegerForKey("NOTE");
	note->tagCount = { 1, NULL };
	
	// 캐릭터 출력
	createCharacter();

	// 배경 화면 출력
	createBackground();

	// 진행바 출력
	createSlider();

	// 제한시간 출력
	createLimit();

	// 곡 이름 출력
	createName();

	// 콤보 출력
	createCombo();

	// 키보드 이벤트 리스너
	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(InGame::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(InGame::onKeyReleased, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	// 업데이트
	this->scheduleUpdate();

	// 노래 출력
	audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name.c_str(), name.c_str()).c_str());

	// 노래 끝남 콜백 설정
	AudioEngine::setFinishCallback(audioID, CC_CALLBACK_0(InGame::songFinishCallback, this));

	// 노트 초기화
	note->resetNote();

    return true;
}

void InGame::update(float delta)
{
	switch (state)
	{
	case PLAYING:
		// 프레임 증가
		fps += ((float)BPM + 1.0) / 2;

		// 시간 증가
		score += 1.0f / 60.0f;

		// 제한 시간 설정
		setLimit();

		// 시간 증가
		setTime();

		// 속도 설정
		setSpeed();

		// 노트 관리
		manageNote();

		// 콤보 관리
		manageCombo();

		// 오디오 관리
		manageAudio();

		// 진행바 설정
		slider->setPercent(AudioEngine::getCurrentTime(audioID) / duration * 100);

		// 곡 이름 이동
		moveName();

		// 그림자 애니메이션
		shadowSprite = character->getShadow(fps, frameSpeed, state);

		// 캐릭터 액션
		characterSprite = character->action(&fps, frameSpeed, state);

		// 배경 화면 이동
		moveBackground();

		break;

	case WAIT:
		// 프레임 증가
		fps += ((float)BPM + 1.0) / 2;

		// 시간 증가
		score += 1.0f / 60.0f;

		// 제한 시간 설정
		setLimit();

		// 속도 설정
		character->setActionSpeed(DEFAULT, BPM);
		
		// 그리고 시간은 흐른다
		resumeWorld();

		// 진행바 설정
		slider->setPercent(AudioEngine::getCurrentTime(audioID) / duration * 100);

		// 곡 이름 이동
		moveName();

		// 그림자 애니메이션
		shadowSprite = character->getShadow(fps, frameSpeed, state);

		// 캐릭터 액션
		characterSprite = character->action(&fps, frameSpeed, state);

		break;

	case END:
		// 프레임 증가
		fps += ((float)BPM + 1.0) / 2;

		// 시간 증가
		score += 1.0f / 60.0f;

		// 제한 시간 설정
		setLimit();

		// 시간 증가
		setTime();

		// 속도 설정
		character->setActionSpeed(DEFAULT, BPM);

		// 노트 관리
		manageNote();

		// 진행바 설정
		slider->setPercent(100);

		// 끝 알림
		if (stateBackground == B_END &&
			character->endRun(SPEED_DEFAULT, BPM))
		{
			// 데이터 저장
			saveData();

			// 액션 속도 초기화
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);

			// 화면 전환
			Director::getInstance()->replaceScene(ResultScene::createScene());
		}

		// 곡 이름 이동
		moveName();

		// 그림자 애니메이션
		shadowSprite = character->getShadow(fps, frameSpeed, state);

		// 캐릭터 액션
		characterSprite = character->action(&fps, frameSpeed, state);

		// 배경 화면 이동
		endBackground();

		break;
	}
}

void InGame::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		leftPress = TRUE;
		switch (state)
		{
		case PLAYING:
			AudioEngine::setCurrentTime(audioID, AudioEngine::getCurrentTime(audioID) + SOUND_DEFAULT);
			break;

		case PAUSE:
			menuLayer->setMenu(TRUE);
			break;
		}
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		switch (state)
		{
		case PLAYING:
			character->jumpArim(downPress, frameSpeed);
			break;

		case PAUSE:
			menuLayer->setMenu(TRUE);
			break;
		}
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		rightPress = TRUE;
		switch (state)
		{
		case PLAYING:
			AudioEngine::setCurrentTime(audioID, AudioEngine::getCurrentTime(audioID) + SOUND_DEFAULT);
			break;

		case PAUSE:
			menuLayer->setMenu(FALSE);
			break;
		}
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		downPress = TRUE;
		switch (state)
		{
		case PLAYING:
			character->slideArim(downPress, frameSpeed);
			break;

		case PAUSE:
			menuLayer->setMenu(FALSE);
			break;
		}
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		// 메뉴 띄움
		menuLayer->setLayerVisible();

		if (menuLayer->isVisible())
		{
			// 액션 속도 초기화
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);

			// 더 월드
			AudioEngine::pause(audioID);
			characterSprite->pause();

			// 상태 기억
			stateRemember = state;

			// 상태 설정
			state = PAUSE;
		}
		else
		{
			// 그리고 시간은 흐른다
			if (stateRemember == PLAYING)
			{
				AudioEngine::resume(audioID);
			}
			characterSprite->resume();

			// 상태 설정
			state = stateRemember;

			// 슬라이드 체크
			if (state == PLAYING)
			{
				character->slideArim(downPress, frameSpeed);
			}
		}
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		if (menuLayer->isVisible())
		{
			// 메뉴 숨김
			menuLayer->setVisible(false);

			// 액션 속도 초기화
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);

			switch (menuLayer->menu)
			{
			case MenuLayer::MENU::RESTART:
				resetWorld();
				break;

			case MenuLayer::MENU::SONGSELECT:
				Director::getInstance()->replaceScene(SongselectScene::createScene());
				break;

			case MenuLayer::MENU::HOME:
				Director::getInstance()->replaceScene(MainScene::createScene());
				break;
			}
		}
	}
}

void InGame::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		leftPress = FALSE;
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		rightPress = FALSE;
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		downPress = FALSE;
		switch (state)
		{
		case PLAYING:
			character->slideArim(downPress, frameSpeed);
			break;
		}
		break;
	}
}

void InGame::songFinishCallback()
{
	CCLOG("songFinishCallback");

	// 모션 초기화
	if (character->state == Character::STATE::S_ING)
	{
		character->slideArim(FALSE, frameSpeed);
	}

	// 종료 알림
	state = END;

	// 속도 초기화
	speed = DEFAULT;
}

void InGame::createCharacter()
{
	Vec2 defaultPosition = Vec2(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.15);

	character = new Character(defaultPosition);

	shadowSprite = character->getShadow(fps, frameSpeed, state);
	this->addChild(shadowSprite, SHADOW);

	characterSprite = character->action(&fps, frameSpeed, state);
	this->addChild(characterSprite, CHARACTER);
}

void InGame::createBackground()
{
	int random = rand() % BACKGROUND_KIND;
	background = Sprite::create(StringUtils::format("./img/background/ingame_%d.png", random));
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	this->addChild(background, BACKGROUND);

	random = rand() % BACKGROUND_KIND;
	backgroundSub = Sprite::create(StringUtils::format("./img/background/ingame_%d.png", random));
	backgroundSub->setAnchorPoint(Point::ZERO);
	backgroundSub->setPosition(Point(backgroundSub->getContentSize().width, 0));
	this->addChild(backgroundSub, BACKGROUND);

	background_ = Sprite::create("./img/background/ingame_end_.png");
	background_->setAnchorPoint(Point::ZERO);
	background_->setPosition(Vec2(Director::getInstance()->getWinSize().width, 0));
	this->addChild(background_, SUB_FRONT);
}

void InGame::createSlider()
{
	// 배경 생성
	auto blackboardBack = Sprite::createWithSpriteFrameName("background.png");
	blackboardBack->setAnchorPoint(Point::ANCHOR_MIDDLE);
	blackboardBack->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.91));
	this->addChild(blackboardBack, INTERFACE_BACK);

	// 슬라이더 생성
	slider = Slider::create();
	slider->loadBarTexture("blackboard.png", Widget::TextureResType::PLIST);
	slider->loadProgressBarTexture("choke.png", Widget::TextureResType::PLIST);
	slider->loadSlidBallTextureNormal("hand.png", Widget::TextureResType::PLIST);
	slider->setAnchorPoint(Point::ANCHOR_MIDDLE);
	slider->setPosition(Vec2(origin.x + visibleSize.width * 0.486, origin.y + visibleSize.height * 0.899));
	this->addChild(slider, INTERFACE_FRONT);
}

void InGame::createLimit()
{
	// 제한 시간 설정
	limit = UserDefault::getInstance()->getFloatForKey("DURATION");

	// 배경 생성
	limitBack = Sprite::createWithSpriteFrameName("textbox.png");
	limitBack->setAnchorPoint(Point::ANCHOR_MIDDLE);
	limitBack->setPosition(Vec2(origin.x + visibleSize.width * 0.18, origin.y + visibleSize.height * 0.89));
	this->addChild(limitBack, INTERFACE_BACK);

	// 라벨 생성
	limitLabel = Label::createWithSystemFont("", "Courier", 48, Size(195, 51), TextHAlignment::RIGHT, TextVAlignment::CENTER);
	limitLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	limitLabel->setPosition(Vec2(limitBack->getPositionX() + 7, limitBack->getPositionY() - 4));
	if (limit > LIMIT_CHANGE)
	{
		limitLabel->setColor(Color3B(200, 200, 190));
	}
	else
	{
		limitLabel->setColor(Color3B(245, 0, 5));
	}
	this->addChild(limitLabel, INTERFACE_FRONT);

	// 아이콘 생성
	auto icon = Sprite::createWithSpriteFrameName("limit.png");
	icon->setAnchorPoint(Point::ANCHOR_MIDDLE);
	icon->setPosition(Vec2(origin.x + visibleSize.width * 0.103, origin.y + visibleSize.height * 0.885));
	this->addChild(icon, INTERFACE_FRONT);
}

void InGame::createName()
{
	// 배경 생성
	auto nameBack = Sprite::createWithSpriteFrameName("textbox.png");
	nameBack->setAnchorPoint(Point::ANCHOR_MIDDLE);
	nameBack->setPosition(Vec2(origin.x + visibleSize.width * 0.82, origin.y + visibleSize.height * 0.89));
	this->addChild(nameBack, INTERFACE_BACK);

	// 라벨 생성
	nameLabel = Label::createWithSystemFont(StringUtils::format("%s", (' ' + name + ' ').c_str()), "Courier", 48, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	nameLabel->setAnchorPoint(Point::ZERO);
	nameLabel->setColor(Color3B(200, 200, 190));

	// 레이어 생성
	auto nameLayer = Layer::create();
	nameLayer->setContentSize(Size(nameLabel->getContentSize().width, 51));
	nameLayer->setPosition(Vec2(nameBack->getPositionX() - 70, nameBack->getPositionY() - 29));

	// 라벨 출력
	nameLayer->addChild(nameLabel);

	// 스크롤 뷰 생성
	nameScroll = ScrollView::create(Size(195, 51), nameLayer);
	nameScroll->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	nameScroll->setPosition(Vec2(nameBack->getPositionX() - 70, nameBack->getPositionY() - 29));
	nameScroll->setDirection(ScrollView::Direction::HORIZONTAL);
	nameScroll->setBounceable(false);
	nameScroll->setTouchEnabled(false);
	this->addChild(nameScroll, INTERFACE_FRONT);

	// 아이콘 생성
	auto icon = Sprite::createWithSpriteFrameName("songname.png");
	icon->setAnchorPoint(Point::ANCHOR_MIDDLE);
	icon->setPosition(Vec2(origin.x + visibleSize.width * 0.743, origin.y + visibleSize.height * 0.885));
	this->addChild(icon, INTERFACE_FRONT);
}

void InGame::createCombo()
{
	// 콤보 스프라이트 출력
	comboLabel = Label::createWithTTF("Combo", "./font/THEPlaying.TTF", 50, Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::CENTER);
	comboLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	comboLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.87, origin.y + visibleSize.height * 0.76));
	comboLabel->setColor(Color3B::WHITE);
	comboLabel->setOpacity(0);
	this->addChild(comboLabel, INTERFACE_FRONT);

	// 콤보 라벨 출력
	numberLabel = Label::createWithTTF("", "./font/THEPlaying.TTF", 50, Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::CENTER);
	numberLabel->setString(StringUtils::format("%d", combo));
	numberLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	numberLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.9, origin.y + visibleSize.height * 0.69));
	numberLabel->setColor(Color3B::WHITE);
	numberLabel->setOpacity(0);
	this->addChild(numberLabel, INTERFACE_FRONT);
}

void InGame::setTime()
{
	if (speed == FAST)
	{
		time += FAST;
	}
	else if (speed == SLOW)
	{
		time += SLOW;
	}
	else
	{
		time += DEFAULT;
	}
}

void InGame::setSpeed()
{
	if (leftPress && !rightPress)
	{
		speed = SLOW;
		frameSpeed = FAST;
	}
	else if (!leftPress && rightPress)
	{
		speed = FAST;
		frameSpeed = SLOW;
	}
	else
	{
		speed = DEFAULT;
		frameSpeed = DEFAULT;
	}
	character->setActionSpeed(frameSpeed, BPM);
}

void InGame::setLimit()
{
	// 제한 시간 감소
	limit = limit > 0 ? limit - 1.0f / 60.0f : NULL;

	// 제한 시간 적용
	limitLabel->setString(StringUtils::format("%d:%2d", (int)limit / 60, (int)limit % 60));

	if (limitLabel->getColor() != Color3B(245, 0, 5) &&
		limit < LIMIT_CHANGE + 1)
	{
		// 제한 시간 색상 변경
		limitLabel->setColor(Color3B(245, 0, 5));
	}
}

void InGame::setCombo()
{
	// 콤보 스프라이트 액션
	comboLabel->stopAllActions();
	comboLabel->setOpacity(0);
	comboLabel->runAction(getComboAction());

	// 콤보 라벨 설정
	numberLabel->setString(StringUtils::format("%d", combo));

	// 콤보 라벨 액션
	numberLabel->stopAllActions();
	numberLabel->setOpacity(0);
	numberLabel->runAction(getComboAction());
}

void InGame::moveBackground()
{
	// 배경 이동
	background->setPosition(background->getPosition() + Point(-1 * (speed * SPEED_DEFAULT) / 2 * (BPM + 1), 0));
	backgroundSub->setPosition(backgroundSub->getPosition() + Point(-1 * (speed * SPEED_DEFAULT) / 2 * (BPM + 1), 0));

	// 배경이 넘어갔을 때
	if (background->getPosition().x <= -background->getContentSize().width)
	{
		// 배경 삭제
		this->removeChild(background, true);

		// 배경 새로 받아오기
		int random = rand() % BACKGROUND_KIND;
		background = Sprite::create(StringUtils::format("./img/background/ingame_%d.png", random));
		background->setAnchorPoint(Point::ZERO);
		background->setPosition(Point(backgroundSub->getContentSize().width + backgroundSub->getPositionX(), 0));
		this->addChild(background, BACKGROUND);
	}
	if (backgroundSub->getPosition().x <= -backgroundSub->getContentSize().width)
	{
		// 배경 삭제
		this->removeChild(backgroundSub, true);

		// 배경 새로 받아오기
		int random = rand() % BACKGROUND_KIND;
		backgroundSub = Sprite::create(StringUtils::format("./img/background/ingame_%d.png", random));
		backgroundSub->setAnchorPoint(Point::ZERO);
		backgroundSub->setPosition(Point(background->getContentSize().width + background->getPositionX(), 0));
		this->addChild(backgroundSub, BACKGROUND);
	}
}

void InGame::endBackground()
{
	switch (stateBackground)
	{
	case B_FINISH:
		// 배경 이동
		background_->setPosition(background_->getPosition() + Point(-1 * SPEED_DEFAULT / 2 * (BPM + 1), 0));

	case B_DEFAULT:
		// 배경 이동
		background->setPosition(background->getPosition() + Point(-1 * SPEED_DEFAULT / 2 * (BPM + 1), 0));
		backgroundSub->setPosition(backgroundSub->getPosition() + Point(-1 * SPEED_DEFAULT / 2 * (BPM + 1), 0));
		break;
	}

	// 배경이 넘어갔을 때
	if (background->getPosition().x <= -background->getContentSize().width)
	{
		// 배경 삭제
		this->removeChild(background, true);

		// 마무리 배경 받아오기
		background = Sprite::create("./img/background/ingame_end.png");
		background->setAnchorPoint(Point::ZERO);
		background->setPosition(Point(backgroundSub->getContentSize().width + backgroundSub->getPositionX(), 0));
		this->addChild(background, BACKGROUND);

		// 배경 이동
		if (stateBackground == B_DEFAULT)
		{
			background_->setPosition(background->getPosition());
		}

		// 배경 이동 상태 증가
		stateBackground++;
	}
	if (backgroundSub->getPosition().x <= -backgroundSub->getContentSize().width)
	{
		// 배경 삭제
		this->removeChild(backgroundSub, true);

		// 마무리 배경 받아오기
		backgroundSub = Sprite::create("./img/background/ingame_end.png");
		backgroundSub->setAnchorPoint(Point::ZERO);
		backgroundSub->setPosition(Point(background->getContentSize().width + background->getPositionX(), 0));
		this->addChild(backgroundSub, BACKGROUND);

		// 배경 이동
		if (stateBackground == B_DEFAULT)
		{
			background_->setPosition(backgroundSub->getPosition());
		}

		// 배경 이동 상태 증가
		stateBackground++;
	}
}

void InGame::moveName()
{
	int move = nameScroll->getViewSize().width - nameLabel->getContentSize().width;
	static int wait(60);

	if (move >= 0)
	{
		return;
	}
	else if (nameScroll->getContentOffset().x > move)
	{
		nameScroll->setContentOffset(Vec2(nameScroll->getContentOffset().x - 1.0, 0.0));
	}
	else if (wait)
	{
		wait--;
	}
	else
	{
		wait = 60;
		nameScroll->setContentOffset(Vec2(0.0, 0.0));
	}
}

void InGame::manageNote()
{
	// 노트 생성 체크
	if (note->createNoteCheck(time))
	{
		// 서브 뒤 노트 생성
		noteSprite = note->createSubBack(frameSpeed);
		if (noteSprite)
		{
			this->addChild(noteSprite, SUB_BACK);
		}

		// 서브 앞 노트 생성
		noteSprite = note->createSubFront(frameSpeed);
		if (noteSprite)
		{
			this->addChild(noteSprite, SUB_FRONT);
		}

		// 노트 생성
		noteSprite = note->createNote(frameSpeed);
		this->addChild(noteSprite, NOTE);
	}

	// 노트 이동
	note->moveNote(speed, SPEED_DEFAULT, BPM);

	// 노트 충돌 체크
	int tag = note->checkNoteCrash(characterSprite->getBoundingBox());

	if (tag)
	{
		// 효과음 출력
		AudioEngine::play2d("./sound/fx_fall.mp3");

		// 노트 밀어버림
		noteSprite = note->pushNote(tag);

		// 더 월드
		pauseWorld();

		// 넘어진 횟수 증가
		fall++;

		if (combo)
		{
			// 콤보 초기화
			combo = NULL;

			// 콤보 설정
			setCombo();
		}
	}

	// 노트 삭제 체크
	if (note->removeNoteCheck() &&
		note->removeSubBackCheck() &&
		note->removeSubFrontCheck())
	{
		// 서브 뒤 삭제
		noteSprite = note->removeSubBack();
		if (noteSprite)
		{
			this->removeChild(noteSprite, true);
		}

		// 서브 앞 삭제
		noteSprite = note->removeSubFront();
		if (noteSprite)
		{
			this->removeChild(noteSprite, true);
		}

		// 노트 삭제
		noteSprite = note->removeNote();
		this->removeChild(noteSprite, true);
	}
}

void InGame::pauseWorld()
{
	// 더 월드
	AudioEngine::pause(audioID);
	note->pauseNote();
	fps = NULL;
	state = WAIT;
	character->state = Character::STATE::F_ING;
	character->resetFrame();
}

void InGame::resumeWorld()
{
	if (stateTime >= WAIT_FRAMES)
	{
		// 그리고 시간은 흐른다
		AudioEngine::resume(audioID);
		note->resumeNote();
		character->resumeCharacter();
		state = PLAYING;
		stateTime = NULL;
		fps = NULL;

		// 슬라이딩 체크
		if (downPress)
		{
			character->slideArim(downPress, frameSpeed);
		}

		return;
	}
	else if(stateTime >= WAIT_FRAMES - WAIT_FRAMES * 0.15)
	{
		// 일어남 알림
		character->standArim();
	}

	// 시간 증가
	stateTime += ((float)BPM + 1.0) / 2;
}

void InGame::manageAudio()
{
	if (speed != DEFAULT)
	{
		AudioEngine::setCurrentTime(audioID, AudioEngine::getCurrentTime(audioID) + SOUND_DEFAULT * speed);
	}
}

void InGame::resetWorld()
{
	// 노트 삭제
	Sprite* temp;
	while (note->tagCount.remove < note->tagCount.create)
	{
		temp = note->removeSubBack();
		if (temp)
		{
			this->removeChild(temp);
		}
		temp = note->removeSubFront();
		if (temp)
		{
			this->removeChild(temp);
		}
		this->removeChild(note->removeNote());
	}

	// 캐릭터 삭제
	this->removeChild(characterSprite);

	// 그림자 삭제
	this->removeChild(shadowSprite);

	// 제한 시간 삭제
	this->removeChild(limitBack);
	this->removeChild(limitLabel);

	// 콤보 삭제
	this->removeChild(comboLabel);
	this->removeChild(numberLabel);

	// 배경 화면 삭제
	this->removeChild(background);
	this->removeChild(backgroundSub);
	this->removeChild(background_);

	// 할당 해제
	delete character;

	// 노래 정지
	AudioEngine::stop(audioID);

	// 초기화
	leftPress = FALSE;
	rightPress = FALSE;
	downPress = FALSE;
	fps = NULL;
	time = NULL;
	speed = DEFAULT;
	frameSpeed = DEFAULT;
	state = PLAYING;
	stateTime = NULL;
	stateBackground = B_DEFAULT;
	score = NULL;
	fall = NULL;
	combo = NULL;
	comboMax = NULL;
	note->tagCount = { 1, NULL };

	// 노트 초기화
	note->resetNote();

	// 캐릭터 출력
	createCharacter();

	// 콤보 출력
	createCombo();

	// 제한 시간 생성
	createLimit();

	// 배경 화면 출력
	createBackground();

	// 노래 출력
	audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name.c_str(), name.c_str()).c_str());

	// 노래 끝남 콜백 설정
	AudioEngine::setFinishCallback(audioID, CC_CALLBACK_0(InGame::songFinishCallback, this));
}

void InGame::saveData()
{
	// 최대 콤보 설정
	UserDefault::getInstance()->setIntegerForKey("COMBO", comboMax);

	// 들어온 시간 설정
	UserDefault::getInstance()->setFloatForKey("SCORE", score);

	// 넘어진 횟수 설정
	UserDefault::getInstance()->setIntegerForKey("FALL", fall);

	// 클리어 여부 설정
	UserDefault::getInstance()->setBoolForKey("CLEAR", limit > 0 ? TRUE : FALSE);
}

void InGame::manageCombo()
{
	// 노트 넘음 체크
	if (note->checkNoteOver(shadowSprite->getBoundingBox().getMinX()))
	{
		// 콤보 증가
		combo++;

		// 최대 콤보 설정
		comboMax = combo > comboMax ? combo : comboMax;

		// 콤보 설정
		setCombo();
	}
}

Sequence* InGame::getComboAction()
{
	auto action_1 = FadeIn::create(0.1f);
	auto action_2 = ScaleTo::create(0.2f, 1.5);
	auto action_3 = ScaleTo::create(0.2f, 1.0);
	auto action_4 = DelayTime::create(2.0);
	auto action_5 = FadeOut::create(0.5);
	auto comboAction = Sequence::create(action_1, action_2, action_3, action_4, action_5, NULL);
	return comboAction;
}