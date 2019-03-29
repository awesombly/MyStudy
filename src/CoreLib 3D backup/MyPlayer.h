#pragma once
#include "IPlayerState.h"
#include "MyObject.h"
#include "Input.h"
#include "HealthBar.h"
#include "Inventory.h"
#include "DropItem.h"
#include "ScoreItem.h"
#include "BloodRate.h"

class PlayerServant;

class MyPlayer : public MyObject, public ISingleton<MyPlayer>
{
protected:
	const float m_kMaxGravity = -1500.0f;		// 최대 중력치
	const float m_kJumpPower  = 650.0f;			// 점프 힘
	const int	m_kMaxHealth = 100;
	MyVector2   m_prevCursor;			// 디버깅용

	map<EPlayerState, IPlayerState*> m_stateList;			// 상태별 행동
	IPlayerState*					 m_curState;			// 현재 상태
	IPlayerState*					 m_preState;			// 이전 상태
	wstring m_curSpriteName;
	
	bool m_isGround;		// 땅 체크
	bool m_isSword;			// 칼이냐

	EDirection m_inputDirection;	// 전에 누른 방향
	float m_chargeCount;	// 충전 시간
	float m_dashCount;		// 대쉬 인식 시간
	float m_delayDash;		// 대쉬 쿨
	float m_delayFrame;		// 행동간 대기시간
	float m_delayAttack;	// 잉여
	float m_delayCount;		// 고정 딜레이
	float m_delayCharge;	// 차지 쿨
	float m_delayHit;		// 피격 간격

	float m_rateAction	= 1.0f;
	float m_rateMove	= 1.0f;
	float m_rateDefense = 1.0f;
	list<Item*> m_UsingItem;

	vector<MyActor*>	m_ConsumUI;
	PlayerServant* m_Servant;
public:
	static MyVector2  MyCenter;
	static int Score;
private:
	friend class PlayerServant;
	friend class ISingleton<MyPlayer>;
	MyPlayer() = default;
public:
	virtual ~MyPlayer() = default;

	static void SynchPosition();							// 스크롤 위치 동기화
	static MyVector2 getCenter();

	// 아이템류
	virtual void AddUsingItem(Item* pItem);
	virtual void RemoveUsingItem(Item* pItem);
	virtual void UpdateItemState();
	virtual bool isEquipItem(const wstring_view& itemName);
	virtual void UseFlamePotion();

	virtual void DeadEvent(MyActor* target) override;
	virtual bool setState(const EPlayerState state);		// 상태 전이
	virtual bool HeadCheck();
	virtual bool Collision(const float& spf);
	virtual bool MouseCheck();
	virtual bool setSpriteState(const wstring& stateName, const bool& isRepeat = true);
	virtual void setDelayHit(const float& delay, const bool& useHalfAlpha = true);
	virtual void setHp(const float& value) override;
	virtual void operHp(const float& value) override;
	virtual void operScore(const int& value);
	virtual float getRateAction();
	virtual float getRateMove();
	virtual float getRateDefense();

	virtual bool Init(const wstring_view& myName, const EObjType& objType,
					  const string_view& vertexShaderName = "VS_Basic", const string_view& pixelShaderName = "PShader") noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()										noexcept override;

	virtual void Move(const int& x, const int& y)				noexcept override;
	virtual void Move(const float& x, const float& y)			noexcept override;
	virtual void Move(const MyVector2& point)					noexcept override;
	virtual void Move(const POINT& point)						noexcept override;
	virtual void setWH(const int& width, const int& height)		noexcept override;
	virtual void setWH(const float& width, const float& height)	noexcept override;
	virtual void setWH(const MyVector2& width_height)			noexcept override;
	virtual MyActor* clone() override;

	friend class IPlayerState;
	friend class PlayerStateBasic;
	friend class PlayerStateCrawling;
	friend class PlayerStateSword;
	friend class PlayerStateAttack;
	friend class PlayerStateSkill;
	friend class PlayerStateDash;
	friend class PlayerStateHit;
	friend class PlayerStateDead;
};