#include "header.h"



Player::Player() : m_kMaxLife(3)
{
	m_life = m_kMaxLife;
	m_position = { WIDTH / 2, HEIGHT - 2 };
}

Player::~Player()	{}


void Player::Draw(Buffer& buffer) const 
{
	buffer.BufferWrite("£À", (short)m_position.x * 2, (short)m_position.y);

	for (int i = 0; i < m_kMaxLife; i++)
	{
		if(m_life > i)
			buffer.BufferWrite("¢¾", i * 3, HEIGHT + 2);
		else
			buffer.BufferWrite("¢½", i * 3, HEIGHT + 2);
	}
}

void Player::Move(const int& ix, const int& iy)	{
	m_position.x += ix;
	m_position.y += iy;
	if (m_position.x < 1)	m_position.x++;
	if (m_position.x >= WIDTH - 1)	m_position.x--;
}

int Player::getLife() const
{
	return m_life;
}

bool Player::operLife(const int& value)
{
	m_life += value;
	if (m_life <= 0)
		return false;
	return true;
}

bool Player::operator == (const POINT& point) const
{
	if (m_position.x == point.x && m_position.y == point.y)
		return true;
	return false;
}