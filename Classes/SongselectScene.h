#ifndef __SONGSELECT_SCENE_H__
#define __SONGSLECET_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class Song;

class SongselectScene : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(SongselectScene);

	// Ű���� ���� �Լ�
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;

private:
	Size visibleSize;
	Vec2 origin;

	// �뷡 Ŭ���� ����
	Song* song;

	// ����Ʈ ���̾� ����
	Layer* listLayer;

	// Ŀ�� ��������Ʈ ����
	Sprite* cursorSprite;

	// �� ���� ���̾� ����
	Layer* infoLayer;

	// �� ���� �� ����
	Label* infoLabel;

	// ���� ���� ���̾� ����
	Layer* userLayer;

	// ���� ���� �� ����
	Label* userLabel;

	// �� �̹��� ����
	Sprite* image;

	// �޴� ����
	int menu;

	// Ŀ�� ����
	int cursor;

	// ������ ����
	int offset;

	// ����Ʈ �ִ�ġ ����
	int listMax;

	// ����� ���̵� ����
	int audioID;

	// ����Ʈ ���� �ݹ� �Լ�
	void removeListCallback(Ref* sender);

	// ���� ȭ�� ���� �Լ�
	void createBackground();

	// ����Ʈ ���̾� ���� �Լ�
	void createLayer();

	// ����Ʈ ���� �Լ�
	void createList();

	// Ŀ�� ���� �Լ�
	void createCursor();

	// �� ���� ���̾� ���� �Լ�
	void createInfoLayer();

	// �� ���� �� ���� �Լ�
	void createInfoLabel();

	// ���� ���� ���̾� ���� �Լ�
	void createUserLayer();

	// ���� ���� �� ���� �Լ�
	void createUserLabel();

	// �� �̹��� ���� �Լ�
	void createImage();

	// ������ ���� �Լ�
	void setOffset(bool _isUp);

	// �޴� ���� �Լ�
	bool setMenu(bool _isUp);

	// Ŀ�� ���� �Լ�
	void setCursor(bool _isUp);

	// �뷡 ���� �ҷ�����
	void loadSongInfo();

	// �� ���� ���� �Լ�
	void setInfoLabel();

	// ���� ���� ���� �Լ�
	void setUserLabel();

	// ������ �׼� ��ȯ �Լ�
	MoveBy* getPushDownAction();

	// �ø��� �׼� ��ȯ �Լ�
	MoveBy* getPushUpAction();

	// ���� �׼� ��ȯ �Լ�
	Sequence* getRemoveAction();

	// ���� �׼� ��ȯ �Լ�
	Sequence* getCreateAction();

	// ��ũ�� �� �ʺ� ���
	const int SCROLL_WIDTH = 500;

	// ��ũ�� �� ���� ���
	const int SCROLL_HEIGHT = 450;

	// �뷡 ���� ���
	const int SONG_HEIGHT = 80;

	// ����Ʈ �ִ�ġ ���
	const int LIST_MAX = 5;

	// ��ũ�� �ӵ� ���
	const float SCROLL_SPEED = 0.2f;

	// ����Ʈ �� �ʺ� ���
	const int POST_LENGTH = 250;

	// �� �̹��� �ʺ� ���
	const int IMAGE_LENGTH = 310;

public:
	// ������
	SongselectScene();
};

#endif
