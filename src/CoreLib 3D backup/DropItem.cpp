#include "DropItem.h"
#include "ObjectManager.h"



bool DropItem::Init() noexcept
{
	MyObject::Init();
	m_damage = 220;
	m_speed = 0.0f;

	m_myItem = Inventory::Instance->getRandomItem();
	if (m_myItem == nullptr)
		return true;
	setSpriteList(m_myItem->m_myName);
	m_itemCount = rand() % m_myItem->m_itemMaxCount / 2 + 1;

	setGravityPower(350.0f);
	return true;
}

bool DropItem::SetInitItem(Item* pItem, const int& itemCount) noexcept
{
	MyObject::Init();
	m_damage = 220;
	m_speed = 0.0f;

	m_myItem = pItem;
	setSpriteList(m_myItem->m_myName);
	m_itemCount = itemCount;

	setGravityPower(350.0f);
	return true;
}

bool DropItem::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!MyActor::Frame(spf, accTime))	return false;
	m_prevPosition = *this;
	m_curSPF = spf;

	if ((m_delayDelete += spf) >= 10.0f || !CollisionCheck(*ObjectManager::MapRect))
		setEnable(false, true);

	m_curGravityPower -= GravityPower / 2 * spf;
	setY(m_point.y - (m_curGravityPower * spf));

	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Wall])
	{
		if (CollisionCheck(*iter))
		{
			if (m_prevPosition.getY() + m_prevPosition.getHeight() <= iter->getTop())
			{
				setY(iter->m_point.y - m_height);
				m_curGravityPower = (float)m_damage;
			}
		}
	}
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Collider])
	{
		if (CollisionCheck(*iter))
		{
			if (m_prevPosition.getY() + m_prevPosition.getHeight() <= iter->getTop())
			{
				setY(iter->m_point.y - m_height);
				m_curGravityPower = (float)m_damage;
			}
		}
	}

	return true;
}

Item* DropItem::getMyItem()
{
	return m_myItem;
}

int DropItem::getItemCount()
{
	return m_itemCount;
}

MyActor* DropItem::clone()
{
	return (MyActor*)new DropItem(*this);
}