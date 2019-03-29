#include "ChattingBox.h"
#include "Worker.h"
#include "Input.h"

ChattingBox*  ChattingBox::Instance = nullptr;
list<wstring> ChattingBox::m_msgList;
wstring		  ChattingBox::m_inputMsg;

bool ChattingBox::Frame(const float& spf, const float& accTime)	noexcept
{
	if (Input::Instance->getKeyState(VK_CONTROL) == EKeyState::DOWN)
		setEnable(!m_isEnable);
	if (!MyUI::Frame(spf, accTime))	return false;
	return true;
}

bool ChattingBox::Init() noexcept
{
	MyUI::Init();
	Instance = this;
	SetColor(0.9f, 0.9f, 0.9f, &m_vertexList);
	return true;
}

bool ChattingBox::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable)	return false;
	RenderAction(pDContext);
	pDContext->VSSetShader(m_pVShader, nullptr, 0);
	pDContext->PSSetShader(m_pPShader, nullptr, 0);
	pDContext->IASetInputLayout(m_pInputLayout);
	pDContext->PSSetShaderResources(0, 0, nullptr);
	PostRender(pDContext);

	int i = 3;
	for (auto iter = m_msgList.rbegin();
		 iter != m_msgList.rend() && i < 10; iter++, i++)
	{
		WriteManager::Instance->DrawTextW({ getLeft() + m_textSpace, getBottom() - i * m_textSpace, 1000, 1000 }, *iter);
	}
	if(Input::isChatting)
		WriteManager::Instance->DrawTextW({ getLeft() + m_textSpace, getBottom() - 2 * m_textSpace, 1000, 1000 }, m_inputMsg + L"_");

	return true;
}


void ChattingBox::MouseOverEvent()
{}

void ChattingBox::MouseDownEvent()
{}

void ChattingBox::MouseDragEvent()
{
	m_point = { Input::Instance->getCursor() - posToClick };
}

void ChattingBox::MouseUpEvent()
{}

void ChattingBox::MouseFreeEvent()
{}

MyActor* ChattingBox::clone()
{
	return (MyActor*)new ChattingBox(*this);
}