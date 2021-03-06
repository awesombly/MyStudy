#include "MainClass.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "LobbyScene.h"


GameObject*					MainClass::m_pSkyBox = nullptr;
MainClass*				    MainClass::m_curScene = nullptr;
map<ESceneName, MainClass*> MainClass::m_SceneList;



bool MainClass::Init() noexcept
{
	// 스카이 박스
	m_pSkyBox = new GameObject(L"SkyBox", { new SkySphere(20, L"SkySphere", L"CubeMap/grassenvmap1024.dds"), new CTransformer(Vector3::Zero, Quaternion::Right * 0.05f, Vector3::Zero) }, EObjType::Dummy);
	m_pSkyBox->SetScale(Vector3::One * 100);
	m_pSkyBox->isGlobal(true);

	//// 로딩 이미지
	//ObjectManager::Get().ReadSpriteScript();

	// 씬 설정
	m_SceneList[ESceneName::Intro] = new IntroScene();
	m_SceneList[ESceneName::Main] = new GameScene();
	m_SceneList[ESceneName::Lobby] = new LobbyScene();
	///
	ErrorMessage(__FUNCTION__ + " -> End."s);
	SetScene(ESceneName::Intro, false);
	return true;
}


bool MainClass::Frame() noexcept
{
	m_pSkyBox->Frame(Timer::SPF, Timer::AccumulateTime);
	m_curScene->Frame();

	// 씬 전환
	if (Input::GetKeyState(VK_NUMPAD1) == EKeyState::DOWN)
		SetScene(ESceneName::Intro);
	if (Input::GetKeyState(VK_NUMPAD2) == EKeyState::DOWN)
		SetScene(ESceneName::Main);
	if (Input::GetKeyState(VK_NUMPAD3) == EKeyState::DOWN)
		SetScene(ESceneName::Lobby);
	return true;
}

bool MainClass::Render() noexcept
{
	m_pSkyBox->Render(DxManager::Get().GetDContext());
	m_curScene->Render();
	return true;
}

bool MainClass::Release() noexcept
{
	m_curScene->Release();
	return true;
}

// 씬 전환
static const float LoadingSPF = 0.04f;
void MainClass::SetScene(const ESceneName& sceneName, const bool& useRelease) noexcept
{
	if (m_curScene == m_SceneList[sceneName])
		return;
	if (useRelease)
		m_curScene->Release();
	ErrorMessage(__FUNCTION__ + " -> Start."s);

	m_curScene = m_SceneList[sceneName];
	// 쓰레드 실행
	std::thread initer(&MainClass::Init, m_curScene);
	initer.detach();
}









//					윈도우 핸들,         호출한 윈도우 핸들
//  호출형식(언어)  ?                    보여줄 속성값
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPWSTR	  lpCmdLine, int   nCmdShow)
{
	// 윈도우 생성
	static MainClass mainWindow;
	mainWindow.SetClientRect(250, 250, 1024, 700);

	// 윈도우 설정
	if (mainWindow.SetWindow(hInstance, nCmdShow, L"MyGame"))
	{
		// 윈도우 가동
		mainWindow.GameRun();	
	}
	
	return 0;
	lpCmdLine;	hPrevInstance;
}