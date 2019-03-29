// MapFormDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "0619 PainWnd2.h"
#include "MapFormDialog.h"
#include "afxdialogex.h"


// MapFormDialog 대화 상자

IMPLEMENT_DYNAMIC(MapFormDialog, CFormView)

MapFormDialog::MapFormDialog()
	: CFormView(IDD_MapFormDialog)
{

}

MapFormDialog::~MapFormDialog()
{
}

void MapFormDialog::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

MapFormDialog* MapFormDialog::CreateOne(CWnd* pParent)
{
	MapFormDialog* pForm = new MapFormDialog();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}


BEGIN_MESSAGE_MAP(MapFormDialog, CFormView)
END_MESSAGE_MAP()


// MapFormDialog 메시지 처리기
