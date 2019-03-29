#include "SamplerState.h"

SamplerState::SamplerState()
{
	m_curFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_curTextureMode = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pSamplerState = nullptr;
}

bool SamplerState::Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pDContext) noexcept
{
	if (Input::Instance->getKeyState(VK_NUMPAD1) == EKeyState::DOWN)
	{
		switch (m_curFilter)
		{
		case D3D11_FILTER_MIN_MAG_MIP_POINT:
		{
			m_curFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}break;
		case D3D11_FILTER_MIN_MAG_MIP_LINEAR:
		{
			m_curFilter = D3D11_FILTER_ANISOTROPIC;
		}break;
		default:
			m_curFilter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		}
		Update(pDevice, pDContext);
	}
	if (Input::Instance->getKeyState(VK_NUMPAD2) == EKeyState::DOWN)
	{
		switch (m_curTextureMode)
		{
		case D3D11_TEXTURE_ADDRESS_BORDER:
		{
			m_curTextureMode = D3D11_TEXTURE_ADDRESS_CLAMP;
		}break;
		case D3D11_TEXTURE_ADDRESS_CLAMP:
		{
			m_curTextureMode = D3D11_TEXTURE_ADDRESS_MIRROR;
		}break;
		case D3D11_TEXTURE_ADDRESS_MIRROR:
		{
			m_curTextureMode = D3D11_TEXTURE_ADDRESS_WRAP;
		}break;
		default:
		{
			m_curTextureMode = D3D11_TEXTURE_ADDRESS_BORDER;
		}break;
		}
		Update(pDevice, pDContext);
	}

	return true;
}

void SamplerState::Update(ID3D11Device* pDevice, ID3D11DeviceContext* pDContext)
{
	if (m_pSamplerState != nullptr)
		m_pSamplerState->Release();

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	
	samplerDesc.AddressU = m_curTextureMode;
	samplerDesc.AddressV = m_curTextureMode;
	samplerDesc.AddressW = m_curTextureMode;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 1;
	samplerDesc.MipLODBias = 0;
	samplerDesc.Filter = m_curFilter;
	pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	pDContext->PSSetSamplers(0, 1, &m_pSamplerState);
}