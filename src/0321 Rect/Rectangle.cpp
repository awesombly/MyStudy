#include "stdafx.h"
#include "Rectangle.h"
#include <cmath>
#include <iostream>
#include <tchar.h>



Point::Point() {}
Point::Point(const int& ix, const int & iy) : x(ix), y(iy)	{}
Point::~Point() {}



Rectangle::Rectangle()	{}

Rectangle::Rectangle(const int& x, const int& y, const int& width, const int& height) 
	: m_point(x, y), m_width(width), m_height(height)	{}

Rectangle::~Rectangle()	{}


void Rectangle::MoveRect(const int& x, const int& y)
{
	m_point.x += x;
	m_point.y += y;
}

bool Rectangle::CollisionCheck(const Rectangle& rect) const
{
	if (m_point.x <= rect.m_point.x + rect.m_width &&
		m_point.x + m_width >= rect.m_point.x &&
		m_point.y <= rect.m_point.y + rect.m_height &&
		m_point.y + m_height >= rect.m_point.y)
		return true;
	return false;
}

Rectangle* Rectangle::Intersection(const Rectangle& rect) const
{
	// 충돌 체크
	if (!CollisionCheck(rect)) return nullptr;

	Rectangle* pRect = new Rectangle();

	// 큰 left, 큰 top, 작은 right, 작은 bottom
	pRect->m_point.x = (int)std::fmax(m_point.x, rect.m_point.x);
	pRect->m_point.y = (int)std::fmax(m_point.y, rect.m_point.y);
	pRect->m_width   = (int)std::fmin(m_point.x + m_width, rect.m_point.x + rect.m_width) - pRect->m_point.x;
	pRect->m_height  = (int)std::fmin(m_point.y + m_height, rect.m_point.y + rect.m_height) - pRect->m_point.y;

	return pRect;
}

Rectangle* Rectangle::Union(const Rectangle& rect) const
{
	//if (!CollisionCheck(rect)) return nullptr;
	Rectangle* pRect = new Rectangle();

	// 작은 left, 작은 top, 큰 right, 큰 bottom
	pRect->m_point.x = (int)std::fmin(m_point.x, rect.m_point.x);
	pRect->m_point.y = (int)std::fmin(m_point.y, rect.m_point.y);
	pRect->m_width   = (int)std::fmax(m_point.x + m_width, rect.m_point.x + rect.m_width) - pRect->m_point.x;
	pRect->m_height  = (int)std::fmax(m_point.y + m_height, rect.m_point.y + rect.m_height) - pRect->m_point.y;

	return pRect;
}

void Rectangle::DrawRect()  const
{
	DrawRect(*this);
}

void Rectangle::DrawRect(const Rectangle& rect)  const
{
	try
	{
		std::cout << "Left : " << rect.getLeft() << ",\tTop : " << rect.getTop() << ",\tRight : " << rect.getRight() << ",\tBottom : " << rect.getBottom() << std::endl;
	}
	catch (...)
	{
		std::cout << "null" << std::endl;
	}

	int right = rect.getRight();
	int bottom = rect.getBottom();
	for (int h = 0; h < right; h++)
	{
		for (int w = 0; w < bottom; w++)
		{
			if (h < rect.m_point.y || w < rect.m_point.x)
			{
				std::cout << "·";
				continue;
			}
			std::cout << "■";
		}
		std::cout << std::endl;
	}
}

Point Rectangle::getPoint() const
{
	return m_point;
}
int Rectangle::getWidth() const
{
	return m_width;
}
int Rectangle::getHeight() const
{
	return m_height;
}
Point Rectangle::getCenter() const
{
	Point center = { center.x = m_point.x + m_width / 2,
		center.y = m_point.y + m_height / 2 };
	return center;
}
int Rectangle::getLeft() const
{
	return m_point.x;
}
int Rectangle::getRight() const
{
	return m_point.x + m_width;
}
int Rectangle::getTop() const
{
	return m_point.y;
}
int Rectangle::getBottom() const
{
	return m_point.y + m_height;
}
void Rectangle::setPoint(const int& x, const int& y)
{
	m_point = { x, y };
}
void Rectangle::setWidth(const int& width)
{
	m_width = width;
}
void Rectangle::setHeight(const int& height)
{
	m_height = height;
}
void Rectangle::setCenter(const int& x, const int& y)
{
	m_point.x = x - m_width / 2;
	m_point.y = y - m_height / 2;
}


Rectangle& Rectangle::operator +(const Rectangle& rect)
{
	return *Union(rect);
}

Rectangle& Rectangle::operator *(const Rectangle& rect)
{
	return *Intersection(rect);
}

Rectangle& Rectangle::operator +(const int& iValue)
{
	Rectangle* tempRect = new Rectangle(*this);
	tempRect->MoveRect(iValue, iValue);
	
	return *tempRect;
}
Rectangle& Rectangle::operator -(const int& iValue)
{
	Rectangle* tempRect = new Rectangle(*this);
	tempRect->MoveRect(-iValue, -iValue);

	return *tempRect;
}
Rectangle& operator +(const int& iValue, const Rectangle& rect)
{
	Rectangle* tempRect = new Rectangle(rect);
	tempRect->MoveRect(iValue, iValue);

	return *tempRect;
}
Rectangle& operator -(const int& iValue, const Rectangle& rect)
{
	Rectangle* tempRect = new Rectangle(rect);
	tempRect->MoveRect(-iValue, -iValue);

	return *tempRect;
}