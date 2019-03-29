#include "stdafx.h"
#include "WriterEx.h"






bool WriterEx::Init(const filesystem::path& filePath, Interface* pInter) noexcept
{
	m_filePath = filePath;
	m_ofStream.open(m_filePath);
	if (!m_ofStream.is_open())
	{
		OutputDebugString(L"파일 열기 실패!");
		return false;
	}

	return GlobalEx::Init(pInter);
}

bool WriterEx::Export()  noexcept
{
	m_ofStream.setf(ios_base::fixed);
	m_ofStream.precision(4);
	// 씬, 메테리얼, 오브젝트 정보 출력
	ExportScene();
	ExportMaterial();
	ExportObject();

	MessageBox(GetActiveWindow(), L"파일을 출력하였습니다.", m_filePath.c_str(), MB_OK);
	return true;
}

void WriterEx::ExportScene()
{
	m_ofStream << "Scene "
		<< m_Scene.SPF << " "
		<< m_Scene.TotalFrame << " " 
		<< m_Scene.MeshCount << " " 
		<< m_Scene.MaxWeight << " " 
		<< m_Scene.BindPose << endl;
}

void WriterEx::ExportMaterial()
{
	m_ofStream << "Material" << " " << m_Materials.size() << endl;
	for (auto& iter : m_Materials)
	{
		m_ofStream << iter.MaterialIndex << " "
			<< iter.SubMaterials.size() << " "
			//<< iter.MaterialName << " "
			//<< iter.MaterialClassName << " "
			<< iter.TexInfoList.size();
		for (int i = 0; i < iter.TexInfoList.size(); ++i)
		{
			m_ofStream << " " << iter.TexInfoList[i].TexName;
		}
		m_ofStream << endl;
		if(iter.SubMaterials.size() >= 1)
			ExportSubMaterial(iter);
	}
}

void WriterEx::ExportSubMaterial(const MaterialData& parent)
{
	for (auto& iter : parent.SubMaterials)
	{
		m_ofStream << iter.MaterialIndex << " "
			<< iter.SubMaterials.size() << " "
			//<< iter.MaterialName << " "
			//<< iter.MaterialClassName << " "
			<< iter.TexInfoList.size();
		for (int i = 0; i < iter.TexInfoList.size(); ++i)
		{
			m_ofStream << " " << iter.TexInfoList[i].TexName;
		}
		m_ofStream << endl;
		if (iter.SubMaterials.size() >= 1)
			ExportSubMaterial(iter);
	}
}

void WriterEx::ExportObject()
{
	m_ofStream << "Mesh " << m_Objects.size() << endl;
	for (auto& [name, object] : m_Objects)
	{
		/*switch (object.ClassType)
		{
		case EClassType::Geom:
		{	
			m_ofStream << "Geom ";
		}	break;
		case EClassType::Dummy:
		{
			m_ofStream << "Dummy ";
		}	break;
		case EClassType::Bone:
		{
			m_ofStream << "Bone";
		}	break;
		case EClassType::Biped:
		{
			m_ofStream << "Biped ";
		}	break;
		}*/
		if (object.MaterialRef >= m_Materials.size())
			object.MaterialRef = 0;
		m_ofStream << (int)object.ClassType << " "
			<< object.MaterialRef << " "
			<< object.ObjName << " "
			<< object.ParentName << endl;
		m_ofStream
			<< object.matLocal._11 << " " << object.matLocal._12 << " " << object.matLocal._13 << " " << object.matLocal._14 << " "
			<< object.matLocal._21 << " " << object.matLocal._22 << " " << object.matLocal._23 << " " << object.matLocal._24 << " "
			<< object.matLocal._31 << " " << object.matLocal._32 << " " << object.matLocal._33 << " " << object.matLocal._34 << " "
			<< object.matLocal._41 << " " << object.matLocal._42 << " " << object.matLocal._43 << " " << object.matLocal._44 << endl;
		m_ofStream << "Box "
			<< object.BoundBox.pmin.x << " "
			<< object.BoundBox.pmin.y << " "
			<< object.BoundBox.pmin.z << " "
			<< object.BoundBox.pmin.x << " "
			<< object.BoundBox.pmin.y << " "
			<< object.BoundBox.pmin.z << endl;
		m_ofStream << "Vertex " << object.VertexList.size() << endl;
		for (auto& iter : object.VertexList)
		{
			if (iter.col.w != 0.0f && iter.col.w >= m_Materials[object.MaterialRef].SubMaterials.size())
				iter.col.w = 0.0f;
			m_ofStream << iter.col.w << " "
				<< iter.pos.x << " "
				<< iter.pos.y << " "
				<< iter.pos.z << " "
				<< iter.nor.x << " "
				<< iter.nor.y << " "
				<< iter.nor.z << " "
				<< iter.col.x << " "
				<< iter.col.y << " "
				<< iter.col.z << " "
				<< iter.tex.x << " "
				<< iter.tex.y << endl;
		}
		m_ofStream << "Index " << object.IndexList.size();
		for (auto& iter : object.IndexList)
		{
			m_ofStream << " " << iter;
		}
		m_ofStream << endl;
		m_ofStream << "Anim " << object.Animations.size() << endl;

		for (auto& iter : object.Animations)
		{
			m_ofStream
				<< iter.position.x << " " << iter.position.y << " " << iter.position.z << " "
				<< iter.rotation.x << " " << iter.rotation.y << " " << iter.rotation.z << " " << iter.rotation.w << " "
				<< iter.scale.x << " " << iter.scale.y << " " << iter.scale.z << " "
				<< iter.matScaleAxis.GetRow(0).x << " " << iter.matScaleAxis.GetRow(0).y << " " << iter.matScaleAxis.GetRow(0).z << " "
				<< iter.matScaleAxis.GetRow(1).x << " " << iter.matScaleAxis.GetRow(1).y << " " << iter.matScaleAxis.GetRow(1).z << " "
				<< iter.matScaleAxis.GetRow(2).x << " " << iter.matScaleAxis.GetRow(2).y << " " << iter.matScaleAxis.GetRow(2).z << endl;
		}
	}
}

bool WriterEx::Release() noexcept
{
	m_ofStream.close();
	return true;
}