#include "stdafx.h"
#include "GlobalEx.h"
#include <filesystem>


bool GlobalEx::Init(Interface* pInter) noexcept
{
	m_pInter = pInter;
	m_pRoot = pInter->GetRootNode();

	// Scene 정보 입력
	m_Interval = m_pInter->GetAnimRange();
	
	//m_Scene.FrameSpeed	 = GetFrameRate();
	m_Scene.TickPerFrame = GetTicksPerFrame();

	m_Scene.FirstFrame = m_Interval.Start() / m_Scene.TickPerFrame;
	m_Scene.LastFrame = m_Interval.End() / m_Scene.TickPerFrame;

	m_Scene.TotalFrame = (m_Interval.End() - m_Interval.Start()) / m_Scene.TickPerFrame;
	m_Scene.SPF = 1.0f / GetFrameRate();

	PreProcess(m_pRoot);

	//m_Scene.MeshCount = ;
	m_Scene.MaxWeight = 1;
	m_Scene.BindPose = false;

	// 데이터 변환
	MtlToMaterials();
	MatToObjects();
	TriangleToVertexIndex();
	// 애니메이션 저장
	AnimationSetting();
	return true;
}


void GlobalEx::PreProcess(INode* pNode)
{
	AddObject(pNode);
	AddMaterial(pNode);

	// 재귀호출로 모든 노드를 추가
	int numChild = pNode->NumberOfChildren();
	for (int i = 0; i < numChild; ++i)
	{
		INode* pChild = pNode->GetChildNode(i);
		PreProcess(pChild);
	}
}

void GlobalEx::AddObject(INode* pNode)
{
	// 오브젝트 추가
	ObjectState objState = pNode->EvalWorldState(m_Interval.Start());

	if (objState.obj != nullptr)
	{
		// 타켓 클래스인 오브젝트는 불필요
		if (objState.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
		{
			return;
		}
		// Geom과 Helper 오브젝트만 추가
		switch (objState.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
		case HELPER_CLASS_ID:
		{
			//int
			MSTR nodeName = pNode->GetName();
			nodeName.Replace(L" ", L"_");
			m_Matrixs[nodeName.data()];
			auto pMat = &m_Matrixs[nodeName.data()];
			
			pMat->Name = nodeName;
			//m_Matrixs[nodeName].Index
			pMat->pNode = pNode;
			pMat->pObject = pNode->GetObjectRef();
			pMat->pControl = pNode->GetTMController();
			pMat->ClassType = EClassType::Geom;

			if (pMat->pObject != nullptr)
			{
				// 본 오브젝트라면
				if(pMat->pObject->ClassID() == Class_ID(BONE_CLASS_ID, 0))
					pMat->ClassType = EClassType::Bone;
				// 더미라면
				if(pMat->pObject->ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
					pMat->ClassType = EClassType::Dummy;
				if (pMat->pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||		// 루트 제외 바이패드들
					pMat->pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID)			// 루트 바이패드
					pMat->ClassType = EClassType::Biped;
			}
			pMat->NodeTM = pNode->GetNodeTM(m_Interval.Start());
			pMat->InvNodeTM = Inverse(pMat->NodeTM);

		}	break;
		}
	}
}

void GlobalEx::AddMaterial(INode* pNode)
{
	auto pMtl = pNode->GetMtl();
	if (pMtl != nullptr)
	{
		for (int i = 0; i < m_MtlList.size(); ++i)
		{
			if (m_MtlList[i] == pMtl)
				return;
		}
		m_MtlList.push_back(pMtl);
	}
}

//int GlobalEx::GetMaterialRef() {}

//void GlobalEx::SetBindPose(const bool& useBindPose = false)
//{
//
//}


void GlobalEx::MtlToMaterials()
{
	MSTR str;
	for (int iMtl = 0; iMtl < m_MtlList.size(); ++iMtl)
	{
		Mtl* pMtl = m_MtlList[iMtl];

		MaterialData Parent;
		Parent.MaterialIndex = iMtl;
		str = pMtl->GetName();
		str.Replace(L" ", L"_");
		Parent.MaterialName = str.data();
		pMtl->GetClassName(str);
		str.Replace(L" ", L"_");
		Parent.MaterialClassName = str.data();
		
		if(pMtl->NumSubMtls() >= 1)
			AddSubMaterial(&Parent, m_MtlList[iMtl]);
		if (pMtl->NumSubTexmaps() >= 1)
			AddSubTexture(&Parent, pMtl);
		m_Materials.push_back(std::move(Parent));
	}
}

void GlobalEx::AddSubMaterial(MaterialData* pParent, Mtl* pMtl)
{
	MSTR str;
	for (int iSub = 0; iSub < pMtl->NumSubMtls(); ++iSub)
	{
		auto pSubMtl = pMtl->GetSubMtl(iSub);

		MaterialData Parent;
		Parent.MaterialIndex = iSub;
		str = pSubMtl->GetName();
		str.Replace(L" ", L"_");
		Parent.MaterialName = str.data();
		pSubMtl->GetClassName(str);
		str.Replace(L" ", L"_");
		Parent.MaterialClassName = str.data();

		if (pSubMtl->NumSubMtls() >= 1)
			AddSubMaterial(&Parent, pSubMtl);
		if (pSubMtl->NumSubTexmaps() >= 1)
			AddSubTexture(&Parent, pSubMtl);
		pParent->SubMaterials.push_back(std::move(Parent));
	}
}

void GlobalEx::AddSubTexture(MaterialData* pParent, Mtl* pMtl)
{
	for (int iSub = 0; iSub < pMtl->NumSubTexmaps(); ++iSub)
	{
		auto pTexMap = pMtl->GetSubTexmap(iSub);
		if (pTexMap == nullptr)
			continue;
		if (pTexMap != nullptr)
		{
			if (pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
			{
				StdMat* std = (StdMat*)pMtl;
				if (!std->MapEnabled(iSub))
					continue;
			}
		}

		TexInfo texInfo;
		texInfo.Index = iSub;
		//texInfo.Type = iSub;
		
		if (pTexMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
		{
			filesystem::path mapName = ((BitmapTex*)pTexMap)->GetMapName();
			MSTR str = mapName.filename().c_str();
			str.Replace(L" ", L"_");
			texInfo.TexName = str.data();
		}
		
		pParent->TexInfoList.push_back(texInfo);
	}
}



void GlobalEx::MatToObjects()
{
	MSTR str;
	for (auto&[name, MatIn] : m_Matrixs)
	{
		INode* pNode = MatIn.pNode;
		ObjectData object;
		object.ClassType = MatIn.ClassType;
		// 노드 이름
		str = pNode->GetName();
		str.Replace(L" ", L"_");
		object.ObjName = str.data();
		// 부모 노드 이름
		auto pParent = pNode->GetParentNode();
		if (pParent != nullptr && !pParent->IsRootNode())
		{
			str = pParent->GetName();
			str.Replace(L" ", L"_");
			object.ParentName = str.data();
		}
		// NodeTM 입력
		//auto matrix = pNode->GetNodeTM(m_Interval.Start());
		object.matMaxLocal = MatIn.NodeTM * Inverse(pNode->GetParentTM(m_Interval.Start()));
		TransD3DStruct(&object.matLocal, object.matMaxLocal);
		TransD3DStruct(&object.NodeTM, MatIn.NodeTM);
		// 메쉬 입력
		SetObjectMesh(pNode, &object);
		
		m_Objects.emplace(object.ObjName, std::move(object));
	}
}

void GlobalEx::SetObjectMesh(INode* pNode, ObjectData* pObject)
{
	Matrix3 matTM = pNode->GetObjTMAfterWSM(m_Interval.Start());
	// 작업중 or 작업완료 파이프라인 결과에 엑세스 함으로 현재 노드의 결과를 돌려줌..
	ObjectState os = pNode->EvalWorldState(m_Interval.Start());
	if (os.obj == nullptr || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)
		return;

	bool needDelete;
	TriObject* pTri = GetTriObjectFromNode(pNode, m_Interval.Start(), &needDelete);
	if (pTri == nullptr)
		return;
	
	Mesh* pMesh = &pTri->GetMesh();
	// 바운딩 박스
	pMesh->buildBoundingBox();
	Box3 box = pMesh->getBoundingBox(&matTM);
	TransD3DStruct(&pObject->BoundBox.pmax, box.pmax);
	TransD3DStruct(&pObject->BoundBox.pmin, box.pmin);

	int v0, v1, v2;
	// NegativeScale 검사
	if (DotProd(CrossProd(matTM.GetRow(0), matTM.GetRow(1)), matTM.GetRow(2)) < 0.0)
	{
		v0 = 2;		v1 = 1; 	v2 = 0;
	}
	else
	{
		v0 = 0;		v1 = 1;		v2 = 2;
	}

	pObject->Triangles.resize(pMesh->getNumFaces());
	int size = (int)pObject->Triangles.size();
	// 트라이앵글 정점 채우기
	for (int i = 0; i < size; ++i)
	{
		Matrix3 matInverse = Inverse(pNode->GetNodeTM(m_Interval.Start()));

		Point3 v;

		if (pMesh->getNumVerts() >= 1)
		{
			v = pMesh->verts[pMesh->faces[i].v[v0]] * matTM * matInverse;
			TransD3DStruct(&pObject->Triangles[i].ver[0].pos, v);

			v = pMesh->verts[pMesh->faces[i].v[v2]] * matTM * matInverse;
			TransD3DStruct(&pObject->Triangles[i].ver[1].pos, v);

			v = pMesh->verts[pMesh->faces[i].v[v1]] * matTM * matInverse;
			TransD3DStruct(&pObject->Triangles[i].ver[2].pos, v);
		}

		// 텍스처 채우기
		if (pMesh->getNumTVerts() > 0)
		{
			pObject->Triangles[i].ver[0].tex.x = pMesh->tVerts[pMesh->tvFace[i].t[v0]].x;
			pObject->Triangles[i].ver[0].tex.y = 1.0f - pMesh->tVerts[pMesh->tvFace[i].t[v0]].y;
			pObject->Triangles[i].ver[1].tex.x = pMesh->tVerts[pMesh->tvFace[i].t[v2]].x;
			pObject->Triangles[i].ver[1].tex.y = 1.0f - pMesh->tVerts[pMesh->tvFace[i].t[v2]].y;
			pObject->Triangles[i].ver[2].tex.x = pMesh->tVerts[pMesh->tvFace[i].t[v1]].x;
			pObject->Triangles[i].ver[2].tex.y = 1.0f - pMesh->tVerts[pMesh->tvFace[i].t[v1]].y;
		}
		
		pObject->Triangles[i].ver[0].col = Point4{ 1, 1, 1, 1 };
		pObject->Triangles[i].ver[1].col = Point4{ 1, 1, 1, 1 };
		pObject->Triangles[i].ver[2].col = Point4{ 1, 1, 1, 1 };

		// 컬러 채우기
		if (pMesh->getNumVertCol() > 0)
		{
			pObject->Triangles[i].ver[0].col.x = pMesh->vertCol[pMesh->vcFace[i].t[v0]].x;
			pObject->Triangles[i].ver[0].col.y = pMesh->vertCol[pMesh->vcFace[i].t[v0]].y;
			pObject->Triangles[i].ver[0].col.z = pMesh->vertCol[pMesh->vcFace[i].t[v0]].z;
			pObject->Triangles[i].ver[0].col.w = 1.0f;
			pObject->Triangles[i].ver[1].col.x = pMesh->vertCol[pMesh->vcFace[i].t[v2]].x;
			pObject->Triangles[i].ver[1].col.y = pMesh->vertCol[pMesh->vcFace[i].t[v2]].y;
			pObject->Triangles[i].ver[1].col.z = pMesh->vertCol[pMesh->vcFace[i].t[v2]].z;
			pObject->Triangles[i].ver[1].col.w = 1.0f;
			pObject->Triangles[i].ver[2].col.x = pMesh->vertCol[pMesh->vcFace[i].t[v1]].x;
			pObject->Triangles[i].ver[2].col.y = pMesh->vertCol[pMesh->vcFace[i].t[v1]].y;
			pObject->Triangles[i].ver[2].col.z = pMesh->vertCol[pMesh->vcFace[i].t[v1]].z;
			pObject->Triangles[i].ver[2].col.w = 1.0f;
		}

		pMesh->buildNormals();
		// 노말 정점 채우기
		int vert = pMesh->faces[i].getVert(v0);
		Point3 vNormal = GetVertexNormal(pMesh, i, pMesh->getRVertPtr(vert));
		TransD3DStruct(&pObject->Triangles[i].ver[0].nor, vNormal);

		vert = pMesh->faces[i].getVert(v2);
		vNormal = GetVertexNormal(pMesh, i, pMesh->getRVertPtr(vert));
		TransD3DStruct(&pObject->Triangles[i].ver[1].nor, vNormal);

		vert = pMesh->faces[i].getVert(v1);
		vNormal = GetVertexNormal(pMesh, i, pMesh->getRVertPtr(vert));
		TransD3DStruct(&pObject->Triangles[i].ver[2].nor, vNormal);
		
		// 서브 메테리얼 인덱스
		pObject->Triangles[i].ver[0].col.w  
			= pObject->Triangles[i].ver[1].col.w  
			= pObject->Triangles[i].ver[2].col.w 
			= pMesh->faces[i].getMatID();
	}

	if (needDelete)
	{
		delete pTri;
	}
	// 메테리얼 참조값 얻기
	pObject->MaterialRef = 0;
	for (WORD ref = 0; ref < m_MtlList.size(); ++ref)
	{
		if (m_MtlList[ref] == pNode->GetMtl())
		{
			pObject->MaterialRef = ref;
			break;
		}
	}
}

void GlobalEx::TriangleToVertexIndex()
{
	size_t vertex = 0;
	size_t vertexSize = 0;
	for (auto&[name, object] : m_Objects)
	{
		size_t tempSize = object.Triangles.size();
		for (size_t temp = 0; temp < tempSize; ++temp)
		{
			bool notFound = false;
			for (int iTry = 0; iTry < 3; ++iTry)
			{
				notFound = true;
				vertexSize = object.VertexList.size();
				for (vertex = 0; vertex < vertexSize; ++vertex)
				{
					if (object.Triangles[temp].ver[iTry] == object.VertexList[vertex])
					{
						object.IndexList.push_back((WORD)vertex);
						notFound = false;
						break;
					}
				}

				if (notFound)
				{
					object.VertexList.emplace_back(std::move(object.Triangles[temp].ver[iTry]));
					object.IndexList.push_back((WORD)object.VertexList.size() - 1);
				}
			}
		}
	}
	// 월드 정점 -> 로컬화
	//for (auto&[name, object] : m_Objects)
	//{
	//	Matrix3 matNoSR = object.NodeTM;
	//	matNoSR.NoTrans();
	//	matNoSR.NoScale();
	//	for (auto& iter : object.VertexList)
	//	{
	//		iter.pos = Inverse(object.NodeTM) * iter.pos;
	//		iter.nor = Inverse(matNoSR) * iter.nor;
	//	}
	//}
}

TriObject* GlobalEx::GetTriObjectFromNode(INode* pNode, const TimeValue& t, bool* neenDelete)
{
	*neenDelete = false;
	Object* pObj = pNode->EvalWorldState(t).obj;
	if (pObj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject* pTri = (TriObject*)pObj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (pObj != pTri)
			*neenDelete = true;
		return pTri;
	}
	return nullptr;
}

Point3 GlobalEx::GetVertexNormal(Mesh* pMesh, const int& faceNo, RVertex* rv)
{
	Face* pFace = &pMesh->faces[faceNo];
	
	DWORD smGroup = pFace->smGroup;
	int numNormals = rv->rFlags & NORCT_MASK;
	Point3 vNormal;
	
	if (rv->rFlags & SPECIFIED_NORMAL)
	{
		vNormal = rv->rn.getNormal();
	}
	else if(numNormals != 0 && smGroup != 0)
	{
		if (numNormals == 1)
		{
			vNormal = rv->rn.getNormal();
		}
		else
		{
			for (int i = 0; i < numNormals; ++i)
			{
				if (rv->ern[i].getSmGroup() & smGroup)
				{
					vNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else
	{
		vNormal = pMesh->getFaceNormal(faceNo);
	}
	return vNormal;
}

void GlobalEx::TransD3DStruct(D3D_MATRIX* desc, const Matrix3& src)
{
	Point3 row;
	row = src.GetRow(0);
	desc->_11 = row.x;
	desc->_12 = row.z;
	desc->_13 = row.y;
	row = src.GetRow(2);
	desc->_21 = row.x;
	desc->_22 = row.z;
	desc->_23 = row.y;
	row = src.GetRow(1);
	desc->_31 = row.x;
	desc->_32 = row.z;
	desc->_33 = row.y;
	row = src.GetRow(3);
	desc->_41 = row.x;
	desc->_42 = row.z;
	desc->_43 = row.y;

	desc->_14 = desc->_24 = desc->_34 = 0.0f;
	desc->_44 = 1.0f;
}
void GlobalEx::TransD3DStruct(Matrix3* desc, const Matrix3& src)
{
	Point3 row;
	row = src.GetRow(0);
	desc->SetRow(0, { row.x, row.z, row.y });
	row = src.GetRow(2);
	desc->SetRow(1, { row.x, row.z, row.y });
	row = src.GetRow(1);
	desc->SetRow(2, { row.x, row.z, row.y });
	row = src.GetRow(3);
	desc->SetRow(3, { row.x, row.z, row.y });
}
void GlobalEx::TransD3DStruct(Point3* desc, const Point3& src)
{
	desc->x = src.x;
	desc->y = src.z;
	desc->z = src.y;
}

bool GlobalEx::EqualPoint(const Point2& point1, const Point2& point2)
{
	if (fabs(point1.x - point2.x) <= EPSILON &&
		fabs(point1.y - point2.y) <= EPSILON)
	{
		return true;
	}
	return false;
}
bool GlobalEx::EqualPoint(const Point3& point1, const Point3& point2)
{
	if (fabs(point1.x - point2.x) <= EPSILON &&
		fabs(point1.y - point2.y) <= EPSILON &&
		fabs(point1.z - point2.z) <= EPSILON )
	{
		return true;
	}
	return false;
}
bool GlobalEx::EqualPoint(const Point4& point1, const Point4& point2)
{
	if (fabs(point1.x - point2.x) <= EPSILON &&
		fabs(point1.y - point2.y) <= EPSILON &&
		fabs(point1.z - point2.z) <= EPSILON &&
		fabs(point1.w - point2.w) <= EPSILON)
	{
		return true;
	}
	return false;
}
bool GlobalEx::EqualPoint(const Quat& point1, const Quat& point2)
{
	if (fabs(point1.x - point2.x) <= EPSILON &&
		fabs(point1.y - point2.y) <= EPSILON &&
		fabs(point1.z - point2.z) <= EPSILON &&
		fabs(point1.w - point2.w) <= EPSILON)
	{
		return true;
	}
	return false;
}

Point3 GlobalEx::Lerp(const Point3& start, const Point3& end, const float& time)
{
	return (1 - time) * start + time * end;
}
Quat GlobalEx::Lerp(const Quat& start, const Quat& end, const float& time)
{
	return (1 - time) * start + time * end;
}