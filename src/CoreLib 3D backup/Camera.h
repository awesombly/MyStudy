#pragma once
#include "header.h"
#include "IDxBasis.h"

struct VSCB
{
	D3DXMATRIX  m_matWorld;
	D3DXMATRIX  m_matView;
	D3DXMATRIX  m_matProj;
	D3DXVECTOR4 m_meshColor;
};

class Camera : public IDxBasis 
{
protected:
	D3DXMATRIX  m_matWorld;			// 월드
	D3DXMATRIX  m_matView;			// 뷰
	D3DXMATRIX  m_matProj;			// 투영

	//D3DXVECTOR3 m_Position = { 0.0f, 0.0f, 0.0f };	 // 카메라 위치
	D3DXVECTOR3 m_Look = { 0.0f, 0.0f, -1.0f };			 // 보는 기준점
	D3DXVECTOR3 m_At   = { 0.0f, 0.0f, 0.0f };			 // 바라볼 방향
	D3DXVECTOR3 m_Up   = { 0.0f, 1.0f, 0.0f };			 // 직교용
	D3DXVECTOR4 m_Rotation = { 0.0f, 0.0f, 0.0f, 0.0f }; // 카메라 각도

	float m_Aspect;					// 종횡비
	float m_FOV  = PI * 0.5f;		// 시야각
	float m_Near = 1.0f;			// 최소 가시 거리
	float m_Far  = 100.0f;			// 최대 가시 거리
public:
	VSCB			m_VSCB;				// 변환될 행렬
	ID3D11Buffer*	m_pConstBuffer;
protected:
	HRESULT CreateConstBuffer(ID3D11Device* pDevice, const void* data, const UINT& dataSize);
	void UpdateConstBuffer();
	void UpdateMatrix();
public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
public:
	Camera() = default;
	virtual ~Camera() = default;
};