#pragma once
#include "MyActor.h"

enum class EKeyState : char;

enum class EItemType : char
{
	Etc = 0,
	Equip,
	Consum,
	All = 99
};


class MyUI : public MyActor
{
protected:
	static MyVector2 posToClick;			// Ŭ�� ������ ��ü ������ ��ǥ
	EKeyState		 m_curState;
public:
	static bool		 isDraging;
protected:
	// ��� ���� ����
	virtual HRESULT CreateVertexBuffer(ID3D11Device* pDevice, ID3D11Buffer** pVertexBuffer, vector<Vertex_PNCT>* pVertexList) const;	
	// UI ��ȣ�ۿ�
	virtual void FrameAction(const float& spf);
	virtual void RenderAction(ID3D11DeviceContext* pDContext);
	// ��ȣ�ۿ�� �߻� �̺�Ʈ
	virtual void MouseOverEvent();
	virtual void MouseDownEvent();
	virtual void MouseDragEvent();
	virtual void MouseUpEvent();
	virtual void MouseFreeEvent();

	virtual void UpdateVertexList(ID3D11DeviceContext* pDContext, const bool& useListUpdate = false) override;
	virtual void UpdateVertexList(ID3D11DeviceContext* pDContext, ID3D11Buffer** pVertexBuffer, vector<Vertex_PNCT>* pVertexList, const bool& useListUpdate = false);	// ��� ����
	virtual void UpdateVertexList(ID3D11DeviceContext* pDContext, ID3D11Buffer** pVertexBuffer, vector<Vertex_PNCT>* pVertexList, const MyRect& rect, const bool& useListUpdate = false);
	virtual void UpdateVertexList(ID3D11DeviceContext* pDContext, ID3D11Buffer** pVertexBuffer, vector<Vertex_PNCT>* pVertexList, const MyVector4& vector4, const bool& useListUpdate = false);
public:
	virtual void SetColor(const float& red, const float& green, const float& blue);
	virtual void SetColor(const float& red, const float& green, const float& blue, vector<Vertex_PNCT>* pVertexList);
	virtual MyActor* clone();

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
public:
	using MyActor::MyActor;
	virtual ~MyUI() = default;
};