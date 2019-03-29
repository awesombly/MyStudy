#include "Inventory.h"
#include "ObjectManager.h"



bool Inventory::Init(const wstring_view& myName, const EObjType& objType,
					 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	m_myName = std::forward<const wstring_view>(myName);
	m_objType = objType;
	SetShaderLayout(std::forward<const string_view>(vertexShaderName),
					std::forward<const string_view>(pixelShaderName));
	m_vertexList.resize(6);
	setSpriteList(L"basic");
	CreateVertexList(DxManager::m_pD3dDevice);

	MyUI::Init();

	m_slotWidth  = (float)getWidth()  / (ColsCount + 1);
	m_slotHeight = (float)getHeight() / (RowsCount + 3);
	
	m_iterWidth = m_slotWidth / (ColsCount + 1);
	m_iterHeight = m_slotHeight / (RowsCount + 3);
	// 슬롯 생성
	for (int row = 1; row < RowsCount + 1; row++)
	{
		for (int col = 0; col < ColsCount; col++)
		{
			m_SlotList.push_back(new Slot(L"Slot", EObjType::UI));
			m_SlotList.back()->setRect((getX() + m_slotWidth  * col + m_iterWidth  * (col + 1)),
									  (getY() + m_slotHeight * row + m_iterHeight * (row + 1)),
									  m_slotWidth, m_slotHeight);
			m_SlotList.back()->Init();
			m_SlotList.back()->m_pMyParent = this;
		}
	}

	// 템 설명
	m_pTextBox = new Item(L"TextBox", EObjType::UI);
	m_pTextBox->setRect(0.0f, 0.0f, m_slotWidth * 2.5f, m_slotHeight * 1.6f);

	// 탭 버튼
	m_TabButtons[EItemType::All] = new TabButton(L"TabButton", EObjType::UI);
	m_TabButtons[EItemType::All]->setRect(getX() + m_iterWidth, getY() + m_iterHeight, m_slotWidth, m_slotWidth);
	m_TabButtons[EItemType::All]->setClickEvent(&Inventory::setTabButton, EItemType::All);

	m_TabButtons[EItemType::Equip] = new TabButton(L"TabButton", EObjType::UI);
	m_TabButtons[EItemType::Equip]->setRect(getX() + m_iterWidth + m_slotWidth, getY() + m_iterHeight, m_slotWidth, m_slotWidth);
	m_TabButtons[EItemType::Equip]->setClickEvent(&Inventory::setTabButton, EItemType::Equip);
	m_TabButtons[EItemType::Equip]->setSpriteList(L"equip");

	m_TabButtons[EItemType::Consum] = new TabButton(L"TabButton", EObjType::UI);
	m_TabButtons[EItemType::Consum]->setRect(getX() + m_iterWidth + m_slotWidth * 2, getY() + m_iterHeight, m_slotWidth, m_slotWidth);
	m_TabButtons[EItemType::Consum]->setClickEvent(&Inventory::setTabButton, EItemType::Consum);
	m_TabButtons[EItemType::Consum]->setSpriteList(L"consum");

	m_TabButtons[EItemType::Etc] = new TabButton(L"TabButton", EObjType::UI);
	m_TabButtons[EItemType::Etc]->setRect(getX() + m_iterWidth + m_slotWidth * 3, getY() + m_iterHeight, m_slotWidth, m_slotWidth);
	m_TabButtons[EItemType::Etc]->setClickEvent(&Inventory::setTabButton, EItemType::Etc);
	m_TabButtons[EItemType::Etc]->setSpriteList(L"etc");

	// 퀵슬롯
	int cols = 0;
	QuickSlot* pQuick = nullptr;
	float quickWidth = m_slotWidth * 0.7f;
	float quickHeight = m_slotHeight * 0.7f;

	pQuick = new QuickSlot(L"QuickSlot", EObjType::UI);
	pQuick->setRect(Window::getClientRect().right * 0.06f + quickWidth * cols + m_iterWidth * cols++,
					Window::getClientRect().bottom * 0.85f, quickWidth, quickHeight);
	pQuick->Init();
	pQuick->m_myHotKey = '1';
	m_QuickSlots.push_back(pQuick);

	pQuick = new QuickSlot(L"QuickSlot", EObjType::UI);
	pQuick->setRect(Window::getClientRect().right * 0.06f + quickWidth * cols + m_iterWidth * cols++,
					Window::getClientRect().bottom * 0.85f, quickWidth, quickHeight);
	pQuick->Init();
	pQuick->m_myHotKey = '2';
	m_QuickSlots.push_back(pQuick);

	pQuick = new QuickSlot(L"QuickSlot", EObjType::UI);
	pQuick->setRect(Window::getClientRect().right * 0.06f + quickWidth * cols + m_iterWidth * cols++,
					Window::getClientRect().bottom * 0.85f, quickWidth, quickHeight);
	pQuick->Init();
	pQuick->m_myHotKey = '3';
	m_QuickSlots.push_back(pQuick);

	pQuick = new QuickSlot(L"QuickSlot", EObjType::UI);
	pQuick->setRect(Window::getClientRect().right * 0.06f + quickWidth * cols + m_iterWidth * cols++,
					Window::getClientRect().bottom * 0.85f, quickWidth, quickHeight);
	pQuick->Init();
	pQuick->m_myHotKey = '4';
	m_QuickSlots.push_back(pQuick);

	ReadItemDatabase();
	setEnable(false);
	return true;
}

bool Inventory::Frame(const float& spf, const float& accTime) noexcept
{
	for (auto& iter : m_QuickSlots)
		iter->Frame(spf, accTime);
	Equipment::Instance->Frame(spf, accTime);

	// 활성화
	if (Input::Instance->getKeyState('R') == EKeyState::DOWN)
		setEnable(!m_isEnable);
	// 아이템 지속
	for (auto& iter : m_ItemDatabase)
		iter.second->Frame(spf, accTime);

	if (!m_isEnable) return false;

	// 마우스 체크 먼저
	for (auto& iter : m_SlotList)
		iter->Frame(spf, accTime);
	for (auto& iter : m_TabButtons)
		iter.second->Frame(spf, accTime);

	MyUI::Frame(spf, accTime);
	return true;
}

bool Inventory::Render(ID3D11DeviceContext* pDContext) noexcept
{
	for (auto& iter : m_QuickSlots)
		iter->Render(pDContext);
	Equipment::Instance->Render(pDContext);

	if (MyUI::Render(pDContext))
	{
		for (auto& iter : m_TabButtons)
			iter.second->Render(pDContext);
		for (auto& iter : m_SlotList)
			iter->Render(pDContext);
		// 드래그 아이콘
		for (auto& iter : m_SlotList)
			iter->AfterRender(pDContext);
	}
	for (auto& iter : m_QuickSlots)
		iter->AfterRender(pDContext);
	if (Equipment::Instance->isEnable())
	{
		for (auto& iter : Equipment::Instance->m_SlotList)
			iter->AfterRender(pDContext);
	}
	return true;
}

void Inventory::AddItem(Item* pItem, int count)
{
	pItem->m_itemTotalCount += count;
	for (auto& iter : m_SlotList)
	{
		if (iter->GetMyItem() == pItem)
		{
			if (iter->m_itemCount < pItem->m_itemMaxCount)
			{
				int subCount = pItem->m_itemMaxCount - iter->m_itemCount;
				iter->m_itemCount += min(subCount, count);
				count -= min(subCount, count);
				if (count <= 0)
				{
					UpdateItemCount();
					return;
				}
			}
		}
	}
	// 없으면 슬롯에 추가
	for (auto& iter : m_SlotList)
	{
		if (iter->GetMyItem() == nullptr)
		{
			iter->SetMyItem(pItem);
			iter->m_itemCount = min(pItem->m_itemMaxCount, count);
			count -= min(pItem->m_itemMaxCount, count);
			if (count <= 0)
			{
				UpdateItemCount();
				return;
			}
		}
	}
	pItem->m_itemTotalCount -= count;
	UpdateItemCount();

	wostringstream ostr;
	ostr << __FUNCTION__ << L", 아이템 습득 실패!" << endl;
	OutputDebugString(ostr.str().c_str());
}

bool Inventory::SubtractItem(Item* pItem, int count)
{
	pItem->m_itemTotalCount -= count;
	for (auto& iter : m_SlotList)
	{
		if (iter->GetMyItem() == pItem)
		{
			if (iter->m_itemCount > 0)
			{
				int subCount = iter->m_itemCount - count;
				iter->m_itemCount = max(0, subCount);
				if (iter->m_itemCount <= 0)
					iter->SetMyItem(nullptr);
				count = -subCount;
				if (count <= 0)
				{
					UpdateItemCount();
					return true;
				}
			}
		}
	}
	wostringstream ostr;
	ostr << __FUNCTION__ << L", 아이템 감소 수량 이상!" << endl;
	OutputDebugString(ostr.str().c_str());
	return false;
}

Item* Inventory::getRandomItem()
{
	int index = rand() % m_ItemDatabase.size();
	auto pItem = m_ItemDatabase.begin();
	for (int i = 0; i < index; i++)
		pItem++;
	return pItem->second;
}

bool Inventory::ReadItemDatabase()
{
	FILE* fp;
	_wfopen_s(&fp, L"./itemData.txt", L"rt");
	//imbue(std::locale("kor"));
	//wcout.imbue(locale("korean"));
	// 지역화 언어를 한글로 바꿈
	_wsetlocale(LC_ALL, L"korean");
	if (fp == nullptr)
	{
		wostringstream ostr;
		ostr << __FUNCTION__ << ", 파일 읽기 실패!" << endl;
		OutputDebugString(ostr.str().c_str());
		return false;
	}

	TCHAR _buffer[100] = { 0, };
	TCHAR _itemName[25] = { 0, };

	Item* pItem = nullptr;
	int _itemTypeIndex = 0;
	EItemType _itemType = EItemType::Etc;
	int _price = 0;
	int _maxCount = 0;

	float _value = 0.0f;
	float _action = 0.0f;
	float _move = 0.0f;
	float _defense = 0.0f;

	map<wstring, void (Item::*)()> itemEvent;
	itemEvent[L"Potion"]		= &Item::UseHealthPotion;
	itemEvent[L"FlamePotion"]	= &Item::UseFlamePotion;
	itemEvent[L"SteelPotion"]	= &Item::UseSteelPotion;
	//itemEvent[L"HolyWater"]		= &Item::UseHolyWater;

	//_fgetts(_buffer, _countof(_buffer), fp);					// 한줄 읽기
	while (wcscmp(_buffer, L"end\n"))
	{
		_fgetts(_buffer, _countof(_buffer), fp);
		_stscanf_s(_buffer, L"%s %d %d %d", _itemName, 25, &_itemTypeIndex, &_price, &_maxCount);	// 이름, 타입, 가격, 최대수량
		_itemType = (EItemType)_itemTypeIndex;

		_fgetts(_buffer, _countof(_buffer), fp);
		_stscanf_s(_buffer, L"%f %f %f %f", &_value, &_action, &_move, &_defense);					// 값, 공속, 이속, 방어

		_fgetts(_buffer, _countof(_buffer), fp);		// 템 설명
		
		pItem = new Item(L"ItemIcon", EObjType::Item);
		pItem->setSpriteList(_itemName);
		pItem->m_myName = _itemName;
		pItem->m_itemText = _buffer;
		pItem->m_itemType = _itemType;
		pItem->m_itemPrice = _price;
		pItem->m_itemMaxCount = _maxCount;

		pItem->m_usingValue  = _value;
		pItem->m_rateAction  = _action;
		pItem->m_rateMove	 = _move;
		pItem->m_rateDefense = _defense;

		if (itemEvent.find(pItem->m_myName) != itemEvent.end())
		{
			pItem->ItemEvent = itemEvent[pItem->m_myName];
		}
		else
		{
			pItem->ItemEvent = &Item::UseUsingItem;
		}
		AddItemDatabase(pItem);

		_fgetts(_buffer, _countof(_buffer), fp);
	}

	fclose(fp);
	return true;
}

void Inventory::AddItemDatabase(Item* pItem)
{
	pItem->setWH(m_slotWidth - m_iterWidth / 2, m_slotHeight - m_iterHeight / 2);
	m_ItemDatabase[pItem->m_myName.data()] = pItem;
}

void Inventory::UpdateItemCount()
{
	for (auto& iter : m_QuickSlots)
	{
		if (iter->GetMyItem() == nullptr)
			continue;
		if ((iter->m_itemCount = iter->GetMyItem()->m_itemTotalCount) == 0)
			iter->SetMyItem(nullptr);
	}
}

void Inventory::setTabButton(const EItemType& itemType)
{
	for (auto& iter : m_TabButtons)
	{
		if(itemType == EItemType::All || iter.first == itemType)
			iter.second->SetColor(1.0f, 1.0f, 1.0f);
		else
			iter.second->SetColor(0.3f, 0.3f, 0.3f);
	}

	for (auto& iter : m_ItemDatabase)
	{
		if (itemType == EItemType::All || iter.second->m_itemType == itemType)
		{
			iter.second->SetColor(1.0f, 1.0f, 1.0f);
			iter.second->setEnable(true);
		}
		else
		{
			iter.second->SetColor(0.3f, 0.3f, 0.3f);
			iter.second->setEnable(false);
		}
	}
}

void Inventory::MouseDragEvent()
{
	static MyVector2 moveVector;
	moveVector = Input::Instance->getCursor() - posToClick - m_point;
	m_point += moveVector;
	for (auto& iter : m_SlotList)
		iter->m_point += moveVector;
	for (auto& iter : m_TabButtons)
		iter.second->m_point += moveVector;
}

bool Inventory::Release() noexcept
{
	MyUI::Release();
	return true;
}

MyActor* Inventory::clone()
{
	return nullptr;
}