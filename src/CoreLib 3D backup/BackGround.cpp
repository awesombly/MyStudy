#include "BackGround.h"
#include "Window.h"



void BackGround::UpdateVertexList(ID3D11DeviceContext* pDContext, const bool& useListUpdate)
{
	if (useListUpdate)
	{
		m_proCoord.x = (getX() - Window::DrawPoint.x / 4) / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.y = -((getY() - Window::DrawPoint.y / 4) / Window::getClientRect().bottom * 2.0f - 1);
		m_proCoord.z = (getRight() - Window::DrawPoint.x / 4) / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.w = -((getBottom() - Window::DrawPoint.y / 4) / Window::getClientRect().bottom * 2.0f - 1);

		m_vertexList[0].pos = { m_proCoord.x, m_proCoord.y, 0.0f };
		m_vertexList[1].pos = { m_proCoord.z, m_proCoord.y, 0.0f };
		m_vertexList[2].pos = { m_proCoord.x, m_proCoord.w, 0.0f };
		//m_vertexList[3] = m_vertexList[2];
		//m_vertexList[4] = m_vertexList[1];
		m_vertexList[3].pos = { m_proCoord.z, m_proCoord.w, 0.0f };
	}
	pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}


//bool BackGround::Init() noexcept
//{
//	MyActor::Init();
//	return true;
//}
//
//bool BackGround::Frame(const float& spf, const float& accTime) noexcept
//{
//	if (!m_isEnable)	return false;
//	return true;
//	spf; accTime;
//}
//
//bool BackGround::Render(ID3D11DeviceContext* pDContext) noexcept
//{
//	if (!m_isEnable)	return false;
//	PrevRender(pDContext);
//	PostRender(pDContext);
//	return true;
//}
//bool BackGround::Release() noexcept
//{
//	MyActor::Release();
//	return true;
//}
//
//MyActor* BackGround::clone()
//{
//	return (MyActor*)new BackGround(*this);
//}