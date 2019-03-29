#include "header.h"


Block::Block() {}
Block::Block(const int& ix, const int& iy)
{
	m_position = { ix, iy };
}
Block::~Block() {}

void Block::Move(const int& ix, const int& iy)
{
	m_position.x += ix;
	m_position.y += iy;
}

void Block::Draw(Buffer& buffer) const
{
	buffer.BufferWrite("бр", (short)m_position.x * 2, (short)m_position.y);
}

void Block::setPosition(const int& ix, const int& iy)
{
	m_position.x = ix;
	m_position.y = iy;
}

POINT Block::getPosition() const
{
	return m_position;
}


Object* Block::clone()
{
	return new Block(*this);
}