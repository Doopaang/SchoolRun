#ifndef __NOTE_INFO_H__
#define __NOTE_INFO_H__

#include "cocos2d.h"

USING_NS_CC;

class Note
{
public:
	// 카운트 구조체
	struct Tag
	{
		int create;
		int remove;
	};

	// 카운트 변수
	Tag tagCount;

	// 리스트 생성 함수
	bool createList();

	// 노트 생성 체크 함수
	bool createNoteCheck(const float _time);

	// 서브 뒤 노트 생성 함수
	Sprite* createSubBack(const float _speed);

	// 서브 앞 노트 생성 함수
	Sprite* createSubFront(const float _speed);

	// 노트 생성 함수
	Sprite* createNote(const float _speed);

	// 노트 이동 함수
	void moveNote(const float _speed, const float _SPEED_DEFAULT, int _BPM);

	// 서브 뒤 삭제 체크 함수
	bool removeSubBackCheck();

	// 서브 앞 삭제 체크 함수
	bool removeSubFrontCheck();

	// 노트 삭제 체크 함수
	bool removeNoteCheck();

	// 서브 뒤 삭제 함수
	Sprite* removeSubBack();

	// 서브 앞 삭제 함수
	Sprite* removeSubFront();

	// 노트 삭제 함수
	Sprite* removeNote();

	// 노트 충돌 체크 함수
	int checkNoteCrash(Rect _rect);

	// 노트 밀어버림 함수
	Sprite* pushNote(int _tag);

	// 노트 멈춤 함수
	void pauseNote();

	// 노트 재시작 함수
	void resumeNote();

	// 노트 넘음 체크 함수
	bool checkNoteOver(float _x);

	// 노트 초기화 함수
	void Note::resetNote();

private:
	Size visibleSize;
	Vec2 origin;

	// 파일 관련 변수
	FILE* file;

	// 노트 링크드 리스트 구조체
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

	// 링크드 리스트 머리 변수
	List* head;

	// 링크드 리스트 꼬리 변수
	List* tail;

	// 리스트 검색 함수
	List* searchList(int _tag);

	// 리스트 삭제 함수
	void deleteList();

	// 서브 존재 확인 함수
	bool checkSub(List* _temp, bool _sub);

	// 서브 뒤 스프라이트 설정 함수
	Sprite* getSubBackSprite(List* _temp);

	// 서브 앞 스프라이트 설정 함수
	Sprite* getSubFrontSprite(List* _temp);

	// 노트 스프라이트 설정 함수
	Sprite* getNoteSprite(List* _temp);

	// 기본 속도 상수
	const float SPEED_DEFAULT = 2.0f;

	// 노트 종류 상수
	enum NOTE_KIND
	{
		NONE, TIRE, BOX, TRASH, BARRICADE, BIKE, SIGN, LONGBOX, LONGAD, HUMAN, NAVIGATOR, CONTAINER, STONE, CRAIN, H_BEAM
	};

	// 서브 종류 상수
	enum SUB
	{
		BACK, FRONT
	};

	// 노트 넘음 상수
	enum OVER
	{
		_NONE, OVER, FALL
	};

public:
	// 생성자
	Note(std::string _name);

	// 소멸자
	~Note();
};

#endif
