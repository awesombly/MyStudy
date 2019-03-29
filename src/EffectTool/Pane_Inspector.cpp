#include "stdafx.h"
#include "EffectTool.h"
#include "Pane_Inspector.h"


IMPLEMENT_DYNAMIC(Pane_Inspector, CDockablePane)

Pane_Inspector::Pane_Inspector()
{}


Pane_Inspector::~Pane_Inspector()
{}

BEGIN_MESSAGE_MAP(Pane_Inspector, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// MapPane �޽��� ó����

int Pane_Inspector::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->m_wndForm = Dialog_Inspector::CreateOne(this);

	return 0;
}

void Pane_Inspector::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndForm)
	{
		// �ʺ�, ���� �缳��, z�� ����
		m_wndForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}

int Pane_Inspector::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
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

