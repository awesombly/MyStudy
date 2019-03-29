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
	D3DXMATRIX  m_matWorld;			// ����
	D3DXMATRIX  m_matView;			// ��
	D3DXMATRIX  m_matProj;			// ����

	//D3DXVECTOR3 m_Position = { 0.0f, 0.0f, 0.0f };	 // ī�޶� ��ġ
	D3DXVECTOR3 m_Look = { 0.0f, 0.0f, -1.0f };			 // ���� ������
	D3DXVECTOR3 m_At   = { 0.0f, 0.0f, 0.0f };			 // �ٶ� ����
	D3DXVECTOR3 m_Up   = { 0.0f, 1.0f, 0.0f };			 // ������
	D3DXVECTOR4 m_Rotation = { 0.0f, 0.0f, 0.0f, 0.0f }; // ī�޶� ����

	float m_Aspect;					// ��Ⱦ��
	float m_FOV  = PI * 0.5f;		// �þ߰�
	float m_Near = 1.0f;			// �ּ� ���� �Ÿ�
	float m_Far  = 100.0f;			// �ִ� ���� �Ÿ�
public:
	VSCB			m_VSCB;				// ��ȯ�� ���
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