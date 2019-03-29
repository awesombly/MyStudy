#include "ScrollBar.h"
#include "SoundManager.h"
#include "DxManager.h"
#include "Timer.h"


bool ScrollBar::Init() noexcept
{
	MyUI::Init();
	m_ScrollSprite = m_curSprite;
	m_ScrollVertex = m_vertexList;
	CreateVertexBuffer(DxManager::GetDevice(), &m_ScrollBuffer, &m_ScrollVertex);
	setSpriteList(L"scrollBack");
	SoundManager::Instance->setMasterVolume(50.0f);
	
	//MouseFreeEvent();
	ClickEvent = nullptr;
	m_buttonText.clear();
	m_ScrollRect.setWH((int)(getWidth() * 0.1f), getHeight());
	m_ScrollRect.setCenter(getCenter());

	m_curScrollRate = 0.5f;
	return true;
}

bool ScrollBar::Frame(const float& spf, const float& accTime) noexcept
{
	if (!MyUI::Frame(spf, accTime))	return false;
	return true;
}

bool ScrollBar::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!MyUI::Render(pDContext))	return false;
	WriteManager::Instance->m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	WriteManager::Instance->DrawTextW({ getX() - 90, getY() + 10, 100, getHeight() }, m_buttonText);
	WriteManager::Instance->m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	
	// 스크롤 버튼
	m_ScrollSprite->m_pTexture->SetShaderResource(pDContext);
	UpdateVertexList(pDContext, &m_ScrollBuffer, &m_ScrollVertex, m_ScrollRect, true);
	pDContext->IASetVertexBuffers(0, 1, &m_ScrollBuffer, &Stride, &Offset);
	pDContext->DrawIndexed(6, 0, 0);
	return true;
}

void ScrollBar::setText(const wstring_view& string)
{
	m_buttonText = std::forward<const wstring_view>(string);
}

void ScrollBar::MouseDragEvent()
{
	if (Input::Instance->getMouseState(1) == EKeyState::HOLD)
	{
		static MyVector2 moveVector;
		moveVector = Input::Instance->getCursor() - posToClick - m_point;
		m_point += moveVector;
		m_ScrollRect.m_point += moveVector;
	}
	else
	{
		m_curScrollRate = (Input::getCursor().x - getX()) / (float)getWidth();
		m_curScrollRate = max(0.01f, min(1.0f, m_curScrollRate));
		m_ScrollRect.setCenterX(getX() + m_curScrollRate * getWidth());
		//UpdateVertexList(pDContext, &m_ScrollBuffer, &m_ScrollVertex, m_ScrollRect, false);
		if (ClickEvent == nullptr)
		{
			ErrorMessage(""s + __FUNCTION__ + " -> 등록된 함수 없음!");
		}
		else
			(this->*ClickEvent)();
	}
}

void ScrollBar::setScrollRate(const float& rate)
{
	m_curScrollRate = rate;
	m_curScrollRate = max(0.0f, min(1.0f, m_curScrollRate));
	m_ScrollRect.setCenterX(getX() + m_curScrollRate * getWidth());
	if (ClickEvent == nullptr)
	{
		ErrorMessage(""s + __FUNCTION__ + " -> 등록된 함수 없음!");
	}
	else
		(this->*ClickEvent)();
}

void ScrollBar::UpdateMaxFrame()
{
	Timer::Instance->setDirFrame((int)(m_curScrollRate * 300));
}
void ScrollBar::UpdateGameSpeed()
{
	Timer::Instance->setGameSpeed(m_curScrollRate * 2);
}
void ScrollBar::UpdateSoundVolume()
{
	if (m_curScrollRate <= 0.011f)
		m_curScrollRate = 0.0f;
	SoundManager::Instance->setMasterVolume(m_curScrollRate);
}

void ScrollBar::setClickEvent(void(ScrollBar::*func)())
{
	ClickEvent = func;
}

void ScrollBar::Move(const MyVector2& point) noexcept
{
	MyRect::Move(point);
	m_ScrollRect.Move(point);
}

bool ScrollBar::Release() noexcept
{
	MyUI::Release();
	return true;
}

MyActor* ScrollBar::clone()
{
	return (MyActor*)new ScrollBar(*this);
}