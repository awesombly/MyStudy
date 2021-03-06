#pragma once


// Dialog_Inspector 대화 상자

class Dialog_Inspector : public CFormView
{
private:
	// Name, SRT
	CEdit m_ObjectName;
	CEdit m_EditPos[3];
	CEdit m_EditRot[3];
	CEdit m_EditScale[3];
	// CheckBox
	CButton m_isEnable;
	CButton m_isBillBoard;
	CButton m_isStatic;


	// Component
	CListBox m_Components;
	// Preset
	CListBox m_CompPreset;
	CButton  m_CompDelete;
	CButton  m_CompAdd;

	CString m_tempString;
	bool m_dontUpdate = false;
public:
	static Dialog_Inspector* Instance;
	static GameObject* SelectObject;
	static Component*  SelectComponent;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Inspector };
#endif
	static Dialog_Inspector* Dialog_Inspector::CreateOne(CWnd* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	bool ErrorCheck()	   noexcept;
	void AddComponentNameType(wstring& pName, const Component* pComp) noexcept;
public:
	void UpdateInspector() noexcept;
	void UpdateComponents() noexcept;
	void UpdateObjectData(GameObject* pObject) noexcept;
	void ClearInspector()  noexcept;
	void AddCompPreset(Component* pComp) noexcept;
public:
	afx_msg void OnEnChangePosx();
	afx_msg void OnEnChangePosy();
	afx_msg void OnEnChangePosz();
	afx_msg void OnEnChangeRotx();
	afx_msg void OnEnChangeRoty();
	afx_msg void OnEnChangeRotz();
	afx_msg void OnEnChangeScalex();
	afx_msg void OnEnChangeScaley();
	afx_msg void OnEnChangeScalez();
	afx_msg void OnEnChangeObjectname();
	afx_msg void OnBnClickedisenable();
	afx_msg void OnBnClickedisbillboard();
	afx_msg void OnBnClickedisstatic();

	afx_msg void OnLbnSelchangeInspectorComponents();
	afx_msg void OnBnClickedInspectorCompdelete();
	afx_msg void OnBnClickedInspectorCompadd();

	DECLARE_DYNAMIC(Dialog_Inspector)
	Dialog_Inspector();   // 표준 생성자입니다.
	virtual ~Dialog_Inspector();
	afx_msg void OnBnClickedInspectorSetproto();
};
