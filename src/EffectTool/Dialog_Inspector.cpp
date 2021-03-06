// Dialog_Inspector.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Dialog_Inspector.h"
#include "afxdialogex.h"
#include "Dialog_Hierarchy.h"
#include "Pane_Component.h"
#include "Pane_Preset.h"

Dialog_Inspector* Dialog_Inspector::Instance = nullptr;
GameObject* Dialog_Inspector::SelectObject	 = nullptr;
Component* Dialog_Inspector::SelectComponent = nullptr;

// Dialog_Inspector 대화 상자

IMPLEMENT_DYNAMIC(Dialog_Inspector, CFormView)

Dialog_Inspector::Dialog_Inspector()
	: CFormView(IDD_DIALOG_Inspector)
{
	Instance = this;
}

Dialog_Inspector::~Dialog_Inspector()
{
}

void Dialog_Inspector::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ObjectName, m_ObjectName);

	DDX_Control(pDX, IDC_PosX, m_EditPos[0]);
	DDX_Control(pDX, IDC_PosY, m_EditPos[1]);
	DDX_Control(pDX, IDC_PosZ, m_EditPos[2]);

	DDX_Control(pDX, IDC_RotX, m_EditRot[0]);
	DDX_Control(pDX, IDC_RotY, m_EditRot[1]);
	DDX_Control(pDX, IDC_RotZ, m_EditRot[2]);

	DDX_Control(pDX, IDC_ScaleX, m_EditScale[0]);
	DDX_Control(pDX, IDC_ScaleY, m_EditScale[1]);
	DDX_Control(pDX, IDC_ScaleZ, m_EditScale[2]);

	DDX_Control(pDX, IDC_isEnable, m_isEnable);
	DDX_Control(pDX, IDC_isBillBoard, m_isBillBoard);
	DDX_Control(pDX, IDC_isStatic, m_isStatic);
	DDX_Control(pDX, IDC_Inspector_Components, m_Components);
	DDX_Control(pDX, ID_Inspector_CompDelete, m_CompDelete);
	DDX_Control(pDX, ID_Inspector_CompAdd, m_CompAdd);
	DDX_Control(pDX, IDC_Inspector_CompPreset, m_CompPreset);
}

Dialog_Inspector* Dialog_Inspector::CreateOne(CWnd* pParent)
{
	Dialog_Inspector* pForm = new Dialog_Inspector();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}


BEGIN_MESSAGE_MAP(Dialog_Inspector, CFormView)
	ON_EN_CHANGE(IDC_PosX,			&Dialog_Inspector::OnEnChangePosx)
	ON_EN_CHANGE(IDC_PosY,			&Dialog_Inspector::OnEnChangePosy)
	ON_EN_CHANGE(IDC_PosZ,			&Dialog_Inspector::OnEnChangePosz)
	ON_EN_CHANGE(IDC_RotX,			&Dialog_Inspector::OnEnChangeRotx)
	ON_EN_CHANGE(IDC_RotY,			&Dialog_Inspector::OnEnChangeRoty)
	ON_EN_CHANGE(IDC_RotZ,			&Dialog_Inspector::OnEnChangeRotz)
	ON_EN_CHANGE(IDC_ScaleX,		&Dialog_Inspector::OnEnChangeScalex)
	ON_EN_CHANGE(IDC_ScaleY,		&Dialog_Inspector::OnEnChangeScaley)
	ON_EN_CHANGE(IDC_ScaleZ,		&Dialog_Inspector::OnEnChangeScalez)
	ON_EN_CHANGE(IDC_ObjectName,	&Dialog_Inspector::OnEnChangeObjectname)
	ON_BN_CLICKED(IDC_isEnable,		&Dialog_Inspector::OnBnClickedisenable)
	ON_BN_CLICKED(IDC_isBillBoard,	&Dialog_Inspector::OnBnClickedisbillboard)
	ON_BN_CLICKED(IDC_isStatic,		&Dialog_Inspector::OnBnClickedisstatic)
	ON_LBN_SELCHANGE(IDC_Inspector_Components, &Dialog_Inspector::OnLbnSelchangeInspectorComponents)
	ON_BN_CLICKED(ID_Inspector_CompDelete, &Dialog_Inspector::OnBnClickedInspectorCompdelete)
	ON_BN_CLICKED(ID_Inspector_CompAdd, &Dialog_Inspector::OnBnClickedInspectorCompadd)
	ON_BN_CLICKED(ID_Inspector_SetProto, &Dialog_Inspector::OnBnClickedInspectorSetproto)
END_MESSAGE_MAP()


// ============================= 정의 함수 =================================

void Dialog_Inspector::UpdateInspector() noexcept
{
	m_dontUpdate = true;

	// Position
	m_tempString = std::to_wstring(SelectObject->GetPosition().x).c_str();
	m_EditPos[0].SetWindowTextW(m_tempString.Left(m_tempString.Find('.') + 2));

	m_tempString = std::to_wstring(SelectObject->GetPosition().y).c_str();
	m_EditPos[1].SetWindowTextW(m_tempString.Left(m_tempString.Find('.') + 2));

	m_tempString = std::to_wstring(SelectObject->GetPosition().z).c_str();
	m_EditPos[2].SetWindowTextW(m_tempString.Left(m_tempString.Find('.') + 2));
	// Rotation
	m_tempString = std::to_wstring(SelectObject->GetRotation().x).c_str();
	m_EditRot[0].SetWindowTextW(m_tempString.Left(m_tempString.Find('.') + 2));

	m_tempString = std::to_wstring(SelectObject->GetRotation().y).c_str();
	m_EditRot[1].SetWindowTextW(m_tempString.Left(m_tempString.Find('.') + 2));

	m_tempString = std::to_wstring(SelectObject->GetRotation().z).c_str();
	m_EditRot[2].SetWindowTextW(m_tempString.Left(m_tempString.Find('.') + 2));
	// Scale
	m_tempString = std::to_wstring(SelectObject->GetScale().x).c_str();
	m_EditScale[0].SetWindowTextW(m_tempString.Left(m_tempString.Find('.') + 2));

	m_tempString = std::to_wstring(SelectObject->GetScale().y).c_str();
	m_EditScale[1].SetWindowTextW(m_tempString.Left(m_tempString.Find('.') + 2));

	m_tempString = std::to_wstring(SelectObject->GetScale().z).c_str();
	m_EditScale[2].SetWindowTextW(m_tempString.Left(m_tempString.Find('.') + 2));

	m_dontUpdate = false;
}

void Dialog_Inspector::UpdateComponents() noexcept
{
	SelectComponent = nullptr;
	m_Components.ResetContent();
	for (auto& outIter : SelectObject->GetComponentList())
	{
		for (auto& inIter : outIter.second)
		{
			wstring name = inIter->m_myName.c_str();
			AddComponentNameType(name, inIter);

			int index = m_Components.AddString(name.c_str());
			m_Components.SetItemData(index, (DWORD_PTR)inIter);
		}
	}
	m_Components.SetCurSel(0);
	int index = m_Components.GetCurSel();
	if (index == LB_ERR)
	{
		SelectComponent = nullptr;
	}
	else
	{
		SelectComponent = (Component*)m_Components.GetItemData(index);
	}
	Pane_Component::Instance->SetCompDialog(SelectComponent);
	Pane_Preset::Instance->SetCompDialog(SelectComponent);
}


void Dialog_Inspector::UpdateObjectData(GameObject* pObject) noexcept
{
	SelectObject = pObject;
	if (SelectObject == nullptr)
	{
		// 인스펙터 초기화, 비활성화
		ClearInspector();
		return;
	}
	// 인스펙터 활성화
	m_ObjectName.EnableWindow(1);
	m_EditPos[0].EnableWindow(1);
	m_EditPos[1].EnableWindow(1);
	m_EditPos[2].EnableWindow(1);
	m_EditRot[0].EnableWindow(1);
	m_EditRot[1].EnableWindow(1);
	m_EditRot[2].EnableWindow(1);
	m_EditScale[0].EnableWindow(1);
	m_EditScale[1].EnableWindow(1);
	m_EditScale[2].EnableWindow(1);
	m_isEnable.EnableWindow(1);
	m_isBillBoard.EnableWindow(1);
	m_isStatic.EnableWindow(1);
	m_Components.EnableWindow(1);

	// 인스펙터 갱신
	m_ObjectName.SetWindowTextW(SelectObject->m_myName.c_str());
	UpdateInspector();
	m_isEnable.SetCheck(SelectObject->isEnable());
	m_isBillBoard.SetCheck(SelectObject->isBillBoard());
	m_isStatic.SetCheck(SelectObject->isStatic());

	UpdateComponents();
}

void Dialog_Inspector::ClearInspector() noexcept
{
	// 비활성화
	m_ObjectName.EnableWindow(0);
	m_EditPos[0].EnableWindow(0);
	m_EditPos[1].EnableWindow(0);
	m_EditPos[2].EnableWindow(0);
	m_EditRot[0].EnableWindow(0);
	m_EditRot[1].EnableWindow(0);
	m_EditRot[2].EnableWindow(0);
	m_EditScale[0].EnableWindow(0);
	m_EditScale[1].EnableWindow(0);
	m_EditScale[2].EnableWindow(0);
	m_isEnable.EnableWindow(0);
	m_isBillBoard.EnableWindow(0);
	m_isStatic.EnableWindow(0);
	m_Components.EnableWindow(0);
	// 초기화
	m_ObjectName.SetWindowTextW(L"Name");

	m_EditPos[0].SetWindowTextW(L"0.0");
	m_EditPos[1].SetWindowTextW(L"0.0");
	m_EditPos[2].SetWindowTextW(L"0.0");

	m_EditRot[0].SetWindowTextW(L"0.0");
	m_EditRot[1].SetWindowTextW(L"0.0");
	m_EditRot[2].SetWindowTextW(L"0.0");

	m_EditScale[0].SetWindowTextW(L"0.0");
	m_EditScale[1].SetWindowTextW(L"0.0");
	m_EditScale[2].SetWindowTextW(L"0.0");

	m_isEnable.SetCheck(false);
	m_isBillBoard.SetCheck(false);
	m_isStatic.SetCheck(false);

	m_Components.ResetContent();
	SelectComponent = nullptr;
}

bool Dialog_Inspector::ErrorCheck() noexcept
{
	if (m_dontUpdate ||
		SelectObject == nullptr)
	{
		return false;
	}
	return true;
}

void Dialog_Inspector::AddComponentNameType(wstring& name, const Component* pComp) noexcept
{
	switch (pComp->m_comptType)
	{
	case EComponent::Renderer:
	{
		switch (((Renderer*)pComp)->m_eRenderType)
		{
		case ERenderType::Basic:
		{
			name += L" (Renderer)";
		}	break;
		case ERenderType::Particle:
		{
			name += L" (Particle)";
		}	break;
		case ERenderType::HeightMap:
		{
			name += L" (HeightMap)";
		}	break;
		}
	}	break;
	case EComponent::Instancer:
	{
		name += L" (Instancing)";
	}	break;
	case EComponent::Animation:
	{
		name += L" (Animation)";
	}	break;
	case EComponent::Collider:
	{
		name += L" (Collider)";
	}	break;
	case EComponent::Transformer:
	{
		name += L" (Transformer)";
	}	break;
	case EComponent::UI:
	{
		name += L" (UI)";
	}	break;
	}
}

void Dialog_Inspector::AddCompPreset(Component* pComp) noexcept
{
	pComp->m_pParent = nullptr;
	ObjectManager::GetInstance().SetProtoComponent(pComp);

	auto tempStr = pComp->m_myName;
	//AddComponentNameType(tempStr, pComp);

	auto item = m_CompPreset.InsertString(0, tempStr.c_str());
	m_CompPreset.SetItemData(item, (DWORD_PTR)pComp);
}


////////////////////////////////////////////////// Dialog_Inspector 메시지 처리기

void Dialog_Inspector::OnEnChangeObjectname()
{
	m_ObjectName.GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}
	SelectObject->m_myName = m_tempString.GetBuffer();

	auto item = Dialog_Hierarchy::Instance->m_TreeList.GetSelectedItem();
	Dialog_Hierarchy::Instance->m_TreeList.SetItemText(item, SelectObject->m_myName.c_str());
}

void Dialog_Inspector::OnEnChangePosx()
{
	m_EditPos[0].GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}

	try
	{
		SelectObject->SetPositionX(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
	}
}
void Dialog_Inspector::OnEnChangePosy()
{
	m_EditPos[1].GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}

	try
	{
		SelectObject->SetPositionY(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
	}
}
void Dialog_Inspector::OnEnChangePosz()
{
	m_EditPos[2].GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}

	try
	{
		SelectObject->SetPositionZ(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
	}
}

void Dialog_Inspector::OnEnChangeRotx()
{
	m_EditRot[0].GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}

	try
	{
		SelectObject->SetRotationX(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
	}
}
void Dialog_Inspector::OnEnChangeRoty()
{
	m_EditRot[1].GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}

	try
	{
		SelectObject->SetRotationY(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
	}
}
void Dialog_Inspector::OnEnChangeRotz()
{
	m_EditRot[2].GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}

	try
	{
		SelectObject->SetRotationZ(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
	}
}


void Dialog_Inspector::OnEnChangeScalex()
{
	m_EditScale[0].GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}

	try
	{
		SelectObject->SetScaleX(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
	}
}
void Dialog_Inspector::OnEnChangeScaley()
{
	m_EditScale[1].GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}

	try
	{
		SelectObject->SetScaleY(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
	}
}
void Dialog_Inspector::OnEnChangeScalez()
{
	m_EditScale[2].GetWindowTextW(m_tempString);
	if (!ErrorCheck())
	{
		return;
	}

	try
	{
		SelectObject->SetScaleZ(std::stof(m_tempString.GetBuffer()));
	}
	catch (...)
	{
	}
}


void Dialog_Inspector::OnBnClickedisenable()
{
	SelectObject->isEnable(m_isEnable.GetCheck());
}
void Dialog_Inspector::OnBnClickedisbillboard()
{
	SelectObject->isBillBoard(m_isBillBoard.GetCheck());
}
void Dialog_Inspector::OnBnClickedisstatic()
{
	SelectObject->isStatic(m_isStatic.GetCheck());
}

// 컴포넌트 클릭
void Dialog_Inspector::OnLbnSelchangeInspectorComponents()
{
	int index = m_Components.GetCurSel();
	if (index == LB_ERR)
	{
		//MessageBox(L"파일을 선택해 주세요.", L"삐빅-");
		//SelectComponent = nullptr;
		return;
	}
	SelectComponent = (Component*)m_Components.GetItemData(index);
	
	Pane_Component::Instance->SetCompDialog(SelectComponent);
	Pane_Preset::Instance->SetCompDialog(SelectComponent);
}

// 컴포넌트 제거
void Dialog_Inspector::OnBnClickedInspectorCompdelete()
{
	auto item = m_Components.GetCurSel();
	if (item == NULL)
	{
		MessageBox(L"객체를 선택해 주세요.", L"삐빅-");
		return;
	}
	if (SelectComponent == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	SelectObject->RemoveComponent(SelectComponent);
	SelectComponent = nullptr;
	m_Components.DeleteString(item);
	//UpdateComponents();
	//m_Components.DeleteItem(item);
}

// 컴포넌트 추가
void Dialog_Inspector::OnBnClickedInspectorCompadd()
{
	int index = m_CompPreset.GetCurSel();
	if (index == LB_ERR || SelectObject == nullptr) 
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}	
	Component* pComp = (Component*)m_CompPreset.GetItemData(index);
	SelectObject->AddComponent(pComp->clone());
	UpdateComponents();
}

// 컴포넌트 등록
void Dialog_Inspector::OnBnClickedInspectorSetproto()
{
	if (SelectComponent == nullptr)
	{
		MessageBox(L"대상을 선택해 주세요.", L"삐빅-");
		return;
	}
	AddCompPreset(SelectComponent->clone());
}
