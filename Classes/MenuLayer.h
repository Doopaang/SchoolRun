#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class MenuLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(MenuLayer);
	
	// �޴� ����
	int menu;

	// ���̾� Ȱ��ȭ ���� �Լ�
	void setLayerVisible();

	// �޴� ���� �Լ�
	void setMenu(bool _left);

	// �޴� ���
	enum MENU
	{
		RESTART, SONGSELECT, HOME
	};

private:
	Size visibleSize;
	Vec2 origin;

	// ��������Ʈ ���� ����
	Sprite* on[HOME + 1];

	// ��� �÷� ���̾� ���� �Լ�
	void createBackground();
	
	// �޴� ���� �Լ�
	void createMenu();

	// �޴� Ŀ�� ���� �Լ�
	void setMenuCursor(int _pastMenu);

	// z���� ���
	enum ZORDER
	{
		OFF, ON
	};

public:
	// ������
	MenuLayer();
};

#endif
