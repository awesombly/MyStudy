#pragma once
#include "header.h"

class RenderDepthView
{
public:
	ID3D11RenderTargetView*	m_pRTView;			// ������ �ĸ� ���� �� ������ �� ����
	ID3D11DepthStencilView*	m_pDepthSView;		// ���� ���ٽ� ��
protected:
	HRESULT CreateRenderTargetView(ID3D11DeviceContext* pDContext, IDXGISwapChain*& pSwapChain);
	HRESULT CreateDepthStencilView(const DXGI_SWAP_CHAIN_DESC& swapDesc);
public:
	HRESULT Create(ID3D11DeviceContext* pDContext, IDXGISwapChain* pSwapChain, const DXGI_SWAP_CHAIN_DESC& swapDesc);
	void Setting(ID3D11DeviceContext* pDContext);
	void ClearView(ID3D11DeviceContext* pDContext);

	RenderDepthView() = default;
	virtual ~RenderDepthView() = default;
};

