#include "MyBitMap.h"
#include "BitMapManager.h"

MyRect* MyBitMap::MapRect = new MyRect(0, 0, 1000, 800);

MyBitMap::MyBitMap() : m_crossLength(0.0f), m_isEnable(false)
{
	m_objType = EObjType::Bitmap;
	Init();
}

MyBitMap::MyBitMap(const wstring& objName, const bool& insertFront) : MyBitMap()
{
	BitMapManager::Instance->AddObjBitMap(objName, this, insertFront);
}

MyBitMap::~MyBitMap()
{
	Release();
}

bool MyBitMap::Init()
{
	return true;
}

bool MyBitMap::Frame()
{
	if (!m_isEnable)	return false;
	return true;
}

bool MyBitMap::Render()
{
	if (!m_isEnable)	return false;

	// (맵)
	if (m_curSprite->m_maskRGB != 0)
	{
		m_curSprite->m_mainBit->DrawMask(*this, m_curSprite->m_srcRect, m_curSprite->m_maskRGB);
		return true;
	}
	// 마스크 파일 없을시(배경)
	else if (m_curSprite->m_maskBit == NULL)
	{
		m_curSprite->m_mainBit->DrawScroll(*this, m_curSprite->m_srcRect, SRCCOPY);
		return true;
	}

	return false;
}

bool MyBitMap::Release()
{
	return true;
}

void MyBitMap::setSpriteList(list<Sprite>* sprite)
{
	m_pSpriteList = sprite;
	m_curSprite = m_pSpriteList->begin();
	setSrcRect(m_curSprite->m_srcRect);
}

MyBitMap* MyBitMap::setSpriteList(const wstring& stateName)
{
	m_pSpriteList = BitMapManager::Instance->getSpriteList(m_myName, stateName);
	m_curSprite = m_pSpriteList->begin();
	setSrcRect(m_curSprite->m_srcRect);
	return this;
}

void MyBitMap::setEnable(const bool& isEnable)
{
	m_isEnable = isEnable;
}

void MyBitMap::setCrossLength()
{
	m_crossLength = (float)sqrt(pow(m_width, 2) + pow(m_height, 2));
}

void MyBitMap::setSrcRect(const MyRect& rect)
{
	//m_curSprite->m_srcRect = rect;
	//this->setRect(rect);
	m_crossLength = (float)sqrt(pow(m_width, 2) + pow(m_height, 2));
}

MyRect& MyBitMap::getSrcRect() const
{
	return m_curSprite->m_srcRect;
}

bool MyBitMap::isEnable()
{
	return m_isEnable;
}

MyBitMap* MyBitMap::clone()
{
	return new MyBitMap(*this);
}