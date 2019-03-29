#include "MyObject.h"
#include "ObjectManager.h"

const float MyObject::GravityPower = 9.8f * 130.0f;
const float MyObject::BounceGravity = 9.8f * 80.0f;
//Point* MyObject::PlayerCenter = &(MyPlayer::GetInstance().m_myCenter);

MyObject::MyObject() : m_speed(0.0f), m_curDirection(EDirection::Left), m_isRepeat(true), m_canHit(true),
					   m_rotation(0.0f), m_preRotation(0.0f), m_healthPoint(100), 
					   m_bouncePower(0, 0), m_dirPoint(0, 0)
{ 
	m_hRotateDC = CreateCompatibleDC(Window::m_hScreenDC);
}

MyObject::MyObject(const wstring& objName, const EObjType& type, const int& damage) : MyObject()
{
	m_isDelete = false;
	m_objType = type;
	m_damage = damage;
	BitMapManager::Instance->AddObjBitMap(objName, this);
}

bool MyObject::Init()
{
	if (!MyBitMap::Init()) return false;

	if (m_objType == EObjType::EnemyAttack)
	{
		m_speed = 200.0f;
	}
	else if (m_objType == EObjType::PlayerAttack)
	{
		m_speed = 100.0f;
		m_isDelete = false;
	}
	else
		m_speed = 0.0f;
	m_curDirection = EDirection::Left;
	m_isRepeat = true;
	m_canHit = true;
	m_rotation = 0.0f;
	m_preRotation = 0.0f;
	m_healthPoint = 100;
	m_bouncePower = (0, 0);
	//m_hRotateBitmap = NULL;
	//m_hRotateMaskBitmap = NULL;
	//m_hRotateOldBitmap = NULL;
	return true;
}

void MyObject::setTarget(const Point& target)
{
	//m_rotation = getAngle(target, getCenter());
	m_dirPoint = getCenter().toPointNormalize(target);
	//setCrossLength();
}

void MyObject::setParameter(const float& speed, const int& damage, const float& rotation, const int& health)
{
	m_speed = speed;
	m_damage = damage;
	m_rotation = rotation;
	m_healthPoint = health;
}

void MyObject::setHp(const int& value)
{
	m_healthPoint = value;
}

void MyObject::operHp(const int& value)
{
	m_healthPoint += value;
	if (m_healthPoint <= 0)
		setEnable(this);
}

void MyObject::operRotation(const float& angle)
{
	m_rotation += angle;
	if (m_rotation >= 360.0f)
		m_rotation -= 360.0f;
}

void MyObject::setBounce(const float& angle, const int power)
{
 	m_curJumpPower = 0.0f;
	m_bouncePower.x = cosf(getRadian(angle)) * power;
	m_bouncePower.y = sinf(getRadian(angle)) * power;
	m_bounceUnit = m_bouncePower.Normalize();
}
void MyObject::operBounce()
{
	if (m_bouncePower.x * m_bounceUnit.x < 0.0f)
	{
		m_bouncePower = 0;
		return;
	}
	Move(m_bouncePower.x * Timer::SPF, m_bouncePower.y * Timer::SPF);
	m_bouncePower = m_bouncePower - (m_bounceUnit * BounceGravity * Timer::SPF);
}

void MyObject::Move(const float& x, const float& y) noexcept
{
	MyRect::Move(x * m_speed * Timer::SPF, y * m_speed * Timer::SPF);
}

void MyObject::Move(const int& x, const int& y) noexcept
{

	MyRect::Move(x * m_speed * Timer::SPF, y * m_speed * Timer::SPF);
}
void MyObject::Move(const Point& point) noexcept
{
	MyRect::Move(point.x * m_speed * Timer::SPF, point.y * m_speed * Timer::SPF);
}
void MyObject::Move(const POINT& point) noexcept
{
	MyRect::Move(point.x * m_speed * Timer::SPF, point.y * m_speed * Timer::SPF);
}


void MyObject::setEnable(const bool& isEnable)
{
	if (isEnable)
	{
		m_isEnable = true;
	}
	else
	{
		ObjectManager::Instance->DisableObject((MyBitMap*)this);
	}
}

bool MyObject::Frame()
{
	if (!m_isEnable)	return false;

	if (m_objType == EObjType::EnemyAttack)
	{
		Move(m_dirPoint);
	}
	else if (m_objType == EObjType::PlayerAttack)
	{
		if (m_isDelete)
		{
			setEnable(false);
			return false;
		}
		operBounce();
		m_curJumpPower -= GravityPower * Timer::SPF;
		setY((int)(m_point.y - (m_curJumpPower * Timer::SPF)));
		for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Enemy])
		{
			if (!iter->isEnable())
				continue;
			if (CollisionCheck(iter->getCenter().getDistance(getCenter()), m_width / 2.0f))
			{
				ObjectManager::Instance->TakeObject(L"Explosion")->setCenter(m_point);
				SoundManager::Instance->Play("explosion", false);
				m_isDelete = true;
			}
		}
	}

	if (! CollisionCheck(*MyBitMap::MapRect))
	{
		setEnable(false);
	}

	return true;
}

bool MyObject::Render()
{
	if (!m_isEnable)	return false;
	
	// 방향 없을시
	//else if (m_rotation == 0.0f)
	//{
		// mask-And : mask-XOR : main-XOR 

	if (m_objType == EObjType::PlayerAttack)
	{
		m_curSprite->m_maskBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCAND, false);
		m_curSprite->m_mainBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCINVERT, false);
		m_curSprite->m_maskBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCINVERT, false);
	}
	else
	{
		m_curSprite->m_maskBit->Draw(getCenter(), m_curSprite->m_srcRect, SRCAND);
		m_curSprite->m_mainBit->Draw(getCenter(), m_curSprite->m_srcRect, SRCINVERT);
		m_curSprite->m_maskBit->Draw(getCenter(), m_curSprite->m_srcRect, SRCINVERT);
	}

	//}
	//else
	//{
	//	// 방향 있을시, 이전 방향과 같을시 그냥 드로우
	//	if (abs(m_rotation - m_preRotation) > 10)
	//	{
	//		m_preRotation = m_rotation;
	//		m_curSprite->m_mainBit->SetRotationBitMap(&m_hRotateDC, &m_hRotateBitmap, &m_hRotateOldBitmap, m_curSprite->m_srcRect, m_rotation, m_crossLength);
	//		m_curSprite->m_maskBit->SetRotationBitMap(&m_hRotateDC, &m_hRotateMaskBitmap, &m_hRotateOldBitmap, m_curSprite->m_srcRect, m_rotation, m_crossLength);
	//	}
	//	m_curSprite->m_maskBit->DrawRotation(&m_hRotateMaskBitmap, &m_hRotateOldBitmap, getCenter(), SRCAND, m_crossLength);
	//	m_curSprite->m_mainBit->DrawRotation(&m_hRotateBitmap, &m_hRotateOldBitmap, getCenter(), SRCINVERT, m_crossLength);
	//	m_curSprite->m_maskBit->DrawRotation(&m_hRotateMaskBitmap, &m_hRotateOldBitmap, getCenter(), SRCINVERT, m_crossLength);
	//}

	// 스프라이트 프레임 계산
	if (m_curSprite->m_frame != 0.0f)
	{
		m_frameCount += Timer::SPF;
		if (m_frameCount >= m_curSprite->m_frame)
		{
			m_frameCount = 0.0f;
			if (++m_curSprite == m_pSpriteList->end())
			{
				m_curSprite = m_pSpriteList->begin();
			}
		}
	}

	return true;
}

bool MyObject::Release()
{
	MyBitMap::Release();
	return true;
}

bool MyObject::isEnable()
{
	if (m_healthPoint <= 0)
		return false;
	return m_isEnable;
}

MyBitMap* MyObject::clone()
{
	return (MyBitMap*)new MyObject(*this);
}