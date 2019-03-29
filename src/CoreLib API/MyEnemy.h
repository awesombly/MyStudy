#pragma once
#include "MyObject.h"
#include "IEnemyState.h"
#include "MyPlayer.h"


// �ൿ�� Ȯ�� ����

class MyEnemy : public MyObject
{
public:
	friend class IEnemyState;
	MyEnemy(const wstring& objName);
	virtual ~MyEnemy() = default;
protected:
	//static map<EState, wstring> StateToString;	// �׿�
	map<EState, IEnemyState*> m_stateList;			// ���º� �ൿ
	IEnemyState*			  m_curState;			// ���� ����
	FiniteStateMachine*		  m_myFSM;				// FSM ��ü(�Ⱦ�)
	
	int   m_attackRadius;	// �� ���� ����
	int   m_nearRadius;		// ���� ���� ����
	
	float m_angle;			// �̵� ����
	float m_distance;		// �� �÷��̾� �Ÿ�
	float m_delayFrame;		// �ൿ�� ���ð�
	float m_delayAttack;	// ���� ����
	float m_delayDodge;		// ȸ�� ����
	float m_delayHit;		// �ǰ� ����
public:
	virtual bool  setState(const EState state);				// ���� ���� ����
	// FSM �Լ��ε� �Ⱦ��°� ������
	virtual EState getCurrentState();						// ���� ���� ��ȯ
	virtual bool  setEventTransition(const EEvent& event);	// �̺�Ʈ �߻� ����
	virtual void  setFSM();									// FSM ����
	
	virtual bool setSpriteState(const wstring& stateName, const bool& isRepeat = true) override;
	virtual void operBounce() override;

	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual MyBitMap* clone() override;

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