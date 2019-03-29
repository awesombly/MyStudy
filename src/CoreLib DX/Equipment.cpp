#include "Equipment.h"
#include "Inventory.h"
#include "DxManager.h"
#include "MyPlayer.h"

bool Equipment::Init(const wstring_view& myName, const EObjType& objType,
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

	m_slotWidth = Inventory::Instance->m_slotWidth;
	m_slotHeight = Inventory::Instance->m_slotHeight;

	Slot* pSlot = nullptr;
	pSlot = new Slot(L"Slot", EObjType::UI);
	pSlot->setRect(getX() + getWidth() * 0.5f - m_slotWidth / 2, getY() + getHeight() * 0.2f - m_slotHeight / 2,
				   m_slotWidth, m_slotHeight);
	pSlot->Init();
	pSlot->m_pMyParent = this;
	m_SlotList.push_back(pSlot);

	pSlot = new Slot(L"Slot", EObjType::UI);
	pSlot->setRect(getX() + getWidth() * 0.2f - m_slotWidth / 2, getY() + getHeight() * 0.4f - m_slotHeight / 2,
				   m_slotWidth, m_slotHeight);
	pSlot->Init();
	pSlot->m_pMyParent = this;
	m_SlotList.push_back(pSlot);

	pSlot = new Slot(L"Slot", EObjType::UI);
	pSlot->setRect(getX() + getWidth() * 0.8f - m_slotWidth / 2, getY() + getHeight() * 0.4f - m_slotHeight / 2,
				   m_slotWidth, m_slotHeight);
	pSlot->Init();
	pSlot->m_pMyParent = this;
	m_SlotList.push_back(pSlot);

	pSlot = new Slot(L"Slot", EObjType::UI);
	pSlot->setRect(getX() + getWidth() * 0.35f - m_slotWidth / 2, getY() + getHeight() * 0.75f - m_slotHeight / 2,
				   m_slotWidth, m_slotHeight);
	pSlot->Init();
	pSlot->m_pMyParent = this;
	m_SlotList.push_back(pSlot);

	pSlot = new Slot(L"Slot", EObjType::UI);
	pSlot->setRect(getX() + getWidth() * 0.65f - m_slotWidth / 2, getY() + getHeight() * 0.75f - m_slotHeight / 2,
				   m_slotWidth, m_slotHeight);
	pSlot->Init();
	pSlot->m_pMyParent = this;
	m_SlotList.push_back(pSlot);

	m_pPlayerImage = new MyUI(L"Player", EObjType::UI);
	m_pPlayerImage->setWH(50, 75);
	m_pPlayerImage->setCenter(getCenter());
	
	setEnable(false);
	return true;
}


bool Equipment::Frame(const float& spf, const float& accTime) noexcept
{
	// 활성화
	if (Input::Instance->getKeyState('E') == EKeyState::DOWN)
		setEnable(!m_isEnable);
	if (!m_isEnable) return false;

	// 마우스 체크 먼저
	for (auto& iter : m_SlotList)
		iter->Frame(spf, accTime);
	m_pPlayerImage->Frame(spf, accTime);

	MyUI::Frame(spf, accTime);
	return true;
}

bool Equipment::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!MyUI::Render(pDContext))	return false;

	for (auto& iter : m_SlotList)
		iter->Render(pDContext);
	m_pPlayerImage->Render(pDContext);

	static wstringstream ostr;
	ostr.str(L"");
	ostr << L"공속 : " << MyPlayer::Instance->getRateAction() << L"\n"
		 << L"이속 : " << MyPlayer::Instance->getRateMove() << L"\n"
		 << L"방어 : " << MyPlayer::Instance->getRateDefense() << L"\n";
	WriteManager::Instance->DrawTextW({ getX() + getWidth() * 0.05f, getY() + getHeight() * 0.73f, 100, 400 }, ostr.str());
	return true;
}

void Equipment::MouseDragEvent()
{
	static MyVector2 moveVector;
	moveVector = Input::Instance->getCursor() - posToClick - m_point;
	m_point += moveVector;
	for (auto& iter : m_SlotList)
		iter->m_point += moveVector;
	m_pPlayerImage->m_point += moveVector;
}

bool Equipment::Release() noexcept
{
	MyUI::Release();
	return true;
}

MyActor* Equipment::clone()
{
	return nullptr;
}