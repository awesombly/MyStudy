#include "MyObject.h"
#include "ObjectManager.h"
#include "SoundManager.h"

const float MyObject::GravityPower  = 9.8f * 130.0f;
const float MyObject::BounceGravity = 9.8f * 80.0f;

bool MyObject::Init() noexcept
{
	MyActor::Init();
	m_isRepeat = true;
	m_canNextSprite = false;
	m_bouncePower = 0.0f;
	m_bounceUnit = { 0.0f, 0.0f };
	m_curGravityPower = 0.0f;
	m_curSPF = 0.0f;
	
	m_dirPoint = { 0.0f, 0.0f };
	m_healthPoint = 100;
	//m_damage = 10;
	//m_speed = 100;
	m_delayDelete = 0.0f;
	m_originWH = getWH();
	return true;
}

bool MyObject::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!MyActor::Frame(spf, accTime))	return false;
	m_curSPF = spf;
	m_prevPosition = *this;

	if(m_objType == EObjType::EnemyAttack)
	{
		if ((m_delayDelete += spf) >= 8.0f)
			setEnable(false, true);
		Move(m_dirPoint);
	}
	else// if (m_objType == EObjType::PlayerAttack)
	{
		if ((m_delayDelete += spf) >= 4.0f)
			setEnable(false, true);
		operBounce(spf);
		m_curGravityPower -= GravityPower * spf;
		setY(m_point.y - (m_curGravityPower * spf));
		if (m_myName == L"FlamePotion")
		{
			for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Wall])
			{
				if (CollisionWall(iter))
					DeadEvent(iter);
			}
		}
	}

	return true;
}

void MyObject::EndSpriteEvent()
{
	if (m_isRepeat)
	{
		m_curSprite = m_pSpriteList->begin();
	}
	else
	{
		m_curSprite--;
		m_canNextSprite = true;
	}
}

void MyObject::DeadEvent(MyActor* target)
{
	setEnable(false, true);
	if (m_objType == EObjType::PlayerAttack)
	{
		ObjectManager::Instance->TakeObject(L"Explosion", 64, 64)
			->setCenter(getCenter() + (target->getCenter() - getCenter()) / 2);
		SoundManager::Instance->Play("explosion", false);
	}
	else if (m_myName == L"FlamePotion")
	{
		ObjectManager::Instance->TakeObject(L"FlameBomb", 230, 360)
			->setCenter(getCenterX() , getBottom() - 170);
		SoundManager::Instance->Play("explosion", false);
	}
}

void MyObject::UpdateTextureUV()
{
	static MyVector4 posVector;
	if (m_curDirection == EDirection::Left)
	{
		posVector = m_curSprite->m_texVector4.getPosVector(m_curSprite->m_pTexture->getTextureWidth(), m_curSprite->m_pTexture->getTextureHeight());
		m_vertexList[0].tex = { posVector.x, posVector.y };
		m_vertexList[1].tex = { posVector.z, posVector.y };
		m_vertexList[2].tex = { posVector.x, posVector.w };
		//m_vertexList[3].tex = m_vertexList[2].tex;
		//m_vertexList[4].tex = m_vertexList[1].tex;
		m_vertexList[3].tex = { posVector.z, posVector.w };
	}
	else
	{
		posVector = m_curSprite->m_texVector4.getPosVector(m_curSprite->m_pTexture->getTextureWidth(), m_curSprite->m_pTexture->getTextureHeight());
		m_vertexList[0].tex = { posVector.z, posVector.y };
		m_vertexList[1].tex = { posVector.x, posVector.y };
		m_vertexList[2].tex = { posVector.z, posVector.w };
		//m_vertexList[3].tex = m_vertexList[2].tex;
		//m_vertexList[4].tex = m_vertexList[1].tex;
		m_vertexList[3].tex = { posVector.x, posVector.w };
	}
}

bool MyObject::CollisionWall(MyRect* object, const bool& bottomCheck)
{
	if (!CollisionCheck(*object))
		return false;
	if (m_prevPosition.getY() + m_prevPosition.getHeight() <= object->getTop())
	{
		setY(object->m_point.y - m_height);
		m_curGravityPower = 0.0f;
		return true;
	}
	else if (bottomCheck && m_prevPosition.getY() >= object->getBottom())
	{
		//setY(object->getBottom());
		m_curGravityPower -= GravityPower * 1.5f * m_curSPF;
		setBottom(m_prevPosition.getBottom());
	}
	else if (m_prevPosition.getX() + m_prevPosition.getWidth() <= object->getLeft())
	{
		setX(object->getLeft() - m_width - 1);
	}
	else if (m_prevPosition.getX() >= object->getRight())
	{
		setX(object->getRight() + 1);
	}
	return false;
}

void MyObject::setParameter(const int& health, const int& damage, const float& speed)
{
	m_healthPoint = (float)health;
	m_damage = damage;
	m_speed = speed;
}

void MyObject::setTarget(const MyVector2& targetPos)
{
	//m_rotation = getAngle(target, getCenter());
	m_dirPoint = getCenter().toPointNormalize(targetPos);
}

void MyObject::setGravityPower(const float& gravity)
{
	m_curGravityPower = gravity;
}

void MyObject::setHp(const float& value)
{
	m_healthPoint = value;
}

void MyObject::operHp(const float& value)
{
	m_healthPoint += value;
	m_healthPoint = max(0.0f, min(100.0f, m_healthPoint));
	if (m_healthPoint == 0)
		DeadEvent(this);
}

int MyObject::getHealth()
{
	return (int)m_healthPoint;
}

void MyObject::setBounce(const float& angle, const int power)
{
	m_curGravityPower = 0.0f;
	m_bouncePower.x = cosf(DegreeToRadian(angle)) * power;
	m_bouncePower.y = sinf(DegreeToRadian(angle)) * power;
	m_bounceUnit = m_bouncePower.Normalize();
}

bool MyObject::operBounce(const float& spf)
{
	if (m_bouncePower.x * m_bounceUnit.x < 0.0f)
	{
		m_bouncePower = 0;
		return false;
	}
	MyRect::Move(m_bouncePower.x * spf, m_bouncePower.y * spf);
	m_bouncePower -= (m_bounceUnit * BounceGravity * spf);
	return true;
}

void MyObject::Move(const int& x, const int& y)		noexcept
{
	static float moveSPF;
	moveSPF = m_curSPF * m_speed;
	MyRect::Move(x * moveSPF, y * moveSPF);
}
void MyObject::Move(const float& x, const float& y)	noexcept
{
	static float moveSPF;
	moveSPF = m_curSPF * m_speed;
	MyRect::Move(x * moveSPF, y * moveSPF);
}
void MyObject::Move(const MyVector2& point)			noexcept
{
	static float moveSPF;
	moveSPF = m_curSPF * m_speed;
	MyRect::Move(point.x * moveSPF, point.y * moveSPF);
}
void MyObject::Move(const POINT& point)				noexcept
{
	static float moveSPF;
	moveSPF = m_curSPF * m_speed;
	MyRect::Move(point.x * moveSPF, point.y * moveSPF);
}

bool MyObject::isEnable()
{
	if (m_healthPoint <= 0)
		return false;
	return m_isEnable;
}

MyActor* MyObject::clone()
{
	return (MyActor*)new MyObject(*this);
}