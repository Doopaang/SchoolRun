#include "Character.h"
#include "InGame.h"

Character::Character(Vec2 _position) : state(DEFAULT), frame(NULL), yoyo(FALSE)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ĳ���� ����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/character/character.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/character/shadow.plist");

	// ĳ���� ��������Ʈ ����
	character = Sprite::create();
	character->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	character->setPosition(_position);

	// �׸��� ��������Ʈ ����
	shadow = Sprite::create();
	shadow->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	shadow->setPosition(_position + Vec2(0.0f, -10.0f));

	// �׼� ����
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
	// �׼� ����
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
		// ���� ���� ��
		case J_SINGLE:
		case J_DOUBLE:
			if (*_fps >= _frameSpeed * JUMP_FPS)
			{
				// fps �ʱ�ȭ
				*_fps = NULL;

				// ������ �ѱ��
				animationJump(JUMP_FRAMES);

				// �ִϸ��̼� ���
				animation(character, JUMP_FRAME_START);
			}
			break;

		// �����̵� ���� ��
		case S_ING:
			if (*_fps >= _frameSpeed * SLIDE_FPS)
			{
				// fps �ʱ�ȭ
				*_fps = NULL;

				// ������ �ѱ��
				animationSlide(SLIDE_FRAMES, TRUE);

				// �ִϸ��̼� ���
				animation(character, SLIDE_FRAME_START);
			}
			break;

		// �����̵尡 ������ ��
		case S_ED:
			if (*_fps >= _frameSpeed * SLIDED_FPS)
			{
				// fps �ʱ�ȭ
				*_fps = NULL;

				// ������ �ѱ��
				animationSlide(SLIDED_FRAMES, FALSE);

				// �ִϸ��̼� ���
				animation(character, SLIDED_FRAME_START);
			}
			break;

		// �޸��� ���� ��
		default:
			if (*_fps >= _frameSpeed * RUN_FPS)
			{
				// fps �ʱ�ȭ
				*_fps = NULL;

				// ������ �ѱ��
				animationRun(RUN_FRAMES);

				// �ִϸ��̼� ���
				animation(character, RUN_FRAME_START);
			}
		}
		break;

	case InGame::WORLD_STATE::WAIT:
		switch (state)
		{
		// �Ѿ����� ��
		case F_ING:
			if (*_fps >= _frameSpeed * FALL_FPS)
			{
				// fps �ʱ�ȭ
				*_fps = NULL;

				// ������ �ѱ��
				animationFallAndStand(FALL_FRAMES);

				// �ִϸ��̼� ���
				animation(character, FALL_FRAME_START - 1);
			}
			break;

		// �Ͼ ��
		case F_ED:
			if (*_fps >= _frameSpeed * FALL_FPS)
			{
				// fps �ʱ�ȭ
				*_fps = NULL;

				// ������ �ѱ��
				animationFallAndStand(STAND_FRAMES);

				// �ִϸ��̼� ���
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
	// ������ �ѱ��
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
		// ������ �ѱ��
		frame++;
	}
	else if (frame == _FRAMES - 1 &&
		!_ing)
	{
		// ������ �ѱ��
		frame++;

		// ���� ����
		state = DEFAULT;
	}
}

void Character::animationJump(const int _FRAMES)
{
	// ������ �ѱ��
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
		// ������ �ѱ��
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
	// ���� �׼�
	if (!_downPress)
	{
		// �̱� ���� �׼�
		if (state < J_SINGLE)
		{
			// �ʱ�ȭ
			state = J_SINGLE;
			yoyo = FALSE;
			resetFrame();

			// ����
			character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", JUMP_FRAME_START)));
			
			// �׼� ����
			character->runAction(jumping);
		}
		// ���� ���� �׼�
		else if (state < J_DOUBLE)
		{
			// �ʱ�ȭ
			state = J_DOUBLE;
			yoyo = FALSE;
			resetFrame();

			// ����
			character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", JUMP_FRAME_START)));
			character->stopAllActions();

			// �׼� ����
			character->runAction(jumping);
		}
	}
}

void Character::slideArim(bool _downPress, const float _frameSpeed)
{
	if (_downPress)
	{
		// �ʱ�ȭ
		state = S_ING;
		yoyo = FALSE;
		resetFrame();

		// ��ġ �ʱ�ȭ
		resetAction();

		// �����̵�
		character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", SLIDE_FRAME_START)));
	}
	else
	{
		// �ʱ�ȭ
		state = S_ED;
		resetFrame();

		// ���ƿ���
		character->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%d.png", SLIDED_FRAME_START)));
	}
}

void Character::standArim()
{
	if (state != F_ED)
	{
		// �ʱ�ȭ
		state = F_ED;
		resetFrame();
	}
}

void Character::resumeCharacter()
{
	// �׸��� �ð��� �帥��
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
	// ĳ���� �̵�
	character->setPosition(character->getPosition() + Point(_speed / 2 * (_BPM + 1), 0));
	shadow->setPosition(shadow->getPosition() + Point(_speed / 2 * (_BPM + 1), 0));

	// ĳ���Ͱ� ȭ���� ������ ��
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
		// ���� ���� ��
		case J_SINGLE:
		case J_DOUBLE:
			if (_fps >= _frameSpeed * JUMP_FPS)
			{
				// �ִϸ��̼� ���
				animation(shadow, JUMP_FRAME_START + IS_SHADOW);
			}
			break;

		// �����̵� ���� ��
		case S_ING:
			if (_fps >= _frameSpeed * SLIDE_FPS)
			{
				// �ִϸ��̼� ���
				animation(shadow, SLIDE_FRAME_START + IS_SHADOW);
			}
			break;

		// �����̵尡 ������ ��
		case S_ED:
			if (_fps >= _frameSpeed * SLIDED_FPS)
			{
				// �ִϸ��̼� ���
				animation(shadow, SLIDED_FRAME_START + IS_SHADOW);
			}
			break;

		// �޸��� ���� ��
		default:
			if (_fps >= _frameSpeed * RUN_FPS)
			{
				// �ִϸ��̼� ���
				animation(shadow, RUN_FRAME_START + IS_SHADOW);
			}
		}
		break;

	case InGame::WORLD_STATE::WAIT:
		switch (state)
		{
		// �Ѿ����� ��
		case F_ING:
			if (_fps >= _frameSpeed * FALL_FPS)
			{
				// �ִϸ��̼� ���
				animation(shadow, FALL_FRAME_START - 1 + IS_SHADOW);
			}
			break;

		// �Ͼ ��
		case F_ED:
			if (_fps >= _frameSpeed * FALL_FPS)
			{
				// �ִϸ��̼� ���
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