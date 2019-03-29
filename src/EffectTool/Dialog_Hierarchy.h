#pragma once



class Dialog_Hierarchy : public CFormView
{
public:
	static Dialog_Hierarchy* Instance;

	CButton   m_DuplicateButton;
	CButton   m_RemoveButton;
	CTreeCtrl m_TreeList;

	HTREEITEM m_ClickItem = nullptr;
	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Hierarchy };
#endif
	static Dialog_Hierarchy* CreateOne(CWnd* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()
public:
	void AddTreeItem(GameObject* pObject);	// �׸� �߰�
	GameObject* GetSelectObject();			// ���� �׸� ��ü ��ȯ
protected:
	void AddTreeNode(GameObject* pObject, const HTREEITEM& parent, const int& level);
	// ��������
	void ExportObject(const filesystem::path fpath, GameObject* pObject);
	void ExportChild(ofstream& outStream, GameObject* pObject);

	afx_msg void OnBnClickedDuplicate();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnTvnSelchangedTreetlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTvnBegindragTreelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedComposite();

	DECLARE_DYNAMIC(Dialog_Hierarchy)
	Dialog_Hierarchy();   // ǥ�� �������Դϴ�.
	virtual ~Dialog_Hierarchy();

	afx_msg void OnBnClickedExport();
public:
	afx_msg void OnBnClickedSetproto();
};