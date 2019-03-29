#include "stdafx.h"
#include "ObjectEx.h"




void ObjectEx::AnimationSetting() noexcept
{
	bool pos, rot, scale;

	Point3 normalPos   = { 0.0f, 0.0f, 0.0f };
	Quat   normalRot   = { 0.0f, 0.0f, 0.0f, 1.0f };
	Point3 normalScale = { 0.0f, 0.0f, 0.0f };

	m_deltaTick = m_Scene.TickPerFrame;
	for (auto& [name, mat] : m_Matrixs)
	{
		if (!m_Objects[name].matMaxLocal.IsIdentity())
		{
			// 첫번째 애니메이션
			AffineParts Affine;
			decomp_affine(m_Objects[name].matMaxLocal, &Affine);
			AnimData anim;
			anim.position = { Affine.t.x, Affine.t.z, Affine.t.y };
			anim.rotation = { Affine.q.x, Affine.q.z, Affine.q.y, Affine.q.w };
			anim.scale = { Affine.k.x, Affine.k.z, Affine.k.y };
			Matrix3 matScale;
			Affine.u.MakeMatrix(matScale);
			TransD3DStruct(&anim.matScaleAxis, matScale);
			m_Objects[name].Animations.emplace_back(std::move(anim));
		}

		if (AnimationCheck(mat.pNode, &pos, &rot, &scale))
		{
			// 애니메이션 추출
			if (pos)
			{
				SetAnimPosition(mat.pNode, &mat);
			}
			if (rot)
			{
				SetAnimRotation(mat.pNode, &mat);
			}
			if (scale)
			{
				SetAnimScale(mat.pNode, &mat);
			}

			SetAnimData(&mat, &m_Objects[name]);
		}
		//else
		//{
		//	AffineParts affine;
		//	GetDecompAffine(m_Interval.Start(), mat.pNode, &affine);
		//	AnimData anim;
		//	anim.position = { affine.t.x, affine.t.z, affine.t.y };
		//	anim.rotation = { affine.q.x, affine.q.z, affine.q.y, affine.q.w };
		//	anim.scale = { affine.k.x, affine.k.z, affine.k.y };
		//	Matrix3 matAxis;
		//	affine.u.MakeMatrix(matAxis);
		//	TransD3DStruct(&anim.matScaleAxis, matAxis);
		//
		//	if (EqualPoint(anim.position, normalPos) &&
		//		EqualPoint(anim.rotation, normalRot) &&
		//		EqualPoint(anim.scale, normalScale))
		//		continue;
		//
		//	m_Objects[name].Animations.emplace_back();
		//}
	}

	// + Visibility Track?
}

void ObjectEx::SetAnimData(RowObjectData* pData, ObjectData* pObject) noexcept
{
	WORD prePosIndex = 0, preRotIndex = 0, preScaleIndex = 0;
	pObject->Animations.resize(m_Scene.TotalFrame + 1);
	
	// Position 세팅
	bool isFirst = true;
	for (auto& iter : pData->PosTrack)
	{
		if (isFirst)
		{
			isFirst = false;
			continue;
		}
		pObject->Animations[iter.Frame].position = iter.vValue;

		// 보간 작업
		if (prePosIndex + 1 < iter.Frame)
		{
			WORD frameCnt = iter.Frame - prePosIndex;
			for (WORD inter = prePosIndex + 1; inter < iter.Frame; ++inter)
			{
				pObject->Animations[inter].position = Lerp(pObject->Animations[prePosIndex].position, pObject->Animations[iter.Frame].position, (float)inter / frameCnt);
			}
		}
		prePosIndex = iter.Frame;
	}
	// Position 프레임이 덜 나왔을시
	if (prePosIndex < m_Scene.TotalFrame)
	{
		for (int i = prePosIndex + 1; i <= m_Scene.TotalFrame; ++i)
		{
			pObject->Animations[i].position = pObject->Animations[prePosIndex].position;
		}
	}
	// Rotation 세팅
	isFirst = true;
	for (auto& iter : pData->RotTrack)
	{
		if (isFirst)
		{
			isFirst = false;
			continue;
		}
		pObject->Animations[iter.Frame].rotation = iter.qValue;

		// 보간 작업
		if (preRotIndex + 1 < iter.Frame)
		{
			WORD frameCnt = iter.Frame - preRotIndex;
			for (WORD inter = preRotIndex + 1; inter < iter.Frame; ++inter)
			{
				pObject->Animations[inter].rotation = Lerp(pObject->Animations[preRotIndex].rotation, pObject->Animations[iter.Frame].rotation, (float)inter / frameCnt);
			}
		}
		preRotIndex = iter.Frame;
	}
	// Rotation 프레임이 덜 나왔을시
	if (preRotIndex < m_Scene.TotalFrame)
	{
		for (int i = preRotIndex + 1; i <= m_Scene.TotalFrame; ++i)
		{
			pObject->Animations[i].rotation = pObject->Animations[preRotIndex].rotation;
		}
	}

	// Scale 세팅
	isFirst = true;
	for (auto& iter : pData->ScaleTrack)
	{
		if (isFirst)
		{
			isFirst = false;
			continue;
		}
		pObject->Animations[iter.Frame].scale = iter.vValue;
		iter.qValue.MakeMatrix(pObject->Animations[iter.Frame].matScaleAxis);

		// 보간 작업
		if (preScaleIndex + 1 < iter.Frame)
		{
			WORD frameCnt = iter.Frame - preScaleIndex;
			Quat lerpAxis;
			for (WORD inter = preScaleIndex + 1; inter < iter.Frame; ++inter)
			{
				pObject->Animations[inter].scale = Lerp(pObject->Animations[preScaleIndex].scale, pObject->Animations[iter.Frame].scale, (float)inter / frameCnt);
				pObject->Animations[inter].matScaleAxis = pObject->Animations[iter.Frame].matScaleAxis;
			}
		}
		preScaleIndex = iter.Frame;
	}
	// Scale 프레임이 덜 나왔을시
	if (preScaleIndex < m_Scene.TotalFrame)
	{
		for (int i = preScaleIndex + 1; i <= m_Scene.TotalFrame; ++i)
		{
			pObject->Animations[i].scale = pObject->Animations[preScaleIndex].scale;
			pObject->Animations[i].matScaleAxis = pObject->Animations[preScaleIndex].matScaleAxis;
		}
	}

}

void ObjectEx::SetAnimPosition(INode* pNode, RowObjectData* pData) noexcept
{
	TimeValue start = m_Interval.Start();
	TimeValue end = m_Interval.End();
	AffineParts affine;

	for (TimeValue t = start; t <= end; t += m_deltaTick)
	{
		GetDecompAffine(t, pNode, &affine);

		AnimTrack anim;
		anim.Frame = (WORD)(t / m_Scene.TickPerFrame);
		anim.vValue = { affine.t.x, affine.t.z, affine.t.y };
		pData->PosTrack.emplace_back(std::move(anim));
	}
}

void ObjectEx::SetAnimRotation(INode* pNode, RowObjectData* pData) noexcept
{
	TimeValue start = m_Interval.Start();
	TimeValue end = m_Interval.End();
	AffineParts affine;

	for (TimeValue t = start; t <= end; t += m_deltaTick)
	{
		GetDecompAffine(t, pNode, &affine);

		AnimTrack anim;
		anim.Frame = (WORD)(t / m_Scene.TickPerFrame);
		anim.qValue = { affine.q.x, affine.q.z, affine.q.y, affine.q.w };
		pData->RotTrack.emplace_back(std::move(anim));
	}
}

void ObjectEx::SetAnimScale(INode* pNode, RowObjectData* pData) noexcept
{
	TimeValue start = m_Interval.Start();
	TimeValue end = m_Interval.End();
	AffineParts affine;

	for (TimeValue t = start; t <= end; t += m_deltaTick)
	{
		GetDecompAffine(t, pNode, &affine);

		AnimTrack anim;
		anim.Frame = (WORD)(t / m_Scene.TickPerFrame);
		anim.vValue = { affine.k.x, affine.k.z, affine.k.y };
		anim.qValue = { affine.u.x, affine.u.z, affine.u.y, affine.u.w };
		pData->ScaleTrack.emplace_back(std::move(anim));
	}
}


bool ObjectEx::AnimationCheck(INode* pNode, bool* pPos, bool* pRot, bool* pScale) noexcept
{
	AffineParts affine;
	Point3 firstPos, firstRotAxis, firstScaleFactor;
	float firstRotAngle;
	// 첫 프레임의 SRT 값을 뽑음
	GetDecompAffine(m_Interval.Start(), pNode, &affine, &firstRotAxis, &firstRotAngle);
	firstPos	   = affine.t;
	firstScaleFactor = affine.k;
	
	TimeValue start = m_Interval.Start() + GetTicksPerFrame();
	TimeValue end = m_Interval.End();

	*pPos = *pRot = *pScale = false;
	float rotAngle;
	Point3 rotAxis;
	// 애니메이션 순회하며 검사 -> 첫 프레임 값과 다를시 애니메이션 존재.
	for (TimeValue t = start; t <= end; t += m_deltaTick)
	{
		GetDecompAffine(t, pNode, &affine, &rotAxis, &rotAngle);

		if (*pPos == false)
		{
			if (!EqualPoint(affine.t, firstPos))
			{
				*pPos = true;
			}
		}
		if (*pRot == false)
		{
			if(!EqualPoint(rotAxis, firstRotAxis) ||
			   fabs(rotAngle - firstRotAngle) > EPSILON)
			{
				*pRot = true;
			}
		}
		if (*pScale == false)
		{
			if (!EqualPoint(affine.k, firstScaleFactor))
			{
				*pScale = true;
			}
		}
		if (*pPos && *pRot && *pScale)
			break;
	}
	return (*pPos || *pRot || *pScale);
}

bool ObjectEx::GetDecompAffine(const TimeValue t, INode* pNode, AffineParts* pAffine, Point3* pRotAxis, float* pRotAngle) noexcept
{
	// 월드 행렬 * 부모 역행렬
	Matrix3 matTM = pNode->GetNodeTM(t) * Inverse(pNode->GetParentTM(t));
	decomp_affine(matTM, pAffine);

	if (pRotAngle != nullptr && pRotAxis != nullptr)
	{
		AngAxisFromQ(pAffine->q, pRotAngle, *pRotAxis);
	}
	return true;
}