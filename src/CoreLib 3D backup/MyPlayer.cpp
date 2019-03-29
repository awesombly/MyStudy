#include "MyPlayer.h"
#include "ObjectManager.h"
#include "PlayerServant.h"
#include "SoundManager.h"

MyVector2  MyPlayer::MyCenter = { 0, 0 };
int MyPlayer::Score = 0;


bool MyPlayer::Init(const wstring_view& myName, const EObjType& objType,
					const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	MyActor::Init(std::forward<const wstring_view>(myName), objType,
		 std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName));
	MyObject::Init();

	setWH(49, 68);
	setCenter(100, 100);
	m_originWH = getWH();

	m_curDirection = EDirection::Right;
	m_isSword = false;
	m_isGround = false;
	m_healthPoint = 100;
	m_speed = 400.0f;
	m_delayFrame = 0.0f;
	m_delayAttack = 0.0f;
	m_delayCount = 0.0f;
	m_delayCharge = 0.0f;
	m_delayHit = 0.0f;

	// 상태 입력
	m_stateList.try_emplace(EPlayerState::Basic, new PlayerStateBasic());
	m_stateList.try_emplace(EPlayerState::Crawling, new PlayerStateCrawling());
	m_stateList.try_emplace(EPlayerState::Sword, new PlayerStateSword());
	m_stateList.try_emplace(EPlayerState::Attack, new PlayerStateAttack());
	m_stateList.try_emplace(EPlayerState::Skill, new PlayerStateSkill());
	m_stateList.try_emplace(EPlayerState::Dash, new PlayerStateDash());
	m_stateList.try_emplace(EPlayerState::Hit, new PlayerStateHit());
	m_stateList.try_emplace(EPlayerState::Dead, new PlayerStateDead());
	m_curState = m_stateList[EPlayerState::Basic];
	setState(EPlayerState::Basic);

	// 소모품 UI
	MyUI* pConsum = nullptr;
	m_ConsumUI.reserve(5);
	for (int i = 0; i < 5; i++)
	{
		pConsum = new MyUI(L"ItemIcon", EObjType::UI);
		pConsum->setRect((float)(Window::getClientRect().right - 330 + i * 60), Window::getClientRect().bottom * 0.76f, 50, 50);
		m_ConsumUI.push_back(pConsum);
	}

	m_Servant = new PlayerServant(L"Player", EObjType::Collider);
	m_Servant->SetPixelShader("PHalfAlpha");
	//m_Servant->SetColor(0.5f, 0.5f, 0.5f);
	m_Servant->setEnable(false);

	ObjectManager::Instance->PushObject(this);
	return true;
}


bool MyPlayer::Frame(const float& spf, const float& accTime) noexcept
{
	if (!MyActor::Frame(spf * m_rateAction, accTime)) return false;
	m_prevPosition = *this;
	getCenter();
	m_curSPF = spf;

	m_delayFrame  -= spf * m_rateAction;
	m_delayAttack -= spf * m_rateMove;
	m_delayCharge -= spf * m_rateMove;
	m_delayDash   -= spf * m_rateMove;
	m_delayCount  -= spf;
	m_dashCount   -= spf;
	if (m_delayHit > 0.0f)
	{
		if((m_delayHit -= spf) < 0.0f)
			m_pPShader = DxManager::Instance->m_PShaderList["PShader"];
	}
	
	m_curState->Process(this);		// 실질적 행동

	MouseCheck();
	Collision(spf);
	SynchPosition();

	int i = 0;
	for (auto& iter : m_UsingItem)
	{
		if (iter->m_itemType == EItemType::Consum)
		{
			m_ConsumUI[i]->setSpriteList(iter->m_myName);
			m_ConsumUI[i++]->Frame(spf, accTime);
		}
	}

	m_Servant->Frame(spf, accTime);
	return true;
	accTime;
}

bool MyPlayer::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (m_isEnable == false)	return false;
	PrevRender(pDContext);
	PostRender(pDContext);
	if (Input::isDebug)
	{
		static wostringstream ostr;
		ostr.str(L"");
		ostr << L"CursorPos(" << Input::getCursor().x << L",  " << Input::getCursor().y << L")    " << L"Center(" << (int)MyObject::getCenter().x << L",  " << (int)MyObject::getCenter().y << L")";
		WriteManager::Instance->DrawTextW({ 10, 45, 350, 100 }, ostr.str().c_str());
	}

	int i = 0;
	for (auto& iter : m_UsingItem)
	{
		if (iter->m_itemType == EItemType::Consum)
		{
			m_ConsumUI[i]->setSpriteList(iter->m_myName);
			static float colorRate = 0.0f;
			colorRate = iter->m_delayUsing / iter->m_usingValue;
			m_ConsumUI[i]->SetColor(colorRate, colorRate, colorRate);
			m_ConsumUI[i++]->Render(pDContext);
		}
	}
	m_Servant->Render(pDContext);
	return true;
}


bool MyPlayer::MouseCheck()
{
	// 마우스
	if (Input::Instance->getMouseState(1) == EKeyState::DOWN)
	{
		m_Servant->setPoint(Input::getCursor());
	}

	if (Input::isDebug)
	{
		// 리셋
		if (Input::Instance->getMouseState(0) == EKeyState::DOWN)
		{
			setHp(100);
			setEnable(true);
			setState(EPlayerState::Basic);
			setCenter(Input::Instance->getCursor().x + (int)Window::DrawPoint.x, Input::Instance->getCursor().y);
		}
		if (Input::Instance->getMouseState(1) == EKeyState::DOWN)
		{
			ObjectManager::Instance->TakeObject(L"Fairy")->setCenter(Input::Instance->getCursor().x + (int)Window::DrawPoint.x, Input::Instance->getCursor().y);
		}
	}
	// 벽 생성
	static MyActor* pWall = nullptr;
	if (Input::Instance->getMouseState(2) == EKeyState::DOWN &&
		isEquipItem(L"CreateStone"))
	{
		m_prevCursor = Input::Instance->getCursor();
		pWall = ObjectManager::Instance->PushObject(new MyActor(L"ObjectSteel", EObjType::Collider));
		pWall->setRect(m_prevCursor.x + Window::DrawPoint.x, m_prevCursor.y + Window::DrawPoint.y, 0, 0);
	}
	else if (Input::Instance->getMouseState(2) == EKeyState::HOLD && 
			 isEquipItem(L"CreateStone"))
	{
		pWall->setWH(myStd::AbsT(Input::getCursor().x - m_prevCursor.x), myStd::AbsT(Input::getCursor().y - m_prevCursor.y));
	}
	//
	if (Input::Instance->getKeyState('F') == EKeyState::HOLD)
	{
		ObjectManager::Instance->TakeObject(L"ScoreItem", { Input::Instance->getCursor().x + (int)Window::DrawPoint.x, Input::Instance->getCursor().y });
	}
	// 템 만듬
	if (Input::Instance->getKeyState('V') == EKeyState::DOWN)
	{
		ObjectManager::Instance->TakeObject(L"ItemIcon", getCenter());
	}
	return true;
}


bool MyPlayer::Collision(const float& spf)
{
	// 중력, 점프
	if (m_curGravityPower > m_kMaxGravity)
		m_curGravityPower -= GravityPower * spf;
	else
		m_curGravityPower = m_kMaxGravity;
	setY(m_point.y - (m_curGravityPower * spf));

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
				operHp(-(float)((MyObject*)iter)->m_damage);
				ObjectManager::Instance->TakeObject(L"Bomb", 32, 32)
					->setCenter(MyObject::getCenter() - (MyObject::getCenter() - iter->getCenter()) / 4);
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
			iter->setEnable(false, true);
			ObjectManager::Instance->TakeObject(L"Explosion", 32, 32)
				->setCenter(MyObject::getCenter() - (MyObject::getCenter() - iter->getCenter()) / 4);
			if (m_delayHit <= 0.0f &&
				m_curState->m_myState != EPlayerState::Dead)
			{
				operHp(-(float)((MyObject*)iter)->m_damage);
				setState(EPlayerState::Hit);
				setBounce(getAngle(iter->getCenter(), this->getCenter()), 150);
			}
		}
	}

	// 아이템
	for (auto iter : ObjectManager::Instance->m_ObjectList[EObjType::Item])
	{
		if (!iter->isEnable() ||
			Input::Instance->getKeyState(VK_UP) != EKeyState::DOWN)
			continue;
		if (CollisionCheck(*iter))
		{
			iter->setEnable(false, true);
			Inventory::Instance->AddItem(((DropItem*)iter)->getMyItem(), ((DropItem*)iter)->getItemCount());
			break;
		}
	}
	// 점수
	for (auto iter : ObjectManager::Instance->m_ObjectList[EObjType::Score])
	{
		if (!iter->isEnable())
			continue;
		if (CollisionCheck(iter->getCenter().getDistance(MyObject::getCenter()), m_width / 4.0f))
		{
			// 점수 증가
			operScore(((ScoreItem*)iter)->GetScoreValue());
			iter->setEnable(false, true);
		}
	}

	// 벽, 장애물
	m_isGround = false;
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Wall])
	{
		if (CollisionWall(iter, false))
		{
			m_isGround = true;
			if (Input::isDebug)
				ObjectManager::Instance->TakeObject(L"Clash")->setCenter(getCenter());
		}
	}
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Collider])
	{
		if (CollisionWall(iter, false))
		{
			m_isGround = true;
			if (Input::isDebug)
				ObjectManager::Instance->TakeObject(L"Clash")->setCenter(getCenter());
		}
	}

	return true;
}

void MyPlayer::DeadEvent(MyActor* target)
{
	return;
	target;
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
		for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Wall])
		{
			if (rect.CollisionCheck(*iter))
			{
				if (m_prevPosition.getY() >= iter->getBottom())
				{
					setState(EPlayerState::Crawling);
					setBottom(m_prevPosition.getBottom());
					return true;
				}
			}
		}
		for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Collider])
		{
			if (rect.CollisionCheck(*iter))
			{
				if (m_prevPosition.getY() >= iter->getBottom())
				{
					setState(EPlayerState::Crawling);
					setBottom(m_prevPosition.getBottom());
					return true;
				}
			}
		}
	}
	return false;
}

void MyPlayer::SynchPosition()
{
	Window::DrawPoint = Instance->MyObject::getCenter();
	Window::DrawPoint.y = 0.0f;
	Window::DrawPoint.x -= Window::getClientRect().right / 2;
	if (Window::DrawPoint.x < ObjectManager::MapRect->getX())
		Window::DrawPoint.x = (float)ObjectManager::MapRect->getX();
	else if (Window::DrawPoint.x > ObjectManager::MapRect->getRight() - Window::getClientRect().right)
		Window::DrawPoint.x = (float)ObjectManager::MapRect->getRight() - Window::getClientRect().right;
}

bool MyPlayer::setSpriteState(const wstring& stateName, const bool& isRepeat)
{
	if (m_curSpriteName == stateName)
		return false;
	if (!isRepeat)
		m_canNextSprite = false;

	m_isRepeat = isRepeat;
	MyActor::setSpriteList(stateName);
	m_curSpriteName = stateName;
	return true;
}

void MyPlayer::setDelayHit(const float& delay, const bool& useHalfAlpha)
{
	if(m_delayHit < delay)
		m_delayHit = delay;
	if(useHalfAlpha)
		SetPixelShader("PHalfAlpha");
}

void MyPlayer::setHp(const float& value)
{
	MyObject::setHp(value);
	HealthBar::Instance->setHealthRate(m_healthPoint / 100.0f);
}
void MyPlayer::operHp(const float& value)
{
	float newValue = value;
	if (value < 0)
		newValue /= m_rateDefense;
	MyObject::operHp(newValue);
	HealthBar::Instance->setHealthRate(m_healthPoint / 100.0f);
}

void MyPlayer::operScore(const int& value)
{
	Score += value;
	BloodRate::Instance->operHealthRate(0.0015f);
}

float MyPlayer::getRateAction()
{
	return m_rateAction;
}
float MyPlayer::getRateMove()
{
	return m_rateMove;
}
float MyPlayer::getRateDefense()
{
	return m_rateDefense;
}

void MyPlayer::AddUsingItem(Item* pItem)
{
	if (pItem == nullptr)
		return;
	m_UsingItem.push_back(pItem);
	UpdateItemState();
}

void MyPlayer::RemoveUsingItem(Item* pItem)
{
	if (pItem == nullptr)
		return;
	auto iter = find(m_UsingItem.begin(), m_UsingItem.end(), pItem);
	if (iter == m_UsingItem.end())
	{
		ErrorMessage(""s + __FUNCTION__ + " -> 아이템 찾기 실패!");
		return;
	}
	if ((*iter)->m_myName == L"ShadowServant")
		m_Servant->setEnable(false);
	m_UsingItem.erase(iter);
	UpdateItemState();
}

void MyPlayer::UpdateItemState()
{
	m_rateMove = 1.0f;
	m_rateAction = 1.0f;
	m_rateDefense = 1.0f;
	for (auto& iter : m_UsingItem)
	{
		m_rateMove += iter->m_rateMove;
		m_rateAction += iter->m_rateAction;
		m_rateDefense += iter->m_rateDefense;
	}
}

bool MyPlayer::isEquipItem(const wstring_view& itemName)
{
	for (auto& iter : Equipment::Instance->m_SlotList)
	{
		if (iter->GetMyItem() == nullptr)
			continue;
		if (iter->GetMyItem()->m_myName == itemName)
			return true;
	}
	return false;

	//find(Equipment::Instance->m_SlotList.begin(), Equipment::Instance->m_SlotList.end(), 
		 //[](auto iter) {
		/*if (iter->GetMyItem()->m_myName == itemName)
			return true;*/
		//return false;
		//} );
	//if (iter != Equipment::Instance->m_SlotList.end())
	//	return true;
	//return false;
}

void MyPlayer::UseFlamePotion()
{
	if (m_curDirection == EDirection::Right)
	{
		auto obj = ObjectManager::Instance->TakeObject(L"FlamePotion",70, 70);
		obj->setCenter(getRight() + 15, (int)MyObject::getCenter().y - 10);
		if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::HOLD ||
			Input::Instance->getKeyState('S') == EKeyState::HOLD)
			((MyObject*)obj)->setBounce(355, 1000);
		else if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('W') == EKeyState::HOLD)
				 ((MyObject*)obj)->setBounce(290, 1000);
		else
			((MyObject*)obj)->setBounce(315, 1000);
	}
	else
	{
		auto obj = ObjectManager::Instance->TakeObject(L"FlamePotion", 70, 70);
		obj->setCenter(getLeft() - 15, (int)MyObject::getCenter().y - 10);
		if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::HOLD ||
			Input::Instance->getKeyState('S') == EKeyState::HOLD)
			((MyObject*)obj)->setBounce(185, 1000);
		else if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
				 Input::Instance->getKeyState('W') == EKeyState::HOLD)
				 ((MyObject*)obj)->setBounce(250, 1000);
		else
			((MyObject*)obj)->setBounce(225, 1000);
	}
	SoundManager::Instance->PlayVariation("bash", false, 2);
}

void MyPlayer::Move(const int& x, const int& y)		noexcept
{
	static float moveSPF;
	moveSPF = m_curSPF * m_speed * m_rateMove;
	MyRect::Move(x * moveSPF, y * moveSPF);
}
void MyPlayer::Move(const float& x, const float& y)	noexcept
{
	static float moveSPF;
	moveSPF = m_curSPF * m_speed * m_rateMove;
	MyRect::Move(x * moveSPF, y * moveSPF);
}
void MyPlayer::Move(const MyVector2& point)			noexcept
{
	static float moveSPF;
	moveSPF = m_curSPF * m_speed * m_rateMove;
	MyRect::Move(point.x * moveSPF, point.y * moveSPF);
}
void MyPlayer::Move(const POINT& point)				noexcept
{
	static float moveSPF;
	moveSPF = m_curSPF * m_speed * m_rateMove;
	MyRect::Move(point.x * moveSPF, point.y * moveSPF);
}

void MyPlayer::setWH(const int& width, const int& height)	noexcept
{
	m_width = width;
	m_height = height;
	setBottom(m_prevPosition.getBottom());
}
void MyPlayer::setWH(const float& width, const float& height)	noexcept
{
	m_width = (int)width;
	m_height = (int)height;
	setBottom(m_prevPosition.getBottom());
}
void MyPlayer::setWH(const MyVector2& width_height)			noexcept
{
	m_width = (int)width_height.x;
	m_height = (int)width_height.y;
	setBottom(m_prevPosition.getBottom());
}


MyVector2 MyPlayer::getCenter()
{
	return MyCenter = { Instance->m_point.x + Instance->m_width / 2, Instance->m_point.y + Instance->m_height / 2 };
}

MyActor* MyPlayer::clone()
{
	return nullptr;
}