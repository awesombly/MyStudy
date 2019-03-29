#pragma once
#include "stdafx.h"
#include "ObjectEx.h"

class WriterEx : public ObjectEx
{
protected:
	wofstream		 m_ofStream;
	filesystem::path m_filePath;
protected:
	//void PreProcess(INode* pNode) noexcept;		// INode ��ȸ
	void ExportScene();
	void ExportMaterial();
	void ExportObject();
private:
	void ExportSubMaterial(const MaterialData& parent);
public:
	bool Export()  noexcept;						// ���� ���

	bool Init(const filesystem::path& filePath, Interface* pInter) noexcept; // �ʱ�ȭ
	bool Release() noexcept;						// ����
public:
	WriterEx() = default;
	virtual ~WriterEx() = default;
};

