#include "LoadingScene.h"
#include "AudioEngine.h"
#include "InGame.h"
#include "SongselectScene.h"
#include "Note.h"

LoadingScene::LoadingScene() :songLoad(FALSE), fxLoad(FALSE), time(NULL)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 파일 불러오기
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

	// 업데이트
	this->scheduleUpdate();

	// 로딩 화면 출력
	createLoading();

	// 모든 사운드 종료
	AudioEngine::stopAll();

	// 노래 이름 설정
	name = UserDefault::getInstance()->getStringForKey("SONGNAME");

	// 노래 로딩
	AudioEngine::preload(StringUtils::format("./song/%s/%s.mp3", name.c_str(), name.c_str()), CC_CALLBACK_1(LoadingScene::songLoadCallback, this));

	// 효과음 로딩
	AudioEngine::preload("./sound/fx_fall.mp3", CC_CALLBACK_1(LoadingScene::fxLoadCallback, this));

	// 노트 할당
	note = new Note(name);

	// 시간 저장
	time = GetTickCount();

	return true;
}

void LoadingScene::update(float delta)
{
	if (GetTickCount() > time + 1.0 / 60)
	{
		// 시간 저장
		time = GetTickCount();

		// 노트 로딩
		if (songLoad &&
			fxLoad &&
			!note->createList())
		{
			// 노트 저장
			UserDefault::getInstance()->setIntegerForKey("NOTE", (int)note);

			// 화면 전환
			Director::getInstance()->replaceScene(InGame::createScene());
		}
	}

}

void LoadingScene::songLoadCallback(bool isSuccess)
{
	CCLOG("songLoadCallback");

	if (isSuccess)
	{
		// 로딩 알림
		songLoad = TRUE;
	}
	else
	{
		// 돌아가기
		Director::getInstance()->replaceScene(SongselectScene::createScene());
	}
}

void LoadingScene::fxLoadCallback(bool isSuccess)
{
	CCLOG("fxLoadCallback");

	if (isSuccess)
	{
		// 로딩 알림
		fxLoad = TRUE;
	}
	else
	{
		// 돌아가기
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