#include "Character.h"
#include "InGame.h"

Character::Character(Vec2 _position) : state(DEFAULT), frame(NULL), yoyo(FALSE)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 캐릭터 설정
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/character/character.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/character/shadow.plist");

	// 캐릭터 스프라이트 생성
	character = Sprite::create();
	character->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	character->setPosition(_position);

	// 그림자 스프라이트 생성
	shadow = Sprite::create();
	shadow->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	shadow->setPosition(_position + Vec2(0.0f, -10.0f));

	// 액션 설정
	auto action_2 = MoveBy::create(JUMP_TIME * 0.5, Point(0, JUMP_HEIGHT));
	auto action_3 = EaseOut::create(action_2, EASE);
	auto action_4 = MoveTo::create(JUMP_TIME * 0.5, _position);
	auto action_5 = EaseIn::create(action_4, EASE);
	auto action_6 = CallFuncN::create((CC_CALLBACK_0(Character::jumpEndCallback, this)));
	jumping = Sequence::create(action_3, action_5, action_6, NULL);
	jumping->retain();

	comeback = MoveTo::create(0.2f, _position);
	comeback->retain();
}

Character::~Character()
{
	// 액션 해제
	jumping->release();
	comeback->release();
}

Sprite* Character::action(float* _fps, const float _frameSpeed, int _state)
{
	switch (_state)
	{
	case InGame::WORLD_STATE::PLAYING:
	case InGame::WORLD_STATE::END:
		switch (state)
		{
		// 점프 중일 때
		case J_SINGLE:
		case J_DOUBLE:
			if (*_fps >= _frameSpeed * JUMP_FPS)
			{
				// fps 초기화
				*_fps = NULL;

				// 프레임 넘기기
				animationJump(JUMP_FRAMES);

				// 애니메이션 출력
				animation(character, JUMP_FRAME_START);
			}
			break;

		// 슬라이드 중일 때
		case S_ING:
			if (*_fps >= _frameSpeed * SLIDE_FPS)
			{
				// fps 초기화
				*_fps = NULL;

				// 프레임 넘기기
				animationSlide(SLIDE_FRAMES, TRUE);

				// 애니메이션 출력
				animation(character, SLIDE_FRAME_START);
			}
			break;

		// 슬라이드가 끝났을 때
		case S_ED:
			if (*_fps >= _frameSpeed * SLIDED_FPS)
			{
				// fps 초기화
				*_fps = NULL;

				// 프레임 넘기기
				animationSlide(SLIDED_FRAMES, FALSE);

				// 애니메이션 출력
				animation(character, SLIDED_FRAME_START);
			}
			break;

		// 달리는 중일 때
		default:
			if (*_fps >= _frameSpeed * RUN_FPS)
			{
				// fps 초기화
				*_fps = NULL;

				// 프레임 넘기기
				animationRun(RUN_FRAMES);

				// 애니메이션 출력
				animation(character, RUN_FRAME_START);
			}
		}
		break;

	case InGame::WORLD_STATE::WAIT:
		switch (state)
		{
		// 넘어졌을 때
		case F_ING:
			if (*_fps >= _frameSpeed * FALL_FPS)
			{
				// fps 초기화
				*_fps = NULL;

				// 프레임 넘기기
				animationFallAndStand(FALL_FRAMES);

				// 애니메이션 출력
				animation(character, FALL_FRAME_START - 1);
			}
			break;

		// 일어날 때
		case F_ED:
			if (*_fps >= _frameSpeed * FALL_FPS)
			{
				// fps 초기화
				*_fps = NULL;

				// 프레임 넘기기
				animationFallAndStand(STAND_FRAMES);

				// 애니메이션 출력
				animation(character, STAND_FRAME_START - 1);
			}
			break;
		}
		break;
	}

	return character;
}

void Character::animationRun(const int _FRAMES)
{
	// 프레임 넘기기
	if (frame >= _FRAMES - 1)
	{
		resetFrame();
	}
	else
	{
		frame++;
	}
}

void Character::animationSlide(const int _FRAMES, bool _ing)
{
	if (frame < _FRAMES - 1)
	{
		// 프레임 넘기기
		frame++;
	}
	else if (frame == _FRAMES - 1 &&
		!_ing)
	{
		// 프레임 넘기기
		frame++;

		// 상태 설정
		state = DEFAULT;
	}
}

void Character::animationJump(const int _FRAMES)
{
	// 프레임 넘기기
	if (yoyo)
	{
		if (frame <= NULL + 1)
		{
			yoyo = FALSE;
			frame--;
		}
		else
		{
			frame--;
		}
	}
	else
	{
		if (frame >= _FRAMES - 1)
		{
			yoyo = TRUE;
			frame--;
		}
		else
		{
			frame++;
		}
	}
}

void Character::animationFallAndStand(const int _FRAMES)
{
	if (frame < _FRAMES)
	{
		// 프레임 넘기기
		frame++;
	}
}

void Character::animation(Sprite* _target, const int _FRAME_START)
{
	_target->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", frame + _FRAME_START)));
}

void Character::resetAction()
{
	character->stopAllActions();
	character->runAction(comeback);
}

void Character::jumpArim(bool _downPress, const float _frameSpeed)
{
	// 점프 액션
	if (!_downPress)
	{
		// 싱글 점프 액션
		if (state < J_SINGLE)
		{
			// 초기화
			state = J_SINGLE;
			yoyo = FALSE;
			resetFrame();

			// 점프
			character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", JUMP_FRAME_START)));
			
			// 액션 설정
			character->runAction(jumping);
		}
		// 더블 점프 액션
		else if (state < J_DOUBLE)
		{
			// 초기화
			state = J_DOUBLE;
			yoyo = FALSE;
			resetFrame();

			// 점프
			character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", JUMP_FRAME_START)));
			character->stopAllActions();

			// 액션 설정
			character->runAction(jumping);
		}
	}
}

void Character::slideArim(bool _downPress, const float _frameSpeed)
{
	if (_downPress)
	{
		// 초기화
		state = S_ING;
		yoyo = FALSE;
		resetFrame();

		// 위치 초기화
		resetAction();

		// 슬라이드
		character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", SLIDE_FRAME_START)));
	}
	else
	{
		// 초기화
		state = S_ED;
		resetFrame();

		// 돌아오기
		character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", SLIDED_FRAME_START)));
	}
}

void Character::standArim()
{
	if (state != F_ED)
	{
		// 초기화
		state = F_ED;
		resetFrame();
	}
}

void Character::resumeCharacter()
{
	// 그리고 시간은 흐른다
	state = DEFAULT;
	yoyo = FALSE;
	resetFrame();
}

void Character::resetFrame()
{
	frame = NULL;
}

bool Character::endRun(const int _speed, int _BPM)
{
	// 캐릭터 이동
	character->setPosition(character->getPosition() + Point(_speed / 2 * (_BPM + 1), 0));
	shadow->setPosition(shadow->getPosition() + Point(_speed / 2 * (_BPM + 1), 0));

	// 캐릭터가 화면을 나갔을 때
	if (character->getPositionX() >= origin.x + visibleSize.width)
	{
		return TRUE;
	}
	return FALSE;
}

void Character::setActionSpeed(const float _frameSpeed, int _BPM)
{
	if (_frameSpeed == 0.8f)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.25f * (_BPM + 1) / 2);
	}
	else if (_frameSpeed == 1.5f)
	{
		Director::getInstance()->getScheduler()->setTimeScale(0.666f * (_BPM + 1) / 2);
	}
	else
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f * (_BPM + 1) / 2);
	}
}

Sprite* Character::getShadow(float _fps, const float _frameSpeed, int _state)
{
	switch (_state)
	{
	case InGame::WORLD_STATE::PLAYING:
	case InGame::WORLD_STATE::END:
		switch (state)
		{
		// 점프 중일 때
		case J_SINGLE:
		case J_DOUBLE:
			if (_fps >= _frameSpeed * JUMP_FPS)
			{
				// 애니메이션 출력
				animation(shadow, JUMP_FRAME_START + IS_SHADOW);
			}
			break;

		// 슬라이드 중일 때
		case S_ING:
			if (_fps >= _frameSpeed * SLIDE_FPS)
			{
				// 애니메이션 출력
				animation(shadow, SLIDE_FRAME_START + IS_SHADOW);
			}
			break;

		// 슬라이드가 끝났을 때
		case S_ED:
			if (_fps >= _frameSpeed * SLIDED_FPS)
			{
				// 애니메이션 출력
				animation(shadow, SLIDED_FRAME_START + IS_SHADOW);
			}
			break;

		// 달리는 중일 때
		default:
			if (_fps >= _frameSpeed * RUN_FPS)
			{
				// 애니메이션 출력
				animation(shadow, RUN_FRAME_START + IS_SHADOW);
			}
		}
		break;

	case InGame::WORLD_STATE::WAIT:
		switch (state)
		{
		// 넘어졌을 때
		case F_ING:
			if (_fps >= _frameSpeed * FALL_FPS)
			{
				// 애니메이션 출력
				animation(shadow, FALL_FRAME_START - 1 + IS_SHADOW);
			}
			break;

		// 일어날 때
		case F_ED:
			if (_fps >= _frameSpeed * FALL_FPS)
			{
				// 애니메이션 출력
				animation(shadow, STAND_FRAME_START - 1 + IS_SHADOW);
			}
			break;
		}
		break;
	}

	return shadow;
}

void Character::jumpEndCallback()
{
	CCLOG("jumpEndCallback");

	switch (state)
	{
	case J_SINGLE:
	case J_DOUBLE:
		state = DEFAULT;
		break;
	}
}