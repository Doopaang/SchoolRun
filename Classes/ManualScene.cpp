#include "ManualScene.h"
#include "MainScene.h"
#include "SongselectScene.h"
#include "Character.h"
#include "AudioEngine.h"

ManualScene::ManualScene() : page(HOW2PLAY), leftPress(FALSE), rightPress(FALSE), downPress(FALSE), fps(NULL)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ���� �ҷ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites/manual_sprites.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/notes/notes.plist");

}

ManualScene::~ManualScene()
{
	// �Ҵ� ����
	delete character;
}

Scene* ManualScene::createScene()
{
	auto scene = Scene::create();

	auto layer = ManualScene::create();
	scene->addChild(layer);

	return scene;
}

bool ManualScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// ���� ȭ�� ����
	createBackground();

	// Ÿ��Ʋ �� ����
	createTitleLabel();

	// ������ �� ����
	createPageLabel();

	// �������̽� ��ư ����
	createInterfaceButton();

	// Ű ���� ���̾� ����
	createHow2Play();

	// ��Ʈ ���� 1 ���̾� ����
	createNote1();

	// ��Ʈ ���� 2 ���̾� ����
	createNote2();

	// ��Ʈ ���� 3 ���̾� ����
	createNote3();

	// ��Ʈ ���� 4 ���̾� ����
	createNote4();

	// ��Ʈ ���� ������ ���̾� ����
	createNote5();

	// ũ���� ���̾� ����
	createCredits();

	// Ű���� �̺�Ʈ ������
	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(ManualScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(ManualScene::onKeyReleased, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	// ������Ʈ
	this->scheduleUpdate();

	return true;
}

void ManualScene::update(float delta)
{
	// ������ ����
	fps += ((float)BPM + 1.0) / 2;

	// �ӵ� ����
	setSpeed();

	// �׸��� �ִϸ��̼�
	shadowSprite = character->getShadow(fps, frameSpeed, NULL);

	// ĳ���� �׼�
	characterSprite = character->action(&fps, frameSpeed, NULL);
}

void ManualScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		left->selected();
		leftPress = TRUE;
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		up->selected();
		character->jumpArim(downPress, frameSpeed);
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		right->selected();
		rightPress = TRUE;
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		down->selected();
		downPress = TRUE;
		character->slideArim(downPress, frameSpeed);
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		spaceBar->selected();
		AudioEngine::play2d("./sound/fx_page.mp3");
		setPage();
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		esc->selected();
		break;
	}
}

void ManualScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		left->unselected();
		leftPress = FALSE;
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		up->unselected();
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		right->unselected();
		rightPress = FALSE;
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		down->unselected();
		downPress = FALSE;
		character->slideArim(downPress, frameSpeed);
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
		spaceBar->unselected();
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		esc->unselected();
		Director::getInstance()->popScene();
	}
}

void ManualScene::createBackground()
{
	auto background = Sprite::create(StringUtils::format("./img/background/manual_background.png"));

	background->setPosition(Vec2(origin.x + visibleSize.width * 0.5,
		origin.y + visibleSize.height - background->getContentSize().height * 0.5));

	this->addChild(background, -1);
}

void ManualScene::createTitleLabel()
{
	titleLabel = Label::createWithTTF(UTF8(StringUtils::format("%s", getTitleLabel())), "./font/THEPlaying.TTF", 72, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	titleLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	titleLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.1, origin.y + visibleSize.height * 0.85));
	titleLabel->setColor(Color3B(255, 213, 89));
	this->addChild(titleLabel);
}

void ManualScene::createPageLabel()
{
	pageLabel = Label::createWithTTF(StringUtils::format("%d/%d", page + 1, CREDITS + 1), "./font/THEPlaying.TTF", 45, Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::CENTER);
	pageLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	pageLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.9, origin.y + visibleSize.height * 0.15));
	pageLabel->setColor(Color3B::WHITE);
	this->addChild(pageLabel);
}

void ManualScene::createInterfaceButton()
{
	// �����̽� �� ���
	auto spaceBarNormal = Sprite::createWithSpriteFrameName("spacebar_normal.png");
	auto spaceBarPress = Sprite::createWithSpriteFrameName("spacebar_press.png");

	spaceBar = MenuItemSprite::create(spaceBarNormal, spaceBarPress);
	spaceBar->setAnchorPoint(Point::ANCHOR_MIDDLE);
	spaceBar->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.15));
	spaceBar->setEnabled(false);
	this->addChild(spaceBar);

	// �����̽� �� �� ���
	auto spaceLabel = Label::createWithTTF(UTF8("���� ��������"), "./font/THEPlaying.TTF", 26, Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::CENTER);
	spaceLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	spaceLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.09));
	spaceLabel->setColor(Color3B::WHITE);
	spaceLabel->setOpacity(150);
	this->addChild(spaceLabel);

	// ESC ���
	auto escNormal = Sprite::createWithSpriteFrameName("esc_normal.png");
	auto escPress = Sprite::createWithSpriteFrameName("esc_press.png");

	esc = MenuItemSprite::create(escNormal, escPress);
	esc->setAnchorPoint(Point::ANCHOR_MIDDLE);
	esc->setPosition(Vec2(origin.x + visibleSize.width * 0.9, origin.y + visibleSize.height * 0.85));
	esc->setEnabled(false);
	this->addChild(esc);

	// ESC �� ���
	auto escLabel = Label::createWithTTF(UTF8("�ݱ�"), "./font/THEPlaying.TTF", 26, Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::CENTER);
	escLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	escLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.9, origin.y + visibleSize.height * 0.79));
	escLabel->setColor(Color3B::WHITE);
	escLabel->setOpacity(150);
	this->addChild(escLabel);
}

void ManualScene::createHow2Play()
{
	// ���̾� ����
	auto how2Play = Layer::create();
	how2Play->setVisible(true);
	this->addChild(how2Play);

	// ���� ����Ű ���
	auto leftNormal = Sprite::createWithSpriteFrameName("left_normal.png");
	auto leftPress = Sprite::createWithSpriteFrameName("left_press.png");

	left = MenuItemSprite::create(leftNormal, leftPress);
	left->setAnchorPoint(Point::ANCHOR_MIDDLE);
	left->setPosition(Vec2(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.45));
	left->setEnabled(false);
	how2Play->addChild(left);

	// ���� ����Ű ���
	auto upNormal = Sprite::createWithSpriteFrameName("up_normal.png");
	auto upPress = Sprite::createWithSpriteFrameName("up_press.png");

	up = MenuItemSprite::create(upNormal, upPress);
	up->setAnchorPoint(Point::ANCHOR_MIDDLE);
	up->setPosition(Vec2(origin.x + visibleSize.width * 0.26, origin.y + visibleSize.height * 0.54));
	up->setEnabled(false);
	how2Play->addChild(up);

	// ������ ����Ű ���
	auto rightNormal = Sprite::createWithSpriteFrameName("right_normal.png");
	auto rightPress = Sprite::createWithSpriteFrameName("right_press.png");

	right = MenuItemSprite::create(rightNormal, rightPress);
	right->setAnchorPoint(Point::ANCHOR_MIDDLE);
	right->setPosition(Vec2(origin.x + visibleSize.width * 0.32, origin.y + visibleSize.height * 0.45));
	right->setEnabled(false);
	how2Play->addChild(right);

	// �Ʒ��� ����Ű ���
	auto downNormal = Sprite::createWithSpriteFrameName("down_normal.png");
	auto downPress = Sprite::createWithSpriteFrameName("down_press.png");

	down = MenuItemSprite::create(downNormal, downPress);
	down->setAnchorPoint(Point::ANCHOR_MIDDLE);
	down->setPosition(Vec2(origin.x + visibleSize.width * 0.26, origin.y + visibleSize.height * 0.45));
	down->setEnabled(false);
	how2Play->addChild(down);

	// ����Ű �� ���
	auto arrowLabel = Label::createWithTTF(UTF8("����"), "./font/THEPlaying.TTF", 26, Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::CENTER);
	arrowLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	arrowLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.26, origin.y + visibleSize.height * 0.39));
	arrowLabel->setColor(Color3B::WHITE);
	arrowLabel->setOpacity(150);
	how2Play->addChild(arrowLabel);

	// ĳ���� ���
	Vec2 characterPosition = Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.35);

	character = new Character(characterPosition);
	shadowSprite = character->getShadow(fps, frameSpeed, NULL);
	how2Play->addChild(shadowSprite, 1);

	characterSprite = character->action(&fps, frameSpeed, NULL);
	how2Play->addChild(characterSprite, 2);

	// ������ �߰�
	pages[HOW2PLAY] = how2Play;
}

void ManualScene::createNote1()
{
	// ���̾� ����
	auto note1 = Layer::create();
	note1->setVisible(false);
	this->addChild(note1);

	// Ÿ�̾� ���
	auto tire = Sprite::createWithSpriteFrameName("note_tire.png");
	tire->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	tire->setPosition(Vec2(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.45));
	note1->addChild(tire);

	auto tireLabel = Label::createWithTTF(UTF8("Ÿ�̾�\n�ϴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	tireLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	tireLabel->setPosition(Vec2(tire->getPosition().x, origin.y + visibleSize.height * 0.35));
	note1->addChild(tireLabel);

	// ���� ���
	auto box = Sprite::createWithSpriteFrameName("note_box.png");
	box->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	box->setPosition(Vec2(origin.x + visibleSize.width * 0.4, origin.y + visibleSize.height * 0.45));
	note1->addChild(box);

	auto boxLabel = Label::createWithTTF(UTF8("����\n�ϴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	boxLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	boxLabel->setPosition(Vec2(box->getPosition().x, origin.y + visibleSize.height * 0.35));
	note1->addChild(boxLabel);

	// �������� ���
	auto trash = Sprite::createWithSpriteFrameName("note_trash.png");
	trash->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	trash->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.45));
	note1->addChild(trash);

	auto trashLabel = Label::createWithTTF(UTF8("������ ��\n�ϴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	trashLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	trashLabel->setPosition(Vec2(trash->getPosition().x, origin.y + visibleSize.height * 0.35));
	note1->addChild(trashLabel);

	// �ٸ����̵� ���
	auto barricade = Sprite::createWithSpriteFrameName("note_barricade.png");
	barricade->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	barricade->setPosition(Vec2(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.45));
	note1->addChild(barricade);

	auto barricadeLabel = Label::createWithTTF(UTF8("�ٸ����̵�\n�ϴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	barricadeLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	barricadeLabel->setPosition(Vec2(barricade->getPosition().x, origin.y + visibleSize.height * 0.35));
	note1->addChild(barricadeLabel);

	// ������ �߰�
	pages[NOTES_1] = note1;
}

void ManualScene::createNote2()
{
	// ���̾� ����
	auto note2 = Layer::create();
	note2->setVisible(false);
	this->addChild(note2);

	// ������ ���
	auto bike = Sprite::createWithSpriteFrameName("note_bike.png");
	bike->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	bike->setPosition(Vec2(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.45));
	note2->addChild(bike);

	auto bikeLabel = Label::createWithTTF(UTF8("������\n�ϴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	bikeLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	bikeLabel->setPosition(Vec2(bike->getPosition().x, origin.y + visibleSize.height * 0.35));
	note2->addChild(bikeLabel);

	// ǥ���� ���
	auto sign = Sprite::createWithSpriteFrameName("note_sign.png");
	sign->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	sign->setPosition(Vec2(origin.x + visibleSize.width * 0.4, origin.y + visibleSize.height * 0.45));
	note2->addChild(sign);

	auto signLabel = Label::createWithTTF(UTF8("ǥ����\n�ϴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	signLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	signLabel->setPosition(Vec2(sign->getPosition().x, origin.y + visibleSize.height * 0.35));
	note2->addChild(signLabel);

	// ���ڵ� ���
	auto longbox = Sprite::createWithSpriteFrameName("note_longbox.png");
	longbox->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	longbox->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.45));
	note2->addChild(longbox);

	auto longboxLabel = Label::createWithTTF(UTF8("���ڵ�\n�ϴ�, �ߴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	longboxLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	longboxLabel->setPosition(Vec2(longbox->getPosition().x, origin.y + visibleSize.height * 0.35));
	note2->addChild(longboxLabel);

	// ������ ���
	auto longad = Sprite::createWithSpriteFrameName("note_longad.png");
	longad->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	longad->setPosition(Vec2(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.45));
	note2->addChild(longad);

	auto longadLabel = Label::createWithTTF(UTF8("������\n�ϴ�, �ߴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	longadLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	longadLabel->setPosition(Vec2(longad->getPosition().x, origin.y + visibleSize.height * 0.35));
	note2->addChild(longadLabel);

	// ������ �߰�
	pages[NOTES_2] = note2;
}

void ManualScene::createNote3()
{
	// ���̾� ����
	auto note3 = Layer::create();
	note3->setVisible(false);
	this->addChild(note3);

	// ��� ���
	auto human = Sprite::createWithSpriteFrameName("note_human.png");
	human->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	human->setPosition(Vec2(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.45));
	note3->addChild(human);

	auto humanLabel = Label::createWithTTF(UTF8("���\n�ϴ�, �ߴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	humanLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	humanLabel->setPosition(Vec2(human->getPosition().x, origin.y + visibleSize.height * 0.35));
	note3->addChild(humanLabel);

	// �� �ȳ� ǥ���� ���
	auto navigatorSub = Sprite::createWithSpriteFrameName("sub_back_navigator.png");
	navigatorSub->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	navigatorSub->setPosition(Vec2(origin.x + visibleSize.width * 0.4, origin.y + visibleSize.height * 0.45));
	note3->addChild(navigatorSub);

	auto navigator = Sprite::createWithSpriteFrameName("note_navigator.png");
	navigator->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
	navigator->setPosition(Vec2(navigatorSub->getPosition().x - navigatorSub->getContentSize().width * 0.5, navigatorSub->getPosition().y + navigatorSub->getContentSize().height));
	note3->addChild(navigator);

	auto navigatorLabel = Label::createWithTTF(UTF8("�� �ȳ� ǥ����\n�ߴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	navigatorLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	navigatorLabel->setPosition(Vec2(navigatorSub->getPosition().x, origin.y + visibleSize.height * 0.35));
	note3->addChild(navigatorLabel);

	// �����̳� ���
	auto container = Sprite::createWithSpriteFrameName("note_container.png");
	container->setAnchorPoint(Point::ANCHOR_MIDDLE);
	container->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.75));
	note3->addChild(container);

	auto containerLabel = Label::createWithTTF(UTF8("�����̳�\n�ߴ�, ���"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	containerLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	containerLabel->setPosition(Vec2(container->getPosition().x, origin.y + visibleSize.height * 0.35));
	note3->addChild(containerLabel);

	// ������ �߰�
	pages[NOTES_3] = note3;
}

void ManualScene::createNote4()
{
	// ���̾� ����
	auto note4 = Layer::create();
	note4->setVisible(false);
	this->addChild(note4);

	// ���� ���
	auto stoneSub = Sprite::createWithSpriteFrameName("sub_front_stone.png");
	stoneSub->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	stoneSub->setPosition(Vec2(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.42));
	note4->addChild(stoneSub);

	auto stone = Sprite::createWithSpriteFrameName("note_stone.png");
	stone->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	stone->setPosition(Vec2(stoneSub->getPosition().x - stoneSub->getContentSize().width * 0.5, stoneSub->getPosition().y));
	note4->addChild(stone);

	auto stoneLabel = Label::createWithTTF(UTF8("����\n�ϴ�, �ߴ�"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	stoneLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	stoneLabel->setPosition(Vec2(stoneSub->getPosition().x, origin.y + visibleSize.height * 0.35));
	note4->addChild(stoneLabel);

	// ũ���� ���
	auto crain = Sprite::createWithSpriteFrameName("note_crain.png");
	crain->setAnchorPoint(Point::ANCHOR_MIDDLE);
	crain->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.7));
	note4->addChild(crain);

	auto crainLabel = Label::createWithTTF(UTF8("ũ����\n���"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	crainLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	crainLabel->setPosition(Vec2(crain->getPosition().x, origin.y + visibleSize.height * 0.35));
	note4->addChild(crainLabel);

	// ������ �߰�
	pages[NOTES_4] = note4;
}

void ManualScene::createNote5()
{
	// ���̾� ����
	auto note5 = Layer::create();
	note5->setVisible(false);
	this->addChild(note5);

	// H�� ���
	auto h_beam = Sprite::createWithSpriteFrameName("note_h-beam.png");
	h_beam->setAnchorPoint(Point::ANCHOR_MIDDLE);
	h_beam->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.75));
	note5->addChild(h_beam);

	auto h_beamLabel = Label::createWithTTF(UTF8("H��\n���"), "./font/THEPlaying.TTF", 32, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	h_beamLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	h_beamLabel->setPosition(Vec2(h_beam->getPosition().x, origin.y + visibleSize.height * 0.35));
	note5->addChild(h_beamLabel);

	// ������ �߰�
	pages[NOTES_5] = note5;
}

void ManualScene::createCredits()
{
	// ���̾� ����
	auto credits = Layer::create();
	credits->setVisible(false);
	this->addChild(credits);

	auto label = Label::createWithTTF(UTF8("��ȹ   20th �����\n\n���α׷���   20th ����ȯ\n\n�׷���   20th ������"), "./font/THEPlaying.TTF", 46, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	label->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
	label->setPosition(Vec2(origin.x + visibleSize.width * 0.1, origin.y + visibleSize.height * 0.7));
	label->setColor(Color3B::WHITE);
	credits->addChild(label);

	// ������ �߰�
	pages[CREDITS] = credits;
}

char* ManualScene::getTitleLabel()
{
	switch (page)
	{
	case HOW2PLAY:
		return "How To Play?";

	case NOTES_1:
	case NOTES_2:
	case NOTES_3:
	case NOTES_4:
	case NOTES_5:
		return "~ Notes ~";

	case CREDITS:
		return "Credits";
	}
	return NULL;
}

void ManualScene::setPage()
{
	// ������ �����
	pages[page]->setVisible(false);

	// ������ ���� ����
	if (page < CREDITS)
	{
		page++;
	}
	else
	{
		page = HOW2PLAY;
	}

	// Ÿ��Ʋ �� ����
	titleLabel->setString(UTF8(StringUtils::format("%s", getTitleLabel())));

	// ������ �� ����
	pageLabel->setString(StringUtils::format("%d/%d", page + 1, CREDITS + 1));

	// ������ ���̱�
	pages[page]->setVisible(true);
}

void ManualScene::setSpeed()
{
	if (leftPress && !rightPress)
	{
		frameSpeed = SLOW;
	}
	else if (!leftPress && rightPress)
	{
		frameSpeed = FAST;
	}
	else
	{
		frameSpeed = DEFAULT;
	}
	character->setActionSpeed(frameSpeed, BPM);
}
