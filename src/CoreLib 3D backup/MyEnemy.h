#pragma once
#include "IEnemyState.h"
#include "MyPlayer.h"



class MyEnemy : public MyObject
{
protected:
	map<EState, IEnemyState*> m_stateList;			// ���º� �ൿ
	IEnemyState*			  m_curState;			// ���� ����

	int   m_attackRadius;	// �� ���� ����
	int   m_nearRadius;		// ���� ���� ����

	float m_angle;			// �̵� ����
	float m_distance;		// �� �÷��̾� �Ÿ�
	float m_delayFrame;		// �ൿ�� ���ð�
	float m_delayAttack;	// ���� ����
	float m_delayDodge;		// ȸ�� ����
	float m_delayHit;		// �ǰ� ����
	
	bool m_canHit;
public:
	using MyObject::MyObject;
	virtual ~MyEnemy() = default;

	virtual void DeadEvent(MyActor* target) override;
	virtual bool Collision();
	virtual bool setState(const EState state);				// ���� ����
	virtual EState getCurrentState();						// ���� ���� ��ȯ

	virtual bool setSpriteState(const wstring& stateName, const bool& isRepeat = true);
	virtual bool operBounce(const float& spf) override;

	virtual bool Init() noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()										noexcept override;
	virtual MyActor* clone();

	friend class IEnemyState;
	friend class EnemyStateBasic;
	friend class EnemyStateAttack;
	friend class EnemyStateDead;
	friend class EnemyStateHit;
	friend class EnemyStateMove;
	friend class EnemyStatePatrol;
	friend class EnemyStatePatrol;
	friend class EnemyStateRunaway;
};