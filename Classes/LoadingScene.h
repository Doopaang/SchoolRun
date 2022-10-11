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

	// ������Ʈ �Լ�
	void update(float delta);

	// �� �̸� ����
	std::string name;

	// ��Ʈ ����
	Note* note;

	// �뷡 �ҷ����� ����
	bool songLoad;

	// ȿ���� �ҷ����� ����
	bool fxLoad;

	// �ð� ����
	unsigned long time;

	// �뷡 �ҷ����� �ݹ� �Լ�
	void songLoadCallback(bool isSuccess);

	// ȿ���� �ҷ����� �ݹ� �Լ�
	void fxLoadCallback(bool isSuccess);

	// �ε� ���� �Լ�
	void createLoading();

	// �ε� ������ ���� ���
	const int FRAMES = 11;

public:
	// ������
	LoadingScene();
};

#endif
