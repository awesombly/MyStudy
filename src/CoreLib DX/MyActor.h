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
	MyVector3 pos;		// ��ġ ��ǥ
	MyVector4 col;		// �÷���
	MyVector2 tex;		// �ؽ��� ��ǥ
};


class MyActor : public MyRect, public IDxBasis
{
protected:
	MyVector4			m_proCoord;			// ���� ��ǥ��
	vector<MyVertex>	m_vertexList;		// ���� �迭
	ID3D11Buffer*		m_pVertexBuffer;	// ���� ����

	ID3D11InputLayout*	m_pInputLayout;		// ��ǲ ���̾ƿ�
	ID3D11VertexShader*	m_pVShader;			// ���� ���̴�
	ID3D11PixelShader*	m_pPShader;			// ���� ���̴�

	list<Sprite>*		m_pSpriteList;		// ��������Ʈ(��ǥ, ������) ����Ʈ
	float				m_frameCount;		// ��������Ʈ ������ ���
	//MyVector2			m_texRate;			// * �ؽ�ó ��ǥ, �׿�
	EDirection			m_curDirection;		// ���� ����
	bool				m_isEnable;	
public:
	list<Sprite>::iterator m_curSprite;		// ���� ��������Ʈ ���ͷ�����
	EObjType			m_objType;			// ��ü Ÿ��
	wstring				m_myName;			// ��ü �̸�
	int					m_damage;			// ������

	//MyActor*			m_TargetObject;		// ��ȣ�ۿ� ��ü
	static UINT Stride;	// ���� ���� ���ÿ�
	static UINT Offset;
protected:
	HRESULT CreateVertexBuffer(ID3D11Device* pDevice);
	MyActor();								// �ʱ�ȭ��
public:
	MyActor(const wstring_view& myName, const EObjType& objType,
			const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	~MyActor() = default;

	virtual MyActor* setSpriteList(list<Sprite>* spriteList);
	virtual MyActor* setSpriteList(const wstring_view& stateName);

	// ����, ���̴� �ʱ�ȭ
	MyActor* CreateVertexList(ID3D11Device* pDevice);
	void SetShaderLayout(const string_view& vertexName, const string_view& pixelName);
	void SetPixelShader(const string_view& pixelName);
	//void SetVetexShader(const string_view& vertexName);
	// ���� ����, ����
	virtual void UpdateTextureUV();
	virtual void UpdateVertexList(ID3D11DeviceContext* pDContext, const bool& useListUpdate = false);
	void SetColor(const float& red, const float& green, const float& blue);
	void SetAlpha(const float& alpha);
	virtual void setDirection(const EDirection& dir);
	// Ȱ��ȭ
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