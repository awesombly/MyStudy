#pragma once
#include "locale.h"
#include "ISingleton.h"
#include "QuickSlot.h"
#include "TabButton.h"
#include "Equipment.h"



class Inventory : public MyUI, public ISingleton<Inventory>
{
protected:
	const int ColsCount = 4;
	const int RowsCount = 4;

	// ½½·Ô Å©±â
	float m_slotWidth;
	float m_slotHeight;
	// ½½·Ô °£°Ý
	float m_iterWidth;
	float m_iterHeight;
	
	map<wstring, Item*> m_ItemDatabase;
	MyActor* m_pTextBox;

	map<EItemType, TabButton*> m_TabButtons;
	int m_money;
public:
	list<Slot*> m_SlotList;
	list<QuickSlot*> m_QuickSlots;
private:
	friend class ISingleton<Inventory>;
	friend class Slot;
	friend class QuickSlot;
	friend class Equipment;
	Inventory() = default;

	virtual bool ReadItemDatabase();
	virtual void AddItemDatabase(Item* pItem);
public:
	virtual ~Inventory() = default;

	virtual void AddItem(Item* pItem, int count);
	virtual bool SubtractItem(Item* pItem, int count);
	virtual Item* getRandomItem();
	//virtual void operMoney(const int& value);
	//virtual void ItemSort();

	virtual void UpdateItemCount();
	virtual void setTabButton(const EItemType&);
	virtual void MouseDragEvent() override;
	virtual MyActor* clone();

	//virtual bool Init()		noexcept override;
	virtual bool Init(const wstring_view& myName, const EObjType& objType,
			 const string_view& vertexShaderName = "VS_Basic", const string_view& pixelShaderName = "PShader") noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};