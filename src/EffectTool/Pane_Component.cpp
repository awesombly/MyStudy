#include "stdafx.h"
#include "EffectTool.h"
#include "Pane_Component.h"


IMPLEMENT_DYNAMIC(Pane_Component, CDockablePane)
Pane_Component* Pane_Component::Instance = nullptr;

Pane_Component::Pane_Component()
{
	Instance = this;
}


Pane_Component::~Pane_Component()
{}

BEGIN_MESSAGE_MAP(Pane_Component, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// MapPane �޽��� ó����

int Pane_Component::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->m_wndNull			= Dialog_Null::CreateOne(this);
	this->m_wndCRenderer	= Dialog_CRenderer::CreateOne(this);
	this->m_wndCAnimation	= Dialog_CAnimation::CreateOne(this);
	this->m_wndCTransformer = Dialog_CTransformer::CreateOne(this);
	this->m_wndCCollder		= Dialog_CCollider::CreateOne(this);

	return 0;
}

void Pane_Component::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndNull)
	{
		// �ʺ�, ���� �缳��, z�� ����
		m_wndNull->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		m_wndNull->ShowWindow(SW_HIDE);
	}
	if (m_wndCRenderer)
	{
		// �ʺ�, ���� �缳��, z�� ����
		m_wndCRenderer->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		m_wndCRenderer->ShowWindow(SW_HIDE);
	}
	if (m_wndCAnimation)
	{
		// �ʺ�, ���� �缳��, z�� ����
		m_wndCAnimation->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		m_wndCAnimation->ShowWindow(SW_HIDE);
	}
	if (m_wndCTransformer)
	{
		// �ʺ�, ���� �缳��, z�� ����
		m_wndCTransformer->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		m_wndCTransformer->ShowWindow(SW_HIDE);
	}
	if (m_wndCCollder)
	{
		// �ʺ�, ���� �缳��, z�� ����
		m_wndCCollder->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		m_wndCCollder->ShowWindow(SW_HIDE);
	}
}

int Pane_Component::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();

	// �θ� desk�̰ų� desk �ڽ��� �̳��϶���
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}

// ������Ʈ Ÿ�Ժ� ����
void Pane_Component::SetCompDialog(const Component* pComp) noexcept
{
	try {
		m_wndCRenderer->ShowWindow(SW_HIDE);
		m_wndCAnimation->ShowWindow(SW_HIDE);
		m_wndCTransformer->ShowWindow(SW_HIDE);
		m_wndCCollder->ShowWindow(SW_HIDE);
		m_wndNull->ShowWindow(SW_HIDE);
		if (pComp == nullptr)
		{
			m_wndNull->ShowWindow(SW_SHOW);
			return;
		}

		bool isNullComp = true;
		switch (pComp->m_comptType)
		{
		case EComponent::Renderer:
		{
			m_wndCRenderer->ShowWindow(SW_SHOW);
			m_wndCRenderer->UpdateEdit(pComp);
			isNullComp = false;
		}	break;
		case EComponent::Animation:
		{
			m_wndCAnimation->ShowWindow(SW_SHOW);
			isNullComp = false;
		}	break;
		case EComponent::Transformer:
		{
			m_wndCTransformer->ShowWindow(SW_SHOW);
			m_wndCTransformer->UpdateEdit(pComp);
			isNullComp = false;
		}	break;
		case EComponent::Collider:
		{
			m_wndCCollder->ShowWindow(SW_SHOW);
			m_wndCCollder->UpdateEdit(pComp);
			isNullComp = false;
		}	break;
		}

		if (isNullComp)
		{
			m_wndNull->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		MessageBox(L"ó���� ���ܰ� �߻� �Ͽ����ϴ�.", L"�ߺ�-");
	}
}