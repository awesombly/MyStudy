#include "header.h"


Dung::Dung() {}
Dung::Dung(const int& ix, const int& iy)
{
	m_position = { ix, iy };
}
Dung::~Dung() {}

void Dung::Move(const int& ix, const int& iy)
{
	m_position.x += ix;
	m_position.y += iy;
}

void Dung::Draw(Buffer& buffer) const
{
	buffer.BufferWrite("ขอ", (short)m_position.x * 2, (short)m_position.y);
}


bool Dung::ColisionCheck() const
{
	if (m_position.y >= HEIGHT)
		return true;
	return false;
}

Object* Dung::clone()
{
	return new Dung(*this);
}

