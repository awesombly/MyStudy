#pragma once
#include "SamplerState.h"		// +input
#include "RasterizerState.h"
#include "RenderDepthView.h"
#include "MyActor.h"
#include "Camera.h"

class DxManager : public IBasis, public ISingleton<DxManager>
{
private:
	ID3D11Device*				m_pD3dDevice;		// 그래픽카드 제어권(리소스 생성)
	ID3D11DeviceContext*		m_pD3dContext;		// 랜더링 처리 및 제어
	IDXGIFactory*				m_pGIFactory;		// Graphic Infrastructure 객체
	D3D11_VIEWPORT				m_ViewPort;			// 뷰포트
	IDXGISwapChain*				m_pSwapChain;		// 전면 및 후면 버퍼 생성 및 제어
	DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;	// SwapChain상태
	RenderDepthView				m_RTDSView;			// 렌타뷰, 깊이 스텐실

	RasterizerState				m_RasterState;		// 레스터라이저 상태
	SamplerState				m_SamplerState;		// 샘플러 상태

	HRESULT						hr;					// 에러 체킹용
	map<wstring, Texture*>		m_TextureList;		// 텍스쳐 맵
public:
	map<string, ID3D11VertexShader*>	m_VShaderList;		// 정점 쉐이더 맵
	map<string, ID3D11PixelShader*>		m_PShaderList;		// 픽셀 쉐이더 맵
	map<string, ID3D11InputLayout*>		m_LayoutList;		// 인풋 레이아웃 맵

	map<wstring, ID3D11BlendState*>		   m_BlenderList;	// 블랜더 상태 맵
	map<wstring, ID3D11DepthStencilState*> m_DepthList;		// 깊이 상태 맵
private:
	HRESULT CreateDevice();
	HRESULT CreateGISwapChain();
	HRESULT SetViewPort();

	Texture* LoadShaderResourceView(const wstring_view& textureName, const wstring_view& extention);
	HRESULT LoadInputLayout(ID3DBlob* pVertexBuf, const string_view& funcName);
public:
	static ID3D11Device*& GetDevice();
	static ID3D11DeviceContext*& GetDContext();

	HRESULT CreateBlendState(const wstring_view& blenderName);
	HRESULT CreateDepthStencilState(const wstring_view& depthName);
	HRESULT LoadVertexShader(const wstring_view& loadUrl, const string_view& funcName);
	HRESULT LoadPixelShader(const wstring_view& loadUrl, const string_view& funcName);
	HRESULT	ResizeDevice(const int& width, const int& height);
	Texture* getTexture(const wstring_view& textureName, const wstring_view& extention = L".png");
		
	virtual bool PrevRender() noexcept;
	virtual bool PostRender() noexcept;
	virtual bool Init() noexcept override;
	virtual bool Frame() noexcept override;
	virtual bool Render() noexcept override;
	virtual bool Release() noexcept override;
private:
	friend class ISingleton<DxManager>;
	DxManager() = default;
public:
	virtual ~DxManager() = default;
};