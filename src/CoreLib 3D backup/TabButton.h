#pragma once
#include "MyUI.h"

class Inventory;

class TabButton : public MyUI
{
public:
	void (Inventory::*ClickEvent)(const EItemType&);
	EItemType m_TabItem;
public:
	using MyUI::MyUI;
	virtual ~TabButton() = default;

	virtual void setClickEvent(void(Inventory::*func)(const EItemType&), const EItemType& itemType);
	//virtual void MouseOverEvent() {}
	virtual void MouseDragEvent() {};
	virtual void MouseUpEvent();
	//virtual void MouseFreeEvent() {};

	virtual MyActor* clone();
	virtual bool Init()		noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};