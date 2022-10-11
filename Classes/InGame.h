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

	// 상태 상수
	enum WORLD_STATE
	{
		PLAYING, WAIT, PAUSE, END
	};

private:
	Size visibleSize;
	Vec2 origin;

	// 노래 이름 변수
	std::string name;

	// 오디오 아이디 변수
	int audioID;

	// BPM 변수
	int BPM;

	// 속도 관련 변수
	float fps;
	float speed;
	float frameSpeed;

	// 배경화면 관련 변수
	Sprite* background;
	Sprite* backgroundSub;
	Sprite* background_;
	Sprite* backgroundLine;
	Sprite* backgroundLineSub;
	Sprite* backgroundLineSub2;

	// 인터페이스 관련 변수
	Slider* slider;
	Sprite* limitBack;
	Label* limitLabel;
	Label* nameLabel;
	ScrollView* nameScroll;
	Label* comboLabel;
	Label* numberLabel;

	// 캐릭터 관련 변수
	Character* character;
	Sprite* characterSprite;
	Sprite* shadowSprite;

	// 노트 관련 변수
	Note* note;
	Sprite* noteSprite;
	float time;

	// 상태 관련 변수
	int state;
	float stateTime;
	int stateBackground;
	int stateRemember;

	// 키 관련 변수
	bool leftPress;
	bool rightPress;
	bool downPress;

	// 기록 관련 변수
	float limit;
	float score;
	int fall;
	int combo;
	int comboMax;

	// 노래 길이 변수
	float duration;

	// 업데이트 함수
	void update(float delta);

	// 키 입력 함수
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)override;

	// 노래 끝남 콜백 함수
	void songFinishCallback();

	// 캐릭터 출력 함수
	void createCharacter();

	// 배경 화면 출력 함수
	void createBackground();

	// 진행바 출력 함수
	void createSlider();
	
	// 제한시간 출력 함수
	void createLimit();

	// 곡 이름 출력 함수
	void createName();

	// 콤보 출력 함수
	void createCombo();

	// 시간 설정 함수
	void setTime();

	// 속도 설정 함수
	void setSpeed();

	// 제한 시간 설정 함수
	void setLimit();

	// 콤보 설정 함수
	void setCombo();

	// 배경 화면 스크롤 함수
	void moveBackground();

	// 배경 화면 끝 함수
	void endBackground();

	// 곡 이름 스크롤 함수
	void moveName();

	// 노트 관리 함수
	void manageNote();

	// 더 월드 함수
	void pauseWorld();

	// 더 월드 해제 함수
	void resumeWorld();

	// 오디오 관리 함수
	void manageAudio();

	// 초기화 함수
	void resetWorld();

	// 데이터 저장 함수
	void saveData();

	// 콤보 관리 함수
	void manageCombo();

	// 콤보 액션 반환 함수
	Sequence* getComboAction();

	// 속도 상수
	const float DEFAULT = 1.0f;
	const float SLOW = 0.8f;
	const float FAST = 1.5f;

	// 기본 속도 상수
	const int SPEED_DEFAULT = 10;

	// 넘어짐 프레임 상수
	const int WAIT_FRAMES = 100;

	// 기본 노래 속도 상수
	const float SOUND_DEFAULT = 0.016666f;

	// 배경 화면 종류 상수
	const int BACKGROUND_KIND = 3;

	// 제한 시간 색깔 변화 시간 상수
	const float LIMIT_CHANGE = 30;

	// z오더 상수
	enum ZORDER
	{
		BACKGROUND, SHADOW, SUB_BACK, NOTE, CHARACTER, SUB_FRONT, INTERFACE_BACK, INTERFACE_FRONT
	};

	// 배경 화면 상태 상수
	enum STATE_BACKGROUND
	{
		B_DEFAULT, B_FINISH, B_END
	};

public:
	// 생성자
	InGame();

	// 소멸자
	~InGame();
};

#endif
