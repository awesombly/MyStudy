#pragma once
#include "MyUI.h"

class Slot;

class Item : public MyUI
{
protected:
public:
	EItemType m_itemType;
	wstring  m_itemText;
	int m_itemPrice;
	int m_itemMaxCount;
	int m_itemTotalCount;

	float m_rateAction	= 0.0f;
	float m_rateMove	= 0.0f;
	float m_rateDefense = 0.0f;

	float m_delayUsing = 0.0f;
	float m_usingValue = 0.0f;

	void (Item::*ItemEvent)();
protected:
	Item() = default;
public:
	Item(const wstring_view& myName, const EObjType& objType,
		 const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	virtual ~Item() = default;

	virtual void UseItem(Slot* pSlot);
	virtual void UseHealthPotion();
	virtual void UseSteelPotion();
	virtual void UseUsingItem();
	virtual void UseFlamePotion();

	virtual void FrameAction(const float& spf) override 
	{ return; spf; };
	virtual void RenderAction(ID3D11DeviceContext* pDContext) override 
	{ return; pDContext; };
	virtual MyActor* clone();

	virtual bool Init()		noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};