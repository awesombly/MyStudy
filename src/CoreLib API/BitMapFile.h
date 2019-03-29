#pragma once
#include "Window.h"

// 원본 비트맵만 저장하는 클래스
class BitMapFile
{
public:
	BitMapFile();
	virtual ~BitMapFile();
private:
	HDC		  m_hMyDC;				// 내 DC
	HBITMAP   m_hBitmap;			// 비트맵 핸들
	HBITMAP   m_hOldBitmap;			// 이전 비트맵 저장

	//HDC		  m_hRotateDC;			// 회전시 쓸 DC?
	//HBITMAP	  m_hRotateBitmap;		// 회전시 비트맵
	//HBITMAP	  m_hRotateOldBitmap;	// 회전시 이전 비트맵

	BITMAP    m_BitInfo;			// 비트맵 정보
	MyRect    m_srcRect;			// 비트맵 전체 Rect
public:
	bool Draw(const Point& pos, const MyRect& srcRect, const DWORD& type);
	bool DrawMask(const MyRect& rect, const MyRect& srcRect, const COLORREF& maskColor);
	bool DrawUI(const MyRect& rect, const MyRect& srcRect, const COLORREF& maskColor);
	bool DrawScroll(const MyRect& rect, const MyRect& srcRect, const DWORD& type);
	bool DrawStretch(const MyRect& rect, const MyRect& srcRect, const DWORD& type, const bool& useReverse);
	bool DrawRotation(HBITMAP* hRotateBitmap, HBITMAP* hRotateOldBitmap, const Point& pos,  const DWORD& type, const float& distance);
	HBITMAP SetRotationBitMap(HDC* hRotateDC, HBITMAP* hRotateBitmap, HBITMAP* hRotateOldBitmap, const MyRect& srcRect, const float& angle, const float& distance);
	bool Load(wstring bitName);
	//bool Load(const TCHAR* fileUrl);
	//bool Load(DWORD bitmapID);
	void setSrcRect(const int& sx, const int& sy, const int& width, const int& height);
	MyRect getSrcRect() const;
	const BITMAP& getBitmapInfo() const;
};

