#include "IEnemyState.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "MyEnemy.h"


// 기본
void EnemyStateBasic::StateInit(MyEnemy* pOwner)
{
	pOwner->setSpriteState(L"basic");
	pOwner->setWH(pOwner->m_originWH.x * 1.2f, pOwner->m_originWH.y);
	pOwner->m_delayFrame = 2.0f;
}
bool EnemyStateBasic::Process(MyEnemy* pOwner)
{
	if (pOwner->m_delayFrame > 0.0f)
		return true;

	if (pOwner->m_distance <= pOwner->m_attackRadius && pOwner->m_delayAttack <= 0.0f)
	{
		pOwner->setState(EState::Attack);
		return true;
	}
	
	pOwner->setState(EState::Patrol);
	return true;
}


// 공격
void EnemyStateAttack::StateInit(MyEnemy* pOwner)
{
	if (pOwner->m_delayAttack > 0.0f)
	{
		pOwner->setState(EState::Basic);
		return;
	}
	pOwner->setSpriteState(L"attack", false);
	pOwner->setWH((int)(pOwner->m_originWH.x * 1.3f), (int)(pOwner->m_originWH.y * 1.15f));
	pOwner->m_delayFrame = 0.8f;
}
bool EnemyStateAttack::Process(MyEnemy* pOwner)
{
	// 샷
	if (pOwner->m_delayFrame > 0.0f)
		return false;
	if (pOwner->m_delayAttack <= 0.0f)
	{
		SoundManager::Instance->PlayVariation("fairyShot", false, 4);
		pOwner->m_delayAttack = 8.0f;
		pOwner->m_delayFrame = 0.9f;
		auto pMissile = ObjectManager::Instance->TakeObject(L"FairyClash");
		if (pOwner->m_curDirection == EDirection::Left)
			pMissile->setCenter(pOwner->getLeft() + pOwner->getWidth() / 4, (int)pOwner->getCenter().y);
		else
			pMissile->setCenter(pOwner->getRight() - pOwner->getWidth() / 4, (int)pOwner->getCenter().y);
		((MyObject*)pMissile)->setTarget(MyPlayer::MyCenter);
		return false;
	}

	if (pOwner->m_canNextSprite)
	{
		pOwner->setState(EState::Basic);
		return true;
	}

	if (pOwner->m_distance <= pOwner->m_attackRadius)
	{
		pOwner->setState(EState::Runaway);
		return true;
	}
	
	pOwner->setState(EState::Patrol);
	return true;
}


// 이동(to 플레이어)
void EnemyStateMove::StateInit(MyEnemy* pOwner)
{
	pOwner->setSpriteState(L"move");
	pOwner->setWH(pOwner->m_originWH * 0.75f);
	pOwner->m_dirPoint = pOwner->m_point.toPointNormalize(MyPlayer::MyCenter) * 0.35f;
	pOwner->m_delayFrame = 1.0f;
}
bool EnemyStateMove::Process(MyEnemy* pOwner)
{
	pOwner->Move(pOwner->m_dirPoint.x, pOwner->m_dirPoint.y);

	if (pOwner->m_delayFrame > 0.0f)
		return false;

	if (pOwner->m_distance <= pOwner->m_attackRadius)
	{
		pOwner->setState(EState::Basic);
		return true;
	}
	
	pOwner->setState(EState::Patrol);
	return true;
}


// 순찰
void EnemyStatePatrol::StateInit(MyEnemy* pOwner)
{
	pOwner->setSpriteState(L"move");
	pOwner->setWH(pOwner->m_originWH * 0.75f);
	pOwner->m_dirPoint = pOwner->m_point.toPointNormalize({ rand() % Window::getClientRect().right + Window::DrawPoint.x, (float)(rand() % Window::getClientRect().bottom)});
	pOwner->m_delayFrame = 1.5f;
}
bool EnemyStatePatrol::Process(MyEnemy* pOwner)
{
	pOwner->Move(pOwner->m_dirPoint.x, pOwner->m_dirPoint.y);

	if (pOwner->m_delayFrame > 0.0f)
		return true;

	if (pOwner->m_distance <= pOwner->m_attackRadius)
	{
		pOwner->setState(EState::Attack);
		return true;
	}

	pOwner->setState(EState::Move);
	return true;
}


// 회피
void EnemyStateRunaway::StateInit(MyEnemy* pOwner)
{
	if (pOwner->m_delayDodge > 0.0f)
	{
		pOwner->setState(EState::Attack);
		return;
	}
	pOwner->setSpriteState(L"standby");
	pOwner->setWH(pOwner->m_originWH.x * 1.15f, pOwner->m_originWH.y);
	pOwner->m_dirPoint = MyPlayer::MyCenter.toPointNormalize(pOwner->m_point) * 3.2f;
	pOwner->m_delayFrame = 0.6f;
	pOwner->m_delayDodge = 7.0f;
	pOwner->m_delayAttack -= 1.5f;
}
bool EnemyStateRunaway::Process(MyEnemy* pOwner)
{
	pOwner->Move(pOwner->m_dirPoint.x, pOwner->m_dirPoint.y);

	if (pOwner->m_delayFrame > 0.0f)
		return false;


	if (pOwner->m_distance <= pOwner->m_nearRadius)
	{
		pOwner->setState(EState::Patrol);
		return true;
	}
	if (pOwner->m_distance <= pOwner->m_attackRadius)
	{
		pOwner->setState(EState::Attack);
	}
	
	pOwner->setState(EState::Basic);
	return true;
}


// 피격
void EnemyStateHit::StateInit(MyEnemy* pOwner)
{
	SoundManager::Instance->PlayVariation("hit", false, 2);
	pOwner->setSpriteState(L"hit", false);
	pOwner->setWH(pOwner->m_originWH.x * 2.1f, pOwner->m_originWH.y * 1.15f);
	pOwner->m_angle = pOwner->getAngle(MyPlayer::MyCenter, pOwner->m_point);
	pOwner->setBounce(pOwner->m_angle, 900);
	pOwner->m_delayFrame = 1.2f;
	pOwner->m_delayHit = 0.08f;
}
bool EnemyStateHit::Process(MyEnemy* pOwner)
{
	pOwner->operBounce(pOwner->m_curSPF);

	if (pOwner->m_delayFrame > 0.0f)
		return false;
	if (pOwner->m_delayHit <=  0.0f)
		pOwner->m_canHit = true;

	pOwner->m_canHit = true;

	if (pOwner->m_distance <= pOwner->m_attackRadius)
	{
		pOwner->setState(EState::Basic);
		return true;
	}

	pOwner->setState(EState::Runaway);
	return true;
}

// 사망
void EnemyStateDead::StateInit(MyEnemy* pOwner)
{
	pOwner->setSpriteState(L"dying", false);
	pOwner->setWH(pOwner->m_originWH);

	pOwner->m_angle = pOwner->getAngle(MyPlayer::MyCenter, pOwner->m_point);
	pOwner->setBounce(pOwner->m_angle, 1300);
	pOwner->m_delayFrame = 5.5f;
	pOwner->m_delayHit = 1.0f;
	ObjectManager::Instance->TakeObject(L"ItemIcon", pOwner->getCenter())->Init();
}
bool EnemyStateDead::Process(MyEnemy* pOwner)
{
	// 딜레이 지나면 처리 무시
	if (pOwner->m_delayFrame <= 0.0f)
	{
		return false;
	}
	if (pOwner->m_delayHit <= 0.0f)
	{
		pOwner->m_delayHit = 999.0f;
		for (int i = 0; i < 40; i++)
		{
			ObjectManager::Instance->TakeObject(L"ScoreItem", { pOwner->getCenter() })
				->Init();
		}
	}

	pOwner->operBounce(pOwner->m_curSPF);

	// 충돌 처리
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Wall])
	{
		pOwner->CollisionWall(iter);
		if (Input::isDebug)
			ObjectManager::Instance->TakeObject(L"Clash")->setCenter(pOwner->getCenter());
	}
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Collider])
	{
		pOwner->CollisionWall(iter);
		if (Input::isDebug)
			ObjectManager::Instance->TakeObject(L"Clash")->setCenter(pOwner->getCenter());
	}

	//if (pOwner->m_delayFrame > 0.0f)
		//return false;
	// 사망 처리
	//pOwner->setEnable(false);

	return false;
}