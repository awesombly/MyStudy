#pragma once
#include "Slot.h"


// 마우스업이벤트(아이템 포인터와 전체갯수, 밖뺄시-슬롯해제), 드래그(
class QuickSlot : public Slot
{
protected:

public:
	DWORD m_myHotKey;
protected:
	QuickSlot() = default;
public:
	QuickSlot(const wstring_view& myName, const EObjType& objType,
		 const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	virtual ~QuickSlot() = default;

	virtual void SwapItem(Slot* pSlot);
	virtual void SetMyItem(Item* pItem);
	virtual void MouseDragEvent() override;
	virtual void MouseUpEvent()   override;

	virtual bool Init()		noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
	virtual MyActor* clone();
};