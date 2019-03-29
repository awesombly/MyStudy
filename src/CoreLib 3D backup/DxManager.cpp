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
		ErrorMessage(""s + __FUNCTION__ + " -> DX 초기화 실패!");
		return false;
	}
	// 랜더링 할 프리미티브, 레스터라이저 타입 지정(점, 선, 삼각형, 뿌릴 속성)
	m_RasterState.SetRasterizerState(m_pD3dDevice, D3D11_CULL_NONE, D3D11_FILL_SOLID);

	// 블랜더, 깊이 상태 생성
	CreateBlendState(L"basic");
	CreateDepthStencilState(L"basic");

	// Writer 초기화
	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pBackBuffer);
	WriteManager::Instance->Init(pBackBuffer);

	//// 화면 변경 막기
	//if (FAILED(m_pGIFactory->MakeWindowAssociation(m_hWnd,
	//			 DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER)))
	//	return false;

	return true;
}

bool DxManager::Frame() noexcept
{
	// 샘플러 적용
	m_SamplerState.Frame(m_pD3dDevice, m_pD3dContext);
	return true;
}

bool DxManager::PrevRender() noexcept
{
	// 렌더 타겟, 깊이-스텐실 뷰 클리어
	m_RTDSView.ClearView(m_pD3dContext);
	// 레스터라이저 상태 적용
	m_RasterState.SetState(m_pD3dContext);
	// 블랜더, 깊이 상태 적용
	m_pD3dContext->OMSetBlendState(m_BlenderList[L"basic"], nullptr, (UINT)-1);
	//m_pD3dContext->OMSetDepthStencilState(m_DepthList[L"basic"], 1);
	// RenderTarget 설정
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
	m_pSwapChain->Present(0, 0);					//  백버퍼의 내용을 클리핑
	return true;
}

bool DxManager::Release() noexcept
{
	// DX 해제
	m_pSwapChain->Release();
	m_pGIFactory->Release();
	m_pD3dContext->Release();
	m_pD3dDevice->Release();
	return true;
}

Texture* DxManager::getTexture(const wstring_view& textureName, const wstring_view& extention)
{
	// 없으면 리소스 로드
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
	// 정점 쉐이더 생성
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
		ErrorMessage(""s + __FUNCTION__ + " -> 레이아웃 생성 실패!");
	}

	if (pOutputBuf != nullptr)
		pOutputBuf->Release();
	if (pErrorBuf != nullptr)
		pErrorBuf->Release();
	return hr;
}

HRESULT DxManager::LoadPixelShader(const wstring_view& loadUrl, const string_view& funcName)
{
	// 픽셀 쉐이더 생성
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
		// 의미구조,     , 타입,                       시작바이트, 입력타입
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

	// 블랜딩 여부
	blendDesc.AlphaToCoverageEnable = true;
	// 8개 배열중 현재는 0번만 사용
	blendDesc.RenderTarget[0].BlendEnable = true;
	// Src : 대상, Desc : 뿌려질 위치, Op : 연산자
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// 알파값
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	// 적용할 컬러 RGB
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	return m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[blenderName.data()]);
}

HRESULT DxManager::CreateDepthStencilState(const wstring_view& depthName)
{
	ID3D11DepthStencilState* pDepthState = nullptr;
	m_DepthList[depthName.data()] = pDepthState;

	// 깊이버퍼 상태값 세팅
	D3D11_DEPTH_STENCIL_DESC dsDescDepth = { 0, };
	dsDescDepth.DepthEnable = true;	
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;			// 깊이 비교 조건
	// 스텐실 상태 지정
	dsDescDepth.StencilEnable = false;	
	dsDescDepth.StencilReadMask = 1;
	dsDescDepth.StencilWriteMask = 1;
	// 전면-후면 페이스 속성, 페이스 노말벡터가 카메라를 볼때 전면이 됨
	dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// 보이는 면의 속성에 따라 적용
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
	// 랜더타겟뷰 초기화, 해제
	m_pD3dContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_pD3dContext->OMSetDepthStencilState(nullptr, 0);
	if (m_RTDSView.m_pRTView != nullptr)	m_RTDSView.m_pRTView->Release();
	// 백버퍼 크기 조절
	m_SwapChainDesc.BufferDesc.Width = width;
	m_SwapChainDesc.BufferDesc.Height = height;
	m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, width, height,
								m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);
	// 랜더타겟뷰 생성, 뷰포트 세팅
	m_RTDSView.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	SetViewPort();

	Window::ResizeWindow({0, 0, width, height});

	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackBuffer);
	WriteManager::Instance->CreateDeviceResource(pBackBuffer);
	pBackBuffer->Release();

	return S_OK;
}

// Dx 초기 생성
HRESULT DxManager::CreateDevice()
{
	D3D_FEATURE_LEVEL pInFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	//	디바이스 생성(어댑터:그래픽 드라이버, 디바이스 드라이버 타입, 소프트웨어 모듈, 디바이스 생성 플래그
	return D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
							 // 사용 할 드라이버 버전, 버전 갯수, 현재 SDK 버전,
							 &pInFeatureLevel, 1, D3D11_SDK_VERSION,
							 // 반환 받을 Device 인터페이스, 반환 받을 드라이버 버전
							 &m_pD3dDevice, nullptr,
							 // 반환 받을 DeviceContext 인터페이스
							 &m_pD3dContext);
}

HRESULT DxManager::CreateGISwapChain()
{
	// GIFactory 생성(DXGI) : GPU 및 관련 기기들 정보를 가지고 제어하는 범용 인터페이스?
	// 해당 객체에 대한 GUID 번호를 반환해주고, 인터페이스에 해당 번호에 대한 객체를 반환해줌
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pGIFactory)))
		return E_FAIL;

	// 스왑 체인 생성
	m_SwapChainDesc = { 0, };
	m_SwapChainDesc.BufferCount = 1;									// 백버퍼 갯수?
	m_SwapChainDesc.OutputWindow = Window::m_hWnd;						// 보여질 윈도우 핸들
	m_SwapChainDesc.Windowed = true;									// 일반=true, 풀 스크린=false
	// 백 버퍼 세팅
	m_SwapChainDesc.BufferDesc.Width = Window::getClientRect().right;	// 해상도 너비
	m_SwapChainDesc.BufferDesc.Height = Window::getClientRect().bottom;	// 해상도 높이
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;				// 재생빈도(주사율)
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;				// 분자 / 분모
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// 백버퍼 출력픽셀 포멧(Unsigned Normalize)
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// 표면 및 리소스 생성 옵션, 버퍼 사용 용도
	// 멀티 샘플링
	m_SwapChainDesc.SampleDesc.Count = 1;								// 픽셀 단위 멀티 샘플링 수(1=X),  ex)안티 앨리어싱
	//SwapDesc.SampleDesc.Quality = 0;									// 이미지 품질 수준

	//SwapDesc.BufferDesc.ScanlineOrdering;								// 주사선 렌더링 모드 형식
	//SwapDesc.BufferDesc.Scaling;										// 크기 조정 모드
	m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;			// 페이지 플리핑(Present)후 백버퍼 보존 여부
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// 스왑 체인의 동작 결정(전체 화면시 해상도 변경x?)

	return m_pGIFactory->CreateSwapChain(m_pD3dDevice, &m_SwapChainDesc, &m_pSwapChain);
}



HRESULT DxManager::SetViewPort()
{
	// 뷰포트 속성 설정   
	m_ViewPort.TopLeftX = 0;				// 가로 시작 지점
	m_ViewPort.TopLeftY = 0;				// 세로 시작 지점
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