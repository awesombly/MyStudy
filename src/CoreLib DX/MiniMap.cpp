#include "MiniMap.h"
#include "ObjectManager.h"
#include "MyPlayer.h"


MiniMap::MiniMap(const wstring_view& myName, const EObjType& objType,
				 const string_view& vertexShaderName, const string_view& pixelShaderName) :
	MyUI::MyUI(std::forward<const wstring_view>(myName), objType,
			   std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName))
{}

bool MiniMap::Init() noexcept
{
	MyUI::Init();
	m_ObjVertexList = m_vertexList;
	CreateVertexBuffer(DxManager::m_pD3dDevice, &m_pObjBuffer, &m_ObjVertexList);

	SetColor(1.0f, 1.0f, 0.8f, &m_vertexList);
	return true;
}

bool MiniMap::Frame(const float& spf, const float& accTime) noexcept
{
	if (Input::Instance->getKeyState('Q') == EKeyState::DOWN)
		setEnable(!m_isEnable);
	if (!MyUI::Frame(spf, accTime))	return false;
	return true;
}

bool MiniMap::Render(ID3D11DeviceContext* pDContext) noexcept
{
 	if (!m_isEnable)	return false;
	RenderAction(pDContext);

	pDContext->VSSetShader(m_pVShader, nullptr, 0);
	pDContext->PSSetShader(m_pPShader, nullptr, 0);
	pDContext->IASetInputLayout(m_pInputLayout);
	pDContext->PSSetShaderResources(0, 0, nullptr);
	PostRender(pDContext);

	pDContext->IASetVertexBuffers(0, 1, &m_pObjBuffer, &Stride, &Offset);
	// 지형, 장애물
	SetColor(0.2f, 0.2f, 0.2f, &m_ObjVertexList);
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Wall])
	{
		float drawWidth = ((float)iter->getWidth() *
			(1 - (float)this->getHeight() / this->getWidth() /
			 ObjectManager::MapRect->getHeight() / ObjectManager::MapRect->getWidth()))
			/ ObjectManager::MapRect->getWidth();
		m_ObjVector4 = { (float)iter->getX() / ObjectManager::MapRect->getWidth(),
			(float)iter->getY() / ObjectManager::MapRect->getHeight(),
			(float)iter->getX() / ObjectManager::MapRect->getWidth() + drawWidth,
			(float)iter->getBottom() / ObjectManager::MapRect->getHeight() };
		m_ObjVector4 = { m_ObjVector4.x	* this->getWidth() + this->getX(),
			m_ObjVector4.y	* this->getHeight() + this->getY(),
			m_ObjVector4.z	* this->getWidth() + this->getX(),
			m_ObjVector4.w * this->getHeight() + this->getY() };

		UpdateVertexList(pDContext, &m_pObjBuffer, &m_ObjVertexList, m_ObjVector4, true);
		pDContext->Draw(6, 0);
	}
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Collider])
	{
		if (!iter->isEnable())
			continue;
		float drawWidth = ((float)iter->getWidth() *
			(1 - (float)this->getHeight() / this->getWidth() /
			 ObjectManager::MapRect->getHeight() / ObjectManager::MapRect->getWidth()))
			/ ObjectManager::MapRect->getWidth();
		m_ObjVector4 = { (float)iter->getX() / ObjectManager::MapRect->getWidth(),
			(float)iter->getY() / ObjectManager::MapRect->getHeight(),
			(float)iter->getX() / ObjectManager::MapRect->getWidth() + drawWidth,
			(float)iter->getBottom() / ObjectManager::MapRect->getHeight() };
		m_ObjVector4 = { m_ObjVector4.x	* this->getWidth() + this->getX(),
			m_ObjVector4.y	* this->getHeight() + this->getY(),
			m_ObjVector4.z	* this->getWidth() + this->getX(),
			m_ObjVector4.w * this->getHeight() + this->getY() };

		UpdateVertexList(pDContext, &m_pObjBuffer, &m_ObjVertexList, m_ObjVector4, true);
		//pDContext->IASetVertexBuffers(0, 1, &m_pObjBuffer, &Stride, &Offset);
		pDContext->Draw(6, 0);
	}
	// 적
	SetColor(1.0f, 0.5f, 0.5f, &m_ObjVertexList);
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Enemy])
	{
		if (!iter->isEnable())
			continue;
		static float drawWidth = ((float)iter->getWidth() *
									(1 -		(float)this->getHeight() / this->getWidth() / 
									ObjectManager::MapRect->getHeight() / ObjectManager::MapRect->getWidth()))
									/ ObjectManager::MapRect->getWidth() * 0.75f;
		static float drawHeight = (float)iter->getHeight() / ObjectManager::MapRect->getHeight() * 0.75f;
		m_ObjVector4 = { (float)iter->getX() / ObjectManager::MapRect->getWidth(),
						 (float)iter->getY() / ObjectManager::MapRect->getHeight(), 
						 (float)iter->getX() / ObjectManager::MapRect->getWidth()  + drawWidth,
						 (float)iter->getY() / ObjectManager::MapRect->getHeight() + drawHeight};
		m_ObjVector4 = { m_ObjVector4.x	* this->getWidth()	+ this->getX(),
						 m_ObjVector4.y	* this->getHeight() + this->getY(),
						 m_ObjVector4.z	* this->getWidth()	+ this->getX(),
						 m_ObjVector4.w * this->getHeight() + this->getY()};

		UpdateVertexList(pDContext, &m_pObjBuffer, &m_ObjVertexList, m_ObjVector4, true);
		pDContext->Draw(6, 0);
	}
	// 템
	SetColor(0.4f, 0.4f, 1.0f, &m_ObjVertexList);
	for (auto& iter : ObjectManager::Instance->m_ObjectList[EObjType::Item])
	{
		if (!iter->isEnable())
			continue;
		static float drawWidth = ((float)iter->getWidth() *
			(1 - (float)this->getHeight() / this->getWidth() /
			 ObjectManager::MapRect->getHeight() / ObjectManager::MapRect->getWidth()))
			/ ObjectManager::MapRect->getWidth();
		static float drawHeight = (float)iter->getHeight() / ObjectManager::MapRect->getHeight();
		m_ObjVector4 = { (float)iter->getX() / ObjectManager::MapRect->getWidth(),
			(float)iter->getY() / ObjectManager::MapRect->getHeight(),
			(float)iter->getX() / ObjectManager::MapRect->getWidth() + drawWidth,
			(float)iter->getY() / ObjectManager::MapRect->getHeight() + drawHeight };
		m_ObjVector4 = { m_ObjVector4.x	* this->getWidth() + this->getX(),
			m_ObjVector4.y	* this->getHeight() + this->getY(),
			m_ObjVector4.z	* this->getWidth() + this->getX(),
			m_ObjVector4.w * this->getHeight() + this->getY() };

		UpdateVertexList(pDContext, &m_pObjBuffer, &m_ObjVertexList, m_ObjVector4, true);
		pDContext->Draw(6, 0);
	}
	// 플레이어
	SetColor(0.2f, 1.0f, 0.5f, &m_ObjVertexList);
	const MyRect* playerRect = MyPlayer::Instance;
	static float drawWidth = ((float)playerRect->getWidth() *
		(1 - (float)this->getHeight() / this->getWidth() /
		 ObjectManager::MapRect->getHeight() / ObjectManager::MapRect->getWidth()))
		/ ObjectManager::MapRect->getWidth();
	m_ObjVector4 = { (float)playerRect->getX() / ObjectManager::MapRect->getWidth(),
					 (float)playerRect->getY() / ObjectManager::MapRect->getHeight(),
					 (float)playerRect->getX() / ObjectManager::MapRect->getWidth() + drawWidth,
					 (float)playerRect->getBottom() / ObjectManager::MapRect->getHeight() };
	m_ObjVector4 = { m_ObjVector4.x	* this->getWidth() + this->getX(),
		m_ObjVector4.y	* this->getHeight() + this->getY(),
		m_ObjVector4.z	* this->getWidth() + this->getX(),
		m_ObjVector4.w * this->getHeight() + this->getY() };

	UpdateVertexList(pDContext, &m_pObjBuffer, &m_ObjVertexList, m_ObjVector4, true);
	pDContext->Draw(6, 0);

	return true;
}

void MiniMap::MouseOverEvent()
{
}

void MiniMap::MouseDownEvent()
{
}

void MiniMap::MouseDragEvent()
{
	m_point = { Input::Instance->getCursor() - posToClick };
}

void MiniMap::MouseUpEvent()
{
}

void MiniMap::MouseFreeEvent()
{
}


bool MiniMap::Release() noexcept
{
	MyUI::Release();
	return true;
}

MyActor* MiniMap::clone()
{
	return (MyActor*)new MiniMap(*this);
}