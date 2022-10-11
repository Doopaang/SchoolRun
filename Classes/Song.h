#ifndef __SONG_H__
#define __SONG_H__

#include "cocos2d.h"

USING_NS_CC;

class Song
{
public:
	// 노래 링크드 리스트 구조체
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

	// 리스트 머리 변수
	List* head;

	// 리스트 꼬리 변수
	List* tail;

	// 리스트 검색 함수
	List* searchList(int _index);

private:
	// 곡 리스트 작성 함수
	void writeList();

	// 링크드 리스트 생성 함수
	void loadList();

	// 링크드 리스트 제거 함수
	void removeList();

public:
	// 생성자
	Song();

	// 소멸자
	~Song();
};

#endif
