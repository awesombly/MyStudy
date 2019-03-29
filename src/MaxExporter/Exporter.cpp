#include "stdafx.h"
#include "Exporter.h"

extern HMODULE g_hModule;

ExportClassDesc ExportClassDesc::exportDesc;

// 다이얼로그 박스 이벤트 처리
INT_PTR CALLBACK ExporterOptionsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Exporter* imp = nullptr;
	switch (message)
	{
	case WM_INITDIALOG:
	{
		imp = (Exporter*)lParam;
		CenterWindow(hWnd, GetParent(hWnd));
		return TRUE;
	}	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_Export_OK:
		{
			//imp->m_Writer.Export();
			EndDialog(hWnd, 1);
			return TRUE;
		}	break;
		}
	}
	case WM_CLOSE:
	{
		EndDialog(hWnd, 0);
		return TRUE;
	}	break;
	}

	return FALSE;
}


int Exporter::ExtCount()
{
	return 1;
}

const TCHAR* Exporter::Ext(int n)
{
	return _T("txt");
	n;
}

const TCHAR* Exporter::LongDesc()
{
	return _T("Max Exporter 0.1");
}

const TCHAR* Exporter::ShortDesc()
{
	return _T("My Max Exporter");
}

const TCHAR* Exporter::AuthorName()
{
	return _T("Taehong Kim.");
}

const TCHAR* Exporter::CopyrightMessage()
{
	return _T("copyright by KGCA");
}

const TCHAR* Exporter::OtherMessage1()
{
	return _T("whalwhal!");
}

const TCHAR* Exporter::OtherMessage2()
{
	return _T("growl~");
}

unsigned int Exporter::Version()
{
	return 1;
}

void Exporter::ShowAbout(HWND hWnd)
{
	// AboutBox..
	return;
	hWnd;
}

BOOL Exporter::SupportsOptions(int ext, DWORD options)
{
	return TRUE;
	ext; options;
}

int Exporter::DoExport(const TCHAR* name, ExpInterface *eInter, Interface* inter, BOOL suppressPrompts, DWORD option)
{
	// Interface(맥스 인터페이스) : 맥스 프로그램의 진입점으로 모든 맥스 기능을 실행, 수정, 확인이 가능

	// 모든 오브젝트(Material, Mesh, Camera, Light..)의 부모 클래스, N트리 구조

	//INode* pRoot = inter->GetRootNode();
	//int childCount = pRoot->NumberOfChildren();
	//auto pChild = pRoot->GetChildNode(1);
	//auto rootName = pRoot->GetName();

	// Point2, Point3, Point4, Matrix3 :[4][3], Quat

	// 특정 시간대에 오브젝트 월드 정보 얻음
	// ObjectState* INode::EvalWorldState(timeValue time, BOOL evalHidden = TRUE);


	if (!suppressPrompts)
	{
		if (DialogBox(g_hModule, MAKEINTRESOURCE(IDD_DIALOG_Export),
					  GetActiveWindow(), ExporterOptionsDlgProc, (LPARAM)this))
		{
			m_Writer.Init(name, inter);
			m_Writer.Export();
			m_Writer.Release();
		}
	}
	
	return TRUE;
	name; eInter; inter; suppressPrompts; option;
}







// ===================== Export Class 재정의 함수들 =======================

int ExportClassDesc::IsPublic()
{
	return TRUE;
}

// 맥스에서 내부 호출되어 익스포터 생성
void* ExportClassDesc::Create(BOOL /*loading = FALSE*/)
{
	return new Exporter();
}

const TCHAR* ExportClassDesc::ClassName()
{
	return _T("Exporter100?");
}

SClass_ID ExportClassDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

Class_ID ExportClassDesc::ClassID()
{
	return MAX_EXPORT_CLASS_ID;
}

const TCHAR* ExportClassDesc::Category()
{
	return _T("Exporter");
}

const TCHAR* ExportClassDesc::InternalName()
{
	return _T("ExportClassDesc");
}

HINSTANCE ExportClassDesc::HInstance()
{
	return g_hModule;
}

ClassDesc2* ExportClassDesc::GetExportDesc()
{
	return &exportDesc;
}