#pragma once

#include "CoreDX.h"
///
#include "MaxImporter.h"
#include "SkySphere.h"
//#include "SkyBox.h"
#include "ParticleSystem.h"
#include "Raycast.h"
#include "Light.h"
#include "PlaneUI.h"
#include "RLine.h"
#include "RSphere.h"
#include "RCube.h"
#include "CTransformer.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"
#include "HeightMap.h"




enum class ESceneName : char {
	Intro = 0,
	Main,
	Lobby,
};

// 전체 씬과 게임의 흐름을 관리
class MainClass : public Core 
{
private:
	static MainClass* m_curScene;
	static map<ESceneName, MainClass*> m_SceneList;
	static GameObject* m_pSkyBox;
public:
	// 씬 설정
	void SetScene(const ESceneName& sceneName, const bool& useRelease = true) noexcept;

	virtual bool Init()					noexcept override;
	virtual bool Frame()				noexcept override;
	virtual bool Render()				noexcept override;
	virtual bool Release()				noexcept override;
public:
	MainClass() = default;
	~MainClass() = default;
};