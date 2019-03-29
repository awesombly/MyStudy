#include "PlayerServant.h"
#include "ObjectManager.h"

//MyVector2  MyPlayer::MyCenter = { 0, 0 };


PlayerServant::PlayerServant(const wstring_view& myName, const EObjType& objType,
							 const string_view& vertexShaderName, const string_view& pixelShaderName)
{
	MyActor::Init(std::forward<const wstring_view>(myName), objType,
		 std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName));
	MyObject::Init();

	setSpriteList(L"basic");
	setWH(49, 68);
	setCenter(100, 100);
	m_originWH = getWH();

	m_curDirection = EDirection::Right;
	m_isSword = false;
	m_isGround = false;
	m_healthPoint = 100;
	m_speed = 0.0f;
	m_delayFrame = 0.0f;
	m_delayAttack = 0.0f;
	m_delayCount = 0.0f;
	m_delayCharge = 0.0f;
	m_delayHit = 999.9f;

	// 상태 입력
	m_stateList[EPlayerState::Basic] = new PlayerStateBasic();
	m_stateList[EPlayerState::Crawling] = new PlayerStateCrawling();
	m_stateList[EPlayerState::Sword] = new PlayerStateSword();
	m_stateList[EPlayerState::Attack] = new PlayerStateAttack();
	m_stateList[EPlayerState::Skill] = new PlayerStateSkill();
	m_stateList[EPlayerState::Dash] = new PlayerStateDash();
	m_stateList[EPlayerState::Hit] = new PlayerStateHit();
	m_stateList[EPlayerState::Dead] = new PlayerStateDead();
	m_curState = m_stateList[EPlayerState::Basic];
	setState(EPlayerState::Basic);

	//ObjectManager::Instance->PushObject(this);
	//return true;
}


bool PlayerServant::Frame(const float& spf, const float& accTime) noexcept
{
	if (!MyActor::Frame(spf * MyPlayer::Instance->getRateAction(), accTime)) return false;
	m_prevPosition = *this;
	//getCenter();
	m_curSPF = spf;

	m_delayFrame   -= spf * MyPlayer::Instance->getRateAction();
	m_delayAttack  -= spf * MyPlayer::Instance->getRateMove();
	m_delayCharge  -= spf * MyPlayer::Instance->getRateMove();
	m_delayDash		= 0.0f;
	m_delayCount   -= spf;
	m_dashCount    -= spf;
	//m_delayHit -= spf;

	m_curState->Process(this);		// 실질적 행동

	MouseCheck();
	Collision(spf);

	return true;
	accTime;
}

bool PlayerServant::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (m_isEnable == false)	return false;
	PrevRender(pDContext);
	PostRender(pDContext);
	return true;
}