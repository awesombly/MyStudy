#pragma once

class MyEnemy;

enum class EState : char
{
	Basic = 0,
	Move,
	Patrol,
	Attack,
	Runaway,
	Hit,
	Dead,
	Count
};

class IEnemyState
{
protected:
public:
	EState   m_myState;
public:
	IEnemyState(const EState& state) : m_myState(state) {};
	virtual ~IEnemyState() = default;
	
	virtual bool Process(MyEnemy* pOwner) = 0;
	virtual void StateInit(MyEnemy* pOwner) = 0;
};

// 기본
class EnemyStateBasic : public IEnemyState
{
public:
	EnemyStateBasic() : IEnemyState(EState::Basic) {};
	virtual ~EnemyStateBasic() = default;
public:
	virtual bool Process(MyEnemy* pOwner) override;
	virtual void StateInit(MyEnemy* pOwner) override;
};

// 이동
class EnemyStateMove : public IEnemyState 
{
public:
	EnemyStateMove() : IEnemyState(EState::Move) {};
	virtual ~EnemyStateMove() = default;
public:
	virtual bool Process(MyEnemy* pOwner) override;
	virtual void StateInit(MyEnemy* pOwner) override;
};

// 순찰
class EnemyStatePatrol : public IEnemyState
{
public:
	EnemyStatePatrol() : IEnemyState(EState::Patrol) {};
	virtual ~EnemyStatePatrol() = default;
public:
	virtual bool Process(MyEnemy* pOwner) override;
	virtual void StateInit(MyEnemy* pOwner) override;
};

// 공격
class EnemyStateAttack : public IEnemyState
{
public:
	EnemyStateAttack() : IEnemyState(EState::Attack) {};
	virtual ~EnemyStateAttack() = default;
public:
	virtual bool Process(MyEnemy* pOwner) override;
	virtual void StateInit(MyEnemy* pOwner) override;
};

// 피격
class EnemyStateHit: public IEnemyState
{
public:
	EnemyStateHit() : IEnemyState(EState::Hit) {};
	virtual ~EnemyStateHit() = default;
public:
	virtual bool Process(MyEnemy* pOwner) override;
	virtual void StateInit(MyEnemy* pOwner) override;
};

// 회피
class EnemyStateRunaway : public IEnemyState
{
public:
	EnemyStateRunaway() : IEnemyState(EState::Runaway) {};
	virtual ~EnemyStateRunaway() = default;
public:
	virtual bool Process(MyEnemy* pOwner) override;
	virtual void StateInit(MyEnemy* pOwner) override;
};

// 사망
class EnemyStateDead : public IEnemyState
{
public:
	EnemyStateDead() : IEnemyState(EState::Dead) {};
	virtual ~EnemyStateDead() = default;
public:
	virtual bool Process(MyEnemy* pOwner) override;
	virtual void StateInit(MyEnemy* pOwner) override;
};

