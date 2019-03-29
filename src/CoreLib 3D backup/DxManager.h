#pragma once
#include "SamplerState.h"		// +input
#include "RasterizerState.h"
#include "RenderDepthView.h"
#include "MyActor.h"
#include "Camera.h"

class DxManager : public IBasis, public ISingleton<DxManager>
{
private:
	ID3D11Device*				m_pD3dDevice;		// �׷���ī�� �����(���ҽ� ����)
	ID3D11DeviceContext*		m_pD3dContext;		// ������ ó�� �� ����
	IDXGIFactory*				m_pGIFactory;		// Graphic Infrastructure ��ü
	D3D11_VIEWPORT				m_ViewPort;			// ����Ʈ
	IDXGISwapChain*				m_pSwapChain;		// ���� �� �ĸ� ���� ���� �� ����
	DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;	// SwapChain����
	RenderDepthView				m_RTDSView;			// ��Ÿ��, ���� ���ٽ�

	RasterizerState				m_RasterState;		// �����Ͷ����� ����
	SamplerState				m_SamplerState;		// ���÷� ����

	HRESULT						hr;					// ���� üŷ��
	map<wstring, Texture*>		m_TextureList;		// �ؽ��� ��
public:
	map<string, ID3D11VertexShader*>	m_VShaderList;		// ���� ���̴� ��
	map<string, ID3D11PixelShader*>		m_PShaderList;		// �ȼ� ���̴� ��
	map<string, ID3D11InputLayout*>		m_LayoutList;		// ��ǲ ���̾ƿ� ��

	map<wstring, ID3D11BlendState*>		   m_BlenderList;	// ���� ���� ��
	map<wstring, ID3D11DepthStencilState*> m_DepthList;		// ���� ���� ��
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