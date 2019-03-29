#include "MyEnemy.h"
#include "ObjectManager.h"
#include <mutex>
#include <memory>

bool MyEnemy::Init() noexcept
{
	if (!MyObject::Init()) return false;

	static std::once_flag flag;
	std::call_once(flag,
		[&]() {
		// 상태 입력
		m_stateList[EState::Basic] = new EnemyStateBasic();
		m_stateList[EState::Move] = new EnemyStateMove();
		m_stateList[EState::Patrol] = new EnemyStatePatrol();
		m_stateList[EState::Attack] = new EnemyStateAttack();
		m_stateList[EState::Hit] = new EnemyStateHit();
		m_stateList[EState::Runaway] = new EnemyStateRunaway();
		m_stateList[EState::Dead] = new EnemyStateDead();
		m_curState = m_stateList[EState::Basic];
	});

	m_canHit = true;
	//m_objType = EObjType::Enemy;
	m_attackRadius = 450 * 450;
	m_nearRadius = 135 * 135;
	m_damage = 10;
	m_healthPoint = 100;
	m_speed = (float)(rand() % 40) + 120;
	setWH(60, 60);
	m_originWH = getWH();
	m_dirPoint = (0.0f, 0.0f);
	m_delayAttack = 0.0f;
	m_delayDodge = 0.0f;
	m_delayFrame = 0.0f;
	m_delayHit = 0.0f;
	return true;
}

bool MyEnemy::Frame(const float& spf, const float& accTime) noexcept
{
	if (!MyActor::Frame(spf, accTime)) return false;
	m_curSPF = spf;

	// 변수 계산
	m_prevPosition = *this;
	m_delayFrame -= spf;
	m_delayAttack -= spf;
	m_delayDodge -= spf;
	m_delayHit -= spf;
	m_distance = m_point.getSquareDistance(MyPlayer::MyCenter);

	if (m_canHit && !Collision())
		return true;
	
	if (!m_curState->Process(this))
		return true;

	// false시 무시
	if (!CollisionCheck(*ObjectManager::MapRect))
	{
		return setState(EState::Move);
	}
	if (m_distance <= m_nearRadius && rand() % 30 == 0)
	{
		return setState(EState::Runaway);
	}

	return true;
	accTime;
}

bool MyEnemy::Collision() 
{
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::PlayerAttack])
	{
		if (!iter->isEnable())
			continue;
		if (iter->CollisionCheck(iter->getCenter().getDistance(getCenter()), m_width / 2.0f))
		{
			m_canHit = false;
			iter->DeadEvent(this);
			int damage = ((MyObject*)iter)->m_damage;
			m_healthPoint -= damage;
			damage /= 5;
			for (int i = 0; i < damage; i++)
			{
				ObjectManager::Instance->TakeObject(L"ScoreItem", { getCenter() });
			}
			if (m_healthPoint <= 0)
				return setState(EState::Dead);
			else
				return setState(EState::Hit);
		}
	}
	return true;
}

bool MyEnemy::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (m_isEnable == false)	return false;
	PrevRender(pDContext);
	PostRender(pDContext);
	return true;
}

void MyEnemy::DeadEvent(MyActor* target)
{
	return;
	target;
}

bool MyEnemy::operBounce(const float& spf)
{
	MyObject::operBounce(spf);
	m_curGravityPower -= (GravityPower / 4 * spf);
	setY(m_point.y - (m_curGravityPower * spf));
	return true;
}

bool MyEnemy::setSpriteState(const wstring& stateName, const bool& isRepeat)
{
	if (!isRepeat)
		m_canNextSprite = false;

	m_isRepeat = isRepeat;
	MyActor::setSpriteList(stateName);
	return true;
}

EState MyEnemy::getCurrentState()
{
	return m_curState->m_myState;
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


MyActor* MyEnemy::clone()
{
	return (MyActor*)new MyEnemy(*this);
}