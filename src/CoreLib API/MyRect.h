#pragma once
#include "header.h"

// + 더하기 빼기 곱하기 나누기...
class Point {
public:
	Point() noexcept = default;
	Point(const int& _x, const int& _y) noexcept : x((float)_x), y((float)_y) {};
	Point(const float& _x, const float& _y) noexcept : x(_x), y(_y) {};
	Point(const Point& _point) noexcept = default;
	Point(Point&& _point) noexcept = default;
	~Point() noexcept = default;
public:
	float x, y;
public:
	Point Normalize()
	{
		float&& length = sqrt(x * x + y * y);
		return { x / length, y / length };
	}
	Point toPointNormalize(Point _point)
	{
		return (_point - *this).Normalize();

	}
	float getLength()
	{
		return sqrt(x * x + y * y);
	}
	float getSquareDistance(const Point& _point)
	{
		return pow(abs(x - _point.x), 2) + pow(abs(y - _point.y), 2);
	}
	float getDistance(const Point& _point)
	{
		return sqrt(pow(abs(x - _point.x), 2) + pow(abs(y - _point.y), 2));
	}
	Point & operator =(const Point& _point)  noexcept = default;
	Point & operator =(const POINT& _point)  noexcept
	{
		x = (float)_point.x;
		y = (float)_point.y;
		return *this;
	}
	Point & operator =(Point&& _point)  noexcept = default;
	Point & operator =(const int& value)  noexcept
	{
		x = (float)value;
		y = (float)value;
		return *this;
	}
	Point & operator =(const float& value)  noexcept
	{
		x = value;
		y = value;
		return *this;
	}
	Point operator +(const Point& _point)  noexcept
	{
		return Point(x + _point.x, y + _point.y);
	}
	Point operator -(const Point& _point)  noexcept
	{
		return Point(x - _point.x, y - _point.y);;
	}
	Point operator +(const int& _value)  noexcept
	{
		return Point(x + _value, y + _value);
	}
	Point operator -(const int& _value)  noexcept
	{
		return Point(x - _value, y - _value);
	}
	Point operator *(const float& _value)  noexcept
	{
		return Point(x * _value, y * _value);
	}
	Point operator /(const float& _value)  noexcept
	{
		return Point(x / _value, y / _value);
	}
};

// + setLeft, Right, Top, Bottom
class MyRect
{
public:
	MyRect()				   noexcept	= default;
	MyRect(const MyRect& rect) noexcept = default;
	MyRect(MyRect&& rect)	   noexcept = default;
	MyRect(const int& x, const int& y, const int& width, const int& height) noexcept;
	MyRect(const float& x, const float& y, const int& width, const int& height) noexcept;
	~MyRect() noexcept;

	virtual void Move(const int& x, const int& y)		noexcept;
	virtual void Move(const float& x, const float& y)	noexcept;
	virtual void Move(const Point& point)				noexcept;
	virtual void Move(const POINT& point)				noexcept;

	bool CollisionCheck(const POINT& point)  const noexcept;
	bool CollisionCheck(const MyRect& rect)  const noexcept;
	bool CollisionCheck(const float& distance, const float& targetRadius) const noexcept;
	MyRect Intersection(const MyRect& rect) const noexcept;
	MyRect Union(const MyRect& rect)		 const noexcept;
	void DrawRect() const noexcept;
	void DrawRect(const MyRect& rect)		 const noexcept;

	Point getPoint()	const noexcept;
	Point getCenter()	const noexcept;
	Point getWH()		const noexcept;
	int getWidth()		const noexcept;
	int getHeight()		const noexcept;
	int getX()			const noexcept;
	int getY()			const noexcept;
	int getLeft()		const noexcept;
	int getRight()		const noexcept;
	int getTop()		const noexcept;
	int getBottom()		const noexcept;

	virtual void setWidth(const int& width)						noexcept;
	virtual void setHeight(const int& height)					noexcept;
	virtual void setWH(const int& width, const int& height)		noexcept;
	virtual void setWH(const float& width, const float& height)	noexcept;
	virtual void setWH(const Point& width_height)				noexcept;

	virtual void setX(const int& x)							noexcept;
	virtual void setX(const float& x)						noexcept;
	virtual void setY(const int& y)							noexcept;
	virtual void setY(const float& y)						noexcept;
	virtual void setPoint(const int& x, const int& y)		noexcept;
	virtual void setPoint(const float& x, const float& y)	noexcept;
	virtual void setCenter(const int& x, const int& y)		noexcept;
	virtual void setCenter(const float& x, const float& y)	noexcept;
	virtual void setCenter(const Point& point)				noexcept;
	virtual void setCenter(const POINT& point)				noexcept;
	
	virtual void setRect(const MyRect& rect)				noexcept;
	virtual void setRect(const RECT& rect)					noexcept;
	virtual void setRect(const int& x, const int& y, const int& width, const int& height) noexcept;
	virtual void setRect(const float& x, const float& y, const int& width, const int& height) noexcept;
	virtual void RECTset(RECT* rect)	const noexcept;

	// 연산자 내용 이상한게 있음
	virtual MyRect& operator =(const MyRect&) noexcept = default;
	virtual MyRect& operator =(MyRect&&)	  noexcept = default;
	MyRect& operator =(const RECT& rect)	  noexcept;
	//MyRect& operator +(const MyRect&)		  noexcept;
	//MyRect& operator *(const MyRect&)		  noexcept;
	//MyRect operator +(const int&)			  noexcept;
	//MyRect operator -(const int&)			  noexcept;
	//friend MyRect& operator +(const int&, const MyRect& rect) noexcept;
	//friend MyRect& operator -(const int&, const MyRect& rect) noexcept;

	static float getAngle(const int& sx, const int& sy, const int& dx, const int& dy);
	static float getAngle(const Point& sP, const Point& dP);
	static float getRadian(const float& angle);
public:
	Point m_point;
	int m_width, m_height;
};

