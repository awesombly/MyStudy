#pragma once
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include "MyUI.h"


class ChattingBox : public MyUI
{
protected:
	const int m_textSpace = 25;
public:
	static ChattingBox*  Instance;
	static list<wstring> m_msgList;
	static wstring		 m_inputMsg;
public:
	// 상호작용시 발생 이벤트
	virtual void MouseOverEvent() override;
	virtual void MouseDownEvent() override;
	virtual void MouseDragEvent() override;
	virtual void MouseUpEvent()   override;
	virtual void MouseFreeEvent() override;

	MyActor* clone();
	virtual bool Init() noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;

	//ChattingBox();
	using MyUI::MyUI;
	virtual ~ChattingBox() = default;
};