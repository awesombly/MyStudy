#pragma once
#include "Slot.h"


// 마우스업이벤트(아이템 포인터와 전체갯수, 밖뺄시-슬롯해제), 드래그(
class QuickSlot : public Slot
{
public:
	DWORD m_myHotKey;
public:
	using Slot::Slot;
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