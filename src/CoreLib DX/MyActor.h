#pragma once
#include "WriteManager.h"
//#include "MyRect.h"
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

class MyVertex
{
public:
	MyVertex() = default;
	~MyVertex() = default;
public:
	MyVector3 pos;		// 위치 좌표
	MyVector4 col;		// 컬러값
	MyVector2 tex;		// 텍스쳐 좌표
};


class MyActor : public MyRect, public IDxBasis
{
protected:
	MyVector4			m_proCoord;			// 투영 좌표계
	vector<MyVertex>	m_vertexList;		// 정점 배열
	ID3D11Buffer*		m_pVertexBuffer;	// 정점 버퍼

	ID3D11InputLayout*	m_pInputLayout;		// 인풋 레이아웃
	ID3D11VertexShader*	m_pVShader;			// 정점 쉐이더
	ID3D11PixelShader*	m_pPShader;			// 정점 쉐이더

	list<Sprite>*		m_pSpriteList;		// 스프라이트(좌표, 프레임) 리스트
	float				m_frameCount;		// 스프라이트 프레임 계산
	//MyVector2			m_texRate;			// * 텍스처 좌표, 잉여
	EDirection			m_curDirection;		// 현재 방향
	bool				m_isEnable;	
public:
	list<Sprite>::iterator m_curSprite;		// 현재 스프라이트 이터레이터
	EObjType			m_objType;			// 객체 타입
	wstring				m_myName;			// 객체 이름
	int					m_damage;			// 데미지

	//MyActor*			m_TargetObject;		// 상호작용 객체
	static UINT Stride;	// 정점 버퍼 세팅용
	static UINT Offset;
protected:
	HRESULT CreateVertexBuffer(ID3D11Device* pDevice);
	MyActor();								// 초기화용
public:
	MyActor(const wstring_view& myName, const EObjType& objType,
			const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	~MyActor() = default;

	virtual MyActor* setSpriteList(list<Sprite>* spriteList);
	virtual MyActor* setSpriteList(const wstring_view& stateName);

	// 정점, 쉐이더 초기화
	MyActor* CreateVertexList(ID3D11Device* pDevice);
	void SetShaderLayout(const string_view& vertexName, const string_view& pixelName);
	void SetPixelShader(const string_view& pixelName);
	//void SetVetexShader(const string_view& vertexName);
	// 정점 갱신, 설정
	virtual void UpdateTextureUV();
	virtual void UpdateVertexList(ID3D11DeviceContext* pDContext, const bool& useListUpdate = false);
	void SetColor(const float& red, const float& green, const float& blue);
	void SetAlpha(const float& alpha);
	virtual void setDirection(const EDirection& dir);
	// 활성화
	virtual void setEnable(const bool& isEnable, const bool& putDisable = false);
	virtual bool isEnable();
	virtual void EndSpriteEvent();
	virtual void DeadEvent(MyActor* target);
	virtual MyActor* clone();

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool PrevRender(ID3D11DeviceContext* pDContext)		noexcept;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool PostRender(ID3D11DeviceContext* pDContext)		noexcept;
	virtual bool Release()										noexcept override;
};