#pragma once
#include "SamplerState.h"		// +input
#include "RasterizerState.h"
#include "MyActor.h"


class DxManager : public IBasis, public ISingleton<DxManager>
{
protected:
	IDXGIFactory*				m_pGIFactory;		// Graphic Infrastructure ��ü
	ID3D11RenderTargetView*		m_pRTView;			// ������ �ĸ� ���� �� ������ �� ����
	D3D11_VIEWPORT				m_ViewPort;			// ����Ʈ
	IDXGISwapChain*				m_pSwapChain;		// ���� �� �ĸ� ���� ���� �� ����
	DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;	// SwapChain����

	RasterizerState				m_RasterState;		// �����Ͷ����� ����
	SamplerState				m_SamplerState;		// ���÷� ����, (�ؽ�ó�� ����?)

	HRESULT						hr;					// ���� üŷ��
	map<wstring, Texture*>		m_TextureList;		// �ؽ��� ��
public:
	static ID3D11Device*		m_pD3dDevice;		// �׷���ī�� �����(���ҽ� ����)
	ID3D11DeviceContext*		m_pD3dContext;		// ������ ó�� �� ����
	
	map<wstring, ID3D11BlendState*>		m_BlenderList;		// ���� ���� ��
	map<string, ID3D11InputLayout*>		m_LayoutList;		// ��ǲ ���̾ƿ� ��
	map<string, ID3D11PixelShader*>		m_PShaderList;		// �ȼ� ���̴� ��
	map<string, ID3D11VertexShader*>	m_VShaderList;		// ���� ���̴� ��
private:
	friend class ISingleton<DxManager>;
	DxManager() = default;
public:
	virtual ~DxManager() = default;

	HRESULT CreateDevice();
	HRESULT CreateGISwapChain();
	HRESULT CreateRTView();
	HRESULT SetViewPort();

	HRESULT CreateBlendState(const wstring_view& blenderName);
	Texture* getTexture(const wstring_view& textureName, const wstring_view& extention = L".png");

	Texture* LoadShaderResourceView(const wstring_view& textureName, const wstring_view& extention);
	HRESULT LoadVertexShader(const wstring_view& loadUrl, const string_view& funcName);
	HRESULT LoadPixelShader(const wstring_view& loadUrl, const string_view& funcName);
	HRESULT LoadInputLayout(ID3DBlob* pVertexBuf, const string_view& funcName);

	HRESULT	ResizeDevice(const int& width, const int& height);
		
	virtual bool PrevRender() noexcept;
	virtual bool PostRender() noexcept;
	virtual bool Init() noexcept override;
	virtual bool Frame() noexcept override;
	virtual bool Render() noexcept override;
	virtual bool Release() noexcept override;
};