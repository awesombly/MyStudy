#pragma once
#include "stdafx.h"
#include "ObjectEx.h"

class WriterEx : public ObjectEx
{
protected:
	wofstream		 m_ofStream;
	filesystem::path m_filePath;
protected:
	//void PreProcess(INode* pNode) noexcept;		// INode 순회
	void ExportScene();
	void ExportMaterial();
	void ExportObject();
private:
	void ExportSubMaterial(const MaterialData& parent);
public:
	bool Export()  noexcept;						// 파일 출력

	bool Init(const filesystem::path& filePath, Interface* pInter) noexcept; // 초기화
	bool Release() noexcept;						// 해제
public:
	WriterEx() = default;
	virtual ~WriterEx() = default;
};

