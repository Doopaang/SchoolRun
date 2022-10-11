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

	// ���� �ҷ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites/ingame_sprites.plist");

	// �뷡 �̸� ����
	name = UserDefault::getInstance()->getStringForKey("SONGNAME");

	// BPM ����
	BPM = UserDefault::getInstance()->getIntegerForKey("BPM");

	// �뷡 ���� ����
	duration = UserDefault::getInstance()->getFloatForKey("DURATION");
}

InGame::~InGame()
{
	// �Ҵ� ����
	delete character;

	// �뷡 ����
	AudioEngine::stop(audioID);

	// �뷡 ����
	AudioEngine::uncache(StringUtils::format("./song/%s/%s.mp3", name.c_str(), name.c_str()));
}

Scene* InGame::createScene()
{
    auto scene = Scene::create();
    
	// �ΰ��� ���̾� ����
    auto layer = InGame::create();
    scene->addChild(layer);

	// �޴� ���̾� ����
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

	// ��Ʈ �Ҵ�
	note = (Note*)UserDefault::getInstance()->getIntegerForKey("NOTE");
	note->tagCount = { 1, NULL };
	
	// ĳ���� ���
	createCharacter();

	// ��� ȭ�� ���
	createBackground();

	// ����� ���
	createSlider();

	// ���ѽð� ���
	createLimit();

	// �� �̸� ���
	createName();

	// �޺� ���
	createCombo();

	// Ű���� �̺�Ʈ ������
	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(InGame::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(InGame::onKeyReleased, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	// ������Ʈ
	this->scheduleUpdate();

	// �뷡 ���
	audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name.c_str(), name.c_str()).c_str());

	// �뷡 ���� �ݹ� ����
	AudioEngine::setFinishCallback(audioID, CC_CALLBACK_0(InGame::songFinishCallback, this));

	// ��Ʈ �ʱ�ȭ
	note->resetNote();

    return true;
}

void InGame::update(float delta)
{
	switch (state)
	{
	case PLAYING:
		// ������ ����
		fps += ((float)BPM + 1.0) / 2;

		// �ð� ����
		score += 1.0f / 60.0f;

		// ���� �ð� ����
		setLimit();

		// �ð� ����
		setTime();

		// �ӵ� ����
		setSpeed();

		// ��Ʈ ����
		manageNote();

		// �޺� ����
		manageCombo();

		// ����� ����
		manageAudio();

		// ����� ����
		slider->setPercent(AudioEngine::getCurrentTime(audioID) / duration * 100);

		// �� �̸� �̵�
		moveName();

		// �׸��� �ִϸ��̼�
		shadowSprite = character->getShadow(fps, frameSpeed, state);

		// ĳ���� �׼�
		characterSprite = character->action(&fps, frameSpeed, state);

		// ��� ȭ�� �̵�
		moveBackground();

		break;

	case WAIT:
		// ������ ����
		fps += ((float)BPM + 1.0) / 2;

		// �ð� ����
		score += 1.0f / 60.0f;

		// ���� �ð� ����
		setLimit();

		// �ӵ� ����
		character->setActionSpeed(DEFAULT, BPM);
		
		// �׸��� �ð��� �帥��
		resumeWorld();

		// ����� ����
		slider->setPercent(AudioEngine::getCurrentTime(audioID) / duration * 100);

		// �� �̸� �̵�
		moveName();

		// �׸��� �ִϸ��̼�
		shadowSprite = character->getShadow(fps, frameSpeed, state);

		// ĳ���� �׼�
		characterSprite = character->action(&fps, frameSpeed, state);

		break;

	case END:
		// ������ ����
		fps += ((float)BPM + 1.0) / 2;

		// �ð� ����
		score += 1.0f / 60.0f;

		// ���� �ð� ����
		setLimit();

		// �ð� ����
		setTime();

		// �ӵ� ����
		character->setActionSpeed(DEFAULT, BPM);

		// ��Ʈ ����
		manageNote();

		// ����� ����
		slider->setPercent(100);

		// �� �˸�
		if (stateBackground == B_END &&
			character->endRun(SPEED_DEFAULT, BPM))
		{
			// ������ ����
			saveData();

			// �׼� �ӵ� �ʱ�ȭ
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);

			// ȭ�� ��ȯ
			Director::getInstance()->replaceScene(ResultScene::createScene());
		}

		// �� �̸� �̵�
		moveName();

		// �׸��� �ִϸ��̼�
		shadowSprite = character->getShadow(fps, frameSpeed, state);

		// ĳ���� �׼�
		characterSprite = character->action(&fps, frameSpeed, state);

		// ��� ȭ�� �̵�
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
		// �޴� ���
		menuLayer->setLayerVisible();

		if (menuLayer->isVisible())
		{
			// �׼� �ӵ� �ʱ�ȭ
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);

			// �� ����
			AudioEngine::pause(audioID);
			characterSprite->pause();

			// ���� ���
			stateRemember = state;

			// ���� ����
			state = PAUSE;
		}
		else
		{
			// �׸��� �ð��� �帥��
			if (stateRemember == PLAYING)
			{
				AudioEngine::resume(audioID);
			}
			characterSprite->resume();

			// ���� ����
			state = stateRemember;

			// �����̵� üũ
			if (state == PLAYING)
			{
				character->slideArim(downPress, frameSpeed);
			}
		}
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		if (menuLayer->isVisible())
		{
			// �޴� ����
			menuLayer->setVisible(false);

			// �׼� �ӵ� �ʱ�ȭ
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

	// ��� �ʱ�ȭ
	if (character->state == Character::STATE::S_ING)
	{
		character->slideArim(FALSE, frameSpeed);
	}

	// ���� �˸�
	state = END;

	// �ӵ� �ʱ�ȭ
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
	// ��� ����
	auto blackboardBack = Sprite::createWithSpriteFrameName("background.png");
	blackboardBack->setAnchorPoint(Point::ANCHOR_MIDDLE);
	blackboardBack->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.91));
	this->addChild(blackboardBack, INTERFACE_BACK);

	// �����̴� ����
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
	// ���� �ð� ����
	limit = UserDefault::getInstance()->getFloatForKey("DURATION");

	// ��� ����
	limitBack = Sprite::createWithSpriteFrameName("textbox.png");
	limitBack->setAnchorPoint(Point::ANCHOR_MIDDLE);
	limitBack->setPosition(Vec2(origin.x + visibleSize.width * 0.18, origin.y + visibleSize.height * 0.89));
	this->addChild(limitBack, INTERFACE_BACK);

	// �� ����
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

	// ������ ����
	auto icon = Sprite::createWithSpriteFrameName("limit.png");
	icon->setAnchorPoint(Point::ANCHOR_MIDDLE);
	icon->setPosition(Vec2(origin.x + visibleSize.width * 0.103, origin.y + visibleSize.height * 0.885));
	this->addChild(icon, INTERFACE_FRONT);
}

void InGame::createName()
{
	// ��� ����
	auto nameBack = Sprite::createWithSpriteFrameName("textbox.png");
	nameBack->setAnchorPoint(Point::ANCHOR_MIDDLE);
	nameBack->setPosition(Vec2(origin.x + visibleSize.width * 0.82, origin.y + visibleSize.height * 0.89));
	this->addChild(nameBack, INTERFACE_BACK);

	// �� ����
	nameLabel = Label::createWithSystemFont(StringUtils::format("%s", (' ' + name + ' ').c_str()), "Courier", 48, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	nameLabel->setAnchorPoint(Point::ZERO);
	nameLabel->setColor(Color3B(200, 200, 190));

	// ���̾� ����
	auto nameLayer = Layer::create();
	nameLayer->setContentSize(Size(nameLabel->getContentSize().width, 51));
	nameLayer->setPosition(Vec2(nameBack->getPositionX() - 70, nameBack->getPositionY() - 29));

	// �� ���
	nameLayer->addChild(nameLabel);

	// ��ũ�� �� ����
	nameScroll = ScrollView::create(Size(195, 51), nameLayer);
	nameScroll->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	nameScroll->setPosition(Vec2(nameBack->getPositionX() - 70, nameBack->getPositionY() - 29));
	nameScroll->setDirection(ScrollView::Direction::HORIZONTAL);
	nameScroll->setBounceable(false);
	nameScroll->setTouchEnabled(false);
	this->addChild(nameScroll, INTERFACE_FRONT);

	// ������ ����
	auto icon = Sprite::createWithSpriteFrameName("songname.png");
	icon->setAnchorPoint(Point::ANCHOR_MIDDLE);
	icon->setPosition(Vec2(origin.x + visibleSize.width * 0.743, origin.y + visibleSize.height * 0.885));
	this->addChild(icon, INTERFACE_FRONT);
}

void InGame::createCombo()
{
	// �޺� ��������Ʈ ���
	comboLabel = Label::createWithTTF("Combo", "./font/THEPlaying.TTF", 50, Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::CENTER);
	comboLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	comboLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.87, origin.y + visibleSize.height * 0.76));
	comboLabel->setColor(Color3B::WHITE);
	comboLabel->setOpacity(0);
	this->addChild(comboLabel, INTERFACE_FRONT);

	// �޺� �� ���
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
	// ���� �ð� ����
	limit = limit > 0 ? limit - 1.0f / 60.0f : NULL;

	// ���� �ð� ����
	limitLabel->setString(StringUtils::format("%d:%2d", (int)limit / 60, (int)limit % 60));

	if (limitLabel->getColor() != Color3B(245, 0, 5) &&
		limit < LIMIT_CHANGE + 1)
	{
		// ���� �ð� ���� ����
		limitLabel->setColor(Color3B(245, 0, 5));
	}
}

void InGame::setCombo()
{
	// �޺� ��������Ʈ �׼�
	comboLabel->stopAllActions();
	comboLabel->setOpacity(0);
	comboLabel->runAction(getComboAction());

	// �޺� �� ����
	numberLabel->setString(StringUtils::format("%d", combo));

	// �޺� �� �׼�
	numberLabel->stopAllActions();
	numberLabel->setOpacity(0);
	numberLabel->runAction(getComboAction());
}

void InGame::moveBackground()
{
	// ��� �̵�
	background->setPosition(background->getPosition() + Point(-1 * (speed * SPEED_DEFAULT) / 2 * (BPM + 1), 0));
	backgroundSub->setPosition(backgroundSub->getPosition() + Point(-1 * (speed * SPEED_DEFAULT) / 2 * (BPM + 1), 0));

	// ����� �Ѿ�� ��
	if (background->getPosition().x <= -background->getContentSize().width)
	{
		// ��� ����
		this->removeChild(background, true);

		// ��� ���� �޾ƿ���
		int random = rand() % BACKGROUND_KIND;
		background = Sprite::create(StringUtils::format("./img/background/ingame_%d.png", random));
		background->setAnchorPoint(Point::ZERO);
		background->setPosition(Point(backgroundSub->getContentSize().width + backgroundSub->getPositionX(), 0));
		this->addChild(background, BACKGROUND);
	}
	if (backgroundSub->getPosition().x <= -backgroundSub->getContentSize().width)
	{
		// ��� ����
		this->removeChild(backgroundSub, true);

		// ��� ���� �޾ƿ���
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
		// ��� �̵�
		background_->setPosition(background_->getPosition() + Point(-1 * SPEED_DEFAULT / 2 * (BPM + 1), 0));

	case B_DEFAULT:
		// ��� �̵�
		background->setPosition(background->getPosition() + Point(-1 * SPEED_DEFAULT / 2 * (BPM + 1), 0));
		backgroundSub->setPosition(backgroundSub->getPosition() + Point(-1 * SPEED_DEFAULT / 2 * (BPM + 1), 0));
		break;
	}

	// ����� �Ѿ�� ��
	if (background->getPosition().x <= -background->getContentSize().width)
	{
		// ��� ����
		this->removeChild(background, true);

		// ������ ��� �޾ƿ���
		background = Sprite::create("./img/background/ingame_end.png");
		background->setAnchorPoint(Point::ZERO);
		background->setPosition(Point(backgroundSub->getContentSize().width + backgroundSub->getPositionX(), 0));
		this->addChild(background, BACKGROUND);

		// ��� �̵�
		if (stateBackground == B_DEFAULT)
		{
			background_->setPosition(background->getPosition());
		}

		// ��� �̵� ���� ����
		stateBackground++;
	}
	if (backgroundSub->getPosition().x <= -backgroundSub->getContentSize().width)
	{
		// ��� ����
		this->removeChild(backgroundSub, true);

		// ������ ��� �޾ƿ���
		backgroundSub = Sprite::create("./img/background/ingame_end.png");
		backgroundSub->setAnchorPoint(Point::ZERO);
		backgroundSub->setPosition(Point(background->getContentSize().width + background->getPositionX(), 0));
		this->addChild(backgroundSub, BACKGROUND);

		// ��� �̵�
		if (stateBackground == B_DEFAULT)
		{
			background_->setPosition(backgroundSub->getPosition());
		}

		// ��� �̵� ���� ����
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
	// ��Ʈ ���� üũ
	if (note->createNoteCheck(time))
	{
		// ���� �� ��Ʈ ����
		noteSprite = note->createSubBack(frameSpeed);
		if (noteSprite)
		{
			this->addChild(noteSprite, SUB_BACK);
		}

		// ���� �� ��Ʈ ����
		noteSprite = note->createSubFront(frameSpeed);
		if (noteSprite)
		{
			this->addChild(noteSprite, SUB_FRONT);
		}

		// ��Ʈ ����
		noteSprite = note->createNote(frameSpeed);
		this->addChild(noteSprite, NOTE);
	}

	// ��Ʈ �̵�
	note->moveNote(speed, SPEED_DEFAULT, BPM);

	// ��Ʈ �浹 üũ
	int tag = note->checkNoteCrash(characterSprite->getBoundingBox());

	if (tag)
	{
		// ȿ���� ���
		AudioEngine::play2d("./sound/fx_fall.mp3");

		// ��Ʈ �о����
		noteSprite = note->pushNote(tag);

		// �� ����
		pauseWorld();

		// �Ѿ��� Ƚ�� ����
		fall++;

		if (combo)
		{
			// �޺� �ʱ�ȭ
			combo = NULL;

			// �޺� ����
			setCombo();
		}
	}

	// ��Ʈ ���� üũ
	if (note->removeNoteCheck() &&
		note->removeSubBackCheck() &&
		note->removeSubFrontCheck())
	{
		// ���� �� ����
		noteSprite = note->removeSubBack();
		if (noteSprite)
		{
			this->removeChild(noteSprite, true);
		}

		// ���� �� ����
		noteSprite = note->removeSubFront();
		if (noteSprite)
		{
			this->removeChild(noteSprite, true);
		}

		// ��Ʈ ����
		noteSprite = note->removeNote();
		this->removeChild(noteSprite, true);
	}
}

void InGame::pauseWorld()
{
	// �� ����
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
		// �׸��� �ð��� �帥��
		AudioEngine::resume(audioID);
		note->resumeNote();
		character->resumeCharacter();
		state = PLAYING;
		stateTime = NULL;
		fps = NULL;

		// �����̵� üũ
		if (downPress)
		{
			character->slideArim(downPress, frameSpeed);
		}

		return;
	}
	else if(stateTime >= WAIT_FRAMES - WAIT_FRAMES * 0.15)
	{
		// �Ͼ �˸�
		character->standArim();
	}

	// �ð� ����
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
	// ��Ʈ ����
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

	// ĳ���� ����
	this->removeChild(characterSprite);

	// �׸��� ����
	this->removeChild(shadowSprite);

	// ���� �ð� ����
	this->removeChild(limitBack);
	this->removeChild(limitLabel);

	// �޺� ����
	this->removeChild(comboLabel);
	this->removeChild(numberLabel);

	// ��� ȭ�� ����
	this->removeChild(background);
	this->removeChild(backgroundSub);
	this->removeChild(background_);

	// �Ҵ� ����
	delete character;

	// �뷡 ����
	AudioEngine::stop(audioID);

	// �ʱ�ȭ
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

	// ��Ʈ �ʱ�ȭ
	note->resetNote();

	// ĳ���� ���
	createCharacter();

	// �޺� ���
	createCombo();

	// ���� �ð� ����
	createLimit();

	// ��� ȭ�� ���
	createBackground();

	// �뷡 ���
	audioID = AudioEngine::play2d(StringUtils::format("./song/%s/%s.mp3", name.c_str(), name.c_str()).c_str());

	// �뷡 ���� �ݹ� ����
	AudioEngine::setFinishCallback(audioID, CC_CALLBACK_0(InGame::songFinishCallback, this));
}

void InGame::saveData()
{
	// �ִ� �޺� ����
	UserDefault::getInstance()->setIntegerForKey("COMBO", comboMax);

	// ���� �ð� ����
	UserDefault::getInstance()->setFloatForKey("SCORE", score);

	// �Ѿ��� Ƚ�� ����
	UserDefault::getInstance()->setIntegerForKey("FALL", fall);

	// Ŭ���� ���� ����
	UserDefault::getInstance()->setBoolForKey("CLEAR", limit > 0 ? TRUE : FALSE);
}

void InGame::manageCombo()
{
	// ��Ʈ ���� üũ
	if (note->checkNoteOver(shadowSprite->getBoundingBox().getMinX()))
	{
		// �޺� ����
		combo++;

		// �ִ� �޺� ����
		comboMax = combo > comboMax ? combo : comboMax;

		// �޺� ����
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