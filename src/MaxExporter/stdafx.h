// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#pragma warning(disable: 4201)			// 
#pragma warning(disable: 4819)			// 유니코드 경고 무시
#pragma warning(disable: 4840)			// 

// export, import 정의,,
#ifdef MAXEXPORTER_EXPORTS
#define MAXEXPORTER_API __declspec(dllexport)
#else
#define MAXEXPORTER_API __declspec(dllimport)
#endif


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include "resource.h"
// Max headers
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
#include <utilapi.h>
#include <max.h>
#include <stdmat.h>
#include <decomp.h>
#include <bipexp.h>
#include <phyexp.h>
#include <iskin.h>
///
#include "tchar.h"
#include <filesystem>
#include <fstream>

#pragma comment(lib, "bmm.lib")
#pragma comment(lib, "core.lib")
#pragma comment(lib, "geom.lib")
#pragma comment(lib, "gfx.lib")
#pragma comment(lib, "mesh.lib")
#pragma comment(lib, "maxutil.lib")
#pragma comment(lib, "maxscrpt.lib")
#pragma comment(lib, "gup.lib")
#pragma comment(lib, "paramblk2.lib")

using namespace std;