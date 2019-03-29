#pragma once

class Point {
public:
	friend class Rectangle;
	Point();
	Point(const int& x, const int& y);
	~Point();
private:
	int x, y;
};

class Rectangle
{
public:
	Rectangle();
	Rectangle(const int& x, const int& y, const int& width, const int& height);
	~Rectangle();

	void MoveRect(const int& x, const int& y);
	bool CollisionCheck(const Rectangle& rect) const;
	Rectangle* Intersection(const Rectangle& rect) const;
	Rectangle* Union(const Rectangle& rect) const;
	void DrawRect() const;
	void DrawRect(const Rectangle& rect) const;

	Point getPoint() const;
	Point getCenter() const;
	int getWidth() const;
	int getHeight() const;
	int getLeft() const;
	int getRight() const;
	int getTop() const;
	int getBottom() const;

	void setPoint(const int& x, const int& y);
	void setCenter(const int& x, const int& y);
	void setWidth(const int& width);
	void setHeight(const int& height);

	Rectangle& operator +(const Rectangle&);
	Rectangle& operator *(const Rectangle&);
	Rectangle& operator +(const int&);
	Rectangle& operator -(const int&);
	friend Rectangle& operator +(const int&, const Rectangle& rect);
	friend Rectangle& operator -(const int&, const Rectangle& rect);
private:
	Point m_point;
	int m_width, m_height;
};

