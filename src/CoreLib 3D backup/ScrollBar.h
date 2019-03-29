#pragma once
#include "MyUI.h"


// 스크롤바 이름, 스크롤 수치 표시
class ScrollBar : public MyUI
{
public:
	void (ScrollBar::*ClickEvent)();
	wstring m_buttonText;

	list<Sprite>::iterator m_ScrollSprite;
	ID3D11Buffer*		   m_ScrollBuffer;
	vector<Vertex_PNCT>	   m_ScrollVertex;
	MyRect				   m_ScrollRect;
	
	float m_curScrollRate;
public:
	using MyUI::MyUI;
	virtual ~ScrollBar() = default;

	virtual void setScrollRate(const float& rate);
	virtual void UpdateMaxFrame();
	virtual void UpdateGameSpeed();
	virtual void UpdateSoundVolume();
	// 스크롤바 조절 이벤트
	virtual void setClickEvent(void(ScrollBar::*func)());
	virtual void setText(const wstring_view& string);
	virtual void MouseDragEvent();
	//virtual void MouseUpEvent();
	virtual void Move(const MyVector2& point) noexcept override;

	virtual MyActor* clone();
	virtual bool Init()		noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};