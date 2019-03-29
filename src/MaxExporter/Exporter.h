#pragma once
#include "stdafx.h"
#include "WriterEx.h"

// 플러그인을 구별하는 Super Class ID
#define SCENE_EXPORT_CLASS_ID 0x000A20 

// 3ds Max 폴더 -> SDK/maxsdk/help/gencid.exe 실행시 ID 발급
// Super Class ID 안에서 여러 플러그인을 구별하는 인덱스
#define MAX_EXPORT_CLASS_ID Class_ID(0x307621df, 0x1a0b4cfc)

// 오토데스크 에듀케이션
// game@kgcaschool.com
// kgca!@34



// 파일 출력에 대한 전반적인 정보
class Exporter : public SceneExport
{
public:
	//HWND			hPanel;
	//IUtil
	WriterEx m_Writer;
public:
	int				ExtCount()			 override;		// 출력할 확장자 수
	const TCHAR*	Ext(int n)			 override;		// 출력 확장자 지정
	const TCHAR*	LongDesc()			 override;		// 긴 ASCII 설명	 ex) 'Autodesk 3D Studio File'
	const TCHAR*	ShortDesc()			 override;		// 메뉴-Export 선택시 파일 형식 문자열
	const TCHAR*	AuthorName()		 override;		// ASCII 작성자 이름
	const TCHAR*	CopyrightMessage()	 override;		// ASCII 저작권 메시지
	const TCHAR*	OtherMessage1()		 override;		// 기타 메시지1
	const TCHAR*	OtherMessage2()		 override;		// 작성자, 제조회사 등 기타 메세지
	unsigned int	Version()			 override;		// 작성 버전 * 100,		 ex) v3.01 = 301
	void			ShowAbout(HWND hWnd) override;		// DLL의 About 상자 표시

	BOOL SupportsOptions(int ext, DWORD options) override;
	// Export시 실행
	int DoExport(const TCHAR* name, ExpInterface *eInter, Interface* inter, BOOL suppressPrompts = false, DWORD option = 0) override;
public:
	Exporter() = default;
	virtual ~Exporter() = default;
};



// Plugin 클래스 생성 및 클래스에 대한 정보 관리 및 확인,
// 플로그 인의 윈도우 인스턴스 핸들 및 고유한 클래스 ID를 관리,
// 플로그 인의 프로그램에 대한 일종의 명세서 기능을 함.
class ExportClassDesc : public ClassDesc2
{
public:
	static ExportClassDesc exportDesc;

	int IsPublic() override;
	// 맥스에서 내부 호출되어 익스포터 생성됨
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