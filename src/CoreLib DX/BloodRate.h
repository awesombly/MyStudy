#pragma once
#include "MyUI.h"
#include "ISingleton.h"

class BloodRate : public MyUI
{
protected:
	list<Sprite>::iterator	m_BackSprite;		// �� �ؽ���
	vector<MyVertex>		m_BackVertexList;	// �ĸ� ����
	ID3D11Buffer*			m_pBackBuffer;		// �� ����

	//list<Sprite>::iterator	m_FrontSprite;		// ���� �ؽ���
	//ID3D11Buffer*			m_pFrontBuffer;		// ���� ����
	//vector<MyVertex>		m_FrontVertexList;	// ���� ����
	//float	m_dirHealthRate;
	float	m_curHealthRate;

	ID3D11DeviceContext** m_ppDContext;			// ���޿�
public:
	static BloodRate* Instance;
private:
	BloodRate() = default;
public:
	BloodRate(const wstring_view& myName, const EObjType& objType,
			  const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	~BloodRate() = default;

	void setHealthRate(const float& healthRate);
	bool operHealthRate(const float& value);
	virtual void MouseDragEvent() override;
	virtual void UpdateInterBuffer();
	virtual MyActor* clone() override;

	virtual bool Init(ID3D11DeviceContext* pDContext)			noexcept;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};