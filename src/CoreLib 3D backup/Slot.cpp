#include "Slot.h"
#include "ObjectManager.h"
#include "Inventory.h"
#include "MyPlayer.h"
#include "SoundManager.h"




bool Slot::Init() noexcept
{
	MyUI::Init();
	m_pMyItem = nullptr;
	m_pMyParent = nullptr;

	m_DragVertexList = m_vertexList;
	CreateVertexBuffer(DxManager::GetDevice(), &m_pDragBuffer, &m_DragVertexList);
	m_DragRect = *this;
	return true;
}


bool Slot::Frame(const float& spf, const float& accTime) noexcept
{
	if (!MyUI::Frame(spf, accTime))	return false;
	if (m_pMyItem == nullptr)
		return false;
	// 템 사용
	if(Input::Instance->getMouseState(1) == EKeyState::UP &&
	   CollisionCheck(Input::getCursor()))
		m_pMyItem->UseItem(this);
	return true;
}

bool Slot::Render(ID3D11DeviceContext* pDContext) noexcept
{
	//if (!MyUI::Render(pDContext))	return false;
	if (!m_isEnable) return false;
	RenderAction(pDContext);
	PrevRender(pDContext);
	PostRender(pDContext);

	// 아이템부
	if (m_pMyItem == nullptr)
		return true;
	m_pMyItem->setCenter(getCenter());
	m_pMyItem->Render(pDContext);
	
	// 템 갯수
	if (m_pMyItem->m_itemMaxCount >= 2)
	{
		static wostringstream ostr;
		ostr.str(L"");
		ostr << m_itemCount;
		//WriteManager::Instance->SetFontSize(13);
		WriteManager::Instance->DrawTextW({ getRight() - 22, getBottom() - 45, 20, 20 }, ostr.str());
	}
	return true;
}

void Slot::AfterRender(ID3D11DeviceContext* pDContext)
{
	if (m_pMyItem == nullptr || !m_pMyItem->isEnable())
		return;
	// 템 설명
	if (m_curState == EKeyState::HOLD)
	{
		Inventory::Instance->m_pTextBox->setPoint(getX(), getBottom());
		Inventory::Instance->m_pTextBox->Render(pDContext);
		//WriteManager::Instance->SetFontSize(3);
		WriteManager::Instance->DrawTextW({ getX() + 22, getBottom() + 22, getWidth() + 30, 200 }, m_pMyItem->m_itemText);
	}
	// 드래그 아이콘
	if (m_curState == EKeyState::DOWN)
	{
		pDContext->PSSetShader(DxManager::Instance->m_PShaderList["PHalfAlpha"], nullptr, 0);
		UpdateVertexList(pDContext, &m_pDragBuffer, &m_DragVertexList, m_DragRect, true);
		pDContext->IASetVertexBuffers(0, 1, &m_pDragBuffer, &Stride, &Offset);

		m_pMyItem->m_curSprite->m_pTexture->SetShaderResource(pDContext);
		pDContext->DrawIndexed(6, 0, 0);
	}
}

void Slot::MouseOverEvent()
{
	m_pPShader = DxManager::Instance->m_PShaderList["PButtonOver"];
}

void Slot::MouseDownEvent()
{
	m_pPShader = DxManager::Instance->m_PShaderList["PButtonClick"];
	m_DragRect.setCenter(Input::Instance->getCursor());
}

void Slot::MouseDragEvent()
{
	m_DragRect.setCenter(Input::Instance->getCursor());
}

void Slot::MouseFreeEvent()
{
	m_pPShader = DxManager::Instance->m_PShaderList["PShader"];
}

void Slot::MouseUpEvent()
{
	// 드래그 종료 이벤트
	m_pPShader = DxManager::Instance->m_PShaderList["PShader"];

	if (m_pMyItem == nullptr || !m_pMyItem->isEnable())
		return;
	// 슬롯
	for (auto& iter : Inventory::Instance->m_SlotList)
	{
		if (iter->CollisionCheck(Input::Instance->getCursor()))
		{
			// 인벤 to 인벤
			if (m_pMyParent == Inventory::Instance)
				SwapItem(iter);
			// 장비 to 인벤
			else if (m_pMyParent == Equipment::Instance)
			{
				if(iter->GetMyItem() == nullptr || 
				   iter->GetMyItem()->m_itemType == EItemType::Equip)
					SwapItem(iter, true);
				else
				{
					for (auto& inInter : Inventory::Instance->m_SlotList)
					{
						if (inInter->GetMyItem() == nullptr)
						{
							SwapItem(inInter, true);
							return;
						}
					}
				}
			}
			return;
		}
	}
	// 퀵
	//if (m_pMyItem->m_itemType == EItemType::Consum)
	{
		for (auto& outIter : Inventory::Instance->m_QuickSlots)
		{
			// 슬롯 장착
			if (outIter->CollisionCheck(Input::Instance->getCursor()))
			{
				outIter->SetMyItem(m_pMyItem);
				outIter->m_itemCount = m_pMyItem->m_itemTotalCount;
				return;
			}
		}
	}
	// to 장비
	if (m_pMyItem->m_itemType == EItemType::Equip)
	{
		for (auto& iter : Equipment::Instance->m_SlotList)
		{
			if (iter->CollisionCheck(Input::Instance->getCursor()))
			{
				SoundManager::Instance->PlayVariation("equip", false, 2);
				SwapItem(iter, true);
				return;
			}
		}
	}

	// 아이템 버리기
	if (!Inventory::Instance->CollisionCheck(Input::getCursor()) &&
		(!Equipment::Instance->CollisionCheck(Input::getCursor()) ||
		!Equipment::Instance->isEnable()))
	{
		SoundManager::Instance->Play("dropItem", false);
		((DropItem*)ObjectManager::Instance->TakeObject(L"ItemIcon", MyPlayer::MyCenter))
			->SetInitItem(m_pMyItem, m_itemCount);
		
		m_pMyItem->m_itemTotalCount -= m_itemCount;
		m_pMyItem = nullptr;
		m_itemCount = 0;
		Inventory::Instance->UpdateItemCount();
	}
}

void Slot::SwapItem(Slot* pSlot, const bool& isEquip)
{
	if (isEquip)
	{
		if (m_pMyParent == Equipment::Instance)
			MyPlayer::Instance->RemoveUsingItem(m_pMyItem);
		if (pSlot->m_pMyParent == Equipment::Instance)
			MyPlayer::Instance->RemoveUsingItem(pSlot->m_pMyItem);
	}

	// 같은 템이면 물량 이동
	if (m_pMyItem == pSlot->m_pMyItem)
	{
		int subCount = m_pMyItem->m_itemMaxCount - pSlot->m_itemCount;
		pSlot->m_itemCount += min(subCount, m_itemCount);
		m_itemCount -= min(subCount, m_itemCount);
	}
	// 아니면 교환
	else
	{
		Item* pTempItem = m_pMyItem;
		SetMyItem(pSlot->m_pMyItem);
		pSlot->SetMyItem(pTempItem);
		
		int tempCount = m_itemCount;
		m_itemCount = pSlot->m_itemCount;
		pSlot->m_itemCount = tempCount;
	}

	if (isEquip)
	{
		if(m_pMyParent == Equipment::Instance)
			MyPlayer::Instance->AddUsingItem(m_pMyItem);
		if (pSlot->m_pMyParent == Equipment::Instance)
			MyPlayer::Instance->AddUsingItem(pSlot->m_pMyItem);
		MyPlayer::Instance->UpdateItemState();
	}
}

void Slot::SetMyItem(Item* pItem)
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

Item* Slot::GetMyItem()
{
	return m_pMyItem;
}

bool Slot::Release() noexcept
{
	MyUI::Release();
	return true;
}

MyActor* Slot::clone()
{
	return (MyActor*)new Slot(*this);
}