#pragma once
#include "header.h"

class RenderDepthView
{
public:
	ID3D11RenderTargetView*	m_pRTView;			// 생성된 후면 버퍼 중 랜더링 될 버퍼
	ID3D11DepthStencilView*	m_pDepthSView;		// 깊이 스텐실 뷰
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

