#include "IEnemyState.h"
#include "ObjectManager.h"


// 기본
void EnemyStateBasic::StateInit(MyEnemy* pOwner)
{
	pOwner->setSpriteState(L"basic");
	pOwner->setWH(pOwner->m_originWH);
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
	pOwner->m_delayFrame = 0.9f;
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
		pOwner->m_delayFrame = 0.93f;
		auto pMissile = ObjectManager::Instance->TakeObject(L"FairyClash");
		if (pOwner->m_curDirection == EDirection::Left)
			pMissile->setCenter(pOwner->getLeft(), (int)pOwner->getCenter().y);
		else
			pMissile->setCenter(pOwner->getRight(), (int)pOwner->getCenter().y);
		pMissile->setTarget(MyPlayer::MyCenter);
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
	//if (pOwner->m_delayHit > 0.0f)
	//{
	//	pOwner->setState(EState::Patrol);
	//	pOwner->m_delayHit -= 2.0f;
	//	return;
	//}
	SoundManager::Instance->PlayVariation("hit", false, 2);
	pOwner->setSpriteState(L"hit", false);
	pOwner->setWH(pOwner->m_originWH.x * 2.1f, pOwner->m_originWH.y * 1.15f);
	pOwner->m_angle = pOwner->getAngle(MyPlayer::MyCenter, pOwner->m_point);
	pOwner->setBounce(pOwner->m_angle, 500);
	pOwner->m_delayFrame = 1.2f;
	//pOwner->m_delayHit = 5.0f;
}
bool EnemyStateHit::Process(MyEnemy* pOwner)
{
	pOwner->operBounce();

	if (pOwner->m_delayFrame > 0.0f)
		return false;
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
	pOwner->setBounce(pOwner->m_angle, 650);
	pOwner->m_delayFrame = 5.0f;
}
bool EnemyStateDead::Process(MyEnemy* pOwner)
{
	// 딜레이 지나면 처리 무시
	if (pOwner->m_delayFrame <= 0.0f)
		return false;

	pOwner->operBounce();

	// 충돌 처리
	for (auto& iter : ObjectManager::Instance->m_WallList)
	{
		if (pOwner->CollisionCheck(*iter))
		{
			if (pOwner->m_prePosition.getY() + pOwner->m_prePosition.getHeight() <= iter->getTop())
			{
				pOwner->setY(iter->m_point.y - pOwner->m_height);
			}
			else if (pOwner->m_prePosition.getY() >= iter->getBottom())
			{
				pOwner->setY(iter->getBottom());
			}
			else if (pOwner->m_prePosition.getX() + pOwner->m_prePosition.getWidth() <= iter->getLeft())
			{
				pOwner->setX(iter->getLeft() - pOwner->m_width);
			}
			else if (pOwner->m_prePosition.getX() >= iter->getRight())
			{
				pOwner->setX(iter->getRight());
			}
			if (Input::isDebug)
				ObjectManager::Instance->TakeObject(L"Clash")->setCenter(pOwner->getCenter());
		}
	}

	//if (pOwner->m_delayFrame > 0.0f)
		//return false;
	// 사망 처리
	//pOwner->setEnable(false);

	return false;
}