#pragma once
#include "MyUI.h"
#include "DxManager.h"


template <class FuncClass>
class MyButton : public MyUI
{
public:
	void (FuncClass::*ClickEvent)();
	wstring m_buttonText;
	
	FuncClass* m_pMainClass;
public:
	using MyUI::MyUI;
	virtual ~MyButton<FuncClass>() = default;
	// 버튼 클릭시 이벤트
	virtual void setClickEvent(void(FuncClass::*func)(), FuncClass* main);
	virtual void setText(const wstring_view& string);
	virtual void MouseDragEvent();
	virtual void MouseUpEvent();
	virtual MyActor* clone() override;

	virtual bool Init()		noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};




template <class FuncClass>
bool MyButton<FuncClass>::Init() noexcept
{
	MyUI::Init();
	//MouseFreeEvent();
	ClickEvent = nullptr;
	m_buttonText.clear();
	return true;
}

template <class FuncClass>
bool MyButton<FuncClass>::Frame(const float& spf, const float& accTime) noexcept
{
	if (!MyUI::Frame(spf, accTime))	return false;
	return true;
}

template <class FuncClass>
bool MyButton<FuncClass>::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!MyUI::Render(pDContext))	return false;
	//m_WriteManager.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	WriteManager::Instance->m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	WriteManager::Instance->DrawTextW({ getX(), getY() + 10, getWidth(), getHeight() }, m_buttonText);
	WriteManager::Instance->m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	return true;
}

template <class FuncClass>
void MyButton<FuncClass>::setText(const wstring_view& string)
{
	m_buttonText = std::forward<const wstring_view>(string);
}

template <class FuncClass>
void MyButton<FuncClass>::MouseDragEvent()
{
	if (Input::Instance->getMouseState(1) == EKeyState::HOLD)
		m_point = { Input::Instance->getCursor() - posToClick };
}

template <class FuncClass>
void MyButton<FuncClass>::MouseUpEvent()
{
	if (!CollisionCheck(Input::getCursor()))
		return;
	if (ClickEvent == nullptr)
	{
		ErrorMessage(""s + __func__ + " -> 할당된 함수가 없다");
	}
	else
	{
		(m_pMainClass->*ClickEvent)();
	}
}

template <class FuncClass>
void MyButton<FuncClass>::setClickEvent(void(FuncClass::*func)(), FuncClass* main)
{
	m_pMainClass = main;
	ClickEvent = func;
}

template <class FuncClass>
bool MyButton<FuncClass>::Release() noexcept
{
	MyUI::Release();
	return true;
}

template <class FuncClass>
MyActor* MyButton<FuncClass>::clone()
{
	return (MyActor*)new MyButton<FuncClass>(*this);
}