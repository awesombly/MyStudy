#include "MyActor.h"
#include "ObjectManager.h"

UINT MyActor::Stride = sizeof(Vertex_PNCT);
UINT MyActor::Offset = 0;


MyActor::MyActor(const wstring_view& myName, const EObjType& objType,
				 const string_view& vertexShaderName, const string_view& pixelShaderName)
{
	Init(std::forward<const wstring_view>(myName), objType,
		 std::forward<const string_view>(vertexShaderName), std::forward<const string_view>(pixelShaderName));
}

bool MyActor::Init(const wstring_view& myName, const EObjType& objType,
				   const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	m_myName = std::forward<const wstring_view>(myName);
	m_objType = objType;
	SetShaderLayout(std::forward<const string_view>(vertexShaderName),
					std::forward<const string_view>(pixelShaderName));
	m_vertexList.resize(4);
	setSpriteList(L"basic");
	setWH((int)m_curSprite->m_texVector4.z, (int)m_curSprite->m_texVector4.w);
	CreateVertexList(DxManager::GetDevice());
	CreateConstBuffer(DxManager::GetDevice(), &ObjectManager::MainCamera.m_VSCB, sizeof(VSCB));
	return true;
}


bool MyActor::Init() noexcept
{
	m_curDirection = EDirection::Left;
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

bool MyActor::PrevRender(ID3D11DeviceContext* pDContext) noexcept
{
	// 리소스를 픽셀 쉐이더에 바인딩(시작 인덱스, 쉐이더 자원 갯수, 리소스뷰)
	pDContext->IASetInputLayout(m_pInputLayout);
	// 오브젝트 쉐이더, 버퍼, 리소스 설정 -> 드로우
	UpdateConstBuffer(pDContext);
	pDContext->VSSetConstantBuffers(0, 1, &m_pConstBuffer);	// 상수 버퍼
	pDContext->VSSetShader(m_pVShader, nullptr, 0);			// 정점쉐이더
	pDContext->PSSetShader(m_pPShader, nullptr, 0);			// 픽셀 쉐이더
	pDContext->GSSetShader(nullptr, nullptr, 0);			// 기하 쉐이더	
	// 텍스쳐 리소스 적용 
	m_curSprite->m_pTexture->SetShaderResource(pDContext);
	return true;
}

bool MyActor::PostRender(ID3D11DeviceContext* pDContext) noexcept
{
	UpdateVertexList(pDContext, true);
	pDContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	pDContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pDContext->DrawIndexed(6, 0, 0);
	return true;
}

bool MyActor::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable) return false;
	PrevRender(pDContext);
	PostRender(pDContext);
	return true;
}

bool MyActor::Release() noexcept
{
	return true;
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
	//m_vertexList[3].tex = m_vertexList[2].tex;
	//m_vertexList[4].tex = m_vertexList[1].tex;
	m_vertexList[3].tex = { posVector.z, posVector.w };
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
		//m_vertexList[3] = m_vertexList[2];
		//m_vertexList[4] = m_vertexList[1];
		m_vertexList[3].pos = { m_proCoord.z, m_proCoord.w, 0.0f };
	}
	pDContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_vertexList.at(0), 0, 0);
}

void MyActor::UpdateConstBuffer(ID3D11DeviceContext* pDContext)
{
	// 상수버퍼를 갱신
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	MappedResource = { 0, };
	// 접근하고 수정하고 접근해제?
	pDContext->Map(m_pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	VSCB* pConstData = (VSCB*)MappedResource.pData;		// 바꿀 객체

	*pConstData = ObjectManager::MainCamera.m_VSCB;
	pDContext->Unmap(m_pConstBuffer, 0);
	//pDContext->UpdateSubresource(m_pConstBuffer, 0, nullptr, &ObjectManager::MainCamera.m_VSCB, 0, 0);
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

	//m_vertexList[3] = m_vertexList[2];
	//m_vertexList[4] = m_vertexList[1];

	m_vertexList[3].pos = { m_proCoord.z, m_proCoord.w, 0.0f };
	m_vertexList[3].col = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (FAILED(CreateVertexBuffer(pDevice)) ||
		FAILED(CreateIndexBuffer(pDevice)))
		return nullptr;
	return this;
}

// 정점에 대한 정보를 GPU에 전달
HRESULT MyActor::CreateVertexBuffer(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC initDesc = { 0, };
	initDesc.ByteWidth = (UINT)(m_vertexList.size() * sizeof(Vertex_PNCT));	// 정점크기 * 갯수
	initDesc.Usage = D3D11_USAGE_DEFAULT;				// 사용 모드(CPU->GPU 접근제어)
	initDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 버퍼 타입
	initDesc.CPUAccessFlags = 0;						// CPU 접근 제어
	initDesc.MiscFlags = 0;
	//initDesc.StructureByteStride = 0;
	//initData.SysMemPitch = 0;
	//initData.SysMemSlicePitch = 0;

	D3D11_SUBRESOURCE_DATA InitData = { 0, };
	InitData.pSysMem = &m_vertexList.at(0);		// GPU에 전달 할 메모리

	if (m_pVertexBuffer != nullptr)
		m_pVertexBuffer->Release();
	//					버퍼 생성(버퍼정보,	   소스 정보, 대상 버퍼)
	return pDevice->CreateBuffer(&initDesc, &InitData, &m_pVertexBuffer);
}

HRESULT MyActor::CreateIndexBuffer(ID3D11Device* pDevice)
{
	WORD indices[] = {
		0, 1, 2,
		2, 1, 3
	};

	UINT indexCnt = sizeof(indices) / sizeof(WORD);
		
	D3D11_BUFFER_DESC initDesc{ 0, };
	initDesc.ByteWidth = indexCnt * sizeof(WORD);			// 버퍼 사이즈
	initDesc.Usage = D3D11_USAGE_DEFAULT;
	initDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;			// 버퍼 타입
	initDesc.CPUAccessFlags = 0;
	initDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData { 0, };
	initData.pSysMem = indices;
	return pDevice->CreateBuffer(&initDesc, &initData, &m_pIndexBuffer);
}

HRESULT MyActor::CreateConstBuffer(ID3D11Device* pDevice, const void* data, const UINT& dataSize)
{
	D3D11_BUFFER_DESC initDesc = { 0, };
	initDesc.ByteWidth = dataSize;					// 버퍼 사이즈
	initDesc.Usage = D3D11_USAGE_DYNAMIC;				// 동적 리소스, GPU와 CPU를 통해 접근 가능
	initDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 버퍼 타입
	initDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	initDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0, };
	initData.pSysMem = data;
	
	return pDevice->CreateBuffer(&initDesc, &initData, &m_pConstBuffer);
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