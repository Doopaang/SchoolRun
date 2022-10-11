#ifndef __INGAME_SCENE_H__
#define __INGAME_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui\UISlider.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace experimental;
using namespace cocos2d::ui;

class Character;
class Note;
class MenuLayer;

class InGame : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(InGame);

	// ���� ���
	enum WORLD_STATE
	{
		PLAYING, WAIT, PAUSE, END
	};

private:
	Size visibleSize;
	Vec2 origin;

	// �뷡 �̸� ����
	std::string name;

	// ����� ���̵� ����
	int audioID;

	// BPM ����
	int BPM;

	// �ӵ� ���� ����
	float fps;
	float speed;
	float frameSpeed;

	// ���ȭ�� ���� ����
	Sprite* background;
	Sprite* backgroundSub;
	Sprite* background_;
	Sprite* backgroundLine;
	Sprite* backgroundLineSub;
	Sprite* backgroundLineSub2;

	// �������̽� ���� ����
	Slider* slider;
	Sprite* limitBack;
	Label* limitLabel;
	Label* nameLabel;
	ScrollView* nameScroll;
	Label* comboLabel;
	Label* numberLabel;

	// ĳ���� ���� ����
	Character* character;
	Sprite* characterSprite;
	Sprite* shadowSprite;

	// ��Ʈ ���� ����
	Note* note;
	Sprite* noteSprite;
	float time;

	// ���� ���� ����
	int state;
	float stateTime;
	int stateBackground;
	int stateRemember;

	// Ű ���� ����
	bool leftPress;
	bool rightPress;
	bool downPress;

	// ��� ���� ����
	float limit;
	float score;
	int fall;
	int combo;
	int comboMax;

	// �뷡 ���� ����
	float duration;

	// ������Ʈ �Լ�
	void update(float delta);

	// Ű �Է� �Լ�
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)override;

	// �뷡 ���� �ݹ� �Լ�
	void songFinishCallback();

	// ĳ���� ��� �Լ�
	void createCharacter();

	// ��� ȭ�� ��� �Լ�
	void createBackground();

	// ����� ��� �Լ�
	void createSlider();
	
	// ���ѽð� ��� �Լ�
	void createLimit();

	// �� �̸� ��� �Լ�
	void createName();

	// �޺� ��� �Լ�
	void createCombo();

	// �ð� ���� �Լ�
	void setTime();

	// �ӵ� ���� �Լ�
	void setSpeed();

	// ���� �ð� ���� �Լ�
	void setLimit();

	// �޺� ���� �Լ�
	void setCombo();

	// ��� ȭ�� ��ũ�� �Լ�
	void moveBackground();

	// ��� ȭ�� �� �Լ�
	void endBackground();

	// �� �̸� ��ũ�� �Լ�
	void moveName();

	// ��Ʈ ���� �Լ�
	void manageNote();

	// �� ���� �Լ�
	void pauseWorld();

	// �� ���� ���� �Լ�
	void resumeWorld();

	// ����� ���� �Լ�
	void manageAudio();

	// �ʱ�ȭ �Լ�
	void resetWorld();

	// ������ ���� �Լ�
	void saveData();

	// �޺� ���� �Լ�
	void manageCombo();

	// �޺� �׼� ��ȯ �Լ�
	Sequence* getComboAction();

	// �ӵ� ���
	const float DEFAULT = 1.0f;
	const float SLOW = 0.8f;
	const float FAST = 1.5f;

	// �⺻ �ӵ� ���
	const int SPEED_DEFAULT = 10;

	// �Ѿ��� ������ ���
	const int WAIT_FRAMES = 100;

	// �⺻ �뷡 �ӵ� ���
	const float SOUND_DEFAULT = 0.016666f;

	// ��� ȭ�� ���� ���
	const int BACKGROUND_KIND = 3;

	// ���� �ð� ���� ��ȭ �ð� ���
	const float LIMIT_CHANGE = 30;

	// z���� ���
	enum ZORDER
	{
		BACKGROUND, SHADOW, SUB_BACK, NOTE, CHARACTER, SUB_FRONT, INTERFACE_BACK, INTERFACE_FRONT
	};

	// ��� ȭ�� ���� ���
	enum STATE_BACKGROUND
	{
		B_DEFAULT, B_FINISH, B_END
	};

public:
	// ������
	InGame();

	// �Ҹ���
	~InGame();
};

#endif
