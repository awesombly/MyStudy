#pragma once
#include "Input.h"


class SamplerState
{
public:
	ID3D11SamplerState*			m_pSamplerState;	// 샘플러 상태
	D3D11_FILTER				m_curFilter;		// 현재 필터
	D3D11_TEXTURE_ADDRESS_MODE	m_curTextureMode;	// 현재 텍스처 어드레스 모드
private:
	void Update(ID3D11Device* pDevice, ID3D11DeviceContext* pDContext);
public:
	SamplerState();
	virtual ~SamplerState() = default;

	bool Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pDContext) noexcept;
};