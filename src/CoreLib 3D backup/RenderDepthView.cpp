#include "RenderDepthView.h"
#include "DxManager.h"
#include <wrl.h>



HRESULT RenderDepthView::Create(ID3D11DeviceContext* pDContext, IDXGISwapChain* pSwapChain, const DXGI_SWAP_CHAIN_DESC& swapDesc)
{
	if(FAILED(CreateRenderTargetView(pDContext, pSwapChain)) ||
	   FAILED(CreateDepthStencilView(swapDesc)))
		return E_FAIL;
	return S_OK;
}

void RenderDepthView::Setting(ID3D11DeviceContext* pDContext)
{
	pDContext->OMSetRenderTargets(1, &m_pRTView, nullptr);
}

void RenderDepthView::ClearView(ID3D11DeviceContext* pDContext)
{
	// ��� ����, RGBA
	static float BackColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };			
	pDContext->ClearRenderTargetView(m_pRTView, BackColor);			// ����ۿ� ���
	// ����-���ٽ� ���۸� �ʱ�ȭ, �ʱ�ȭ ��� �÷���, ���̰�, ���ٽǰ�
	pDContext->ClearDepthStencilView(m_pDepthSView, D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/, 1.0f, 0);
}

HRESULT RenderDepthView::CreateRenderTargetView(ID3D11DeviceContext* pDContext, IDXGISwapChain*& pSwapChain)
{
	// DX���� Get() �Լ��� ��뿡 ���ǰ� �ʿ��ϰ�, �ݵ�� Release() �ؾ� �Ѵ�.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	// ����Ÿ�� ����
	if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)) ||
		FAILED(DxManager::GetDevice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRTView)))
		return E_FAIL;

	// ��Ÿ��, ���� ���ٽ� �� ����
	pDContext->OMSetRenderTargets(1, &m_pRTView, m_pDepthSView);
	return S_OK;
}

HRESULT RenderDepthView::CreateDepthStencilView(const DXGI_SWAP_CHAIN_DESC& swapDesc)
{
	// �ؽ��� ����
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC depthDesc = { 0, };
	depthDesc.Width = swapDesc.BufferDesc.Width;
	depthDesc.Height = swapDesc.BufferDesc.Height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// ���� Ÿ��
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	if (FAILED(DxManager::GetDevice()->CreateTexture2D(&depthDesc, nullptr, &pDSTexture)))
		return E_FAIL;
	// ���� ���ٽ� �� ����
	D3D11_DEPTH_STENCIL_VIEW_DESC DSViewDesc = { 0, };
	DSViewDesc.Format = depthDesc.Format;
	DSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSViewDesc.Texture2D.MipSlice = 0;

	return DxManager::GetDevice()->CreateDepthStencilView(pDSTexture.Get(), &DSViewDesc, &m_pDepthSView);
}