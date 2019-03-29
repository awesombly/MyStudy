#pragma once
#include "Input.h"


class SamplerState
{
public:
	ID3D11SamplerState*			m_pSamplerState;	// ���÷� ����
	D3D11_FILTER				m_curFilter;		// ���� ����
	D3D11_TEXTURE_ADDRESS_MODE	m_curTextureMode;	// ���� �ؽ�ó ��巹�� ���
private:
	void Update(ID3D11Device* pDevice, ID3D11DeviceContext* pDContext);
public:
	SamplerState();
	virtual ~SamplerState() = default;

	bool Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pDContext) noexcept;
};