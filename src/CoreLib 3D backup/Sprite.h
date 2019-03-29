#pragma once
#include "MyRect.h"
#include "Texture.h"

// 연속된 스프라이트 중 한장의 이미지에 대한 정보
class Sprite
{
public:
	Texture*	m_pTexture;		// 텍스처 포인터
	MyVector4	m_texVector4;	// 텍스처 투영좌표
	float		m_frame;		// 해당 스프라이트 간격(sec)
public:
	Sprite(const MyVector4& vector4, const float& frame, Texture* pTexture)
	{
		m_pTexture = pTexture;
		m_texVector4 = vector4;
		m_frame = frame;
	};
	virtual ~Sprite() = default;
};