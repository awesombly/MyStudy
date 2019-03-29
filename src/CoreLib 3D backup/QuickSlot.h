#pragma once
#include "Slot.h"


// ���콺���̺�Ʈ(������ �����Ϳ� ��ü����, �ۻ���-��������), �巡��(
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