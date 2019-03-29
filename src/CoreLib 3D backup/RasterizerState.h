#pragma once
#include "header.h"

class RasterizerState
{
public:
	// 상태 변수들
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveType;
	D3D11_RASTERIZER_DESC	 m_RasterizerDesc;
	D3D11_CULL_MODE			 m_CullMode;
	D3D11_FILL_MODE			 m_FillMode;

	// 버텍스, 픽셀 쉐이더
	ID3D11RasterizerState*	 m_pRasterizerState;
public:
	RasterizerState() = default;
	~RasterizerState() = default;

	// 레스터라이저 상태값 지정
	HRESULT SetRasterizerState(ID3D11Device* pDevice, D3D11_CULL_MODE CullMode = D3D11_CULL_FRONT, D3D11_FILL_MODE d3dFillMode= D3D11_FILL_SOLID );
	//HRESULT SetRasterizerState(ID3D11Device* pDevice, D3D11_RASTERIZER_DESC  RSDesc ); 
	// 레스트라이져 상태 지정
	//HRESULT SetCullMode( ID3D11Device* pDevice, D3D11_CULL_MODE CullMode );
	//HRESULT SetFillMode( ID3D11Device* pDevice, D3D11_FILL_MODE FillMode );
	// 프리미티브 유형 지정 
	//void  SetPrimitive(D3D11_PRIMITIVE_TOPOLOGY Primitive);
	//void  SetPrimitive(int iPrimitive);

	// 레스터라이저 상태값 적용 
	HRESULT SetState(ID3D11DeviceContext* pDContext);

};

