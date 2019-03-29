#pragma once
#include "header.h"

class Texture
{
protected:
	ID3D11ShaderResourceView* m_pSRView;
	D3D11_TEXTURE2D_DESC	  m_texDesc;
public:
	Texture() = default;
	~Texture() = default;

	Texture* CreateShaderResourceView(ID3D11Device* pDevice, const wstring_view& srcUrl);
	HRESULT	SetShaderResource(ID3D11DeviceContext* pDContext);
	UINT getTextureWidth();
	UINT getTextureHeight();
};

