#ifndef __NOTE_INFO_H__
#define __NOTE_INFO_H__

#include "cocos2d.h"

USING_NS_CC;

class Note
{
public:
	// ī��Ʈ ����ü
	struct Tag
	{
		int create;
		int remove;
	};

	// ī��Ʈ ����
	Tag tagCount;

	// ����Ʈ ���� �Լ�
	bool createList();

	// ��Ʈ ���� üũ �Լ�
	bool createNoteCheck(const float _time);

	// ���� �� ��Ʈ ���� �Լ�
	Sprite* createSubBack(const float _speed);

	// ���� �� ��Ʈ ���� �Լ�
	Sprite* createSubFront(const float _speed);

	// ��Ʈ ���� �Լ�
	Sprite* createNote(const float _speed);

	// ��Ʈ �̵� �Լ�
	void moveNote(const float _speed, const float _SPEED_DEFAULT, int _BPM);

	// ���� �� ���� üũ �Լ�
	bool removeSubBackCheck();

	// ���� �� ���� üũ �Լ�
	bool removeSubFrontCheck();

	// ��Ʈ ���� üũ �Լ�
	bool removeNoteCheck();

	// ���� �� ���� �Լ�
	Sprite* removeSubBack();

	// ���� �� ���� �Լ�
	Sprite* removeSubFront();

	// ��Ʈ ���� �Լ�
	Sprite* removeNote();

	// ��Ʈ �浹 üũ �Լ�
	int checkNoteCrash(Rect _rect);

	// ��Ʈ �о���� �Լ�
	Sprite* pushNote(int _tag);

	// ��Ʈ ���� �Լ�
	void pauseNote();

	// ��Ʈ ����� �Լ�
	void resumeNote();

	// ��Ʈ ���� üũ �Լ�
	bool checkNoteOver(float _x);

	// ��Ʈ �ʱ�ȭ �Լ�
	void Note::resetNote();

private:
	Size visibleSize;
	Vec2 origin;

	// ���� ���� ����
	FILE* file;

	// ��Ʈ ��ũ�� ����Ʈ ����ü
	struct List
	{
		int noteTag;
		unsigned int noteTime;
		int noteKind;
		int state;
		Sprite* noteSprite;
		Sprite* subBackSprite;
		Sprite* subFrontSprite;
		List* next;
	};

	// ��ũ�� ����Ʈ �Ӹ� ����
	List* head;

	// ��ũ�� ����Ʈ ���� ����
	List* tail;

	// ����Ʈ �˻� �Լ�
	List* searchList(int _tag);

	// ����Ʈ ���� �Լ�
	void deleteList();

	// ���� ���� Ȯ�� �Լ�
	bool checkSub(List* _temp, bool _sub);

	// ���� �� ��������Ʈ ���� �Լ�
	Sprite* getSubBackSprite(List* _temp);

	// ���� �� ��������Ʈ ���� �Լ�
	Sprite* getSubFrontSprite(List* _temp);

	// ��Ʈ ��������Ʈ ���� �Լ�
	Sprite* getNoteSprite(List* _temp);

	// �⺻ �ӵ� ���
	const float SPEED_DEFAULT = 2.0f;

	// ��Ʈ ���� ���
	enum NOTE_KIND
	{
		NONE, TIRE, BOX, TRASH, BARRICADE, BIKE, SIGN, LONGBOX, LONGAD, HUMAN, NAVIGATOR, CONTAINER, STONE, CRAIN, H_BEAM
	};

	// ���� ���� ���
	enum SUB
	{
		BACK, FRONT
	};

	// ��Ʈ ���� ���
	enum OVER
	{
		_NONE, OVER, FALL
	};

public:
	// ������
	Note(std::string _name);

	// �Ҹ���
	~Note();
};

#endif
