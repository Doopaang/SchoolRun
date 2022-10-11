#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

const std::string UTF8(const std::string& string);

// �޴� ���� ���
enum MENU
{
	START, MANUAL, EXIT
};

class MainScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(MainScene);

	// ���� ����
	bool can;

	// Ű���� ���� �Լ�
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;

	// ȭ�� ��ȯ �Լ�
	void changeScene();

private:
	Size visibleSize;
	Vec2 origin;

	// �޴� ���� ����
	Sprite* menus[EXIT + 1];

	// �޴� ���� ����
	int menu;

	// �ִϸ��̼� �ݹ� �Լ�
	void animationCallback(Ref* sender);

	// ����Ʈ1 �ݹ� �Լ�
	void playPaint1Callback();

	// ����Ʈ2 �ݹ� �Լ�
	void playPaint2Callback();

	// ��� ȭ�� ���� �Լ�
	void createBackground();

	// ��ư ���� �Լ�
	void createButton();

	// �ִϸ��̼� ���� �Լ�
	void createAnimation();

	// �޴� ���� �Լ�
	void setMenu(bool _isUp);

	// ��ư ���� ��� 70
	const int button_height = 70;

public:
	// ������
	MainScene();
};

#endif
