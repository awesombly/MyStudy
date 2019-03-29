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
//	MyVector3 pos;		// ��ġ ��ǥ
//	MyVector4 col;		// �÷���
//	MyVector2 tex;		// �ؽ��� ��ǥ
//};
struct Vertex_PNCT
{
	D3DXVECTOR3 pos;		// ��ġ ��ǥ
	D3DXVECTOR3 nor;		// �븻 ����
	D3DXVECTOR4 col;		// �÷���
	D3DXVECTOR2 tex;		// �ؽ��� ��ǥ
};



class MyActor : public MyRect, public IDxBasis
{
protected:
	MyVector4			m_proCoord;					// ���� ��ǥ��
	vector<Vertex_PNCT>	m_vertexList;				// ���� �迭
	ID3D11Buffer*		m_pVertexBuffer = nullptr;	// ���� ����
	ID3D11Buffer*		m_pIndexBuffer  = nullptr;	// �ε��� ����
	ID3D11Buffer*		m_pConstBuffer  = nullptr;	// ��� ����

	ID3D11InputLayout*	m_pInputLayout	= nullptr;	// ���ؽ� ���̾ƿ�
	ID3D11VertexShader*	m_pVShader		= nullptr;	// ���� ���̴�
	ID3D11PixelShader*	m_pPShader		= nullptr;	// ���� ���̴�

	list<Sprite>*		m_pSpriteList	= nullptr;	// ��������Ʈ(��ǥ, ������) ����Ʈ
	float				m_frameCount	= 0.0f;		// ��������Ʈ ������ ���
	EDirection			m_curDirection;				// ���� ����
	bool				m_isEnable = true;			// ����ֳ�

	// ���� ���� ���ÿ�
	static UINT Stride;	
	static UINT Offset;
public:
	list<Sprite>::iterator m_curSprite;		// ���� ��������Ʈ ���ͷ�����
	EObjType			m_objType;			// ��ü Ÿ��
	wstring				m_myName;			// ��ü �̸�
	int					m_damage;			// ������
protected:
	// ����, ���̴� �ʱ�ȭ
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
	// �����Լ�
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
	// ������ ��ü ������
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