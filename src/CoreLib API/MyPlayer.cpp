#include "MyPlayer.h"
#include "ObjectManager.h"

Point  MyPlayer::MyCenter = { 0, 0 };

MyPlayer::MyPlayer() : m_JumpPower(650.0f), m_MaxGravity(-1300.0f), m_isSword(false)
{																   
	m_curDirection = EDirection::Right;
	m_isRepeat = true;
}

MyPlayer::~MyPlayer() {}

bool MyPlayer::Init()
{
	MyObject::Init();
	BitMapManager::Instance->AddObjBitMap(L"Player", this);
	m_isGround = false;
	m_objType = EObjType::Player;
	m_speed = 500.0f;
	m_curJumpPower = 0.0f;
	
	setWH(49, 68);
	setCenter(100, 100);
	m_originWH = getWH();

	// 상태 입력
	m_stateList[EPlayerState::Basic] = new PlayerStateBasic();
	m_stateList[EPlayerState::Crawling] = new PlayerStateCrawling();
	m_stateList[EPlayerState::Sword] = new PlayerStateSword();
	m_stateList[EPlayerState::Attack] = new PlayerStateAttack();
	m_stateList[EPlayerState::Hit] = new PlayerStateHit();
	m_stateList[EPlayerState::Dead] = new PlayerStateDead();
	m_stateList[EPlayerState::Skill] = new PlayerStateSkill();
	m_curState = m_stateList[EPlayerState::Basic];
	setState(EPlayerState::Basic);
	return true;
}


bool MyPlayer::KeyInput()
{
	// 마우스
	if (Input::isDebug)
	{
		// 리셋
		if (Input::Instance->getMouseState(0) == EKeyState::DOWN)
		{
			m_healthPoint = 100;
			setEnable(true);
			setState(EPlayerState::Basic);
			setCenter(Input::Instance->getCursor().x + (int)Window::DrawPoint.x, Input::Instance->getCursor().y);
		}
		if (Input::Instance->getMouseState(1) == EKeyState::DOWN)
		{
			ObjectManager::Instance->TakeObject(L"Fairy")->setCenter(Input::Instance->getCursor().x + (int)Window::DrawPoint.x, Input::Instance->getCursor().y);
		}

		if (Input::Instance->getMouseState(2) == EKeyState::DOWN)
		{
			static wostringstream ostr;
			m_preCursor = Input::Instance->getCursor();
			ostr.str(L"");
			ostr << "게임좌표 : x(" << m_preCursor.x + Window::DrawPoint.x << "), y(" << m_preCursor.y << ")" << endl;
			OutputDebugString(ostr.str().c_str());
		}
		else if (Input::Instance->getMouseState(2) == EKeyState::UP)
		{
			static wostringstream ostr;
			ostr.str(L"");
			ostr << "너비높이 : width(" << Input::Instance->getCursor().x - m_preCursor.x + Window::DrawPoint.x
				<< "), height(" << Input::Instance->getCursor().y - m_preCursor.y << ")" << endl;
			OutputDebugString(ostr.str().c_str());
		}
	}

	return true;
}


bool MyPlayer::Collision()
{
	// 중력, 점프
	if (m_curJumpPower > m_MaxGravity)
		m_curJumpPower -= GravityPower * Timer::SPF;
	else
		m_curJumpPower = m_MaxGravity;
	setY((int)(m_point.y - (m_curJumpPower * Timer::SPF)));

	// 피격
	if (m_delayHit <= 0.0f && 
		m_curState->m_myState != EPlayerState::Dead)
	{
		for (auto iter : ObjectManager::Instance->m_ObjectList[EObjType::Enemy])
		{
			if (!iter->isEnable())
				continue;
			if (CollisionCheck(*iter))
			{
				m_healthPoint -= iter->m_damage;
				ObjectManager::Instance->TakeObject(L"Bomb")
					->setCenter(MyCenter - (MyCenter - iter->getCenter()) / 4);
				setState(EPlayerState::Hit);
				setBounce(getAngle(iter->getCenter(), this->getCenter()), 150);
			}
		}
	}
	for (auto iter : ObjectManager::Instance->m_ObjectList[EObjType::EnemyAttack])
	{
		if (!iter->isEnable())
			continue;
		if (CollisionCheck(*iter))
		{
			iter->setEnable(false);
			ObjectManager::Instance->TakeObject(L"Explosion")
				->setCenter(MyCenter - (MyCenter - iter->getCenter()) / 4);
			if (m_delayHit <= 0.0f && 
				m_curState->m_myState != EPlayerState::Dead)
			{
				m_healthPoint -= iter->m_damage;
				setState(EPlayerState::Hit);
				setBounce(getAngle(iter->getCenter(), this->getCenter()), 150);
			}
		}
	}

	// 충돌 체크
	m_isGround = false;
	for (auto& iter : ObjectManager::Instance->m_WallList)
	{
		if (CollisionCheck(*iter))
		{
			if (m_prePosition.getY() + m_prePosition.getHeight() <= iter->getTop())
			{
				setY(iter->m_point.y - m_height);
				m_curJumpPower = 0.0f;
				m_isGround = true;
			}
			else if (m_prePosition.getY() >= iter->getBottom())
			{
				setY(iter->getBottom());
				m_curJumpPower -= GravityPower * 1.5f * Timer::SPF;
			}
			else if (m_prePosition.getX() + m_prePosition.getWidth() <= iter->getLeft())
			{
				setX(iter->getLeft() - m_width - 1);
			}
			else if (m_prePosition.getX() >= iter->getRight())
			{
				setX(iter->getRight() + 1);
			}
			if(Input::isDebug)
				ObjectManager::Instance->TakeObject(L"Clash")->setCenter(getCenter());
		}
	}

	return true;
}

bool MyPlayer::Frame()
{
	if (!m_isEnable) return false;

	m_delayFrame  -= Timer::SPF;
	m_delayAttack -= Timer::SPF;
	m_delayDodge  -= Timer::SPF;
	m_delayCharge -= Timer::SPF;
	m_delayHit    -= Timer::SPF;

	getCenter();
	m_prePosition = *this;

	m_curState->Process(this);
	KeyInput();
	Collision();
	 
	SynchPosition();
	return true;
}


bool MyPlayer::Render()
{
	if (m_isEnable == false)	return false;

	if(m_curDirection == EDirection::Left)
	{
		m_curSprite->m_maskBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCAND,	  false);
		m_curSprite->m_mainBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCINVERT, false);
		m_curSprite->m_maskBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCINVERT, false);
	}
	else
	{
		m_curSprite->m_maskBit->DrawStretch(*this, m_curSprite->m_srcRect, SRCAND,	  true);
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

bool MyPlayer::setState(const EPlayerState state)
{
	m_preState = m_curState;
	m_curState = m_stateList[state];
	m_curState->StateInit(this);
	return true;
}

bool MyPlayer::HeadCheck()
{
	// 모션 변경시 상단 벽에 막혀 있다면
	if (m_isGround)
	{
		auto rect = MyRect(getX(), getBottom() - (int)m_originWH.y, (int)m_originWH.x, (int)m_originWH.y);
		for (auto& iter : ObjectManager::Instance->m_WallList)
		{
			if (rect.CollisionCheck(*iter))
			{
				if (m_prePosition.getY() >= iter->getBottom())
				{
					//setState(EPlayerState::Crawling);
					//setBottom(m_prePosition.getBottom());
					return true;
				}
			}
		}
	}
	return false;
}

void MyPlayer::SynchPosition()
{
	Window::DrawPoint = Instance->getPoint();
	Window::DrawPoint.x -= Window::getClientRect().right / 2;
	if (Window::DrawPoint.x < MyBitMap::MapRect->getX())
		Window::DrawPoint.x = (float)MyBitMap::MapRect->getX();
	else if(Window::DrawPoint.x > MyBitMap::MapRect->getRight() - Window::getClientRect().right)
		Window::DrawPoint.x = (float)MyBitMap::MapRect->getRight() - Window::getClientRect().right;
}

bool MyPlayer::setSpriteState(const wstring& stateName, const bool& isRepeat)
{
	if (m_curSpriteName == stateName)
		return false;
	if (!isRepeat)
		m_canNextSprite = false;

	m_isRepeat = isRepeat;
	MyBitMap::setSpriteList(stateName);
	m_curSpriteName = stateName;
	return true;
}

void MyPlayer::setWH(const int& width, const int& height)	noexcept
{
	m_width = width;
	m_height = height;
	setBottom(m_prePosition.getBottom());
}
void MyPlayer::setWH(const float& width, const float& height)	noexcept
{
	m_width = (int)width;
	m_height = (int)height;
	setBottom(m_prePosition.getBottom());
}
void MyPlayer::setWH(const Point& width_height)			noexcept
{
	m_width = (int)width_height.x;
	m_height = (int)width_height.y;
	setBottom(m_prePosition.getBottom());
}

void MyPlayer::setBottom(const int& bottom)	noexcept
{
	setY(bottom - m_height);
}

Point MyPlayer::getCenter()
{
	return MyCenter = { Instance->m_point.x + Instance->m_width / 2, Instance->m_point.y + Instance->m_height / 2 };
}

MyBitMap* MyPlayer::clone()
{
	return nullptr;
}