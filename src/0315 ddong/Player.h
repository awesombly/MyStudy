#pragma once
#include <Windows.h>
#include "Object.h"

class Buffer;
class Object;

class Player final : public Object
{
public:
	Player();
	~Player();
	virtual void Draw(Buffer& buffer) const override;
	virtual void Move(const int& ix, const int& iy) override;

	int getLife() const;
	bool operLife(const int& value);

	bool operator == (const POINT& point) const;
private:
	int m_life;
	const int m_kMaxLife;
};

