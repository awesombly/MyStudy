#include "HealthBar.h"
#include "Window.h"
#include "DxManager.h"
#include <wrl.h>

HealthBar* HealthBar::Instance = nullptr;

bool HealthBar::Init() noexcept
{
	MyUI::Init();
	Instance = this;
	// 후면
	m_BackSprite = m_curSprite;
	CreateVertexBuffer(DxManager::Instance->m_pD3dDevice, &m_pBackBuffer, &m_vertexList);
	// 전면
	setSpriteList(L"Front");
	m_FrontSprite = m_curSprite;
	CreateVertexBuffer(DxManager::Instance->m_pD3dDevice, &m_pFrontBuffer, &m_vertexList);
	// 중간
	setSpriteList(L"Inter");

	UpdateVertexList(DxManager::Instance->m_pD3dContext, true);
	
	m_BackVertexList = m_vertexList;
	UpdateVertexList(DxManager::Instance->m_pD3dContext, &m_pBackBuffer, &m_BackVertexList, true);

	m_FrontVertexList = m_vertexList;
	UpdateVertexList(DxManager::Instance->m_pD3dContext, &m_pFrontBuffer, &m_FrontVertexList, true);

	m_curHealthRate = 1.0f;
	m_dirHealthRate = 1.0f;
	return true;
}

bool HealthBar::Frame(const float& spf, const float& accTime) noexcept
{
	if (!m_isEnable)	return false;
	if (m_curHealthRate > m_dirHealthRate)
	{
		m_curHealthRate -= spf * 0.1f;
		UpdateInterBuffer();
	}
	else if (m_curHealthRate < m_dirHealthRate)
	{
		m_curHealthRate = m_dirHealthRate;
		UpdateInterBuffer();
	}
	
	FrameAction(spf);
	return true;
	spf; accTime;
}


bool HealthBar::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable)	return false;
	m_ppDContext = &pDContext;
	pDContext->VSSetShader(m_pVShader, nullptr, 0);
	pDContext->PSSetShader(m_pPShader, nullptr, 0);
	pDContext->IASetInputLayout(m_pInputLayout);

	// 후면
	m_BackSprite->m_pTexture->SetShaderResource(pDContext);
	UpdateVertexList(pDContext, &m_pBackBuffer, &m_BackVertexList, false);
	pDContext->IASetVertexBuffers(0, 1, &m_pBackBuffer, &Stride, &Offset);
	pDContext->Draw(6, 0);
	// 중간
	m_curSprite->m_pTexture->SetShaderResource(pDContext);
	UpdateVertexList(pDContext, false);
	pDContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	pDContext->Draw(6, 0);
	// 전면
	m_FrontSprite->m_pTexture->SetShaderResource(pDContext);
	UpdateVertexList(pDContext, &m_pFrontBuffer, &m_FrontVertexList, false);
	pDContext->IASetVertexBuffers(0, 1, &m_pFrontBuffer, &Stride, &Offset);
	pDContext->Draw(6, 0);

	RenderAction(pDContext);
	return true;
}

void HealthBar::MouseDragEvent()
{
	MyUI::MouseDragEvent();
	UpdateVertexList(*m_ppDContext, &m_pBackBuffer, &m_BackVertexList, true);
	UpdateVertexList(*m_ppDContext, &m_pVertexBuffer, &m_vertexList, true);
	UpdateVertexList(*m_ppDContext, &m_pBackBuffer, &m_FrontVertexList, true);
	UpdateInterBuffer();
	setHealthRate(m_dirHealthRate);
}

void HealthBar::setHealthRate(const float& healthRate)
{
	m_dirHealthRate = healthRate;
	// 전면 텍스쳐, 정점 갱신
	m_FrontSprite->m_texVector4.z = m_BackSprite->m_texVector4.z * m_dirHealthRate;
	MyVector4 posVector = m_FrontSprite->m_texVector4.getPosVector(m_FrontSprite->m_pTexture->getTextureWidth(), 
																   m_FrontSprite->m_pTexture->getTextureHeight());
	m_FrontVertexList[0].tex = { posVector.x, posVector.y };
	m_FrontVertexList[1].tex = { posVector.z, posVector.y };
	m_FrontVertexList[2].tex = { posVector.x, posVector.w };
	m_FrontVertexList[3].tex = m_FrontVertexList[2].tex;
	m_FrontVertexList[4].tex = m_FrontVertexList[1].tex;
	m_FrontVertexList[5].tex = { posVector.z, posVector.w };

	m_FrontVertexList[1].pos = { m_BackVertexList[0].pos.x + (m_BackVertexList[1].pos.x - m_BackVertexList[0].pos.x) * m_dirHealthRate,
								 m_BackVertexList[1].pos.y, 0.0f };
	m_FrontVertexList[4].pos = m_FrontVertexList[1].pos;
	m_FrontVertexList[5].pos = { m_FrontVertexList[1].pos.x, m_BackVertexList[5].pos.y, 0.0f };
}

void HealthBar::UpdateInterBuffer()
{
	// 중간 정점 갱신
	m_vertexList[1].pos = { m_BackVertexList[0].pos.x + (m_BackVertexList[1].pos.x - m_BackVertexList[0].pos.x) * m_curHealthRate,
		m_BackVertexList[1].pos.y, 0.0f };
	m_vertexList[4].pos = m_vertexList[1].pos;
	m_vertexList[5].pos = { m_vertexList[1].pos.x, m_BackVertexList[5].pos.y, 0.0f };
	m_curSprite->m_texVector4.z = m_BackSprite->m_texVector4.z * m_curHealthRate;
	UpdateTextureUV();
}

bool HealthBar::Release() noexcept
{
	MyUI::Release();
	return true;
}

MyActor* HealthBar::clone()
{
	return nullptr;
}