#ifndef __SONG_H__
#define __SONG_H__

#include "cocos2d.h"

USING_NS_CC;

class Song
{
public:
	// �뷡 ��ũ�� ����Ʈ ����ü
	struct List
	{
		int index;
		std::string name;
		int level;
		int BPM;
		float duration;
		int combo;
		float score;
		int fall;
		Label* label;
		List* prev;
		List* next;
	};

	// ����Ʈ �Ӹ� ����
	List* head;

	// ����Ʈ ���� ����
	List* tail;

	// ����Ʈ �˻� �Լ�
	List* searchList(int _index);

private:
	// �� ����Ʈ �ۼ� �Լ�
	void writeList();

	// ��ũ�� ����Ʈ ���� �Լ�
	void loadList();

	// ��ũ�� ����Ʈ ���� �Լ�
	void removeList();

public:
	// ������
	Song();

	// �Ҹ���
	~Song();
};

#endif
