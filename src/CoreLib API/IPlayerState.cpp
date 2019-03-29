#include "IPlayerState.h"
#include "ObjectManager.h"


// 기본
void PlayerStateBasic::StateInit(MyPlayer* pOwner)
{
	if (pOwner->m_isSword)
	{
		pOwner->setState(EPlayerState::Sword);
		return;
	}
	if (pOwner->m_preState->m_myState == EPlayerState::Sword)
	{
		SoundManager::Instance->Play("change", false);
		pOwner->setSpriteState(L"swordPush", false);
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.3f);
		pOwner->m_delayFrame = 0.85f;
		return;
	}
	pOwner->setSpriteState(L"basic");
	pOwner->setWH(pOwner->m_originWH);
	//pOwner->m_delayFrame = 2.0f;
}
bool PlayerStateBasic::Process(MyPlayer* pOwner)
{
	if (pOwner->m_delayFrame > 0.0f)
	{
		// 좌
		if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('A') == EKeyState::DOWN)
		{
			pOwner->MyObject::Move(-0.4f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('A') == EKeyState::HOLD)
		{
			pOwner->MyObject::Move(-0.4f, 0.0f);
		}
		// 우
		if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('D') == EKeyState::DOWN)
		{
			pOwner->MyObject::Move(0.4f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('D') == EKeyState::HOLD)
		{
			pOwner->MyObject::Move(0.4f, 0.0f);
		}
		return false;
	}
	if (pOwner->m_canNextSprite)
	{
		pOwner->m_canNextSprite = false;
		pOwner->setSpriteState(L"basic");
		pOwner->setWH(pOwner->m_originWH.x, pOwner->m_originWH.y);
	}

	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('A') == EKeyState::DOWN)
	{
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"running");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y);
		pOwner->MyObject::Move(-1, 0);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"running");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y);
		pOwner->MyObject::Move(-1, 0);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::UP ||
			 Input::Instance->getKeyState('A') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"basic");
		pOwner->setWH(pOwner->m_originWH);
	}
	// 우
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"running");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y);
		pOwner->MyObject::Move(1, 0);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"running");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y);
		pOwner->MyObject::Move(1, 0);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::UP ||
			 Input::Instance->getKeyState('D') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"basic");
		pOwner->setWH(pOwner->m_originWH);
		pOwner->setX(pOwner->m_prePosition.getRight() - pOwner->getWidth());
	}
	// 위
	if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
		Input::Instance->getKeyState('W') == EKeyState::HOLD)
	{
	}
	// 아래
	if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::DOWN ||
		Input::Instance->getKeyState('S') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround && pOwner->m_curSpriteName == L"basic")
		{
			pOwner->setState(EPlayerState::Crawling);
			return true;
		}
	}

	// 점프
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround)
		{
			pOwner->m_isGround = false;
			pOwner->m_curJumpPower = pOwner->m_JumpPower;
		}
	}

	// 칼꺼내기
	if (Input::Instance->getKeyState('Z') == EKeyState::DOWN)
	{
		pOwner->setState(EPlayerState::Sword);
		return true;
	}

	// 스킬
	if (Input::Instance->getKeyState('X') == EKeyState::DOWN)
	{
		pOwner->setState(EPlayerState::Skill);
		return true;
	}

	return true;
}


// 엎드림
void PlayerStateCrawling::StateInit(MyPlayer* pOwner)
{
	pOwner->m_delayFrame = 0.3f;
	pOwner->setSpriteState(L"crawlingStart", false);
	pOwner->setWH(pOwner->m_originWH.x * 1.4f,pOwner->m_originWH.y * 0.9f);
}
bool PlayerStateCrawling::Process(MyPlayer* pOwner)
{
	if (pOwner->m_delayFrame > 0.0f)
		return false;
	if (pOwner->m_canNextSprite)
	{
		pOwner->m_canNextSprite = false;
		pOwner->setSpriteState(L"crawlingBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
	}

	// 아래
	if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::UP ||
		Input::Instance->getKeyState('S') == EKeyState::UP)
	{
		if (!pOwner->HeadCheck())
		{
			pOwner->setState(EPlayerState::Basic);
			return true;
		}
	}

	// 좌
	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('A') == EKeyState::DOWN)
	{
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"crawling");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
		pOwner->MyObject::Move(-0.5f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"crawling");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
		pOwner->MyObject::Move(-0.5f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::UP ||
			 Input::Instance->getKeyState('A') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"crawlingBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
	}
	// 우
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"crawling");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f,pOwner->m_originWH.y * 0.55f);
		pOwner->MyObject::Move(0.5f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"crawling");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
		pOwner->MyObject::Move(0.5f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::UP ||
			 Input::Instance->getKeyState('D') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"crawlingBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
	}
	// 위
	if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
		Input::Instance->getKeyState('W') == EKeyState::HOLD)
	{
	}


	// 점프
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
	}

	return true;
}


// 검
void PlayerStateSword::StateInit(MyPlayer* pOwner)
{
	if (pOwner->m_isSword)
	{
		pOwner->setSpriteState(L"swordBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.2f);
		return;
	}
	pOwner->m_delayFrame = 0.85f;
	SoundManager::Instance->Play("change", false);
	pOwner->setSpriteState(L"swordPop", false);
	pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.3f);
	pOwner->m_isSword = true;
}
bool PlayerStateSword::Process(MyPlayer* pOwner)
{
	if (pOwner->m_delayFrame > 0.0f)
	{
		// 좌
		if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('A') == EKeyState::DOWN)
		{
			pOwner->MyObject::Move(-0.4f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('A') == EKeyState::HOLD)
		{
			pOwner->MyObject::Move(-0.4f, 0.0f);
		}
		// 우
		if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('D') == EKeyState::DOWN)
		{
			pOwner->MyObject::Move(0.4f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('D') == EKeyState::HOLD)
		{
			pOwner->MyObject::Move(0.4f, 0.0f);
		}
		return false;
	}

	if (pOwner->m_canNextSprite)
	{
		pOwner->m_canNextSprite = false;
		pOwner->setSpriteState(L"swordBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.2f);
	}


	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('A') == EKeyState::DOWN)
	{
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"swordRunning");
		pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.2f);
		pOwner->MyObject::Move(-1, 0);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"swordRunning");
		pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.2f);
		pOwner->MyObject::Move(-1, 0);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::UP ||
			 Input::Instance->getKeyState('A') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"swordBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.2f);
	}
	// 우
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"swordRunning");
		pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.2f);
		pOwner->MyObject::Move(1, 0);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"swordRunning");
		pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.2f);
		pOwner->MyObject::Move(1, 0);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::UP ||
			 Input::Instance->getKeyState('D') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"swordBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.2f);
		pOwner->setX(pOwner->m_prePosition.getRight() - pOwner->getWidth());
	}
	// 위
	if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
		Input::Instance->getKeyState('W') == EKeyState::HOLD)
	{
	}
	// 아래
	if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::DOWN ||
		Input::Instance->getKeyState('S') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround && pOwner->m_curSpriteName == L"swordBasic")
		{
			pOwner->setState(EPlayerState::Crawling);
			return true;
		}
	}
	else if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('S') == EKeyState::HOLD)
	{
	}
	else if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::UP ||
			 Input::Instance->getKeyState('S') == EKeyState::UP)
	{
	}

	// 점프
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround)
		{
			pOwner->m_isGround = false;
			pOwner->m_curJumpPower = pOwner->m_JumpPower;
		}
	}

	// 칼 넣기
	if (Input::Instance->getKeyState('Z') == EKeyState::DOWN)
	{
		pOwner->m_isSword = false;
		pOwner->setState(EPlayerState::Basic);
		return true;
	}

	// 공격
	if (Input::Instance->getKeyState('X') == EKeyState::DOWN)
	{
		pOwner->setState(EPlayerState::Attack);
		return true;
	}

	return true;
}

// 공격
void PlayerStateAttack::StateInit(MyPlayer* pOwner)
{
	pOwner->setSpriteState(L"swordAttack");
	pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.3f);
	pOwner->m_delayFrame = 0.35f;
}
bool PlayerStateAttack::Process(MyPlayer* pOwner)
{
	// 좌
	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('A') == EKeyState::DOWN)
	{
		//pOwner->m_curDirection = EDirection::Left;
		pOwner->MyObject::Move(-0.65f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		//pOwner->m_curDirection = EDirection::Left;
		pOwner->MyObject::Move(-0.65f, 0.0f);
	}
	// 우
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		//pOwner->m_curDirection = EDirection::Right;
		pOwner->MyObject::Move(0.65f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		//pOwner->m_curDirection = EDirection::Right;
		pOwner->MyObject::Move(0.65f, 0.0f);
	}
	// 점프
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround)
		{
			pOwner->m_isGround = false;
			pOwner->m_curJumpPower = pOwner->m_JumpPower;
		}
	}


	if (pOwner->m_delayFrame > 0.0f)
		return false;
	// 충전
	if (Input::Instance->getKeyState('X') == EKeyState::FREE || 
		pOwner->m_delayFrame <= -1.0f || pOwner->m_delayCharge > 0.0f)
	{
		pOwner->setSpriteState(L"swordAttack2", false);
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.3f);
	}
	else
	{
		pOwner->m_chargeCount += Timer::SPF;
		return false;
	}
	// 베기
	if (pOwner->m_delayFrame > -1000.0f)
	{
		if (pOwner->m_chargeCount <= 0.8f)
		{
			if (pOwner->m_curDirection == EDirection::Right)
				ObjectManager::Instance->TakeObject(L"EffectSlash")->setCenter(pOwner->getRight() + 85, (int)pOwner->MyCenter.y);
			else
				ObjectManager::Instance->TakeObject(L"EffectSlash")->setCenter(pOwner->getLeft() - 85, (int)pOwner->MyCenter.y);
			SoundManager::Instance->PlayVariation("bash", false, 3);
		}
		else
		{
			// 충전시
			if (pOwner->m_curDirection == EDirection::Right)
				ObjectManager::Instance->TakeObject(L"EffectChargeSlash", 400, 350)->setCenter(pOwner->getRight() + 175, (int)pOwner->MyCenter.y - 35);
			else
				ObjectManager::Instance->TakeObject(L"EffectChargeSlash", 400, 350)->setCenter(pOwner->getLeft() - 175, (int)pOwner->MyCenter.y - 35);
			SoundManager::Instance->Play("bashCharge1", false);
			pOwner->m_delayCharge = 7.0f;
		}
		pOwner->m_chargeCount = 0.0f;
		pOwner->m_delayFrame = -1000.0f;
	}
	if (pOwner->m_canNextSprite && pOwner->m_delayFrame <= -1000.4f)
	{
		pOwner->m_canNextSprite = false;
		pOwner->setState(EPlayerState::Sword);
	}

	return true;
}


// 피격
void PlayerStateHit::StateInit(MyPlayer* pOwner)
{
	pOwner->setSpriteState(L"hit", false);
	pOwner->setWH(pOwner->m_originWH);
	pOwner->m_delayFrame = 0.6f;
	pOwner->m_delayHit = 1.2f;
	SoundManager::Instance->PlayVariation("hit", false, 2);
}
bool PlayerStateHit::Process(MyPlayer* pOwner)
{
	if (pOwner->m_healthPoint <= 0 && pOwner->m_delayFrame <= 0.0f)
	{
		// 사망
		//if (!pOwner->m_isDead)
		//{
			//pOwner->m_isDead = true;
			//pOwner->setSpriteState(L"dead", false);
			//pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.15f);
			//SoundManager::Instance->Play("dead", false);
		//}
		pOwner->setState(EPlayerState::Dead);
		return false;
	}
	if (pOwner->m_delayFrame > 0.0f) 
	{
		pOwner->operBounce();
		// 좌
		if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('A') == EKeyState::DOWN)
		{
			pOwner->MyObject::Move(-0.5f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('A') == EKeyState::HOLD)
		{
			pOwner->MyObject::Move(-0.5f, 0.0f);
		}
		// 우
		if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('D') == EKeyState::DOWN)
		{
			pOwner->MyObject::Move(0.5f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('D') == EKeyState::HOLD)
		{
			pOwner->MyObject::Move(0.5f, 0.0f);
		}
		
		return false;
	}

	pOwner->setState(EPlayerState::Basic);
	return true;
}

// 사망
void PlayerStateDead::StateInit(MyPlayer* pOwner)
{
	pOwner->setSpriteState(L"dead", false);
	pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.15f);
	SoundManager::Instance->Play("dead", false);
	pOwner->m_healthPoint = 0;
}
bool PlayerStateDead::Process(MyPlayer* pOwner)
{
	return false;
}

// 스킬
void PlayerStateSkill::StateInit(MyPlayer* pOwner)
{
	pOwner->setSpriteState(L"swordAttack", false);
	pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.3f);
	pOwner->m_delayFrame = 0.25f;
}
bool PlayerStateSkill::Process(MyPlayer* pOwner)
{
	// 좌
	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('A') == EKeyState::DOWN)
	{
		pOwner->MyObject::Move(-0.75f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		pOwner->MyObject::Move(-0.75f, 0.0f);
	}
	// 우
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		pOwner->MyObject::Move(0.75f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		pOwner->MyObject::Move(0.75f, 0.0f);
	}
	// 점프
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround)
		{
			pOwner->m_isGround = false;
			pOwner->m_curJumpPower = pOwner->m_JumpPower;
		}
	}

	// 발사
	if (pOwner->m_canNextSprite && pOwner->m_delayFrame <= 0.0f)
	{
		pOwner->m_canNextSprite = false;
		pOwner->setState(EPlayerState::Basic);

		if (pOwner->m_curDirection == EDirection::Right)
		{
			auto obj = ObjectManager::Instance->TakeObject(L"PlayerBomb");
			obj->setWH(50, 50);
			obj->setCenter(pOwner->getRight() + 15, (int)pOwner->MyCenter.y - 10);
			if (Input::Instance->getKeyState(VK_UP) == EKeyState::FREE &&
				Input::Instance->getKeyState('W') == EKeyState::FREE)
				((MyObject*)obj)->setBounce(340, 850);
			else
				((MyObject*)obj)->setBounce(315, 850);
		}
		else
		{
			auto obj = ObjectManager::Instance->TakeObject(L"PlayerBomb");
			obj->setWH(50, 50);
			obj->setCenter(pOwner->getLeft() - 15, (int)pOwner->MyCenter.y - 10);
			if (Input::Instance->getKeyState(VK_UP) == EKeyState::FREE &&
				Input::Instance->getKeyState('W') == EKeyState::FREE)
				((MyObject*)obj)->setBounce(205, 850);
			else
				((MyObject*)obj)->setBounce(230, 850);
		}
		SoundManager::Instance->PlayVariation("bash", false, 2);
		pOwner->setState(EPlayerState::Basic);
	}

	return true;
}