#include "LoadingScene.h"
#include "AudioEngine.h"
#include "InGame.h"
#include "SongselectScene.h"
#include "Note.h"

LoadingScene::LoadingScene() :songLoad(FALSE), fxLoad(FALSE), time(NULL)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ���� �ҷ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/sprites/loading_sprites.plist");
}

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();

	auto layer = LoadingScene::create();
	scene->addChild(layer);

	return scene;
}

bool LoadingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// ������Ʈ
	this->scheduleUpdate();

	// �ε� ȭ�� ���
	createLoading();

	// ��� ���� ����
	AudioEngine::stopAll();

	// �뷡 �̸� ����
	name = UserDefault::getInstance()->getStringForKey("SONGNAME");

	// �뷡 �ε�
	AudioEngine::preload(StringUtils::format("./song/%s/%s.mp3", name.c_str(), name.c_str()), CC_CALLBACK_1(LoadingScene::songLoadCallback, this));

	// ȿ���� �ε�
	AudioEngine::preload("./sound/fx_fall.mp3", CC_CALLBACK_1(LoadingScene::fxLoadCallback, this));

	// ��Ʈ �Ҵ�
	note = new Note(name);

	// �ð� ����
	time = GetTickCount();

	return true;
}

void LoadingScene::update(float delta)
{
	if (GetTickCount() > time + 1.0 / 60)
	{
		// �ð� ����
		time = GetTickCount();

		// ��Ʈ �ε�
		if (songLoad &&
			fxLoad &&
			!note->createList())
		{
			// ��Ʈ ����
			UserDefault::getInstance()->setIntegerForKey("NOTE", (int)note);

			// ȭ�� ��ȯ
			Director::getInstance()->replaceScene(InGame::createScene());
		}
	}

}

void LoadingScene::songLoadCallback(bool isSuccess)
{
	CCLOG("songLoadCallback");

	if (isSuccess)
	{
		// �ε� �˸�
		songLoad = TRUE;
	}
	else
	{
		// ���ư���
		Director::getInstance()->replaceScene(SongselectScene::createScene());
	}
}

void LoadingScene::fxLoadCallback(bool isSuccess)
{
	CCLOG("fxLoadCallback");

	if (isSuccess)
	{
		// �ε� �˸�
		fxLoad = TRUE;
	}
	else
	{
		// ���ư���
		Director::getInstance()->replaceScene(SongselectScene::createScene());
	}
}

void LoadingScene::createLoading()
{
	auto loadinging = Sprite::createWithSpriteFrameName("_1.png");
	loadinging->setPosition(Vec2(origin.x + visibleSize.width * 0.85, origin.y + visibleSize.height * 0.15));
	this->addChild(loadinging);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.15f);

	for (int count = 1; count <= FRAMES; count++)
	{
		auto frame = SpriteFrameCache::getInstance()->spriteFrameByName(StringUtils::format("_%d.png", count));
		animation->addSpriteFrame(frame);
	}

	auto animate = Animate::create(animation);
	loadinging->runAction(RepeatForever::create(animate));
}