#include "stdafx.h"
#include "Exporter.h"

extern HMODULE g_hModule;

ExportClassDesc ExportClassDesc::exportDesc;

// ���̾�α� �ڽ� �̺�Ʈ ó��
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
	// Interface(�ƽ� �������̽�) : �ƽ� ���α׷��� ���������� ��� �ƽ� ����� ����, ����, Ȯ���� ����

	// ��� ������Ʈ(Material, Mesh, Camera, Light..)�� �θ� Ŭ����, NƮ�� ����

	//INode* pRoot = inter->GetRootNode();
	//int childCount = pRoot->NumberOfChildren();
	//auto pChild = pRoot->GetChildNode(1);
	//auto rootName = pRoot->GetName();

	// Point2, Point3, Point4, Matrix3 :[4][3], Quat

	// Ư�� �ð��뿡 ������Ʈ ���� ���� ����
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







// ===================== Export Class ������ �Լ��� =======================

int ExportClassDesc::IsPublic()
{
	return TRUE;
}

// �ƽ����� ���� ȣ��Ǿ� �ͽ����� ����
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