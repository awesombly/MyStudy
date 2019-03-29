#pragma once
#include "Object.h"

class Object;
class Buffer;

class Dung final : public Object
{
public:
	Dung();
	Dung(const int& ix, const int& iy);
	~Dung();

	virtual void Draw(Buffer& buffer) const override;
	virtual void Move(const int& ix, const int& iy) override;
	virtual bool ColisionCheck() const;
	virtual Object* clone() override;
};

