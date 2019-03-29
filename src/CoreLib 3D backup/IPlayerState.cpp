#include "IPlayerState.h"
#include "ObjectManager.h"
#include "MyPlayer.h"
#include "SoundManager.h"
#include "PlayerServant.h"


// ±âº»
void PlayerStateBasic::StateInit(MyPlayer* pOwner)
{
	if (pOwner->m_isSword)
	{
		if (pOwner->m_preState->m_myState == EPlayerState::Crawling)
		{
			pOwner->setSpriteState(L"crawlingReverse", false);
			pOwner->setWH(pOwner->m_originWH.x * 1.4f, pOwner->m_originWH.y * 0.9f);
			pOwner->m_delayFrame = 0.2f;
			return;
		}
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
	if (pOwner->m_preState->m_myState == EPlayerState::Crawling)
	{
		pOwner->setSpriteState(L"crawlingReverse", false);
		pOwner->setWH(pOwner->m_originWH.x * 1.4f, pOwner->m_originWH.y * 0.9f);
		pOwner->m_delayFrame = 0.2f;
		return;
	}
	pOwner->setSpriteState(L"basic");
	pOwner->setWH(pOwner->m_originWH);
}
bool PlayerStateBasic::Process(MyPlayer* pOwner)
{
	// Ä® ³Ö´ÂÁß
	if (pOwner->m_delayFrame > 0.0f)
	{
		// ÁÂ
		if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('A') == EKeyState::DOWN)
		{
			pOwner->Move(-0.4f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('A') == EKeyState::HOLD)
		{
			pOwner->Move(-0.4f, 0.0f);
		}
		// ¿ì
		if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('D') == EKeyState::DOWN)
		{
			pOwner->Move(0.4f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('D') == EKeyState::HOLD)
		{
			pOwner->Move(0.4f, 0.0f);
		}
		return false;
	}
	if (pOwner->m_canNextSprite)
	{
		pOwner->m_canNextSprite = false;
		pOwner->setSpriteState(L"basic");
		pOwner->setWH(pOwner->m_originWH.x, pOwner->m_originWH.y);
	}

	// µ¿ÀÛ
	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('A') == EKeyState::DOWN)
	{
		if (pOwner->m_inputDirection == EDirection::Left &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Left;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"running");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y);
		pOwner->Move(-1, 0);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"running");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y);
		pOwner->Move(-1, 0);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::UP ||
			 Input::Instance->getKeyState('A') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"basic");
		pOwner->setWH(pOwner->m_originWH);
	}
	// ¿ì
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		if (pOwner->m_inputDirection == EDirection::Right &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Right;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"running");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y);
		pOwner->Move(1, 0);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"running");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y);
		pOwner->Move(1, 0);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::UP ||
			 Input::Instance->getKeyState('D') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"basic");
		pOwner->setWH(pOwner->m_originWH);
		pOwner->setX(pOwner->m_prevPosition.getRight() - pOwner->getWidth());
	}
	// À§
	if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
		Input::Instance->getKeyState('W') == EKeyState::HOLD)
	{
	}
	// ¾Æ·¡
	if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::DOWN ||
		Input::Instance->getKeyState('S') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround && pOwner->m_curSpriteName == L"basic")
		{
			pOwner->setState(EPlayerState::Crawling);
			return true;
		}
	}

	// Á¡ÇÁ
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround)
		{
			pOwner->m_isGround = false;
			pOwner->m_curGravityPower = pOwner->m_kJumpPower;
		}
	}

	// Ä®²¨³»±â
	if (Input::Instance->getKeyState('Z') == EKeyState::DOWN)
	{
		pOwner->setState(EPlayerState::Sword);
		if (pOwner->isEquipItem(L"ShadowServant"))
		{
			pOwner->m_Servant->setEnable(true);
			pOwner->m_Servant->setCenter(pOwner->MyObject::getCenter());
			//pOwner->m_Servant->setState(EPlayerState::Sword);
		}
		return true;
	}

	// ½ºÅ³
	if (Input::Instance->getKeyState('X') == EKeyState::DOWN)
	{
		pOwner->setState(EPlayerState::Skill);
		return true;
	}

	return true;
}


// ¾þµå¸²
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
	pOwner->SetColor(1.0f, 1.0f, 1.0f);
	if (pOwner->m_canNextSprite)
	{
		pOwner->m_canNextSprite = false;
		pOwner->setSpriteState(L"crawlingBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
	}

	// ÈíÇ÷, ÀÏ¾î¼­±â
	if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::HOLD ||
		Input::Instance->getKeyState('S') == EKeyState::HOLD)
	{
		if (pOwner->m_healthPoint < pOwner->m_kMaxHealth && BloodRate::Instance->operHealthRate(-0.3f * pOwner->m_curSPF))
		{
			pOwner->SetColor(1.0f, 0.4f, 0.4f);
			pOwner->operHp(15 * pOwner->m_curSPF);
		}
	} 
	else if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::UP ||
			 Input::Instance->getKeyState('S') == EKeyState::UP)
	{
		if (!pOwner->HeadCheck())
		{
			pOwner->setState(EPlayerState::Basic);
			return true;
		}
	}

	// ÁÂ
	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('A') == EKeyState::DOWN)
	{
		if (pOwner->m_inputDirection == EDirection::Left &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Left;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"crawling");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
		pOwner->Move(-0.5f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"crawling");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
		pOwner->Move(-0.5f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::UP ||
			 Input::Instance->getKeyState('A') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"crawlingBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
	}
	// ¿ì
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		if (pOwner->m_inputDirection == EDirection::Right &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Right;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"crawling");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f,pOwner->m_originWH.y * 0.55f);
		pOwner->Move(0.5f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"crawling");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
		pOwner->Move(0.5f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::UP ||
			 Input::Instance->getKeyState('D') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"crawlingBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.5f, pOwner->m_originWH.y * 0.55f);
	}
	// À§
	if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
		Input::Instance->getKeyState('W') == EKeyState::HOLD)
	{
	}

	// ³»·Á°¡±â
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround && pOwner->getBottom() < ObjectManager::MapWallY)
		{
			pOwner->m_prevPosition.Move(0, 1);
			pOwner->MyRect::Move(0, pOwner->getY() + 1);
			pOwner->SetColor(1.0f, 1.0f, 1.0f);
			pOwner->setState(EPlayerState::Basic);
		}
	}

	return true;
}


// °Ë
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
	// Ä® ²¨³»±â
	if (pOwner->m_delayFrame > 0.0f)
	{
		// ÁÂ
		if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('A') == EKeyState::DOWN)
		{
			pOwner->Move(-0.4f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('A') == EKeyState::HOLD)
		{
			pOwner->Move(-0.4f, 0.0f);
		}
		// ¿ì
		if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('D') == EKeyState::DOWN)
		{
			pOwner->Move(0.4f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('D') == EKeyState::HOLD)
		{
			pOwner->Move(0.4f, 0.0f);
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
		if (pOwner->m_inputDirection == EDirection::Left &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Left;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"swordRunning");
		pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.2f);
		pOwner->Move(-1, 0);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Left;
		pOwner->setSpriteState(L"swordRunning");
		pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.2f);
		pOwner->Move(-1, 0);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::UP ||
			 Input::Instance->getKeyState('A') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"swordBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.2f);
	}
	// ¿ì
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		if (pOwner->m_inputDirection == EDirection::Right &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Right;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"swordRunning");
		pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.2f);
		pOwner->Move(1, 0);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		pOwner->m_curDirection = EDirection::Right;
		pOwner->setSpriteState(L"swordRunning");
		pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.2f);
		pOwner->Move(1, 0);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::UP ||
			 Input::Instance->getKeyState('D') == EKeyState::UP)
	{
		pOwner->setSpriteState(L"swordBasic");
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.2f);
		pOwner->setX(pOwner->m_prevPosition.getRight() - pOwner->getWidth());
	}
	// À§
	if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
		Input::Instance->getKeyState('W') == EKeyState::HOLD)
	{
	}
	// ¾Æ·¡
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

	// Á¡ÇÁ
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround)
		{
			pOwner->m_isGround = false;
			pOwner->m_curGravityPower = pOwner->m_kJumpPower;
		}
	}

	// Ä® ³Ö±â
	if (Input::Instance->getKeyState('Z') == EKeyState::DOWN)
	{
		pOwner->m_isSword = false;
		pOwner->setState(EPlayerState::Basic);
		if (pOwner->isEquipItem(L"ShadowServant"))
		{
			pOwner->m_Servant->setEnable(true);
			pOwner->m_Servant->setCenter(pOwner->MyObject::getCenter());
			//pOwner->m_Servant->setState(EPlayerState::Basic);
		}
		return true;
	}

	// °ø°Ý
	if (Input::Instance->getKeyState('X') == EKeyState::DOWN)
	{
		pOwner->setState(EPlayerState::Attack);
		return true;
	}

	return true;
}

// °ø°Ý
void PlayerStateAttack::StateInit(MyPlayer* pOwner)
{
	pOwner->setSpriteState(L"swordAttack");
	pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.3f);
	pOwner->m_delayFrame = 0.22f;
}
bool PlayerStateAttack::Process(MyPlayer* pOwner)
{
	// ÁÂ
	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('A') == EKeyState::DOWN)
	{
		if (pOwner->m_inputDirection == EDirection::Left &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Left;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->Move(-0.65f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		pOwner->Move(-0.65f, 0.0f);
	}
	// ¿ì
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		if (pOwner->m_inputDirection == EDirection::Right &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Right;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->Move(0.65f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		pOwner->Move(0.65f, 0.0f);
	}
	// Á¡ÇÁ
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround)
		{
			pOwner->m_isGround = false;
			pOwner->m_curGravityPower = pOwner->m_kJumpPower;
		}
	}

	if (pOwner->m_delayFrame > 0.0f)
		return false;
	// ÃæÀü
	if (Input::Instance->getKeyState('X') == EKeyState::FREE || 
		pOwner->m_delayFrame <= -1.0f || pOwner->m_delayCharge > 0.0f)
	{
		pOwner->setSpriteState(L"swordAttack2", false);
		pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.3f);
	}
	else
	{
		pOwner->m_chargeCount += pOwner->m_curSPF;
		return false;
	}
	// º£±â
	if (pOwner->m_delayFrame > -1000.0f)
	{
		if (pOwner->m_chargeCount <= (0.8f / pOwner->m_rateAction))
		{
			if (pOwner->m_curDirection == EDirection::Right)
				ObjectManager::Instance->TakeObject(L"EffectSlash")
					->setCenter(pOwner->getRight() + 85, (int)pOwner->MyObject::getCenter().y);
			else
				ObjectManager::Instance->TakeObject(L"EffectSlash")
					->setCenter(pOwner->getLeft() - 85, (int)pOwner->MyObject::getCenter().y);
			SoundManager::Instance->PlayVariation("bash", false, 3);
		}
		else
		{
			// ÃæÀü½Ã
			if (pOwner->m_curDirection == EDirection::Right)
				ObjectManager::Instance->TakeObject(L"EffectChargeSlash", 400, 350)
					->setCenter(pOwner->getRight() + 175, (int)pOwner->MyObject::getCenter().y - 35);
			else
				ObjectManager::Instance->TakeObject(L"EffectChargeSlash", 400, 350)
					->setCenter(pOwner->getLeft() - 175, (int)pOwner->MyObject::getCenter().y - 35);
			SoundManager::Instance->Play("bashCharge1", false);
			pOwner->m_delayCharge = 7.0f;
		}
		pOwner->m_chargeCount = 0.0f;
		pOwner->m_delayFrame = -1000.0f;
	}
	if (pOwner->m_canNextSprite && pOwner->m_delayFrame <= -1000.3f)
	{
		pOwner->m_canNextSprite = false;
		pOwner->setState(EPlayerState::Sword);
	}

	return true;
}


// ½ºÅ³(ÆøÅº)
void PlayerStateSkill::StateInit(MyPlayer* pOwner)
{
	pOwner->setSpriteState(L"swordAttack", false);
	pOwner->setWH(pOwner->m_originWH.x * 1.3f, pOwner->m_originWH.y * 1.3f);
	pOwner->m_delayFrame = 0.25f;
}
bool PlayerStateSkill::Process(MyPlayer* pOwner)
{
	// ÁÂ
	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('A') == EKeyState::DOWN)
	{
		if (pOwner->m_inputDirection == EDirection::Left &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Left;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->Move(-0.75f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		pOwner->Move(-0.75f, 0.0f);
	}
	// ¿ì
	if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
		Input::Instance->getKeyState('D') == EKeyState::DOWN)
	{
		if (pOwner->m_inputDirection == EDirection::Right &&
			pOwner->m_dashCount > 0.0f && pOwner->m_delayDash <= 0.0f)
		{
			pOwner->m_dashCount = 0.0f;
			pOwner->setState(EPlayerState::Dash);
			return true;
		}
		else
		{
			pOwner->m_inputDirection = EDirection::Right;
			pOwner->m_dashCount = 0.25f;
		}
		pOwner->Move(0.75f, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		pOwner->Move(0.75f, 0.0f);
	}
	// Á¡ÇÁ
	if (Input::Instance->getKeyState(VK_SPACE) == EKeyState::DOWN ||
		Input::Instance->getKeyState('C') == EKeyState::DOWN)
	{
		if (pOwner->m_isGround)
		{
			pOwner->m_isGround = false;
			pOwner->m_curGravityPower = pOwner->m_kJumpPower;
		}
	}

	// ¹ß»ç
	if (pOwner->m_canNextSprite && pOwner->m_delayFrame <= 0.0f)
	{
		pOwner->m_canNextSprite = false;
		pOwner->setState(EPlayerState::Basic);

		if (pOwner->m_curDirection == EDirection::Right)
		{
			auto obj = ObjectManager::Instance->TakeObject(L"PlayerBomb", 50, 50);
			obj->setCenter(pOwner->getRight() + 15, (int)pOwner->MyObject::getCenter().y - 10);
			if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::HOLD ||
				Input::Instance->getKeyState('S') == EKeyState::HOLD)
				((MyObject*)obj)->setBounce(355, 1000);
			else if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
					 Input::Instance->getKeyState('W') == EKeyState::HOLD)
					 ((MyObject*)obj)->setBounce(290, 1000);
			else
				((MyObject*)obj)->setBounce(325, 1000);
		}
		else
		{
			auto obj = ObjectManager::Instance->TakeObject(L"PlayerBomb", 50, 50);
			obj->setWH(50, 50);
			obj->setCenter(pOwner->getLeft() - 15, (int)pOwner->MyObject::getCenter().y - 10);
			if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::HOLD ||
				Input::Instance->getKeyState('S') == EKeyState::HOLD)
				((MyObject*)obj)->setBounce(185, 1000);
			else if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
					 Input::Instance->getKeyState('W') == EKeyState::HOLD)
					 ((MyObject*)obj)->setBounce(250, 1000);
			else
				((MyObject*)obj)->setBounce(215, 1000);
		}
		SoundManager::Instance->PlayVariation("bash", false, 2);
		pOwner->setState(EPlayerState::Basic);
	}

	return true;
}


// ´ë½¬
void PlayerStateDash::StateInit(MyPlayer* pOwner)
{
	pOwner->setSpriteState(L"dash", false);
	pOwner->setWH(pOwner->m_originWH.x * 1.4f, pOwner->m_originWH.y * 0.9f);
	pOwner->m_delayCount = 0.2f;
	pOwner->setDelayHit(0.6f, false);
	pOwner->m_delayDash = 1.3f;
	pOwner->SetColor(1.0f, 1.0f, 1.0f);
	SoundManager::Instance->Play("dash", false);
	if (pOwner->m_inputDirection == EDirection::Right)
	{
		auto obj = ObjectManager::Instance->TakeObject(L"DashWind", pOwner->MyObject::getCenter());
		obj->setDirection(EDirection::Left);
		obj->UpdateTextureUV();
		obj->setBottom(pOwner->getBottom());
		obj->setRight(pOwner->getLeft());
		pOwner->setBounce(355, 600);
	}
	else
	{
		auto obj = ObjectManager::Instance->TakeObject(L"DashWind", pOwner->MyObject::getCenter());
		obj->setDirection(EDirection::Right);
		obj->UpdateTextureUV();
		obj->setBottom(pOwner->getBottom());
		obj->setLeft(pOwner->getRight());
		pOwner->setBounce(185, 600);
	}
}
bool PlayerStateDash::Process(MyPlayer* pOwner)
{
	static bool inputAttack = false;
	
	if (pOwner->m_delayCount > 0.0f)
	{
		pOwner->operBounce(pOwner->m_curSPF);
		// ÁÂ
		if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('A') == EKeyState::DOWN)
		{
			pOwner->Move(-0.5f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('A') == EKeyState::HOLD)
		{
			pOwner->Move(-0.5f, 0.0f);
		}
		// ¿ì
		if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('D') == EKeyState::DOWN)
		{
			pOwner->Move(0.5f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('D') == EKeyState::HOLD)
		{
			pOwner->Move(0.5f, 0.0f);
		}

		if (Input::Instance->getKeyState('X') == EKeyState::DOWN &&
			pOwner->m_isSword == true)
		{
			inputAttack = true;
		}
		pOwner->operBounce(pOwner->m_curSPF);
		return true;
	}

	if (pOwner->m_preState->m_myState == EPlayerState::Attack ||
		inputAttack == true)
	{
		inputAttack = false;
		pOwner->setState(EPlayerState::Attack);
		pOwner->m_delayFrame = 0.06f;
	}
	else
		pOwner->setState(EPlayerState::Basic);
	return true;
}


// ÇÇ°Ý
void PlayerStateHit::StateInit(MyPlayer* pOwner)
{
	pOwner->setSpriteState(L"hit", false);
	pOwner->setWH(pOwner->m_originWH);
	pOwner->m_chargeCount = 0.0f;
	pOwner->m_delayFrame = 0.6f;
	pOwner->setDelayHit(1.2f);
	pOwner->SetColor(1.0f, 1.0f, 1.0f);
	SoundManager::Instance->PlayVariation("hit", false, 2);
}
bool PlayerStateHit::Process(MyPlayer* pOwner)
{
	if (pOwner->m_healthPoint <= 0 && pOwner->m_delayFrame <= 0.0f)
	{
		pOwner->setState(EPlayerState::Dead);
		return false;
	}
	if (pOwner->m_delayFrame > 0.0f) 
	{
		pOwner->operBounce(pOwner->m_curSPF);
		// ÁÂ
		if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('A') == EKeyState::DOWN)
		{
			pOwner->Move(-0.5f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('A') == EKeyState::HOLD)
		{
			pOwner->Move(-0.5f, 0.0f);
		}
		// ¿ì
		if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::DOWN ||
			Input::Instance->getKeyState('D') == EKeyState::DOWN)
		{
			pOwner->Move(0.5f, 0.0f);
		}
		else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('D') == EKeyState::HOLD)
		{
			pOwner->Move(0.5f, 0.0f);
		}
		
		return false;
	}

	pOwner->setState(EPlayerState::Basic);
	return true;
}

// »ç¸Á
void PlayerStateDead::StateInit(MyPlayer* pOwner)
{
	pOwner->setSpriteState(L"dead", false);
	pOwner->setWH(pOwner->m_originWH.x * 1.6f, pOwner->m_originWH.y * 1.15f);
	SoundManager::Instance->Play("dead", false);
	pOwner->m_healthPoint = 0;
	pOwner->m_pPShader = DxManager::Instance->m_PShaderList["PShader"];
	pOwner->m_Servant->setEnable(false);
}
bool PlayerStateDead::Process(MyPlayer* pOwner)
{
	pOwner->getX();
	return false;
}