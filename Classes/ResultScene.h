#ifndef __SONGSELECT_SCENE_H__
#define __SONGSLECET_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

const std::string UTF8(const std::string& string);

class ResultScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(ResultScene);

	// Ű���� ���� �Լ�
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;

private:
	Size visibleSize;
	Vec2 origin;

	// Ŀ�� ����
	Sprite* songselectSelect;
	Sprite* restartSelect;

	// �뷡 �̸� ����
	std::string name;

	// ���� �ð� ����
	float score;

	// �Ѿ��� Ƚ�� ����
	int fall;

	// �ִ� �޺� ����
	int combo;

	// �޴� ����
	int menu;

	// ��� ȭ�� ���� �Լ�
	void createBackground();

	// �޴� ���� �Լ�
	void createMenu();

	// ��� ���� �Լ�
	void createResult();

	// �� ���� ���� �Լ�
	void createInfo();

	// Ŭ���� ���� ���� �Լ�
	void createClear();

	// ȭ�� ��ȯ �Լ�
	void changeScene();

	// Ŀ�� ���� �Լ�
	void setCursor();

	// ������ ���� �Լ�
	void saveData();

	// ���� ������ ���� �Լ�
	void saveUserData();

	// �̹��� �ʺ� ���
	const int IMAGE_LENGTH = 310;

	// �޴� ���
	enum MENU
	{
		RESTART, SONGSELECT
	};

	// Z���� ���
	enum ZORDER
	{
		IMAGE, BACKGROUND, OBJECT, OBJECT_UP
	};

public:
	// ������
	ResultScene();
};

#endif
