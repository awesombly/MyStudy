#pragma once
#include "MyUI.h"
#include "ISingleton.h"

class BloodRate : public MyUI
{
protected:
	list<Sprite>::iterator	m_BackSprite;		// 백 텍스쳐
	vector<Vertex_PNCT>		m_BackVertexList;	// 후면 정점
	ID3D11Buffer*			m_pBackBuffer;		// 백 버퍼

	//list<Sprite>::iterator	m_FrontSprite;		// 전면 텍스쳐
	//ID3D11Buffer*			m_pFrontBuffer;		// 전면 버퍼
	//vector<Vertex_PNCT>		m_FrontVertexList;	// 전면 정점
	//float	m_dirHealthRate;
	float	m_curHealthRate;

	ID3D11DeviceContext** m_ppDContext;			// 전달용
public:
	static BloodRate* Instance;
public:
	using MyUI::MyUI;
	~BloodRate() = default;

	void setHealthRate(const float& healthRate);
	bool operHealthRate(const float& value);
	virtual void MouseDragEvent() override;
	virtual void UpdateInterBuffer();
	virtual MyActor* clone() override;

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};