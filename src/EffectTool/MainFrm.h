
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once
#include "Dialog_Inspector.h"
#include "Dialog_Preset.h"
#include "Dialog_Hierarchy.h"
#include "Pane_Inspector.h"
#include "Pane_Preset.h"
#include "Pane_Hierarchy.h"
#include "Pane_Component.h"


class CMainFrame : public CFrameWndEx
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:
	CTabbedPane		m_Tab;
	Pane_Inspector	m_Inspector;
	Pane_Preset		m_Preset;
	Pane_Hierarchy	m_Hierarchy;
	Pane_Component	m_Control;

// 작업입니다.
public:
	void CreatePaneWindow(CDockablePane* pane, const TCHAR* title, const int& id);
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
};


