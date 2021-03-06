#pragma once

#include "MapFormDialog.h"
// MapPane

class MapPane : public CDockablePane
{
	DECLARE_DYNAMIC(MapPane)

public:
	MapPane();
	virtual ~MapPane();
	MapFormDialog* m_wndForm;
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


