#pragma once
#include "header.h"

class RasterizerState
{
public:
	// ���� ������
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveType;
	D3D11_RASTERIZER_DESC	 m_RasterizerDesc;
	D3D11_CULL_MODE			 m_CullMode;
	D3D11_FILL_MODE			 m_FillMode;

	// ���ؽ�, �ȼ� ���̴�
	ID3D11RasterizerState*	 m_pRasterizerState;
public:
	RasterizerState() = default;
	~RasterizerState() = default;

	// �����Ͷ����� ���°� ����
	HRESULT SetRasterizerState(ID3D11Device* pDevice, D3D11_CULL_MODE CullMode = D3D11_CULL_FRONT, D3D11_FILL_MODE d3dFillMode= D3D11_FILL_SOLID );
	//HRESULT SetRasterizerState(ID3D11Device* pDevice, D3D11_RASTERIZER_DESC  RSDesc ); 
	// ����Ʈ������ ���� ����
	//HRESULT SetCullMode( ID3D11Device* pDevice, D3D11_CULL_MODE CullMode );
	//HRESULT SetFillMode( ID3D11Device* pDevice, D3D11_FILL_MODE FillMode );
	// ������Ƽ�� ���� ���� 
	//void  SetPrimitive(D3D11_PRIMITIVE_TOPOLOGY Primitive);
	//void  SetPrimitive(int iPrimitive);

	// �����Ͷ����� ���°� ���� 
	HRESULT SetState(ID3D11DeviceContext* pDContext);

};

