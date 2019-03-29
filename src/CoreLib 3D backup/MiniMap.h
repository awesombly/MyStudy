#pragma once
#include "MyUI.h"

class MiniMap : public MyUI
{
protected:
	vector<Vertex_PNCT> m_ObjVertexList;
	ID3D11Buffer*	 m_pObjBuffer;
	MyVector4		 m_ObjVector4;
public:
	using MyUI::MyUI;
	virtual ~MiniMap() = default;

	//// UI 상호작용
	//virtual void FrameAction(const float& spf);
	//virtual void RenderAction(ID3D11DeviceContext* pDContext);
	//// 상호작용시 발생 이벤트
	virtual void MouseOverEvent() override;
	virtual void MouseDownEvent() override;
	virtual void MouseDragEvent() override;
	virtual void MouseUpEvent()   override;
	virtual void MouseFreeEvent() override;

	virtual MyActor* clone();

	virtual bool Init()		noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};

