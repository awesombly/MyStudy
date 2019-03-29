#include "MyEffect.h"



MyEffect::MyEffect(const wstring_view& myName, const EObjType& objType,
				   const string_view& vertexShaderName, const string_view& pixelShaderName) :
	MyActor::MyActor(std::forward<const wstring_view>(myName), objType,
					 std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName))
{
	Init();
}

bool MyEffect::Init() noexcept
{
	MyActor::Init();
	return true;
}

//bool MyEffect::Frame(const float& spf, const float& accTime) noexcept
//{
//	if (!m_isEnable) return false;
//
//	// 스프라이트 프레임 계산
//	if (m_curSprite->m_frame != 0.0f)
//	{
//		m_frameCount += spf;
//		if (m_frameCount >= m_curSprite->m_frame)
//		{
//			m_frameCount = 0.0f;
//			if (++m_curSprite == m_pSpriteList->end())
//			{
//				m_curSprite = m_pSpriteList->begin();
//				setEnable(false);
//			}
//		}
//	}
//
//	return true;
//}

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