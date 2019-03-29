#pragma once

enum class EPlayerState :char {
	Basic = 0,
	Crawling,
	Sword,
	Hit,
	Attack,
	Skill,
	Dead,
};

class MyPlayer;

class IPlayerState
{
protected:
public:
	EPlayerState   m_myState;
public:
	IPlayerState(const EPlayerState& state) : m_myState(state) {};
	virtual ~IPlayerState() = default;

	virtual bool Process(MyPlayer* pOwner) = 0;
	virtual void StateInit(MyPlayer* pOwner) = 0;

};

// 기본
class PlayerStateBasic : public IPlayerState
{
public:
	PlayerStateBasic() : IPlayerState(EPlayerState::Basic) {};
	virtual ~PlayerStateBasic() = default;
public:
	virtual bool Process(MyPlayer* pOwner) override;
	virtual void StateInit(MyPlayer* pOwner) override;
};

// 엎드림
class PlayerStateCrawling : public IPlayerState
{
public:
	PlayerStateCrawling() : IPlayerState(EPlayerState::Crawling) {};
	virtual ~PlayerStateCrawling() = default;
public:
	virtual bool Process(MyPlayer* pOwner) override;
	virtual void StateInit(MyPlayer* pOwner) override;
};

// 검
class PlayerStateSword : public IPlayerState
{
public:
	PlayerStateSword() : IPlayerState(EPlayerState::Sword) {};
	virtual ~PlayerStateSword() = default;
public:
	virtual bool Process(MyPlayer* pOwner) override;
	virtual void StateInit(MyPlayer* pOwner) override;
};

// 기술
class PlayerStateSkill : public IPlayerState
{
public:
	PlayerStateSkill() : IPlayerState(EPlayerState::Skill) {};
	virtual ~PlayerStateSkill() = default;
public:
	virtual bool Process(MyPlayer* pOwner) override;
	virtual void StateInit(MyPlayer* pOwner) override;
};

// 공격
class PlayerStateAttack : public IPlayerState
{
public:
	PlayerStateAttack() : IPlayerState(EPlayerState::Attack) {};
	virtual ~PlayerStateAttack() = default;
public:
	virtual bool Process(MyPlayer* pOwner) override;
	virtual void StateInit(MyPlayer* pOwner) override;
};


// 피격
class PlayerStateHit : public IPlayerState
{
public:
	PlayerStateHit() : IPlayerState(EPlayerState::Hit) {};
	virtual ~PlayerStateHit() = default;
public:
	virtual bool Process(MyPlayer* pOwner) override;
	virtual void StateInit(MyPlayer* pOwner) override;
};

// 사망
class PlayerStateDead : public IPlayerState
{
public:
	PlayerStateDead() : IPlayerState(EPlayerState::Dead) {};
	virtual ~PlayerStateDead() = default;
public:
	virtual bool Process(MyPlayer* pOwner) override;
	virtual void StateInit(MyPlayer* pOwner) override;
};