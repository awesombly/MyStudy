#pragma once
#include "MyObject.h"
#include "Inventory.h"


class DropItem : public MyObject
{
protected:
	Item*	m_myItem;
	int		m_itemCount;
public:
	using MyObject::MyObject;
	virtual ~DropItem() = default;

	virtual bool SetInitItem(Item* pItem, const int& itemCount) noexcept;
	virtual Item*	getMyItem();
	virtual int		getItemCount();
	virtual MyActor* clone() override;

	virtual bool Init() noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	//virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()										noexcept override;
};

