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
	const float m_kMaxGravity = -1500.0f;		// �ִ� �߷�ġ
	const float m_kJumpPower  = 650.0f;			// ���� ��
	const int	m_kMaxHealth = 100;
	MyVector2   m_prevCursor;			// ������

	map<EPlayerState, IPlayerState*> m_stateList;			// ���º� �ൿ
	IPlayerState*					 m_curState;			// ���� ����
	IPlayerState*					 m_preState;			// ���� ����
	wstring m_curSpriteName;
	
	bool m_isGround;		// �� üũ
	bool m_isSword;			// Į�̳�

	EDirection m_inputDirection;	// ���� ���� ����
	float m_chargeCount;	// ���� �ð�
	float m_dashCount;		// �뽬 �ν� �ð�
	float m_delayDash;		// �뽬 ��
	float m_delayFrame;		// �ൿ�� ���ð�
	float m_delayAttack;	// �׿�
	float m_delayCount;		// ���� ������
	float m_delayCharge;	// ���� ��
	float m_delayHit;		// �ǰ� ����

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

	static void SynchPosition();							// ��ũ�� ��ġ ����ȭ
	static MyVector2 getCenter();

	// �����۷�
	virtual void AddUsingItem(Item* pItem);
	virtual void RemoveUsingItem(Item* pItem);
	virtual void UpdateItemState();
	virtual bool isEquipItem(const wstring_view& itemName);
	virtual void UseFlamePotion();

	virtual void DeadEvent(MyActor* target) override;
	virtual bool setState(const EPlayerState state);		// ���� ����
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