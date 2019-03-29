#include "MyEnemy.h"
#include "ObjectManager.h"

//map<EState, wstring> MyEnemy::StateToString;


MyEnemy::MyEnemy(const wstring& objName) : m_attackRadius(500 * 500), m_nearRadius(135 * 135),
										   m_delayAttack(0.0f)
{
	m_damage = 10;
	m_objType = EObjType::Enemy;
	m_speed = (float)(rand() % 40) + 120;
	m_healthPoint = 100;
	BitMapManager::Instance->AddObjBitMap(objName, this);
	setFSM();
	m_originWH = { 60, 60 };
}

bool MyEnemy::Init()
{
	if (!MyObject::Init()) return false;

	m_attackRadius = 450 * 450;
	m_nearRadius = 135 * 135;
	m_dirPoint = (0.0f, 0.0f);
	m_delayAttack = 0.0f;
	m_objType = EObjType::Enemy;
	m_speed = (float)(rand() % 40) + 120;
	m_healthPoint = 100;
	m_originWH = { 60, 60 };
	return true;
}

bool MyEnemy::Frame()
{
	if (!m_isEnable) return false;

	// 변수 계산
	m_prePosition = *this;
	m_delayFrame -= Timer::SPF;
	m_delayAttack -= Timer::SPF;
	m_delayDodge -= Timer::SPF;
	m_delayHit -= Timer::SPF;
	m_distance = m_point.getSquareDistance(MyPlayer::MyCenter);

	if (m_canHit)
	{
		for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::PlayerAttack])
		{
			if (!iter->isEnable())
				continue;
			if (iter->CollisionCheck(iter->getCenter().getDistance(getCenter()), m_width / 2.0f) )
			{
				m_healthPoint -= iter->m_damage;
				m_canHit = false;
				if (m_healthPoint <= 0)
					return setState(EState::Dead);
				else
					return setState(EState::Hit);
			}
		}
	}

	if (!m_curState->Process(this))
		return true;

	// false시 무시
	if (!CollisionCheck(*MyBitMap::MapRect))
	{
		return setState(EState::Move);
	}
	if (m_distance <= m_nearRadius && rand() % 30 == 0)
	{
		return setState(EState::Runaway);
	}
	
	return true;
}

bool MyEnemy::Render()
{
	if (!m_isEnable)	return false;

	if (m_curDirection == EDirection::Left)
	{
		m_curSprite->m_maskBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCAND, false);
		m_curSprite->m_mainBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCINVERT, false);
		m_curSprite->m_maskBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCINVERT, false);
	}
	else
	{
		m_curSprite->m_maskBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCAND, true);
		m_curSprite->m_mainBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCINVERT, true);
		m_curSprite->m_maskBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCINVERT, true);
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
		}
	}

	return true;
}

void MyEnemy::operBounce()
{
	MyObject::operBounce();
	m_curJumpPower -= GravityPower / 4 * Timer::SPF;
	setY((int)(m_point.y - (m_curJumpPower * Timer::SPF)));
}

bool MyEnemy::setSpriteState(const wstring& stateName, const bool& isRepeat)
{
	if (!isRepeat)
		m_canNextSprite = false;

	m_isRepeat = isRepeat;
	MyBitMap::setSpriteList(stateName);
	return true;
}

EState MyEnemy::getCurrentState()
{
	return m_curState->m_myState;
}

bool  MyEnemy::setEventTransition(const EEvent& event)
{
	// 현제 상태에 이벤트 발생시 대응하는 상태를 받아옴
	auto state = m_myFSM->getStateTransition(m_curState->m_myState, event);
	// 현재 상태에 대응하는 이벤트 없을시 리턴
	if (state == EState::Count)
		return false;
	m_curState = m_stateList[state];
	m_curState->StateInit(this);
	return true;
}

bool MyEnemy::setState(const EState state)
{
	// 좌우 전환
	if (m_point.x < MyPlayer::MyCenter.x)
		m_curDirection = EDirection::Right;
	else
		m_curDirection = EDirection::Left;

	m_curState = m_stateList[state];
	m_curState->StateInit(this);
	return true;
}

// 행동 가능한 상태 리스트 입력
void  MyEnemy::setFSM()
{
	m_myFSM = FSMManager::Instance->getFSM(m_myName);
	// 상태 입력
	m_stateList[EState::Basic] = new EnemyStateBasic();
	m_stateList[EState::Move] = new EnemyStateMove();
	m_stateList[EState::Patrol] = new EnemyStatePatrol();
	m_stateList[EState::Attack] = new EnemyStateAttack();
	m_stateList[EState::Hit] = new EnemyStateHit();
	m_stateList[EState::Runaway] = new EnemyStateRunaway();
	m_stateList[EState::Dead] = new EnemyStateDead();
	m_curState = m_stateList[EState::Basic];
}

MyBitMap* MyEnemy::clone()
{
	return (MyBitMap*)new MyEnemy(*this);
}