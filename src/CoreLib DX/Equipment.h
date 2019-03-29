#pragma once
#include "Slot.h"



class Equipment : public MyUI, public ISingleton<Equipment>
{
protected:
	//const int ColsCount = 4;
	//const int RowsCount = 4;

	// ½½·Ô Å©±â
	float m_slotWidth;
	float m_slotHeight;

	map<wstring, Item*> m_ItemDatabase;
	MyUI*				m_pPlayerImage;

	MyActor* m_pTextBox;
public:
	list<Slot*> m_SlotList;
protected:
	friend class ISingleton<Equipment>;
	friend class Slot;
	Equipment() = default;
public:
	virtual ~Equipment() = default;
;
	virtual void MouseDragEvent() override;
	virtual MyActor* clone();

	//virtual bool Init()		noexcept override;
	bool Init(const wstring_view& myName, const EObjType& objType,
			  const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader") noexcept;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};