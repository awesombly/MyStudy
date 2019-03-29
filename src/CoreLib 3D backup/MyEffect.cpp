#include "MyEffect.h"



bool MyEffect::Init() noexcept
{
	MyActor::Init();
	return true;
}


void MyEffect::EndSpriteEvent()
{
	m_curSprite = m_pSpriteList->begin();
	setEnable(false, true);
}

bool MyEffect::Render(ID3D11DeviceContext* pDContext) noexcept
{
	MyActor::Render(pDContext);
	return true;
}


void MyEffect::UpdateTextureUV()
{
	static MyVector4 posVector;
	if (m_curDirection == EDirection::Left)
	{
		posVector = m_curSprite->m_texVector4.getPosVector(m_curSprite->m_pTexture->getTextureWidth(), m_curSprite->m_pTexture->getTextureHeight());
		m_vertexList[0].tex = { posVector.x, posVector.y };
		m_vertexList[1].tex = { posVector.z, posVector.y };
		m_vertexList[2].tex = { posVector.x, posVector.w };
		m_vertexList[3].tex = m_vertexList[2].tex;
		m_vertexList[4].tex = m_vertexList[1].tex;
		m_vertexList[5].tex = { posVector.z, posVector.w };
	}
	else
	{
		posVector = m_curSprite->m_texVector4.getPosVector(m_curSprite->m_pTexture->getTextureWidth(), m_curSprite->m_pTexture->getTextureHeight());
		m_vertexList[0].tex = { posVector.z, posVector.y };
		m_vertexList[1].tex = { posVector.x, posVector.y };
		m_vertexList[2].tex = { posVector.z, posVector.w };
		m_vertexList[3].tex = m_vertexList[2].tex;
		m_vertexList[4].tex = m_vertexList[1].tex;
		m_vertexList[5].tex = { posVector.x, posVector.w };
	}
}

MyActor* MyEffect::clone()
{
	return (MyActor*)new MyEffect(*this);
}