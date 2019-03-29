#pragma once
#include "WriteManager.h"
#include "Sprite.h"
#include "IDxBasis.h"

enum class EDirection : bool {
	Left = 0,
	Right,
};

enum class EObjType : char {
	Player,
	Enemy,
	Effect,
	PlayerAttack,
	EnemyAttack,
	Wall,
	Collider,
	Image,
	UI,
	Item,
	Score,
};

//struct Vertex_PNCT
//{
//	MyVector3 pos;		// 위치 좌표
//	MyVector4 col;		// 컬러값
//	MyVector2 tex;		// 텍스쳐 좌표
//};
struct Vertex_PNCT
{
	D3DXVECTOR3 pos;		// 위치 좌표
	D3DXVECTOR3 nor;		// 노말 벡터
	D3DXVECTOR4 col;		// 컬러값
	D3DXVECTOR2 tex;		// 텍스쳐 좌표
};



class MyActor : public MyRect, public IDxBasis
{
protected:
	MyVector4			m_proCoord;					// 투영 좌표계
	vector<Vertex_PNCT>	m_vertexList;				// 정점 배열
	ID3D11Buffer*		m_pVertexBuffer = nullptr;	// 정점 버퍼
	ID3D11Buffer*		m_pIndexBuffer  = nullptr;	// 인덱스 버퍼
	ID3D11Buffer*		m_pConstBuffer  = nullptr;	// 상수 버퍼

	ID3D11InputLayout*	m_pInputLayout	= nullptr;	// 버텍스 레이아웃
	ID3D11VertexShader*	m_pVShader		= nullptr;	// 정점 쉐이더
	ID3D11PixelShader*	m_pPShader		= nullptr;	// 정점 쉐이더

	list<Sprite>*		m_pSpriteList	= nullptr;	// 스프라이트(좌표, 프레임) 리스트
	float				m_frameCount	= 0.0f;		// 스프라이트 프레임 계산
	EDirection			m_curDirection;				// 현재 방향
	bool				m_isEnable = true;			// 살아있냐

	// 정점 버퍼 세팅용
	static UINT Stride;	
	static UINT Offset;
public:
	list<Sprite>::iterator m_curSprite;		// 현재 스프라이트 이터레이터
	EObjType			m_objType;			// 객체 타입
	wstring				m_myName;			// 객체 이름
	int					m_damage;			// 데미지
protected:
	// 정점, 쉐이더 초기화
	MyActor* CreateVertexList(ID3D11Device* pDevice);
	HRESULT CreateVertexBuffer(ID3D11Device* pDevice);
	HRESULT CreateIndexBuffer(ID3D11Device* pDevice);
	HRESULT CreateConstBuffer(ID3D11Device* pDevice, const void* data, const UINT& dataSize);
	void SetShaderLayout(const string_view& vertexName, const string_view& pixelName);
	void SetPixelShader(const string_view& pixelName);
	virtual void UpdateConstBuffer(ID3D11DeviceContext* pDContext);
	virtual void UpdateVertexList(ID3D11DeviceContext* pDContext, const bool& useListUpdate = false);
public:
	virtual void UpdateTextureUV();
	virtual MyActor* setSpriteList(list<Sprite>* spriteList);
	virtual MyActor* setSpriteList(const wstring_view& stateName);
	// 잡잡함수
	virtual void EndSpriteEvent();
	virtual void DeadEvent(MyActor* target);
	virtual void setDirection(const EDirection& dir);
	virtual void setEnable(const bool& isEnable, const bool& putDisable = false);
	virtual bool isEnable();
	void SetColor(const float& red, const float& green, const float& blue);
	void SetAlpha(const float& alpha);
	virtual MyActor* clone();

	virtual bool PrevRender(ID3D11DeviceContext* pDContext)		noexcept;
	virtual bool PostRender(ID3D11DeviceContext* pDContext)		noexcept;
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	// 실질적 객체 생성자
	virtual bool Init(const wstring_view& myName, const EObjType& objType,
					  const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept;
protected:
	MyActor() = default;
	MyActor(MyActor&) = default;
	MyActor(MyActor&&) = default;
	MyActor& operator = (const MyActor&) = default;
	MyActor& operator = (MyActor&&) = default;
public:
	MyActor(const wstring_view& myName, const EObjType& objType,
			const string_view& vertexShaderName = "VS_Basic", const string_view& pixelShaderName = "PShader");
	~MyActor() = default;
};