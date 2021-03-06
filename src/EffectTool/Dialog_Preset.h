
#pragma once

// Dialog_Preset 대화 상자
class GameObject;

class Dialog_Preset : public CFormView
{
public:
	static Dialog_Preset* Instance;
	CListBox m_ListBox;

	static Dialog_Preset* CreateOne(CWnd* pParent);
	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Preset };
#endif

	void LoadFromFile(const filesystem::path fpath);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedCreate();
public:
	DECLARE_DYNAMIC(Dialog_Preset)
	Dialog_Preset();   // 표준 생성자입니다.
	virtual ~Dialog_Preset();
};
