
// 0619 PainWnd2View.cpp: CMy0619PainWnd2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "0619 PainWnd2.h"
#endif

#include "0619 PainWnd2Doc.h"
#include "0619 PainWnd2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy0619PainWnd2View

IMPLEMENT_DYNCREATE(CMy0619PainWnd2View, CView)

BEGIN_MESSAGE_MAP(CMy0619PainWnd2View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy0619PainWnd2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMy0619PainWnd2View 생성/소멸

CMy0619PainWnd2View::CMy0619PainWnd2View()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMy0619PainWnd2View::~CMy0619PainWnd2View()
{
}

BOOL CMy0619PainWnd2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMy0619PainWnd2View 그리기

void CMy0619PainWnd2View::OnDraw(CDC* /*pDC*/)
{
	CMy0619PainWnd2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMy0619PainWnd2View 인쇄


void CMy0619PainWnd2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy0619PainWnd2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMy0619PainWnd2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMy0619PainWnd2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMy0619PainWnd2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy0619PainWnd2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy0619PainWnd2View 진단

#ifdef _DEBUG
void CMy0619PainWnd2View::AssertValid() const
{
	CView::AssertValid();
}

void CMy0619PainWnd2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy0619PainWnd2Doc* CMy0619PainWnd2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy0619PainWnd2Doc)));
	return (CMy0619PainWnd2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy0619PainWnd2View 메시지 처리기
