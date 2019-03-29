#include "Texture.h"
#include "DxManager.h"


// 리소스 파일로부터 텍스쳐를 얻어 쉐이더리소스뷰로 만듬
Texture* Texture::CreateShaderResourceView(ID3D11Device* pDevice, const wstring_view& srcUrl)
{
	D3DX11CreateShaderResourceViewFromFile(pDevice, srcUrl.data(), NULL, NULL, &m_pSRView, NULL);
	if (m_pSRView == nullptr)
	{
		while (1)
		{
			ErrorMessage(""s + __FUNCTION__ + " -> Load Error!");
			Sleep(1000);
		}
		return nullptr;
	}
	ID3D11Resource* src;
	ID3D11Texture2D* tex2D;
	
	m_pSRView->GetResource(&src);
	src->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tex2D);
	tex2D->GetDesc(&m_texDesc);

	src->Release();
	tex2D->Release();

	return this;
}

// 콘텍스트 : 텍스쳐 설정
HRESULT	Texture::SetShaderResource(ID3D11DeviceContext* pDContext)
{
	if (m_pSRView == nullptr)
	{
		ErrorMessage(""s + __FUNCTION__ + " -> nullptr!");
		return E_FAIL;
	}
	pDContext->PSSetShaderResources(0, 1, &m_pSRView);
	return S_OK;
}

UINT Texture::getTextureWidth()
{
	return m_texDesc.Width;
}
UINT Texture::getTextureHeight()
{
	return m_texDesc.Height;
}