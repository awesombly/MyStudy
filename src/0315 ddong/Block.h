#pragma once
#include "Object.h"

class Object;
class Buffer;

class Block final : public Object
{
public:
	Block();
	Block(const int& ix, const int& iy);
	~Block();

	virtual void Draw(Buffer& buffer) const override;
	virtual void Move(const int& ix, const int& iy) override;
	virtual void setPosition(const int& x, const int& y) override;
	virtual POINT getPosition() const override;
	virtual Object* clone() override;
};
