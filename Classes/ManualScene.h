#ifndef __MANUAL_SCENE_H__
#define __MANUAL_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

// ������ ���
enum PAGE
{
	HOW2PLAY, NOTES_1, NOTES_2, NOTES_3, NOTES_4, NOTES_5, CREDITS
};

class Character;

class ManualScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(ManualScene);

	// Ű���� ���� �Լ�
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)override;

private:
	Size visibleSize;
	Vec2 origin;

	// Ÿ��Ʋ �� ����
	Label* titleLabel;

	// ������ �� ����
	Label* pageLabel;

	// �����̽� �� ����
	MenuItemSprite* spaceBar;

	// ESC ����
	MenuItemSprite* esc;

	// ���� ����Ű ����
	MenuItemSprite* left;

	// ���� ����Ű ����
	MenuItemSprite* up;

	// ������ ����Ű ����
	MenuItemSprite* right;

	// �Ʒ��� ����Ű ����
	MenuItemSprite* down;

	// ĳ���� ����
	Character* character;

	// ĳ���� ��������Ʈ ����
	Sprite* characterSprite;

	// �׸��� ��������Ʈ ����
	Sprite* shadowSprite;

	// ������ ���� ����
	Layer* pages[CREDITS + 1];

	// ������ ����
	int page;

	// ���� ���� ����
	bool leftPress;

	// ������ ���� ����
	bool rightPress;

	// �Ʒ��� ���� ����
	bool downPress;

	// fps ����
	float fps;

	// ������ �ӵ� ����
	float frameSpeed;

	// ������Ʈ �Լ�
	void update(float delta);

	// ���� ȭ�� ���� �Լ�
	void createBackground();

	// Ÿ��Ʋ �� ���� �Լ�
	void createTitleLabel();

	// ������ �� ���� �Լ�
	void createPageLabel();

	// �������̽� ��ư ���� �Լ�
	void createInterfaceButton();

	// Ű ���� ���̾� ���� �Լ�
	void createHow2Play();

	// ��Ʈ ���� 1 ���̾� ���� �Լ�
	void createNote1();

	// ��Ʈ ���� 2 ���̾� ���� �Լ�
	void createNote2();

	// ��Ʈ ���� 3 ���̾� ���� �Լ�
	void createNote3();

	// ��Ʈ ���� 4 ���̾� ���� �Լ�
	void createNote4();

	// ��Ʈ ���� ������ ���̾� ���� �Լ�
	void createNote5();

	// ũ���� ���̾� ���� �Լ�
	void createCredits();

	// Ÿ��Ʋ �� ���� ��ȯ �Լ�
	char* getTitleLabel();

	// ������ ���� �Լ�
	void setPage();

	// �ӵ� ���� �Լ�
	void setSpeed();

	// �⺻ �ӵ� ���
	const float DEFAULT = 1.0f;

	// ���� �ӵ� ���
	const float SLOW = 1.5f;

	// ���� �ӵ� ���
	const float FAST = 0.8f;

	// BPM ���
	const int BPM = 2;

public:
	// ������
	ManualScene();

	// �Ҹ���
	~ManualScene();
};

#endif
