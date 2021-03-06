#include "MainClass.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "EndingScene.h"
#include "DxClient.h"
#include "Worker.h"

MainClass*				   MainClass::m_curScene	= nullptr;
map<ESceneName, MainClass*> MainClass::m_SceneList;

MyActor*		MainClass::m_loadingBitmap = nullptr;
MyActor*				   MainClass::m_backGround	= nullptr;
MyActor*				   MainClass::m_mapMap		= nullptr;

HealthBar*		MainClass::m_HealthBar = nullptr;
BloodRate*		MainClass::m_BloodRate = nullptr;
MiniMap*		MainClass::m_MiniMap = nullptr;
ChattingBox*	MainClass::m_ChatBox = nullptr;

bool MainClass::Init() noexcept
{
	srand(unsigned int(time(0)));
	//std::async
	m_ClientServer.Init(10000, "192.168.0.8");
	//m_ClientServer.PushServer(new DxClientRecv(), true);
	//m_ClientServer.PushServer(new DxClientSend(), true);
	//m_ClientServer.PushServer(new Worker(), true);

	m_SceneList[ESceneName::Intro] = new IntroScene();
	m_SceneList[ESceneName::Main] = new GameScene();
	m_SceneList[ESceneName::Ending] = new EndingScene();
	m_curScene = m_SceneList[ESceneName::Intro];

	if (!m_ObjectManager.Init() || !m_SoundManager.Init())
	{
		ErrorMessage(""s + __FUNCTION__ + ", 초기화 실패!");
		return false;
	}

	// 정점 쉐이더 로드
	m_DxManager.LoadVertexShader(L"./Shader.hlsl", "VS_Basic");
	// 픽셀 쉐이더 로드
	m_DxManager.LoadPixelShader(L"./Shader.hlsl", "PShader");
	m_DxManager.LoadPixelShader(L"./Shader.hlsl", "PPureColor");
	m_DxManager.LoadPixelShader(L"./Shader.hlsl", "PHalfAlpha");
	m_DxManager.LoadPixelShader(L"./Shader.hlsl", "PButtonClick");
	m_DxManager.LoadPixelShader(L"./Shader.hlsl", "PButtonOver");
	m_DxManager.LoadPixelShader(L"./Shader.hlsl", "PRedColor");
	// 폰트 설정
	m_WriteManager.SetText({ 0, 0 }, L"", D2D1::ColorF::Black, 50, L"Yu Gothic");
	m_WriteManager.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_WriteManager.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	// 로딩화면
	m_ObjectManager.PushObject(m_loadingBitmap = new MyActor(L"KGCA", EObjType::UI))
		->setRect(getClientRect());
	static float loadingTime = 0.0f;
	while (1)
	{
		//if (m_Timer.Frame())
		{
			m_DxManager.PrevRender();
			m_loadingBitmap->Render(m_DxManager.GetDContext());
			m_DxManager.PostRender();
			//if ((loadingTime += m_Timer.SPF) > 2.0f)
				break;
		}
	}

	//m_SceneList[ESceneName::Intro]->Init();
	return true;
}

bool MainClass::Frame() noexcept
{
	m_curScene->Frame();

	m_SoundManager.Frame();
	if (m_Input.getKeyState(VK_F1) == EKeyState::DOWN)
		m_SoundManager.setBGM("BGM/Mischievous Masquerade");
	if (m_Input.getKeyState(VK_F2) == EKeyState::DOWN)
		m_SoundManager.setBGM("BGM/Sandbag Mini Game");
	if (m_Input.getKeyState(VK_F3) == EKeyState::DOWN)
		m_SoundManager.setBGM("BGM/Brawl Breaks");
	if (m_Input.getKeyState(VK_F4) == EKeyState::DOWN)
		m_SoundManager.setBGM("BGM/PLAY ROUGH");
	return true;
}

bool MainClass::Render() noexcept
{
	m_curScene->Render();
	m_SoundManager.Render();
	return true;
}

bool MainClass::Release() noexcept
{
	m_curScene->Release();
	WSACleanup();
	return true;
}

// 씬 전환
void MainClass::setScene(const ESceneName& sceneName)
{
	m_curScene->Release();
	m_curScene = m_SceneList[sceneName];
	m_curScene->Init();
}


void MainClass::setIntroScene()
{
	setScene(ESceneName::Intro);
}
void MainClass::setMainScene()
{
	setScene(ESceneName::Main);
}
void MainClass::ShowEnd()
{
	exit(0);
}
void MainClass::ShowConfigure()
{
	m_Settings.setEnable(!m_Settings.isEnable());
}
void MainClass::ShowInventory()
{
	m_Inventory.setEnable(!m_Inventory.isEnable());
}
void MainClass::ShowEquip()
{
	m_Equipment.setEnable(!m_Equipment.isEnable());
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