
// EffectTool.h: EffectTool 응용 프로그램의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "CoreDX.h"

#include "MaxImporter.h"
#include "Raycast.h"
#include "PlaneUI.h"
#include "Light.h"
#include "RLine.h"
#include "RCube.h"
#include "RSphere.h"
#include "SkySphere.h"
#include "HeightMap.h"
#include "CTransformer.h"
#include "CEventTimer.h"
#include "FrustumBox.h"
#include "ParticleSystem.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"
//#include "EnviDySphere.h"
//#include "Character.h"
//#include "CPlaneShadow.h"
//#include "QuadTree.h"



// 엔진 연동용 코어 인터페이스
class CoreMFC : public Core
{
public:
	static MaxImporter* pParser;
public:
	GameObject* m_pSkyBox;
	GameObject* m_pSelectLine;
	GameObject* m_pFrustumBox;
	D3DXVECTOR3 m_rayPoint;

	void MousePicking(GameObject* pObject) noexcept;
	void ObjectMoveCheck(const float& spf) noexcept;

	bool Init()		noexcept override;
	bool Frame()	noexcept override;
	bool Render()	noexcept override;
	bool Release()	noexcept override;

	CoreMFC() = default;
	~CoreMFC() = default;
};


// 실제 메인
class CEffectToolApp : public CWinAppEx
{
public:
	CEffectToolApp();
	static CoreMFC CoreLib;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEffectToolApp theApp;
