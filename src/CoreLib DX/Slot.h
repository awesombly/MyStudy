#pragma once
#include "Item.h"


class Slot : public MyUI
{
protected:
	Item* m_pMyItem;					// ����-������

	ID3D11Buffer* m_pDragBuffer;
	vector<MyVertex> m_DragVertexList;	// �巡�� ����
	MyRect m_DragRect;
public:
	int m_itemCount;
	MyUI* m_pMyParent;
protected:
	friend class QuickSlot;
	Slot() = default;
public:
	Slot(const wstring_view& myName, const EObjType& objType,
			  const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	virtual ~Slot() = default;
	
	// ��ó�� �� �̹���, ���� ��ȯ
	virtual void AfterRender(ID3D11DeviceContext* pDContext);
	virtual void SwapItem(Slot* pSlot, const bool& isEquip = false);
	virtual void SetMyItem(Item* pItem);
	virtual Item* GetMyItem();
	// �̺�Ʈ ������
	virtual void MouseOverEvent() override;
	virtual void MouseDownEvent() override;
	virtual void MouseDragEvent() override;
	virtual void MouseUpEvent()   override;
	virtual void MouseFreeEvent() override;

	virtual bool Init()		noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
	virtual MyActor* clone();
};