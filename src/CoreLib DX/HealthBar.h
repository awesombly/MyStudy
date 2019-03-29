#pragma once
#include "MyUI.h"
#include "ISingleton.h"

class HealthBar : public MyUI//, public ISingleton<HealthBar>
{
protected:
	list<Sprite>::iterator	m_BackSprite;		// �� �ؽ���
	vector<MyVertex>		m_BackVertexList;	// �ĸ� ����
	ID3D11Buffer*			m_pBackBuffer;		// �� ����

	list<Sprite>::iterator	m_FrontSprite;		// ���� �ؽ���
	ID3D11Buffer*			m_pFrontBuffer;		// ���� ����
	vector<MyVertex>		m_FrontVertexList;	// ���� ����
	float	m_dirHealthRate;
	float	m_curHealthRate;

	ID3D11DeviceContext** m_ppDContext;			// ���޿�
public:
	static HealthBar* Instance;
private:
	//friend class ISingleton<HealthBar>;
public:
	using MyUI::MyUI;
	~HealthBar() = default;

	void setHealthRate(const float& healthRate);
	virtual void MouseDragEvent() override;
	virtual void UpdateInterBuffer();
	virtual MyActor* clone() override;

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
};