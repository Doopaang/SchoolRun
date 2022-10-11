#include "Note.h"
#include "InGame.h"

Note::Note(std::string _name) :head(NULL), tail(NULL), tagCount({ 1, NULL })
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ���� ����
	file = fopen(StringUtils::format("./song/%s/%s", _name.c_str(), _name.c_str()).c_str(), "r");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./img/notes/notes.plist");
}

Note::~Note()
{
	// ��Ʈ ����
	deleteList();
}

bool Note::createList()
{
	List *note = new List;
	
	// ���� �ҷ�����
	fscanf(file, "%d %d %d", &note->noteTag, &note->noteTime, &note->noteKind);
	note->state = _NONE;
	note->next = NULL;

	if (note->noteTag == 1 ||
		note->noteTag == tail->noteTag + 1)
	{
		if (!head)
		{
			// �Ӹ�, ���� ����
			head = note;
			tail = note;
		}
		else
		{
			// ���� ����
			tail->next = note;
			tail = note;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

Note::List* Note::searchList(int _tag)
{
	List* temp(head);

	if (_tag > tail->noteTag)
	{
		return NULL;
	}

	for (int count(1); count < _tag; count++)
	{
		temp = temp->next;
	}

	return temp;
}

bool Note::createNoteCheck(const float _time)
{
	List* temp(searchList(tagCount.create));

	return tagCount.create <= tail->noteTag &&
		_time >= temp->noteTime ?
		TRUE :
		FALSE;
}

Sprite* Note::createSubBack(const float _speed)
{
	List* temp(searchList(tagCount.create));

	if (!checkSub(temp, BACK))
	{
		return NULL;
	}

	// ��������Ʈ ����
	temp->subBackSprite = getSubBackSprite(temp);

	return temp->subBackSprite;
}

Sprite* Note::createSubFront(const float _speed)
{
	List* temp(searchList(tagCount.create));

	if (!checkSub(temp, FRONT))
	{
		return NULL;
	}

	// ��������Ʈ ����
	temp->subFrontSprite = getSubFrontSprite(temp);

	return temp->subFrontSprite;
}

Sprite* Note::createNote(const float _speed)
{
	List* temp(searchList(tagCount.create));

	// ��������Ʈ ����
	temp->noteSprite = getNoteSprite(temp);

	// �±� ī��Ʈ
	tagCount.create++;
	tagCount.remove ?
		NULL :
		tagCount.remove++;

	return temp->noteSprite;
}

bool Note::checkSub(List* _temp, bool _sub)
{
	// ���� �� ����
	if (_sub == BACK)
	{
		switch (_temp->noteKind)
		{
		case NAVIGATOR:
		case CONTAINER:
			return TRUE;

		default:
			return FALSE;
		}
	}
	// ���� �� ����
	else
	{
		switch (_temp->noteKind)
		{
		case STONE:
			return TRUE;

		default:
			return FALSE;
		}
	}
}

Sprite* Note::getSubBackSprite(List* _temp)
{
	switch (_temp->noteKind)
	{
	case NAVIGATOR:
		_temp->subBackSprite = Sprite::createWithSpriteFrameName("sub_back_navigator.png");
		_temp->subBackSprite->setAnchorPoint(Point::ZERO);
		_temp->subBackSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.2));
		break;

	case CONTAINER:
		_temp->subBackSprite = Sprite::createWithSpriteFrameName("sub_back_container.png");
		_temp->subBackSprite->setAnchorPoint(Point::ZERO);
		_temp->subBackSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.125));
		break;
	}

	return _temp->subBackSprite;
}

Sprite* Note::getSubFrontSprite(List* _temp)
{
	switch (_temp->noteKind)
	{
	case STONE:
		_temp->subFrontSprite = Sprite::createWithSpriteFrameName("sub_front_stone.png");
		_temp->subFrontSprite->setAnchorPoint(Point::ZERO);
		_temp->subFrontSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.13));
		break;
	}

	return _temp->subFrontSprite;
}

Sprite* Note::getNoteSprite(List* _temp)
{
	switch (_temp->noteKind)
	{
	case TIRE:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_tire.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.145));
		break;

	case CONTAINER:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_container.png");
		_temp->noteSprite->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height));
		break;

	case NAVIGATOR:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_navigator.png");
		_temp->noteSprite->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, _temp->subBackSprite->getPosition().y + _temp->subBackSprite->getContentSize().height));
		break;

	case BOX:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_box.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.14));
		break;

	case TRASH:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_trash.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.15));
		break;

	case LONGBOX:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_longbox.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.145));
		break;

	case LONGAD:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_longad.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.15));
		break;

	case HUMAN:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_human.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.15));
		break;

	case STONE:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_stone.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.13));
		break;

	case BARRICADE:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_barricade.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.1));
		break;

	case BIKE:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_bike.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.1));
		break;

	case SIGN:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_sign.png");
		_temp->noteSprite->setAnchorPoint(Point::ZERO + Point(0, 0.555f));
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height * 0.15 - 1));
		break;

	case CRAIN:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_crain.png");
		_temp->noteSprite->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height));
		break;

	case H_BEAM:
		_temp->noteSprite = Sprite::createWithSpriteFrameName("note_h-beam.png");
		_temp->noteSprite->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
		_temp->noteSprite->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height));
		break;
	}

	return _temp->noteSprite;
}

void Note::moveNote(const float _speed, const float _SPEED_DEFAULT, int _BPM)
{
	if (!tagCount.remove)
	{
		return;
	}

	List* temp(searchList(tagCount.remove));

	for (int count(tagCount.remove); count < tagCount.create; count++)
	{
		// ���� �� ��Ʈ �̵�
		if (checkSub(temp, BACK))
		{
			temp->subBackSprite->setPositionX(temp->subBackSprite->getPositionX() - 1 * (_speed * _SPEED_DEFAULT) / 2 * (_BPM + 1));
		}

		// ���� �� ��Ʈ �̵�
		if (checkSub(temp, FRONT))
		{
			temp->subFrontSprite->setPositionX(temp->subFrontSprite->getPositionX() - 1 * (_speed * _SPEED_DEFAULT) / 2 * (_BPM + 1));
		}

		// ��Ʈ �̵�
		temp->noteSprite->setPositionX(temp->noteSprite->getPositionX() - 1 * (_speed * _SPEED_DEFAULT) / 2 * (_BPM + 1));

		// temp ����
		temp = temp->next;
	}
}

bool Note::removeSubBackCheck()
{
	List* temp(searchList(tagCount.remove));

	if (!checkSub(temp, BACK))
	{
		return TRUE;
	}

	return tagCount.remove > NULL &&
		tagCount.remove < tagCount.create &&
		tagCount.remove <= tail->noteTag &&
		temp->subBackSprite->getPositionX() <= -temp->subBackSprite->getContentSize().width ?
		TRUE :
		FALSE;
}

bool Note::removeSubFrontCheck()
{
	List* temp(searchList(tagCount.remove));

	if (!checkSub(temp, FRONT))
	{
		return TRUE;
	}

	return tagCount.remove > NULL &&
		tagCount.remove < tagCount.create &&
		tagCount.remove <= tail->noteTag &&
		temp->subFrontSprite->getPositionX() <= -temp->subFrontSprite->getContentSize().width ?
		TRUE :
		FALSE;
}

bool Note::removeNoteCheck()
{
	List* temp(searchList(tagCount.remove));

	return tagCount.remove > NULL &&
		tagCount.remove < tagCount.create &&
		tagCount.remove <= tail->noteTag &&
		temp->noteSprite->getPositionX() <= -temp->noteSprite->getContentSize().width ?
		TRUE :
		FALSE;
}

Sprite* Note::removeSubBack()
{
	List* temp(searchList(tagCount.remove));

	if (!checkSub(temp, BACK))
	{
		return FALSE;
	}

	return temp->subBackSprite;
}

Sprite* Note::removeSubFront()
{
	List* temp(searchList(tagCount.remove));

	if (!checkSub(temp, FRONT))
	{
		return FALSE;
	}

	return temp->subFrontSprite;
}

Sprite* Note::removeNote()
{
	List* temp(searchList(tagCount.remove));

	// �±� ī��Ʈ
	tagCount.remove++;

	return temp->noteSprite;
}

int Note::checkNoteCrash(Rect _rect)
{
	if (!tagCount.remove)
	{
		return NULL;
	}

	List* temp(searchList(tagCount.remove));

	for (int count(tagCount.remove); count < tagCount.create; count++)
	{
		// �浹 üũ
		if (_rect.intersectsRect(temp->noteSprite->getBoundingBox()))
		{
			// ���� ����
			temp->state = FALL;

			return count;
		}

		// temp ����
		temp = temp->next;
	}
	
	return NULL;
}

Sprite* Note::pushNote(int _tag)
{
	List* temp(searchList(_tag));

	if (checkSub(temp, BACK))
	{
		temp->subBackSprite->setPositionX(-temp->subBackSprite->getContentSize().width);
	}
	if (checkSub(temp, FRONT))
	{
		temp->subFrontSprite->setPositionX(-temp->subFrontSprite->getContentSize().width);
	}
	temp->noteSprite->setPositionX(-temp->noteSprite->getContentSize().width);

	return temp->noteSprite;
}

void Note::pauseNote()
{
	List* temp(searchList(tagCount.remove));

	for (int count(tagCount.remove); count < tagCount.create; count++)
	{
		// �� ����
		if (checkSub(temp, BACK))
		{
			temp->subBackSprite->pauseSchedulerAndActions();
		}
		if (checkSub(temp, FRONT))
		{
			temp->subFrontSprite->pauseSchedulerAndActions();
		}
		temp->noteSprite->pauseSchedulerAndActions();

		// temp ����
		temp = temp->next;
	}
}

void Note::resumeNote()
{
	List* temp(searchList(tagCount.remove));

	for (int count(tagCount.remove); count < tagCount.create; count++)
	{
		// �׸��� �ð��� �帥��
		if (checkSub(temp, BACK))
		{
			temp->subBackSprite->resumeSchedulerAndActions();
		}
		if (checkSub(temp, FRONT))
		{
			temp->subFrontSprite->resumeSchedulerAndActions();
		}
		temp->noteSprite->resumeSchedulerAndActions();

		// temp ����
		temp = temp->next;
	}
}

void Note::deleteList()
{
	List* temp(head);

	while(temp->next)
	{
		temp = temp->next;
		delete head;
		head = temp;
	}
	delete temp;
}

bool Note::checkNoteOver(float _x)
{
	if (!tagCount.remove)
	{
		return FALSE;
	}

	List* temp(searchList(tagCount.remove));

	for (int count(tagCount.remove); count < tagCount.create; count++)
	{
		// ���� üũ
		if (temp->state == _NONE &&
			_x > temp->noteSprite->getBoundingBox().getMaxX())
		{
			// ���� ����
			temp->state = OVER;

			return TRUE;
		}

		// temp ����
		temp = temp->next;
	}

	return FALSE;
}

void Note::resetNote()
{
	List* temp(head);

	do
	{
		// ���� �ʱ�ȭ
		temp->state = _NONE;

		// ���� ����Ʈ�� �̵�
		temp = temp->next;
	} while (temp->next);
}