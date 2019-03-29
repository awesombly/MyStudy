#pragma once
#include "stdafx.h"
#include "WriterEx.h"

// �÷������� �����ϴ� Super Class ID
#define SCENE_EXPORT_CLASS_ID 0x000A20 

// 3ds Max ���� -> SDK/maxsdk/help/gencid.exe ����� ID �߱�
// Super Class ID �ȿ��� ���� �÷������� �����ϴ� �ε���
#define MAX_EXPORT_CLASS_ID Class_ID(0x307621df, 0x1a0b4cfc)

// ���䵥��ũ �������̼�
// game@kgcaschool.com
// kgca!@34



// ���� ��¿� ���� �������� ����
class Exporter : public SceneExport
{
public:
	//HWND			hPanel;
	//IUtil
	WriterEx m_Writer;
public:
	int				ExtCount()			 override;		// ����� Ȯ���� ��
	const TCHAR*	Ext(int n)			 override;		// ��� Ȯ���� ����
	const TCHAR*	LongDesc()			 override;		// �� ASCII ����	 ex) 'Autodesk 3D Studio File'
	const TCHAR*	ShortDesc()			 override;		// �޴�-Export ���ý� ���� ���� ���ڿ�
	const TCHAR*	AuthorName()		 override;		// ASCII �ۼ��� �̸�
	const TCHAR*	CopyrightMessage()	 override;		// ASCII ���۱� �޽���
	const TCHAR*	OtherMessage1()		 override;		// ��Ÿ �޽���1
	const TCHAR*	OtherMessage2()		 override;		// �ۼ���, ����ȸ�� �� ��Ÿ �޼���
	unsigned int	Version()			 override;		// �ۼ� ���� * 100,		 ex) v3.01 = 301
	void			ShowAbout(HWND hWnd) override;		// DLL�� About ���� ǥ��

	BOOL SupportsOptions(int ext, DWORD options) override;
	// Export�� ����
	int DoExport(const TCHAR* name, ExpInterface *eInter, Interface* inter, BOOL suppressPrompts = false, DWORD option = 0) override;
public:
	Exporter() = default;
	virtual ~Exporter() = default;
};



// Plugin Ŭ���� ���� �� Ŭ������ ���� ���� ���� �� Ȯ��,
// �÷α� ���� ������ �ν��Ͻ� �ڵ� �� ������ Ŭ���� ID�� ����,
// �÷α� ���� ���α׷��� ���� ������ ���� ����� ��.
class ExportClassDesc : public ClassDesc2
{
public:
	static ExportClassDesc exportDesc;

	int IsPublic() override;
	// �ƽ����� ���� ȣ��Ǿ� �ͽ����� ������
	void* Create(BOOL /*loading = FALSE*/) override;
	const TCHAR* ClassName()	override;
	SClass_ID SuperClassID()	override;
	Class_ID ClassID()			override;
	const TCHAR* Category()		override;
	const TCHAR* InternalName() override;
	HINSTANCE HInstance()		override;
public:
	static ClassDesc2* GetExportDesc();

	ExportClassDesc() = default;
	~ExportClassDesc() = default;
};