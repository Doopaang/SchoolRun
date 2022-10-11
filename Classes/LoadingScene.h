#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

class Note;

class LoadingScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(LoadingScene);

private:
	Size visibleSize;
	Vec2 origin;

	// 업데이트 함수
	void update(float delta);

	// 곡 이름 변수
	std::string name;

	// 노트 변수
	Note* note;

	// 노래 불러오기 변수
	bool songLoad;

	// 효과음 불러오기 변수
	bool fxLoad;

	// 시간 변수
	unsigned long time;

	// 노래 불러오기 콜백 함수
	void songLoadCallback(bool isSuccess);

	// 효과음 불러오기 콜백 함수
	void fxLoadCallback(bool isSuccess);

	// 로딩 생성 함수
	void createLoading();

	// 로딩 프레임 갯수 상수
	const int FRAMES = 11;

public:
	// 생성자
	LoadingScene();
};

#endif
