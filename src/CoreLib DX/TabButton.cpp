#include "TabButton.h"
#include "Inventory.h"
#include "Input.h"


TabButton::TabButton(const wstring_view& myName, const EObjType& objType,
				   const string_view& vertexShaderName, const string_view& pixelShaderName) :
	MyUI::MyUI(std::forward<const wstring_view>(myName), objType,
			   std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName))
{
	Init();
}

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
		wostringstream ostr;
		ostr << __FUNCTION__ << L", 등록된 함수 없음!" << endl;
		OutputDebugString(ostr.str().c_str());
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