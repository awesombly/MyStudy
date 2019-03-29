#include "ScoreItem.h"
#include "ObjectManager.h"
#include "MyPlayer.h"


ScoreItem::ScoreItem(const wstring_view& myName, const EObjType& objType,
				   const string_view& vertexShaderName, const string_view& pixelShaderName) :
	MyObject::MyObject(std::forward<const wstring_view>(myName), objType,
					   std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName))
{}

bool ScoreItem::Init() noexcept
{
	MyObject::Init();
	m_damage = 500 + rand() % 400;
	m_speed = 0.0f;
	m_scoreValue = 10;
	setBounce((float)(rand() % 360), rand() % 100 + 500);
	//SetColor(1.0f, 1.0f, 0.0f);
	return true;
}


bool ScoreItem::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!MyActor::Frame(spf, accTime))	return false;
	m_curSPF = spf;
	if ((m_delayDelete += spf) >= 8.0f)
		setEnable(false, true);

	m_speed += m_damage * spf;
	setTarget(MyPlayer::MyCenter);
	Move(m_dirPoint);
	operBounce(spf);

	return true;
}

int ScoreItem::GetScoreValue()
{
	return m_scoreValue;
}


MyActor* ScoreItem::clone()
{
	return (MyActor*)new ScoreItem(*this);
}