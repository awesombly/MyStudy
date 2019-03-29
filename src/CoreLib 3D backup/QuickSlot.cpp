#include "QuickSlot.h"
#include "ObjectManager.h"
#include "Inventory.h"
//#include "MyPlayer.h"



bool QuickSlot::Init() noexcept
{
	Slot::Init();
	
	return true;
}

bool QuickSlot::Frame(const float& spf, const float& accTime) noexcept
{
	if (!Slot::Frame(spf, accTime))	return false;
	//if (m_pMyItem == nullptr)
	//	return true;

	if (Input::Instance->getKeyState(m_myHotKey) == EKeyState::DOWN && m_pMyItem != nullptr)
	{
		m_pMyItem->UseItem(this);
	}

	return true;
}

bool QuickSlot::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!Slot::Render(pDContext))	return false;
	
	return true;
}

void QuickSlot::MouseUpEvent()
{
	// 드래그 종료 이벤트
	m_pPShader = DxManager::Instance->m_PShaderList["PShader"];

	if (m_pMyItem == nullptr || !m_pMyItem->isEnable())
		return;
	for (auto& iter : Inventory::Instance->m_QuickSlots)
	{
		if (iter->CollisionCheck(Input::Instance->getCursor()))
		{
			SwapItem(iter);
			return;
		}
	}

	// 슬롯 해제
	if (!Inventory::Instance->CollisionCheck(Input::getCursor()))
	{
		m_pMyItem = nullptr;
		m_itemCount = 0;
	}
}

void QuickSlot::SwapItem(Slot* pSlot)
{
	Item* pTempItem = m_pMyItem;
	SetMyItem(pSlot->m_pMyItem);
	pSlot->SetMyItem(pTempItem);

	int tempCount = m_itemCount;
	m_itemCount = pSlot->m_itemCount;
	pSlot->m_itemCount = tempCount;
}

void QuickSlot::SetMyItem(Item* pItem)
{
	m_pMyItem = pItem;
	if (pItem == nullptr)
		return;
	static MyVector4 posVector;
	posVector = pItem->m_curSprite->m_texVector4.getPosVector(pItem->m_curSprite->m_pTexture->getTextureWidth(), pItem->m_curSprite->m_pTexture->getTextureHeight());
	m_DragVertexList[0].tex = { posVector.x, posVector.y };
	m_DragVertexList[1].tex = { posVector.z, posVector.y };
	m_DragVertexList[2].tex = { posVector.x, posVector.w };
	m_DragVertexList[3].tex = m_vertexList[2].tex;
	m_DragVertexList[4].tex = m_vertexList[1].tex;
	m_DragVertexList[5].tex = { posVector.z, posVector.w };
}

void QuickSlot::MouseDragEvent()
{
	if (Input::Instance->getMouseState(1) == EKeyState::HOLD)
	{
		m_point = { Input::Instance->getCursor() - posToClick };
		return;
	}
	Slot::MouseDragEvent();
}

bool QuickSlot::Release() noexcept
{
	Slot::Release();
	return true;
}

MyActor* QuickSlot::clone()
{
	return (MyActor*)new QuickSlot(*this);
}