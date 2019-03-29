#include "Settings.h"
#include "ObjectManager.h"



bool Settings::Init(const wstring_view& myName, const EObjType& objType,
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


	m_ScrollBars[EConfig::MaxFrame] = new ScrollBar(L"ScrollBar", EObjType::UI);
	m_ScrollBars[EConfig::MaxFrame]->setRect(getX() + getWidth() * 0.2f, getY() + getBottom() * 0.25f, getWidth() * 0.7f, getHeight() * 0.1f);
	m_ScrollBars[EConfig::MaxFrame]->Init();
	m_ScrollBars[EConfig::MaxFrame]->setText(L"프레임");
	m_ScrollBars[EConfig::MaxFrame]->setClickEvent(&ScrollBar::UpdateMaxFrame);

	m_ScrollBars[EConfig::GameSpeed] = new ScrollBar(L"ScrollBar", EObjType::UI);
	m_ScrollBars[EConfig::GameSpeed]->setRect(getX() + getWidth() * 0.2f, getY() + getBottom() * 0.4f, getWidth() * 0.7f, getHeight() * 0.1f);
	m_ScrollBars[EConfig::GameSpeed]->Init();
	m_ScrollBars[EConfig::GameSpeed]->setText(L"게임 속도");
	m_ScrollBars[EConfig::GameSpeed]->setClickEvent(&ScrollBar::UpdateGameSpeed);

	m_ScrollBars[EConfig::Volume] = new ScrollBar(L"ScrollBar", EObjType::UI);
	m_ScrollBars[EConfig::Volume]->setRect(getX() + getWidth() * 0.2f, getY() + getBottom() * 0.55f, getWidth() * 0.7f, getHeight() * 0.1f);
	m_ScrollBars[EConfig::Volume]->Init();
	m_ScrollBars[EConfig::Volume]->setText(L"음량");
	m_ScrollBars[EConfig::Volume]->setClickEvent(&ScrollBar::UpdateSoundVolume);
	ResetConfig();

	m_Buttons[EConfig::Close] = new MyButton<Settings>(L"Button", EObjType::UI);
	m_Buttons[EConfig::Close]->setRect(getX() + getWidth() * 0.25f, getY() + getBottom() * 0.7f, getWidth() * 0.2f, getHeight() * 0.1f);
	m_Buttons[EConfig::Close]->setText(L"Close");
	m_Buttons[EConfig::Close]->setClickEvent(&Settings::CloseConfig, this);

	m_Buttons[EConfig::Reset] = new MyButton<Settings>(L"Button", EObjType::UI);
	m_Buttons[EConfig::Reset]->setRect(getX() + getWidth() * 0.55f, getY() + getBottom() * 0.7f, getWidth() * 0.2f, getHeight() * 0.1f);
	m_Buttons[EConfig::Reset]->setText(L"Reset");
	m_Buttons[EConfig::Reset]->setClickEvent(&Settings::ResetConfig, this);

	ObjectManager::Instance->PushObject(this);
	setEnable(false);
	return true;
}

bool Settings::Frame(const float& spf, const float& accTime) noexcept
{
	// 활성화
	if (Input::Instance->getKeyState(VK_TAB) == EKeyState::DOWN)
		setEnable(!m_isEnable);
	if (!m_isEnable) return false;

	for (auto& iter : m_Buttons)
		iter.second->Frame(spf, accTime);
	for (auto& iter : m_ScrollBars)
		iter.second->Frame(spf, accTime);

	MyUI::Frame(spf, accTime);
	return true;
}

bool Settings::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!MyUI::Render(pDContext))	return false;

	for (auto& iter : m_Buttons)
		iter.second->Render(pDContext);
	for (auto& iter : m_ScrollBars)
		iter.second->Render(pDContext);

	return true;
}


void Settings::MouseDragEvent()
{
	static MyVector2 moveVector;
	moveVector = Input::Instance->getCursor() - posToClick - m_point;
	m_point += moveVector;
	for (auto& iter : m_Buttons)
		iter.second->Move(moveVector);
	for (auto& iter : m_ScrollBars)
		iter.second->Move(moveVector);
}

void Settings::ResetConfig()
{
	m_ScrollBars[EConfig::MaxFrame]->setScrollRate(0.2f);
	m_ScrollBars[EConfig::GameSpeed]->setScrollRate(0.5f);
	m_ScrollBars[EConfig::Volume]->setScrollRate(0.5f);
}

void Settings::CloseConfig()
{
	setEnable(false);
}

bool Settings::Release() noexcept
{
	MyUI::Release();
	return true;
}

MyActor* Settings::clone()
{
	return nullptr;
}