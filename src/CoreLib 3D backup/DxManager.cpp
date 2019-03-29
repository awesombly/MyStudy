#include "DxManager.h"
#include <wrl.h>

//ID3D11Device* DxManager::m_pD3dDevice = nullptr;

bool DxManager::Init() noexcept
{
	if (FAILED(CreateDevice()) ||
		FAILED(CreateGISwapChain()) ||
		FAILED(m_RTDSView.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc)) ||
		FAILED(SetViewPort()))
	{
		ErrorMessage(""s + __FUNCTION__ + " -> DX �ʱ�ȭ ����!");
		return false;
	}
	// ������ �� ������Ƽ��, �����Ͷ����� Ÿ�� ����(��, ��, �ﰢ��, �Ѹ� �Ӽ�)
	m_RasterState.SetRasterizerState(m_pD3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);

	// ����, ���� ���� ����
	CreateBlendState(L"basic");
	CreateDepthStencilState(L"basic");

	// Writer �ʱ�ȭ
	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pBackBuffer);
	WriteManager::Instance->Init(pBackBuffer);

	//// ȭ�� ���� ����
	//if (FAILED(m_pGIFactory->MakeWindowAssociation(m_hWnd,
	//			 DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER)))
	//	return false;

	return true;
}

bool DxManager::Frame() noexcept
{
	// ���÷� ����
	m_SamplerState.Frame(m_pD3dDevice, m_pD3dContext);
	return true;
}

bool DxManager::PrevRender() noexcept
{
	// ���� Ÿ��, ����-���ٽ� �� Ŭ����
	m_RTDSView.ClearView(m_pD3dContext);
	// �����Ͷ����� ���� ����
	m_RasterState.SetState(m_pD3dContext);
	// ����, ���� ���� ����
	m_pD3dContext->OMSetBlendState(m_BlenderList[L"basic"], nullptr, (UINT)-1);
	//m_pD3dContext->OMSetDepthStencilState(m_DepthList[L"basic"], 1);
	// RenderTarget ����
	m_RTDSView.Setting(m_pD3dContext);
	WriteManager::Instance->Begin();
	return true;
}

bool DxManager::Render() noexcept
{
	return true;
}

bool DxManager::PostRender() noexcept
{
	WriteManager::Instance->End();
	m_pSwapChain->Present(0, 0);					//  ������� ������ Ŭ����
	return true;
}

bool DxManager::Release() noexcept
{
	// DX ����
	m_pSwapChain->Release();
	m_pGIFactory->Release();
	m_pD3dContext->Release();
	m_pD3dDevice->Release();
	return true;
}

Texture* DxManager::getTexture(const wstring_view& textureName, const wstring_view& extention)
{
	// ������ ���ҽ� �ε�
	auto iter = m_TextureList.find(textureName.data());
	if (iter == m_TextureList.end())
		return LoadShaderResourceView(textureName, extention);
	return iter->second;
}

Texture* DxManager::LoadShaderResourceView(const wstring_view& textureName, const wstring_view& extention)
{
	return m_TextureList[textureName.data()] = (new Texture())
		->CreateShaderResourceView(m_pD3dDevice, L"../../data/image/"s + textureName.data() + extention.data());
}

HRESULT DxManager::LoadVertexShader(const wstring_view& loadUrl, const string_view& funcName)
{
	// ���� ���̴� ����
	ID3D11VertexShader* m_pVShader;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pOutputBuf;
	ID3DBlob* pErrorBuf;
	hr = D3DX11CompileFromFile(loadUrl.data(), nullptr, nullptr, funcName.data(), "vs_5_0",
							   shaderFlags, 0, nullptr, &pOutputBuf, &pErrorBuf, nullptr);
	m_pD3dDevice->CreateVertexShader(pOutputBuf->GetBufferPointer(), pOutputBuf->GetBufferSize(), nullptr, &m_pVShader);
	m_VShaderList[funcName.data()] = m_pVShader;

	if (FAILED(LoadInputLayout(pOutputBuf, funcName)))
	{
		ErrorMessage(""s + __FUNCTION__ + " -> ���̾ƿ� ���� ����!");
	}

	if (pOutputBuf != nullptr)
		pOutputBuf->Release();
	if (pErrorBuf != nullptr)
		pErrorBuf->Release();
	return hr;
}

HRESULT DxManager::LoadPixelShader(const wstring_view& loadUrl, const string_view& funcName)
{
	// �ȼ� ���̴� ����
	ID3D11PixelShader* m_pPShader;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pOutputBuf = nullptr;
	ID3DBlob* pErrorBuf = nullptr;
	hr = D3DX11CompileFromFile(loadUrl.data(), nullptr, nullptr, funcName.data(), "ps_5_0",
							   shaderFlags, 0, nullptr, &pOutputBuf, &pErrorBuf, nullptr);
	m_pD3dDevice->CreatePixelShader(pOutputBuf->GetBufferPointer(), pOutputBuf->GetBufferSize(), nullptr, &m_pPShader);
	m_PShaderList[funcName.data()] = m_pPShader;

	if (pOutputBuf != nullptr)
		pOutputBuf->Release();
	if (pErrorBuf != nullptr)
		pErrorBuf->Release();
	return hr;
}

HRESULT DxManager::LoadInputLayout(ID3DBlob* pVertexBuf, const string_view& funcName)
{
	ID3D11InputLayout* pInputLayout = nullptr;
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		// �ṉ̀���,     , Ÿ��,                       ���۹���Ʈ, �Է�Ÿ��
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (FAILED(m_pD3dDevice->CreateInputLayout(layout, 4, pVertexBuf->GetBufferPointer(), pVertexBuf->GetBufferSize(), &pInputLayout)))
		return E_FAIL;
	m_LayoutList[funcName.data()] = pInputLayout;
	return S_OK;
}

HRESULT DxManager::CreateBlendState(const wstring_view& blenderName)
{
	D3D11_BLEND_DESC blendDesc = { 0, };

	// ���� ����
	blendDesc.AlphaToCoverageEnable = true;
	// 8�� �迭�� ����� 0���� ���
	blendDesc.RenderTarget[0].BlendEnable = true;
	// Src : ���, Desc : �ѷ��� ��ġ, Op : ������
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// ���İ�
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	// ������ �÷� RGB
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	return m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[blenderName.data()]);
}

HRESULT DxManager::CreateDepthStencilState(const wstring_view& depthName)
{
	ID3D11DepthStencilState* pDepthState = nullptr;
	m_DepthList[depthName.data()] = pDepthState;

	// ���̹��� ���°� ����
	D3D11_DEPTH_STENCIL_DESC dsDescDepth = { 0, };
	dsDescDepth.DepthEnable = true;	
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;			// ���� �� ����
	// ���ٽ� ���� ����
	dsDescDepth.StencilEnable = false;	
	dsDescDepth.StencilReadMask = 1;
	dsDescDepth.StencilWriteMask = 1;
	// ����-�ĸ� ���̽� �Ӽ�, ���̽� �븻���Ͱ� ī�޶� ���� ������ ��
	dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// ���̴� ���� �Ӽ��� ���� ����
	dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	return m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &pDepthState);
}

HRESULT	DxManager::ResizeDevice(const int& width, const int& height)
{
	if (m_pD3dDevice == nullptr)	return E_FAIL;

	WriteManager::Instance->ReleaseDeviceResource();
	// ����Ÿ�ٺ� �ʱ�ȭ, ����
	m_pD3dContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_pD3dContext->OMSetDepthStencilState(nullptr, 0);
	if (m_RTDSView.m_pRTView != nullptr)	m_RTDSView.m_pRTView->Release();
	// ����� ũ�� ����
	m_SwapChainDesc.BufferDesc.Width = width;
	m_SwapChainDesc.BufferDesc.Height = height;
	m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, width, height,
								m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);
	// ����Ÿ�ٺ� ����, ����Ʈ ����
	m_RTDSView.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	SetViewPort();

	Window::ResizeWindow({0, 0, width, height});

	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackBuffer);
	WriteManager::Instance->CreateDeviceResource(pBackBuffer);
	pBackBuffer->Release();

	return S_OK;
}

// Dx �ʱ� ����
HRESULT DxManager::CreateDevice()
{
	D3D_FEATURE_LEVEL pInFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	//	����̽� ����(�����:�׷��� ����̹�, ����̽� ����̹� Ÿ��, ����Ʈ���� ���, ����̽� ���� �÷���
	return D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
							 // ��� �� ����̹� ����, ���� ����, ���� SDK ����,
							 &pInFeatureLevel, 1, D3D11_SDK_VERSION,
							 // ��ȯ ���� Device �������̽�, ��ȯ ���� ����̹� ����
							 &m_pD3dDevice, nullptr,
							 // ��ȯ ���� DeviceContext �������̽�
							 &m_pD3dContext);
}

HRESULT DxManager::CreateGISwapChain()
{
	// GIFactory ����(DXGI) : GPU �� ���� ���� ������ ������ �����ϴ� ���� �������̽�?
	// �ش� ��ü�� ���� GUID ��ȣ�� ��ȯ���ְ�, �������̽��� �ش� ��ȣ�� ���� ��ü�� ��ȯ����
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pGIFactory)))
		return E_FAIL;

	// ���� ü�� ����
	m_SwapChainDesc = { 0, };
	m_SwapChainDesc.BufferCount = 1;									// ����� ����?
	m_SwapChainDesc.OutputWindow = Window::m_hWnd;						// ������ ������ �ڵ�
	m_SwapChainDesc.Windowed = true;									// �Ϲ�=true, Ǯ ��ũ��=false
	// �� ���� ����
	m_SwapChainDesc.BufferDesc.Width = Window::getClientRect().right;	// �ػ� �ʺ�
	m_SwapChainDesc.BufferDesc.Height = Window::getClientRect().bottom;	// �ػ� ����
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;				// �����(�ֻ���)
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;				// ���� / �и�
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// ����� ����ȼ� ����(Unsigned Normalize)
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// ǥ�� �� ���ҽ� ���� �ɼ�, ���� ��� �뵵
	// ��Ƽ ���ø�
	m_SwapChainDesc.SampleDesc.Count = 1;								// �ȼ� ���� ��Ƽ ���ø� ��(1=X),  ex)��Ƽ �ٸ����
	//SwapDesc.SampleDesc.Quality = 0;									// �̹��� ǰ�� ����

	//SwapDesc.BufferDesc.ScanlineOrdering;								// �ֻ缱 ������ ��� ����
	//SwapDesc.BufferDesc.Scaling;										// ũ�� ���� ���
	m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;			// ������ �ø���(Present)�� ����� ���� ����
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// ���� ü���� ���� ����(��ü ȭ��� �ػ� ����x?)

	return m_pGIFactory->CreateSwapChain(m_pD3dDevice, &m_SwapChainDesc, &m_pSwapChain);
}



HRESULT DxManager::SetViewPort()
{
	// ����Ʈ �Ӽ� ����   
	m_ViewPort.TopLeftX = 0;				// ���� ���� ����
	m_ViewPort.TopLeftY = 0;				// ���� ���� ����
	m_ViewPort.Width = (float)m_SwapChainDesc.BufferDesc.Width;
	m_ViewPort.Height = (float)m_SwapChainDesc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	m_pD3dContext->RSSetViewports(1, &m_ViewPort);
	return S_OK;
}


ID3D11Device*& DxManager::GetDevice()
{
	return GetInstance().m_pD3dDevice;
}

ID3D11DeviceContext*& DxManager::GetDContext()
{
	return GetInstance().m_pD3dContext;
}