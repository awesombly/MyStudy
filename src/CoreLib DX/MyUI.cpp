#include "MyUI.h"
#include "DxManager.h"

bool MyUI::isDraging = false;
MyVector2 MyUI::posToClick = { 0, 0 };

MyUI::MyUI(const wstring_view& myName, const EObjType& objType,
				   const string_view& vertexShaderName, const string_view& pixelShaderName) :
	MyActor::MyActor(std::forward<const wstring_view>(myName), objType,
					 std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName)),
		m_curState(EKeyState::FREE)
{
}

bool MyUI::Init() noexcept
{
	MyActor::Init();
	m_curState = EKeyState::FREE;
	return true;
}

bool MyUI::Frame(const float& spf, const float& accTime) noexcept
{
	if (!MyActor::Frame(spf, accTime))	return false;
	FrameAction(spf);
	return true;
	accTime;
}

bool MyUI::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable)	return false;
	RenderAction(pDContext);
	PrevRender(pDContext);
	PostRender(pDContext);
	return true;
}

void MyUI::FrameAction(const float& spf)
{
	// 클릭, 오버, 프리
	if (!isDraging)
	{
		if (CollisionCheck(Input::Instance->getCursor()))
		{
			if (Input::Instance->getMouseState(0) == EKeyState::DOWN)
			{
				if (m_curState != EKeyState::DOWN)
				{
					isDraging = true;
					posToClick = Input::Instance->getCursor() - getPoint();
					m_curState = EKeyState::DOWN;
					MouseDownEvent();
				}
			}
			else
			{
				if (m_curState != EKeyState::HOLD)
				{
					m_curState = EKeyState::HOLD;
					MouseOverEvent();
				}
			}
		}
		else
		{
			if (m_curState != EKeyState::FREE)
			{
				m_curState = EKeyState::FREE;
				MouseFreeEvent();
			}
		}

	}
	// 버튼 초기화
	if (Input::Instance->getMouseState(2) == EKeyState::DOWN)
	{
		if (CollisionCheck(Input::Instance->getCursor()))
		{
			isDraging = false;
			m_curState = EKeyState::FREE;
			MouseFreeEvent();
		}
	}
	return;
	spf;
}

void MyUI::RenderAction(ID3D11DeviceContext* pDContext)
{
	// 드래그중
	if (isDraging && m_curState == EKeyState::DOWN)
	{
		if (Input::Instance->getMouseState(0) == EKeyState::HOLD)
		{
			isDraging = true;
			MouseDragEvent();
		}
		else if (Input::Instance->getMouseState(0) == EKeyState::UP)
		{
			isDraging = false;
			m_curState = EKeyState::UP;
			MouseUpEvent();
		}
	}
	return;
	pDContext;
}

void MyUI::MouseOverEvent()
{
	m_pPShader = DxManager::Instance->m_PShaderList["PButtonOver"];
}

void MyUI::MouseDownEvent()
{
	m_pPShader = DxManager::Instance->m_PShaderList["PButtonClick"];
}

void MyUI::MouseDragEvent()
{
	m_point = { Input::Instance->getCursor() - posToClick };
}

void MyUI::MouseUpEvent()
{
	m_pPShader = DxManager::Instance->m_PShaderList["PShader"];
}

void MyUI::MouseFreeEvent()
{
	m_pPShader = DxManager::Instance->m_PShaderList["PShader"];
}


void MyUI::UpdateVertexList(ID3D11DeviceContext* pDContext, const bool& useListUpdate)
{
	if (useListUpdate)
	{
		m_proCoord.x = (float)getX() / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.y = -((float)getY() / Window::getClientRect().bottom * 2.0f - 1);
		m_proCoord.z = (float)getRight() / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.w = -((float)getBottom() / Window::getClientRect().bottom * 2.0f - 1);

		m_vertexList[0].pos = { m_proCoord.x, m_proCoord.y, 0.0f };
		m_vertexList[1].pos = { m_proCoord.z, m_proCoord.y, 0.0f };
		m_vertexList[2].pos = { m_proCoord.x, m_proCoord.w, 0.0f };
		m_vertexList[3] = m_vertexList[2];
		m_vertexList[4] = m_vertexList[1];
		m_vertexList[5].pos = { m_proCoord.z, m_proCoord.w, 0.0f };
	}
	pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}

void MyUI::UpdateVertexList(ID3D11DeviceContext* pDContext, ID3D11Buffer** pVertexBuffer, vector<MyVertex>* pVertexList, const bool& useListUpdate)
{
	if (useListUpdate)
	{
		m_proCoord.x = (float)getX() / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.y = -((float)getY() / Window::getClientRect().bottom * 2.0f - 1);
		m_proCoord.z = (float)getRight() / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.w = -((float)getBottom() / Window::getClientRect().bottom * 2.0f - 1);

		(*pVertexList)[0].pos = { m_proCoord.x, m_proCoord.y, 0.0f };
		(*pVertexList)[1].pos = { m_proCoord.z, m_proCoord.y, 0.0f };
		(*pVertexList)[2].pos = { m_proCoord.x, m_proCoord.w, 0.0f };
		(*pVertexList)[3] = (*pVertexList)[2];
		(*pVertexList)[4] = (*pVertexList)[1];
		(*pVertexList)[5].pos = { m_proCoord.z, m_proCoord.w, 0.0f };
	}
	pDContext->UpdateSubresource(*pVertexBuffer, 0, nullptr, &(*pVertexList).at(0), 0, 0);
}

void MyUI::UpdateVertexList(ID3D11DeviceContext* pDContext, ID3D11Buffer** pVertexBuffer, vector<MyVertex>* pVertexList, const MyRect& rect, const bool& useListUpdate)
{
	if (useListUpdate)
	{
		m_proCoord.x = (float)rect.getX() / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.y = -((float)rect.getY() / Window::getClientRect().bottom * 2.0f - 1);
		m_proCoord.z = (float)rect.getRight() / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.w = -((float)rect.getBottom() / Window::getClientRect().bottom * 2.0f - 1);

		(*pVertexList)[0].pos = { m_proCoord.x, m_proCoord.y, 0.0f };
		(*pVertexList)[1].pos = { m_proCoord.z, m_proCoord.y, 0.0f };
		(*pVertexList)[2].pos = { m_proCoord.x, m_proCoord.w, 0.0f };
		(*pVertexList)[3] = (*pVertexList)[2];
		(*pVertexList)[4] = (*pVertexList)[1];
		(*pVertexList)[5].pos = { m_proCoord.z, m_proCoord.w, 0.0f };
	}
	pDContext->UpdateSubresource(*pVertexBuffer, 0, nullptr, &(*pVertexList).at(0), 0, 0);
}

void MyUI::UpdateVertexList(ID3D11DeviceContext* pDContext, ID3D11Buffer** pVertexBuffer, vector<MyVertex>* pVertexList, const MyVector4& vector4, const bool& useListUpdate)
{
	if (useListUpdate)
	{
		m_proCoord.x = (float)vector4.x / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.y = -((float)vector4.y / Window::getClientRect().bottom * 2.0f - 1);
		m_proCoord.z = (float)vector4.z / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.w = -((float)vector4.w / Window::getClientRect().bottom * 2.0f - 1);

		(*pVertexList)[0].pos = { m_proCoord.x, m_proCoord.y, 0.0f };
		(*pVertexList)[1].pos = { m_proCoord.z, m_proCoord.y, 0.0f };
		(*pVertexList)[2].pos = { m_proCoord.x, m_proCoord.w, 0.0f };
		(*pVertexList)[3] = (*pVertexList)[2];
		(*pVertexList)[4] = (*pVertexList)[1];
		(*pVertexList)[5].pos = { m_proCoord.z, m_proCoord.w, 0.0f };
	}
	pDContext->UpdateSubresource(*pVertexBuffer, 0, nullptr, &(*pVertexList).at(0), 0, 0);
}

HRESULT MyUI::CreateVertexBuffer(ID3D11Device* pDevice, ID3D11Buffer** pVertexBuffer, vector<MyVertex>* pVertexList)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;				// 사용 모드(CPU->GPU 접근제어)
	bufferDesc.ByteWidth = (UINT)(pVertexList->size() * sizeof(MyVertex));	// 정점크기 * 갯수
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 버퍼 타입
	bufferDesc.CPUAccessFlags = 0;						// CPU 접근 제어

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &pVertexList->at(0);		// GPU에 전달 할 메모리

	if (*pVertexBuffer != nullptr)
		(*pVertexBuffer)->Release();
	//					버퍼 생성(버퍼정보,	   소스 정보, 대상 버퍼)
	return pDevice->CreateBuffer(&bufferDesc, &InitData, pVertexBuffer);
}

void MyUI::SetColor(const float& red, const float& green, const float& blue)
{
	for (auto& iter : m_vertexList)
	{
		iter.col.x = red;
		iter.col.y = green;
		iter.col.z = blue;
	}
}

void MyUI::SetColor(const float& red, const float& green, const float& blue, vector<MyVertex>* pVertexList)
{
	for (auto& iter : *pVertexList)
	{
		iter.col.x = red;
		iter.col.y = green;
		iter.col.z = blue;
	}
}


bool MyUI::Release() noexcept
{
	MyActor::Release();
	return true;
}

MyActor* MyUI::clone()
{
	return (MyActor*)new MyUI(*this);
}