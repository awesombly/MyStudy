#pragma once
#include "MyRect.h"
#include "Texture.h"

// ���ӵ� ��������Ʈ �� ������ �̹����� ���� ����
class Sprite
{
public:
	Texture*	m_pTexture;		// �ؽ�ó ������
	MyVector4	m_texVector4;	// �ؽ�ó ������ǥ
	float		m_frame;		// �ش� ��������Ʈ ����(sec)
public:
	Sprite(const MyVector4& vector4, const float& frame, Texture* pTexture)
	{
		m_pTexture = pTexture;
		m_texVector4 = vector4;
		m_frame = frame;
	};
	virtual ~Sprite() = default;
};