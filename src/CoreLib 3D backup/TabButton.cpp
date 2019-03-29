#include "TabButton.h"
#include "Inventory.h"
#include "Input.h"


bool TabButton::Init() noexcept
{
	MyUI::Init();
	MouseFreeEvent();
	ClickEvent = nullptr;
	return true;
}

bool TabButton::Frame(const float& spf, const float& accTime) noexcept
{
	if (!MyUI::Frame(spf, accTime))	return false;
	return true;
}

bool TabButton::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!MyUI::Render(pDContext))	return false;
	return true;
}


void TabButton::MouseUpEvent()
{
	if (!CollisionCheck(Input::getCursor()))
		return;
	if (ClickEvent == nullptr)
	{
		ErrorMessage(""s + __FUNCTION__ + " -> 등록된 함수 없음!");
	}
	else
		(Inventory::Instance->*ClickEvent)(m_TabItem);
}

void TabButton::setClickEvent(void(Inventory::*func)(const EItemType&), const EItemType& itemType)
{
	m_TabItem = itemType;
	ClickEvent = func;
}

bool TabButton::Release() noexcept
{
	MyUI::Release();
	return true;
}

MyActor* TabButton::clone()
{
	return (MyActor*)new TabButton(*this);
}