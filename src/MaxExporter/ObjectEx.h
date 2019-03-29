#pragma once
#include "stdafx.h"
#include "GlobalEx.h"

class ObjectEx : public GlobalEx
{
public:
	TimeValue m_deltaTick;
protected:
	void AnimationSetting() noexcept override;
private:
	bool AnimationCheck(INode* pNode, bool* pPos, bool* pRot, bool* pScale) noexcept;
	bool GetDecompAffine(const TimeValue t, INode* pNode, AffineParts* pAffine, Point3* pRotAxis = nullptr, float* pRotAngle = nullptr) noexcept;
	void SetAnimData(RowObjectData* pData, ObjectData* pObject) noexcept;
	void SetAnimPosition(INode* pNode, RowObjectData* pData) noexcept;
	void SetAnimRotation(INode* pNode, RowObjectData* pData) noexcept;
	void SetAnimScale(INode* pNode, RowObjectData* pData) noexcept;
public:
	ObjectEx() = default;
	virtual ~ObjectEx() = default;
};