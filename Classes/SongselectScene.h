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

	// 키보드 누름 함수
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;

private:
	Size visibleSize;
	Vec2 origin;

	// 노래 클래스 변수
	Song* song;

	// 리스트 레이어 변수
	Layer* listLayer;

	// 커서 스프라이트 변수
	Sprite* cursorSprite;

	// 곡 정보 레이어 변수
	Layer* infoLayer;

	// 곡 정보 라벨 변수
	Label* infoLabel;

	// 유저 정보 레이어 변수
	Layer* userLayer;

	// 유저 정보 라벨 변수
	Label* userLabel;

	// 곡 이미지 변수
	Sprite* image;

	// 메뉴 변수
	int menu;

	// 커서 변수
	int cursor;

	// 오프셋 변수
	int offset;

	// 리스트 최대치 변수
	int listMax;

	// 오디오 아이디 변수
	int audioID;

	// 리스트 삭제 콜백 함수
	void removeListCallback(Ref* sender);

	// 바탕 화면 생성 함수
	void createBackground();

	// 리스트 레이어 생성 함수
	void createLayer();

	// 리스트 생성 함수
	void createList();

	// 커서 생성 함수
	void createCursor();

	// 곡 정보 레이어 생성 함수
	void createInfoLayer();

	// 곡 정보 라벨 생성 함수
	void createInfoLabel();

	// 유저 정보 레이어 생성 함수
	void createUserLayer();

	// 유저 정보 라벨 생성 함수
	void createUserLabel();

	// 곡 이미지 생성 함수
	void createImage();

	// 오프셋 설정 함수
	void setOffset(bool _isUp);

	// 메뉴 설정 함수
	bool setMenu(bool _isUp);

	// 커서 설정 함수
	void setCursor(bool _isUp);

	// 노래 정보 불러오기
	void loadSongInfo();

	// 곡 정보 설정 함수
	void setInfoLabel();

	// 유저 정보 설정 함수
	void setUserLabel();

	// 내리기 액션 반환 함수
	MoveBy* getPushDownAction();

	// 올리기 액션 반환 함수
	MoveBy* getPushUpAction();

	// 삭제 액션 반환 함수
	Sequence* getRemoveAction();

	// 생성 액션 반환 함수
	Sequence* getCreateAction();

	// 스크로 뷰 너비 상수
	const int SCROLL_WIDTH = 500;

	// 스크롤 뷰 높이 상수
	const int SCROLL_HEIGHT = 450;

	// 노래 높이 상수
	const int SONG_HEIGHT = 80;

	// 리스트 최대치 상수
	const int LIST_MAX = 5;

	// 스크롤 속도 상수
	const float SCROLL_SPEED = 0.2f;

	// 포스트 잇 너비 상수
	const int POST_LENGTH = 250;

	// 곡 이미지 너비 상수
	const int IMAGE_LENGTH = 310;

public:
	// 생성자
	SongselectScene();
};

#endif
