#include "MyActor.h"
#include "ObjectManager.h"

UINT MyActor::Stride = sizeof(MyVertex);
UINT MyActor::Offset = 0;

MyActor::MyActor() : m_pInputLayout(nullptr), m_pPShader(nullptr), m_pVShader(nullptr),
					m_isEnable(true), m_pVertexBuffer(nullptr)
{}

MyActor::MyActor(const wstring_view& myName, const EObjType& objType,
				 const string_view& vertexShaderName, const string_view& pixelShaderName) : MyActor()
{
	m_myName = std::forward<const wstring_view>(myName);
	m_objType = objType;
	SetShaderLayout(vertexShaderName, pixelShaderName);
	m_vertexList.resize(6);
	setSpriteList(L"basic");
	setWH((int)m_curSprite->m_texVector4.z, (int)m_curSprite->m_texVector4.w);
	CreateVertexList(DxManager::m_pD3dDevice);
}

bool MyActor::Init() noexcept
{
	m_curDirection = EDirection::Left;
	//m_pInputLayout = nullptr;
	//m_pPShader = nullptr;
	//m_pVShader = nullptr;
	//m_isEnable = true;
	//m_pVertexBuffer = nullptr;
	return true;
}


bool MyActor::Frame(const float& spf, const float& accTime) noexcept
{
	if (!m_isEnable) return false;

	// 스프라이트 프레임 계산
	if (m_curSprite->m_frame != 0.0f)
	{
		m_frameCount += spf;
		if (m_frameCount >= m_curSprite->m_frame)
		{
			m_frameCount = 0.0f;
			if (++m_curSprite == m_pSpriteList->end())
				EndSpriteEvent();
			UpdateTextureUV();
		}
	}

	return true;
	accTime;
}

void MyActor::EndSpriteEvent()
{
	m_curSprite = m_pSpriteList->begin();
}

void MyActor::DeadEvent(MyActor* target)
{
	target;
	//setEnable(false, true);
}

bool MyActor::PrevRender(ID3D11DeviceContext* pDContext) noexcept
{
	// 오브젝트 쉐이더, 버퍼, 리소스 설정 -> 드로우
	pDContext->VSSetShader(m_pVShader, nullptr, 0);
	pDContext->PSSetShader(m_pPShader, nullptr, 0);

	// 리소스를 픽셀 쉐이더에 바인딩(시작 인덱스, 쉐이더 자원 갯수, 리소스뷰)
	pDContext->IASetInputLayout(m_pInputLayout);
	m_curSprite->m_pTexture->SetShaderResource(pDContext);
	return true;
}

bool MyActor::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable) return false;
	PrevRender(pDContext);
	PostRender(pDContext);
	return true;
}

bool MyActor::PostRender(ID3D11DeviceContext* pDContext) noexcept
{
	UpdateVertexList(pDContext, true);
	pDContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	pDContext->Draw(6, 0);
	return true;
}

bool MyActor::Release() noexcept
{
	return true;
}

MyActor* MyActor::setSpriteList(list<Sprite>* spriteList)
{
	m_pSpriteList = spriteList;
	m_curSprite = m_pSpriteList->begin();
	UpdateTextureUV();
	return this;
}

MyActor* MyActor::setSpriteList(const wstring_view& stateName)
{
	m_pSpriteList = ObjectManager::Instance->getSpriteList(m_myName, stateName);
	m_curSprite = m_pSpriteList->begin();
	UpdateTextureUV();
	return this;
}


// 쉐이더, 레이아웃 맵에서 반환
void MyActor::SetShaderLayout(const string_view& vertexName, const string_view& pixelName)
{
	m_pPShader	   = DxManager::Instance->m_PShaderList[pixelName.data()];
	m_pVShader	   = DxManager::Instance->m_VShaderList[vertexName.data()];
	m_pInputLayout = DxManager::Instance->m_LayoutList[vertexName.data()];
}

void MyActor::SetPixelShader(const string_view& pixelName)
{
	m_pPShader = DxManager::Instance->m_PShaderList[pixelName.data()];
}

void MyActor::UpdateTextureUV()
{
	static MyVector4 posVector;
	posVector = m_curSprite->m_texVector4.getPosVector(m_curSprite->m_pTexture->getTextureWidth(), m_curSprite->m_pTexture->getTextureHeight());
	m_vertexList[0].tex = { posVector.x, posVector.y };
	m_vertexList[1].tex = { posVector.z, posVector.y };
	m_vertexList[2].tex = { posVector.x, posVector.w };
	m_vertexList[3].tex = m_vertexList[2].tex;
	m_vertexList[4].tex = m_vertexList[1].tex;
	m_vertexList[5].tex = { posVector.z, posVector.w };
}

void MyActor::UpdateVertexList(ID3D11DeviceContext* pDContext, const bool& useListUpdate)
{
	if (useListUpdate)
	{
		m_proCoord.x =   (getX()	  - Window::DrawPoint.x) / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.y = -((getY()	  - Window::DrawPoint.y) / Window::getClientRect().bottom * 2.0f - 1);
		m_proCoord.z =   (getRight()  - Window::DrawPoint.x) / Window::getClientRect().right  * 2.0f - 1;
		m_proCoord.w = -((getBottom() - Window::DrawPoint.y) / Window::getClientRect().bottom * 2.0f - 1);
		
		m_vertexList[0].pos = { m_proCoord.x, m_proCoord.y, 0.0f };
		m_vertexList[1].pos = { m_proCoord.z, m_proCoord.y, 0.0f };
		m_vertexList[2].pos = { m_proCoord.x, m_proCoord.w, 0.0f };
		m_vertexList[3] = m_vertexList[2];
		m_vertexList[4] = m_vertexList[1];
		m_vertexList[5].pos = { m_proCoord.z, m_proCoord.w, 0.0f };
	}
	pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}

// 정점 설정 및 버퍼 생성
MyActor* MyActor::CreateVertexList(ID3D11Device* pDevice)
{
	m_proCoord.x =   (getX()	  - Window::DrawPoint.x) / Window::getClientRect().right  * 2.0f - 1;
	m_proCoord.y = -((getY()	  - Window::DrawPoint.y) / Window::getClientRect().bottom * 2.0f - 1);
	m_proCoord.z =   (getRight()  - Window::DrawPoint.x) / Window::getClientRect().right  * 2.0f - 1;
	m_proCoord.w = -((getBottom() - Window::DrawPoint.y) / Window::getClientRect().bottom * 2.0f - 1);

	m_vertexList[0].pos = { m_proCoord.x, m_proCoord.y, 0.0f };
	m_vertexList[0].col = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_vertexList[1].pos = { m_proCoord.z, m_proCoord.y, 0.0f };
	m_vertexList[1].col = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_vertexList[2].pos = { m_proCoord.x, m_proCoord.w, 0.0f };
	m_vertexList[2].col = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_vertexList[3] = m_vertexList[2];
	m_vertexList[4] = m_vertexList[1];

	m_vertexList[5].pos = { m_proCoord.z, m_proCoord.w, 0.0f };
	m_vertexList[5].col = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (FAILED(CreateVertexBuffer(pDevice)))
		return nullptr;
	return this;
}

// 정점에 대한 정보를 GPU에 전달
HRESULT MyActor::CreateVertexBuffer(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;				// 사용 모드(CPU->GPU 접근제어)
	bufferDesc.ByteWidth = (UINT)(m_vertexList.size() * sizeof(MyVertex));	// 정점크기 * 갯수
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 버퍼 타입
	bufferDesc.CPUAccessFlags = 0;						// CPU 접근 제어
	//bufferDesc.MiscFlags = 0;							
	//bufferDesc.StructureByteStride = 0;				
	//InitData.SysMemPitch = 0;
	//InitData.SysMemSlicePitch = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &m_vertexList.at(0);		// GPU에 전달 할 메모리

	if (m_pVertexBuffer != nullptr)
		m_pVertexBuffer->Release();
	//					버퍼 생성(버퍼정보,	   소스 정보, 대상 버퍼)
	return pDevice->CreateBuffer(&bufferDesc, &InitData, &m_pVertexBuffer);
}


void MyActor::SetColor(const float& red, const float& green, const float& blue)
{
	for (auto& iter : m_vertexList)
	{
		iter.col.x = red;
		iter.col.y = green;
		iter.col.z = blue;
	}
}

void MyActor::SetAlpha(const float& alpha)
{
	for (auto& iter : m_vertexList)
	{
		iter.col.w = alpha;
	}
}

void MyActor::setDirection(const EDirection& dir)
{
	m_curDirection = dir;
}

void MyActor::setEnable(const bool& isEnable, const bool& putDisable)
{
	if (putDisable)
		ObjectManager::Instance->DisableObject(this);
	else
		m_isEnable = isEnable;
}

bool MyActor::isEnable()
{
	return m_isEnable;
}

MyActor* MyActor::clone()
{
	return new MyActor(*this);
}