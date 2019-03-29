#pragma once
#include "MyRect.h"
#include "BitMapFile.h"

class Sprite
{
public:
	Sprite() = default;
	Sprite(const MyRect& rect, const float& frame, const COLORREF& maskColor = 0)
	{
		m_maskRGB = maskColor;
		m_srcRect = rect;
		m_frame = frame;
	}
	virtual ~Sprite() = default;
public:
	MyRect m_srcRect;			// �̹��� ��ǥ ��Ʈ
	float  m_frame;				// �ش� ��������Ʈ ����(sec)
	COLORREF    m_maskRGB;
	BitMapFile* m_mainBit;
	BitMapFile* m_maskBit;
};