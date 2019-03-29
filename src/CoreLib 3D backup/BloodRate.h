#pragma once
#include "MyUI.h"
#include "ISingleton.h"

class BloodRate : public MyUI
{
protected:
	list<Sprite>::iterator	m_BackSprite;		// �� �ؽ���
	vector<Vertex_PNCT>		m_BackVertexList;	// �ĸ� ����
	ID3D11Buffer*			m_pBackBuffer;		// �� ����

	//list<Sprite>::iterator	m_FrontSprite;		// ���� �ؽ���
	//ID3D11Buffer*			m_pFrontBuffer;		// ���� ����
	//vector<Vertex_PNCT>		m_FrontVertexList;	// ���� ����
	//float	m_dirHealthRate;
	float	m_curHealthRate;

	ID3D11DeviceContext** m_ppDContext;			// ���޿�
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