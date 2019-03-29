#include "stdafx.h"
#include "EffectTool.h"
#include "Pane_Hierarchy.h"


IMPLEMENT_DYNAMIC(Pane_Hierarchy, CDockablePane)

Pane_Hierarchy::Pane_Hierarchy()
{}


Pane_Hierarchy::~Pane_Hierarchy()
{}

BEGIN_MESSAGE_MAP(Pane_Hierarchy, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// MapPane �޽��� ó����

int Pane_Hierarchy::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->m_wndForm = Dialog_Hierarchy::CreateOne(this);

	return 0;
}

void Pane_Hierarchy::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndForm)
	{
		// �ʺ�, ���� �缳��, z�� ����
		m_wndForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}

int Pane_Hierarchy::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
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

