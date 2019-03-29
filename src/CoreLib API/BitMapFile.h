#pragma once
#include "Window.h"

// ���� ��Ʈ�ʸ� �����ϴ� Ŭ����
class BitMapFile
{
public:
	BitMapFile();
	virtual ~BitMapFile();
private:
	HDC		  m_hMyDC;				// �� DC
	HBITMAP   m_hBitmap;			// ��Ʈ�� �ڵ�
	HBITMAP   m_hOldBitmap;			// ���� ��Ʈ�� ����

	//HDC		  m_hRotateDC;			// ȸ���� �� DC?
	//HBITMAP	  m_hRotateBitmap;		// ȸ���� ��Ʈ��
	//HBITMAP	  m_hRotateOldBitmap;	// ȸ���� ���� ��Ʈ��

	BITMAP    m_BitInfo;			// ��Ʈ�� ����
	MyRect    m_srcRect;			// ��Ʈ�� ��ü Rect
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

