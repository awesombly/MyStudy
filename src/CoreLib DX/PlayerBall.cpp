#include "PlayerBall.h"
#include "ObjectManager.h"


PlayerBall::PlayerBall(const wstring_view& myName, const EObjType& objType,
						const string_view& vertexShaderName, const string_view& pixelShaderName) :
	MyObject::MyObject(std::forward<const wstring_view>(myName), objType,
					std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName))
{
}

bool PlayerBall::Init() noexcept
{
	MyObject::Init();
	m_damage = 500 + rand() % 600;
	m_curAngle = (float)(rand() % 360);
	setBounce(m_curAngle, m_damage);
	setWH(33, 66);
	return true;
}

bool PlayerBall::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!MyActor::Frame(spf, accTime))	return false;
	m_prevPosition = *this;
	m_curSPF = spf;

	// Æ¨±è Ã³¸®
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Collider])
	{
		if (!iter->isEnable() || iter == this)
			continue;
		if (CollisionCheck(*iter))
		{
			if (m_prevPosition.getY() + m_prevPosition.getHeight() <= iter->getTop() ||
				m_prevPosition.getY() >= iter->getBottom())
			{
				m_curAngle = 360.0f - m_curAngle;
			}
			else if (m_prevPosition.getX() + m_prevPosition.getWidth() <= iter->getLeft() ||
					 m_prevPosition.getX() >= iter->getRight())
			{
				m_curAngle = 180.0f - m_curAngle;
			}
			//m_curAngle += 90.0f;
			m_curDirection = (EDirection)(!(bool)m_curDirection);
			m_curAngle = (float)((int)m_curAngle % 360);
			setBounce(m_curAngle, m_damage);
			setWH(49, 68);
		}
	}
	// È­¸é Æ¨±è
	static MyRect newClient = { (int)(Window::getClientRect().left   + Window::DrawPoint.x + 30),
								(int)(Window::getClientRect().top    + Window::DrawPoint.y + 30), 
								(int)(Window::getClientRect().right - Window::getClientRect().left + Window::DrawPoint.x - 30),
								(int)(Window::getClientRect().bottom - Window::getClientRect().top + Window::DrawPoint.y - 30)};
	if (!CollisionCheck(newClient))
	{
		if (m_prevPosition.getY() + m_prevPosition.getHeight() <= newClient.getTop() ||
			m_prevPosition.getY() >= newClient.getBottom())
		{
			m_curAngle = 360.0f - m_curAngle;
		}
		else if (m_prevPosition.getX() + m_prevPosition.getWidth() <= newClient.getLeft() ||
				 m_prevPosition.getX() >= newClient.getRight())
		{
			m_curAngle = 180.0f - m_curAngle;
		}
		m_curAngle = (float)((int)m_curAngle % 360);
		m_curDirection = (EDirection)(!(bool)m_curDirection);
		setBounce(m_curAngle, m_damage);
	}
	operBounce(spf);
	//Move(m_dirPoint);

	return true;
}


bool PlayerBall::operBounce(const float& spf)
{
	//if (m_bouncePower.x * m_bounceUnit.x < 0.0f)
	//{
//		m_bouncePower = 0;
		//return false;
	//}
	MyRect::Move(m_bouncePower.x * spf, m_bouncePower.y * spf);
	//m_bouncePower -= (m_bounceUnit * BounceGravity * spf);
	return true;
}

MyActor* PlayerBall::clone()
{
	return (MyActor*)new PlayerBall(*this);
}
