#include "stdafx.h"
#include "Exporter.h"


HMODULE g_hModule;
//int controlsInit = 0;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		// 핸들 가져옴
		g_hModule = hModule;
		DisableThreadLibraryCalls(g_hModule);
	}	break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
	lpReserved;
}

// =============================== 3d Max 함수들 ==================================

// dll 사용이 불가능할때 제공하는 스트링
MAXEXPORTER_API const TCHAR* LibDescription()
{
	return L"Max Exporter 1";
}

// dll의 플러그인 클래스 수
MAXEXPORTER_API int LibNumberClasses()
{
	return 1;
}

// 플러그인 속성, 클래스 인스턴스 할당 방법
MAXEXPORTER_API ClassDesc* LibClassDesc(int i)
{
	switch (i)
	{
	case 0: 
	{
		return ExportClassDesc::GetExportDesc();
	}
	default: return nullptr;
	}
}

// 버전
MAXEXPORTER_API ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

// 초기화 작업
MAXEXPORTER_API int LibInitialize(void)
{
	return TRUE;
}

// 종료시
MAXEXPORTER_API int LibShutdown(void)
{
	return TRUE;
}



TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (g_hModule != NULL)
		return LoadString(g_hModule, id, buf, _countof(buf)) ? buf : NULL;
	return NULL;
}