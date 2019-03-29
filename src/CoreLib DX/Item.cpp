#include "Item.h"
#include "Inventory.h"
#include "ObjectManager.h"
#include "MyPlayer.h"
#include "SoundManager.h"


Item::Item(const wstring_view& myName, const EObjType& objType,
		   const string_view& vertexShaderName, const string_view& pixelShaderName) :
	MyUI::MyUI(std::forward<const wstring_view>(myName), objType,
			   std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName))
{
	ItemEvent = nullptr;
}


bool Item::Init() noexcept
{
	MyUI::Init();
	m_itemTotalCount = 0;
	m_delayUsing = 0.0f;
	m_usingValue = 0.0f;
	return true;
}

bool Item::Frame(const float& spf, const float& accTime) noexcept
{
	//if (!MyUI::Frame(spf, accTime))	return false;
	if (m_delayUsing > 0.0f)
	{
		if ((m_delayUsing -= spf) < 0.0f)
		{
			m_delayUsing = 0.0f;
			MyPlayer::Instance->RemoveUsingItem(this);
		}
	}
	return true;
	spf; accTime;
}

bool Item::Render(ID3D11DeviceContext* pDContext) noexcept
{
	m_curSprite->m_pTexture->SetShaderResource(pDContext);
	UpdateTextureUV();
	PostRender(pDContext);
	return true;
}

void Item::UseItem(Slot* pSlot)
{
	if (m_itemTotalCount <= 0)
		return;
	switch (m_itemType)
	{
	case EItemType::Etc:
		break;
	case EItemType::Equip:
		if (pSlot->m_pMyParent == Equipment::Instance)
		{
			for (auto& iter : Inventory::Instance->m_SlotList)
			{
				if (iter->GetMyItem() == nullptr)
				{
					pSlot->SwapItem(iter, true);
					return;
				}
			}
		}
		else
		{
			for (auto& iter : Equipment::Instance->m_SlotList)
			{
				if (iter->GetMyItem() == nullptr)
				{
					pSlot->SwapItem(iter, true);
					SoundManager::Instance->PlayVariation("equip", false, 2);
					return;
				}
			}
		}
		break;
	case EItemType::Consum:
		Inventory::Instance->SubtractItem(this, 1);
		if (ItemEvent == nullptr)
		{
			wostringstream ostr;
			ostr << __FUNCTION__ << L", " << m_myName << L", 이벤트 미등록!" << endl;
			OutputDebugString(ostr.str().c_str());
			return;
		}
		(this->*ItemEvent)();
		SoundManager::Instance->PlayVariation("drink", false, 3);
		break;
	case EItemType::All:
		break;
	default:
		break;
	}
}

void Item::UseHealthPotion()
{
	MyPlayer::Instance->operHp(m_usingValue);
}
void Item::UseSteelPotion()
{
	MyPlayer::Instance->setDelayHit(m_usingValue);
	UseUsingItem();
}
void Item::UseUsingItem()
{
	m_delayUsing = m_usingValue;
	MyPlayer::Instance->RemoveUsingItem(this);
	MyPlayer::Instance->AddUsingItem(this);
}
void Item::UseFlamePotion()
{
	MyPlayer::Instance->UseFlamePotion();
}


bool Item::Release() noexcept
{
	MyActor::Release();
	return true;
}

MyActor* Item::clone()
{
	return (MyActor*)new Item(*this);
}