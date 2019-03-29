#include "RasterizerState.h"





HRESULT RasterizerState::SetRasterizerState(ID3D11Device* pDevice, D3D11_CULL_MODE CullMode, D3D11_FILL_MODE d3dFillMode)
{
	memset(&m_RasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
	m_RasterizerDesc.CullMode = m_CullMode = CullMode;
	m_RasterizerDesc.FillMode = m_FillMode = d3dFillMode;
	m_PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pDevice->CreateRasterizerState(&m_RasterizerDesc, &m_pRasterizerState);
	return S_OK;
}

HRESULT RasterizerState::SetState(ID3D11DeviceContext* pDContext)
{
	//if (FAILED(SetRasterizerState(m_CullMode, m_FillMode)))
	//		return E_FAIL;
	pDContext->RSSetState(m_pRasterizerState);
	pDContext->IASetPrimitiveTopology(m_PrimitiveType);
	return S_OK;
}