#include "stdafx.h"
#include "EffectTool.h"
#include "Dialog_Hierarchy.h"
#include "afxdialogex.h"
#include "Dialog_Inspector.h"
#include "Dialog_Preset.h"

Dialog_Hierarchy* Dialog_Hierarchy::Instance = nullptr;

// Dialog_Preset ��ȭ ����
IMPLEMENT_DYNAMIC(Dialog_Hierarchy, CFormView)

Dialog_Hierarchy::Dialog_Hierarchy()
	: CFormView(IDD_DIALOG_Hierarchy)
{
	Instance = this;
}


Dialog_Hierarchy::~Dialog_Hierarchy()
{}


void Dialog_Hierarchy::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TreeList, m_TreeList);
	DDX_Control(pDX, ID_Duplicate, m_DuplicateButton);
	DDX_Control(pDX, ID_Delete, m_RemoveButton);
}

Dialog_Hierarchy* Dialog_Hierarchy::CreateOne(CWnd* pParent)
{
	Dialog_Hierarchy* pForm = new Dialog_Hierarchy();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

BEGIN_MESSAGE_MAP(Dialog_Hierarchy, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TreeList, &Dialog_Hierarchy::OnTvnSelchangedTreetlist)
	ON_BN_CLICKED(ID_Duplicate, &Dialog_Hierarchy::OnBnClickedDuplicate)
	ON_BN_CLICKED(ID_Delete, &Dialog_Hierarchy::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_Focus, &Dialog_Hierarchy::OnBnClickedButton1)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TreeList, &Dialog_Hierarchy::OnTvnBegindragTreelist)
	ON_BN_CLICKED(IDC_Composite, &Dialog_Hierarchy::OnBnClickedComposite)
	ON_BN_CLICKED(ID_Export, &Dialog_Hierarchy::OnBnClickedExport)
	ON_BN_CLICKED(ID_SetProto, &Dialog_Hierarchy::OnBnClickedSetproto)
END_MESSAGE_MAP()


// ���� ������Ʈ ��ȯ
GameObject* Dialog_Hierarchy::GetSelectObject()
{
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"��ü�� ������ �ּ���.", L"�ߺ�-");
		return nullptr;
	}
	return (GameObject*)m_TreeList.GetItemData(item);
}

// ������Ʈ->��� �׸� �߰�
void Dialog_Hierarchy::AddTreeItem(GameObject* pObject)
{
	auto parent = m_TreeList.InsertItem(pObject->m_myName.c_str(), 0, 0, TVI_ROOT, TVI_LAST);
	m_TreeList.SetItemData(parent, (DWORD_PTR)pObject);
	// �ڽ� ��ȸ
	AddTreeNode(pObject, parent, 1);
}
// �ڽ� ��� �߰�
void Dialog_Hierarchy::AddTreeNode(GameObject* pObject, const HTREEITEM& parent, const int& level)
{
	for (auto& iter : *pObject->GetChildList())
	{
		auto ppparent = m_TreeList.InsertItem(iter->m_myName.c_str(), 0, level, parent, TVI_SORT);
		m_TreeList.SetItemData(ppparent, (DWORD_PTR)iter);

		if (!iter->GetChildList()->empty())
			AddTreeNode(iter, ppparent, level + 1);
	}
}

// ������Ʈ ������ ���
void Dialog_Hierarchy::ExportObject(const filesystem::path fpath, GameObject* pObject)
{
	ofstream outStream(fpath);
	if (!outStream.is_open())
	{
		MessageBox(L" : Export Error!", fpath.c_str());
		return;
	}
	//outStream << "������ Ȯ�ο� ���� ��� �Դϴ�." << endl;
	ExportChild(outStream, pObject);

	MessageBox(L"����� �Ϸ�Ǿ����ϴ�.", fpath.c_str());
}
// �ڽ� ��� ���
void Dialog_Hierarchy::ExportChild(ofstream& outStream, GameObject* pObject)
{
	//outStream << "Name " << WCharToChar(pObject->m_myName.c_str()) << " " << endl;
	//outStream.write(reinterpret_cast<const char*>((Transform*)pObject), sizeof(Transform)) << endl;
	
	for (auto&[type, components] : pObject->GetComponentList())
	{
		switch (type)
		{
		 case EComponent::Renderer:
		 {
		 	for (auto& outIter : components)
		 	{
		 		switch (((Renderer*)outIter)->m_eRenderType)
		 		{
		 		 case ERenderType::Basic:
		 		 {
		 		 	//for (auto& iter : ((Renderer*)outIter)->m_vertexList)
		 		 	//{
		 		    //	 outStream << "Position : "
		 		    //		 << iter.pos.x << " "
		 		    //		 << iter.pos.y << " "
		 		    //		 << iter.pos.z << ", "
		 		    //		 << "Normal : "
		 		    //		 << iter.nor.x << " "
		 		    //		 << iter.nor.y << " "
		 		    //		 << iter.nor.z << ", "
		 		    //		 << "Texture : "
		 		    //		 << iter.tex.x << " "
		 		    //		 << iter.tex.y << endl;
		 		 	//}
		 		 }	break;
		 		 case ERenderType::Particle:
		 		 {
		 		 	auto pParticle = (ParticleSystem*)outIter;
		 		 	outStream << "Particle " << WCharToChar(pObject->m_myName.c_str()) << " " << WCharToChar(pParticle->m_srcName.c_str()) << endl;
					outStream.write(reinterpret_cast<const char*>((ParticleInfo*)pParticle), sizeof(ParticleInfo));
		 		 }	break;
		 		}
		 	}
		 }
		}
	}

	//if (auto pRenderer = pObject->GetComponentList(EComponent::Animation);
	//	pRenderer != nullptr)
	//{
	//	for (auto& outIter : *pRenderer)
	//	{
	//		for (auto& anim : ((CAnimation*)outIter)->m_Animations)
	//		{
	//			outStream << "Anim Pos : "
	//				<< anim.m_position.x << " "
	//				<< anim.m_position.y << " "
	//				<< anim.m_position.z << ", "
	//				<< endl
	//				<< "Anim Rot : "
	//				<< anim.m_rotation.x << " "
	//				<< anim.m_rotation.y << " "
	//				<< anim.m_rotation.z << " "
	//				<< anim.m_rotation.w << ", "
	//				<< endl
	//				<< "Anim Sca : "
	//				<< anim.m_scale.x << " "
	//				<< anim.m_scale.y << " "
	//				<< anim.m_scale.z << " "
	//				<< endl << endl;
	//			//<< "Anim Axi : " 
	//			//<< anim.matScaleAxis << endl << endl;
	//		}
	//	}
	//}

	for (auto pChild : pObject->m_childList)
	{
		ExportChild(outStream, pChild);
	}
}


// ============================ Dialog_Preset �޽��� ó���� ===========================


// ����Ʈ Ŭ��
void Dialog_Hierarchy::OnTvnSelchangedTreetlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	auto curItem = pNMTreeView->itemNew.hItem;
	auto pCurObject = (GameObject*)m_TreeList.GetItemData(curItem);

	// ��ü ���� ����
	if (curItem == NULL)
	{
		MessageBox(L"�߸��� ����", L"�ߺ�-");
		return;
	}
	Dialog_Inspector::Instance->UpdateObjectData(pCurObject);
	// ���� ��ư Ȱ��ȭ ó��
	if (pCurObject->isGlobal())
		m_RemoveButton.EnableWindow(0);
	else
		m_RemoveButton.EnableWindow(1);

	// ��� �̵�:����
	if (m_ClickItem != NULL)
	{
		if (m_ClickItem == NULL)
		{
			MessageBox(L"�߸��� ����.", L"�ߺ�-");
			return;
		}
		if (auto parent = m_TreeList.GetNextItem(m_ClickItem, TVGN_PARENT);
			parent == curItem || m_ClickItem == curItem)
		{
			MessageBox(L"�ǹ� ���� �׸� ����.", L"�ߺ�-");
			return;
		}

		// �ڽĵ� ��ü
		auto pObject = (GameObject*)m_TreeList.GetItemData(m_ClickItem);

		// �ڽ����� �˻�
		auto pParent = pCurObject;
		while (pParent->m_pParent != nullptr)
		{
			pParent = pParent->m_pParent;
			if (pParent == pObject)
			{
				MessageBox(L"�ڽ� ��ü�� �̵��� �� �����ϴ�.", L"�ߺ�-");
				return;
			}
		}

		auto item = m_TreeList.InsertItem(pObject->m_myName.c_str(), 0, 1, curItem, TVI_LAST);
		m_TreeList.SetItemData(item, (DWORD_PTR)pObject);

		pObject->SetParent(pCurObject);

		// ������ �߰�
		AddTreeNode(pObject, item, 2);
		// ���� �׸� ����
		m_TreeList.DeleteItem(m_ClickItem);

		m_ClickItem = NULL;
	}


	*pResult = 0;
	pNMTreeView;
}

// ����Ʈ �巡��
void Dialog_Hierarchy::OnTvnBegindragTreelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// ����Ʈ �巡�� ����
	m_ClickItem = NULL;
	*pResult = 0;	pNMHDR;
}


// ���� ��ư
void Dialog_Hierarchy::OnBnClickedDuplicate()
{
	// ����
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"��ü�� ������ �ּ���.", L"�ߺ�-");
		return;
	}
	auto pClone = ((GameObject*)m_TreeList.GetItemData(item))->clone();
	pClone->m_myName = pClone->m_myName + L"(copy)";
	pClone->CutParent();
	ObjectManager::GetInstance().PushObject(pClone);
	// Ʈ�� �׸� �߰�
	AddTreeItem(pClone);
}

// ���� ��ư
void Dialog_Hierarchy::OnBnClickedDelete()
{
	// ����
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"��ü�� ������ �ּ���.", L"�ߺ�-");
		return;
	}
	Dialog_Inspector::Instance->UpdateObjectData(nullptr);
	auto pObject = (GameObject*)m_TreeList.GetItemData(item);

	ObjectManager::GetInstance().RemoveObject(pObject);
	m_TreeList.DeleteItem(item);
}

// ��Ŀ�� ��ư
void Dialog_Hierarchy::OnBnClickedButton1()
{
	// ��ü�� ��Ŀ�� ����
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"��ü�� ������ �ּ���.", L"�ߺ�-");
		return;
	}
	auto pObject = (GameObject*)m_TreeList.GetItemData(item);

	ObjectManager::Cameras[ECamera::Main]->SetPosition(pObject->GetWorldPosition());
	ObjectManager::Cameras[ECamera::Main]->Translate(ObjectManager::Cameras[ECamera::Main]->GetForward() * -(13.0f + D3DXVec3Length(&pObject->GetScale()) / 3));
}

// ���� ��ư
void Dialog_Hierarchy::OnBnClickedComposite()
{
	// ��ü
	if (m_ClickItem != NULL)
	{
		m_ClickItem = NULL;
		return;
	}
	m_ClickItem = m_TreeList.GetSelectedItem();
	if (m_ClickItem == NULL)
	{
		MessageBox(L"��ü�� ������ �ּ���.", L"�ߺ�-");
		return;
	}
}

// �������� ��ư
void Dialog_Hierarchy::OnBnClickedExport()
{
	// ��ü ������ ����
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"��ü�� ������ �ּ���.", L"�ߺ�-");
		return;
	}
	auto pObject = (GameObject*)m_TreeList.GetItemData(item);

	CFileDialog fileDia(false, L"", (pObject->m_myName + L".eff").c_str());				// true = �ҷ�����, false = �����ε�
	if (fileDia.DoModal() != IDOK)
		return;

	std::filesystem::path fpath;
	fpath = fileDia.GetPathName().GetBuffer();

	std::thread exporter(&Dialog_Hierarchy::ExportObject, this, fpath, pObject);
	exporter.detach();
}


// ��� ��ư
void Dialog_Hierarchy::OnBnClickedSetproto()
{
	// ��ü ������ ����
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"��ü�� ������ �ּ���.", L"�ߺ�-");
		return;
	}
	auto pObject = ((GameObject*)m_TreeList.GetItemData(item))->clone();
	
	if (! ObjectManager::GetInstance().SetProtoObject(pObject))
	{
		MessageBox(L"�̹� ��ϵ� �̸� �Դϴ�.", L"�ߺ�-");
		return;
	}
	Dialog_Preset::Instance->m_ListBox.AddString(pObject->m_myName.data());
}