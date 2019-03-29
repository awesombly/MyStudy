#include "MyEffect.h"
#include "ObjectManager.h"



MyEffect::MyEffect(const wstring& objName, const EObjType& type, const int& damage) : MyBitMap::MyBitMap()
{
	m_objType = type;
	m_damage = damage;
	BitMapManager::Instance->AddObjBitMap(objName, this);
}


bool MyEffect::Render()
{
	if(! m_isEnable)	return false;

	// 마스크컬러 or 마스크비트맵
	if (m_curSprite->m_maskRGB != 0)
	{
		m_curSprite->m_mainBit->DrawMask(*this, m_curSprite->m_srcRect, m_curSprite->m_maskRGB);
	}
	else
	{
		m_curSprite->m_maskBit->Draw(getCenter(), m_curSprite->m_srcRect, SRCAND);
		m_curSprite->m_mainBit->Draw(getCenter(), m_curSprite->m_srcRect, SRCINVERT);
		m_curSprite->m_maskBit->Draw(getCenter(), m_curSprite->m_srcRect, SRCINVERT);
	}

	// 스프라이트 프레임 계산
	if (m_curSprite->m_frame != 0.0f)
	{
		m_frameCount += Timer::SPF;
		if (m_frameCount >= m_curSprite->m_frame)
		{
			m_frameCount = 0.0f;
			if (++m_curSprite == m_pSpriteList->end())
			{
				// 이펙트 비활성화
				m_curSprite = m_pSpriteList->begin();
				setEnable(false);
			}
		}
	}

	return true;
}

void MyEffect::setEnable(const bool& isEnable)
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

MyBitMap* MyEffect::clone()
{
	return (MyBitMap*)new MyEffect(*this);
}