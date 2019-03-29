#include "BloodRate.h"
#include "Window.h"
#include "DxManager.h"

BloodRate* BloodRate::Instance = nullptr;


bool BloodRate::Init() noexcept
{
	MyUI::Init();
	Instance = this;
	// 후면
	m_BackSprite = m_curSprite;
	CreateVertexBuffer(DxManager::GetDevice(), &m_pBackBuffer, &m_vertexList);
	// 중간
	setSpriteList(L"Inter");

	m_curHealthRate = 0.0f;
	//m_dirHealthRate = 0.0f;

	UpdateVertexList(DxManager::GetDContext(), true);
	
	m_BackVertexList = m_vertexList;
	UpdateVertexList(DxManager::GetDContext(), &m_pBackBuffer, &m_BackVertexList, true);
	return true;
}

bool BloodRate::Frame(const float& spf, const float& accTime) noexcept
{
	if (!m_isEnable)	return false;

	UpdateInterBuffer();
	FrameAction(spf);
	return true;
	spf; accTime;
}


bool BloodRate::Render(ID3D11DeviceContext* pDContext) noexcept
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
	pDContext->DrawIndexed(6, 0, 0);
	// 중간
	m_curSprite->m_pTexture->SetShaderResource(pDContext);
	UpdateVertexList(pDContext, false);
	pDContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	pDContext->DrawIndexed(6, 0, 0);

	RenderAction(pDContext);
	return true;
}

void BloodRate::MouseDragEvent()
{
	MyUI::MouseDragEvent();
	UpdateVertexList(*m_ppDContext, &m_pBackBuffer, &m_BackVertexList, true);
	UpdateVertexList(*m_ppDContext, &m_pVertexBuffer, &m_vertexList, true);
	UpdateInterBuffer();
	setHealthRate(m_curHealthRate);
}

void BloodRate::setHealthRate(const float& healthRate)
{
	m_curHealthRate = healthRate;
}

bool BloodRate::operHealthRate(const float& value)
{
	if (m_curHealthRate == 0.0f && value < 0.0f)
		return false;
	m_curHealthRate += value;
	m_curHealthRate = min(1.0f, max(0.0f, m_curHealthRate));
	return true;
}


void BloodRate::UpdateInterBuffer()
{
	// 중간 정점 갱신
	m_vertexList[1].pos = { m_BackVertexList[0].pos.x + (m_BackVertexList[1].pos.x - m_BackVertexList[0].pos.x) * m_curHealthRate,
		m_BackVertexList[1].pos.y, 0.0f };
	//m_vertexList[4].pos = m_vertexList[1].pos;
	m_vertexList[3].pos = { m_vertexList[1].pos.x, m_BackVertexList[3].pos.y, 0.0f };
	m_curSprite->m_texVector4.z = m_BackSprite->m_texVector4.z * m_curHealthRate;
	UpdateTextureUV();
}

bool BloodRate::Release() noexcept
{
	MyUI::Release();
	return true;
}

MyActor* BloodRate::clone()
{
	return nullptr;
}