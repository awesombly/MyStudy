#pragma once


// MapFormDialog 대화 상자

class MapFormDialog : public CFormView
{
	DECLARE_DYNAMIC(MapFormDialog)

public:
	MapFormDialog();   // 표준 생성자입니다.
	virtual ~MapFormDialog();

	static MapFormDialog* CreateOne(CWnd* pParent);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MapFormDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
