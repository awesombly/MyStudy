#pragma once
//#include "header.h"
#include <Windows.h>
class Buffer;

class Object
{
public:
	Object() = default;
	virtual ~Object() = default;

	virtual void Draw(Buffer& buffer) const {};
	virtual void Move(const int& ix, const int& iy) {};
	virtual void setPosition(const int& ix, const int& iy) {
		m_position.x = ix;
		m_position.y = iy;
	};
	virtual POINT getPosition() const	{
		return m_position;	
	};
	virtual Object* clone() { return nullptr; };
protected:
	POINT m_position;
};

