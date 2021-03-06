// Dialog_Preset.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Dialog_Preset.h"
#include "afxdialogex.h"
#include "EffectTool.h"
#include "Dialog_Hierarchy.h"
#include "AseParser.h"

Dialog_Preset* Dialog_Preset::Instance = nullptr;
// Dialog_Preset 대화 상자

IMPLEMENT_DYNAMIC(Dialog_Preset, CFormView)

Dialog_Preset::Dialog_Preset()
	: CFormView(IDD_DIALOG_Preset)
{
	Instance = this;
}

Dialog_Preset::~Dialog_Preset()
{}

void Dialog_Preset::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ObjectList, m_ListBox);
}

Dialog_Preset* Dialog_Preset::CreateOne(CWnd* pParent)
{
	Dialog_Preset* pForm = new Dialog_Preset();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);

	return pForm;
}

BEGIN_MESSAGE_MAP(Dialog_Preset, CFormView)
	ON_LBN_SELCHANGE(IDC_ObjectList, &Dialog_Preset::OnLbnSelchangeList1)
	ON_BN_CLICKED(ID_Load, &Dialog_Preset::OnBnClickedLoad)
	ON_BN_CLICKED(ID_Create, &Dialog_Preset::OnBnClickedCreate)
END_MESSAGE_MAP()

// 파일 로더
void Dialog_Preset::LoadFromFile(const filesystem::path fpath)
{
	GameObject* pObject = nullptr;

	wstring fExtention = fpath.extension();
	if (fExtention == L".TXT" || fExtention == L".eff")
	{
		MaxImporter parser;
		parser.CreateFromFile(&pObject, fpath.filename().c_str(), fpath.parent_path().c_str());
	}
	else if (fExtention == L".ASE" || fExtention == L".ase")
	{
		AseParser parser;
		parser.CreateFromFile(&pObject, fpath.filename().c_str(), fpath.parent_path().c_str());
	}
	//else if (fExtention == L".EFF" || fExtention == L".eff")
	//{
	//	ifstream readStream(fpath);
	//	pObject = new GameObject(fpath.filename().c_str());
	//	istringstream inStream;
	//	string subString, objName, srcName;
	//	// Name
	//	std::getline(readStream, subString);
	//	inStream.str(subString);
	//	inStream >> subString >> objName;
	//	pObject->m_myName = CharToWChar(objName.c_str());
	//	// Transform
	//	readStream.read((char*)pObject, sizeof(Transform));
	//	std::getline(readStream, subString);
	//	// Particle
	//	std::getline(readStream, subString);
	//	inStream.str(subString);
	//	inStream >> subString >> objName >> srcName;
	//
	//	auto pParticle = new ParticleSystem(CharToWChar(objName.c_str()), new Particle(), CharToWChar(srcName.c_str()));
	//	readStream.read((char*)(ParticleInfo*)pParticle, sizeof(ParticleInfo));
	//	//readStream.read((char*)pParticle, sizeof(ParticleInfo));
	//	pObject->AddComponent(pParticle);
	//}
	else
	{
		// 아니면 이미지 불러서 생성
		pObject = new GameObject(fpath.filename().c_str(), new RPlane(fpath.filename().c_str()));
	}
	pObject->m_objType = EObjType::Object;
	m_ListBox.AddString(pObject->m_myName.data());
	ObjectManager::GetInstance().SetProtoObject(pObject);
}


// Dialog_Preset 메시지 처리기

void Dialog_Preset::OnLbnSelchangeList1()
{
	// 리스트 선택
}

// 불러오기
void Dialog_Preset::OnBnClickedLoad()
{
	// 불러오기 버튼
	CFileDialog fileDia(true);				// true = 불러오기, false = 저장인듯
	if (fileDia.DoModal() != IDOK)
		return;

	static std::filesystem::path fpath;
	fpath = fileDia.GetPathName().GetBuffer();
	CString str;
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, str);
		if (str == fpath.filename().c_str())
		{
			MessageBox(L"이미 등록된 객체 입니다.", L"Ang~?");
			return;
		}
	}
	std::thread loader(&Dialog_Preset::LoadFromFile, this, fpath);
	loader.detach();
}

// 생성
void Dialog_Preset::OnBnClickedCreate()
{
	// 생성 버튼
	int index = m_ListBox.GetCurSel();
	if (index == LB_ERR)
	{
		MessageBox(L"파일을 선택해 주세요.", L"삐빅-");
		return;
	}
	// 객체 생성
	CString str;
	m_ListBox.GetText(index, str);
	auto iter = ObjectManager::GetInstance().TakeObject(str.GetBuffer());
	auto pParticle = (ParticleSystem*)iter->GetComponent(EComponent::Renderer);
	if (pParticle != nullptr)
	{
		if (pParticle->m_eRenderType == ERenderType::Particle)
			pParticle->isRepeat(true);
	}
	// 하이어라키에 객체 등록
	//auto pList = &Dialog_Hierarchy::Instance->m_TreeList;

	//auto parent = pList->InsertItem(iter->m_myName.c_str(), 0, 0, TVI_ROOT, TVI_LAST);
	//pList->SetItemData(parent, (DWORD_PTR)iter);
	//Dialog_Hierarchy::Instance->AddTreeNode(iter, parent, 1);
	Dialog_Hierarchy::Instance->AddTreeItem(iter);
}