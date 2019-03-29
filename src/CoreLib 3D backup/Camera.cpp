#include "Camera.h"
//#include "Window.h"
#include "DxManager.h"
#include "Timer.h"


bool Camera::Init()	noexcept
{
	//m_Aspect = (float)Window::getClientRect().right / (float)Window::getClientRect().bottom;
	m_Aspect = 1.0f;
	UpdateMatrix();
	CreateConstBuffer(DxManager::GetDevice(), &m_VSCB, sizeof(VSCB));
	return true;
}

bool Camera::Frame(const float& spf, const float& accTime)	noexcept
{
	if (Input::GetInstance().getKeyState(VK_NUMPAD4) == EKeyState::HOLD)
	{
		m_Look.x -= spf;
		m_At.x -= spf;
		//m_Position.x += spf;
	}
	if (Input::GetInstance().getKeyState(VK_NUMPAD6) == EKeyState::HOLD)
	{
		m_Look.x += spf;
		m_At.x += spf;
		//m_Position.x -= spf;
	}
	if (Input::GetInstance().getKeyState(VK_NUMPAD2) == EKeyState::HOLD)
	{
		m_Look.y -= spf;
		m_At.y -= spf;
		//m_Position.y += spf;
	}
	if (Input::GetInstance().getKeyState(VK_NUMPAD8) == EKeyState::HOLD)
	{
		m_Look.y += spf;
		m_At.y += spf;
		//m_Position.y -= spf;
	}
	if (Input::GetInstance().getKeyState(VK_NUMPAD1) == EKeyState::HOLD)
	{
		m_Rotation.x -= spf;
	}
	if (Input::GetInstance().getKeyState(VK_NUMPAD3) == EKeyState::HOLD)
	{
		m_Rotation.x += spf;
	}
	if (Input::GetInstance().getKeyState(VK_NUMPAD7) == EKeyState::HOLD)
	{
		m_Rotation.y += spf;
	}
	if (Input::GetInstance().getKeyState(VK_NUMPAD9) == EKeyState::HOLD)
	{
		m_Rotation.y -= spf;
	}
	if (Input::GetInstance().getKeyState(VK_ADD) == EKeyState::HOLD)
	{
		m_Look.z += spf;
		m_At.z += spf;
	}
	if (Input::GetInstance().getKeyState(VK_SUBTRACT) == EKeyState::HOLD)
	{
		m_Look.z -= spf;
		m_At.z -= spf;
	}
	if (Input::GetInstance().getKeyState(VK_NUMPAD5) == EKeyState::DOWN)
	{
		m_Look = { 0.0f, 0.0f, -1.0f };
		m_At = { 0.0f, 0.0f, 0.0f };
		m_Up = { 0.0f, 1.0f, 0.0f };

		m_Aspect = 1.0f;
		m_FOV  = PI * 0.5f;
		m_Near = 1.0f;
		m_Far  = 100.0f;
		//m_Position = { 0.0f, 0.0f, 0.0f };
		m_Rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	UpdateConstBuffer();
	return true;
	accTime;
}

bool Camera::Render(ID3D11DeviceContext* pDContext)			noexcept
{
	return true;
	pDContext;
}

bool Camera::Release() noexcept
{
	return true;
}


void Camera::UpdateMatrix()
{
	// ������� ��ȯ
	D3DXMatrixIdentity(&m_matWorld);
	
	//D3DXMatrixTransformation(&m_matWorld, nullptr, nullptr, nullptr, &m_At, &m_Rotation, &m_Position );
	//D3DXMatrixTranslation(&m_matWorld, m_Position.x, m_Position.y, m_Position.z);
	// ��� ȸ��
	D3DXMatrixRotationYawPitchRoll(&m_matWorld, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	// �� ��� ��ȯ
	D3DXMatrixLookAtLH(&m_matView, &m_Look, &m_At, &m_Up);
	// ���� ���� ��� ��ȯ    (����,      �þ߰�,   ���μ��� ��, ���ðŸ�
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_FOV, m_Aspect, m_Near, m_Far);
	// ��İ�
	m_matWorld = m_matWorld * m_matView * m_matProj;
	
	// ��ġ ���ȭ
	D3DXMatrixTranspose(&m_VSCB.m_matWorld, &m_matWorld);
	//D3DXMatrixTranspose(&m_VSCB.m_matView, &m_matView);
	//D3DXMatrixTranspose(&m_VSCB.m_matProj, &m_matProj);
	m_VSCB.m_meshColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

HRESULT Camera::CreateConstBuffer(ID3D11Device* pDevice, const void* data, const UINT& dataSize)
{
	D3D11_BUFFER_DESC initDesc = { 0, };
	initDesc.ByteWidth = dataSize;						// ���� ������
	initDesc.Usage = D3D11_USAGE_DYNAMIC;				// ���� ���ҽ�, GPU�� CPU�� ���� ���� ����
	initDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ���� Ÿ��
	initDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	initDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0, };
	initData.pSysMem = data;

	return pDevice->CreateBuffer(&initDesc, &initData, &m_pConstBuffer);
}


void Camera::UpdateConstBuffer()
{
	// ������۸� ����
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	MappedResource = { 0, };
	//D3D11_MAPPED_SUBRESOURCE MappedResource;
	// �����ϰ� �����ϰ� ��������?
	DxManager::GetDContext()->Map(m_pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	VSCB* pConstData = (VSCB*)MappedResource.pData;		// �ٲ� ��ü

	UpdateMatrix();

	pConstData->m_meshColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	DxManager::GetDContext()->Unmap(m_pConstBuffer, 0);
	DxManager::GetDContext()->UpdateSubresource(m_pConstBuffer, 0, nullptr, &m_VSCB, 0, 0);
}