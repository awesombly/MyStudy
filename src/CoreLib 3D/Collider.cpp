//#include "Collider.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"
#include "ObjectManager.h"

const float Collider::PushPower = 60.0f;


Collider::Collider()
{
	ObjectManager::Get().PushCollider(this);
	Init();
}

Collider::Collider(const float& radius)
	: m_radius(radius)
{
	ObjectManager::Get().PushCollider(this);
	Init();
}

Collider::~Collider()
{
	ObjectManager::Get().PopCollider(this, false);
}

bool Collider::Init() noexcept
{
	m_myName = L"CSphere";
	m_comptType = EComponent::Collider;
	m_eCollider = ECollider::Sphere;
	return true;
}

bool Collider::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable)	return false;

	// 충돌 체크
	CollisionAllCheck(spf);
	
	if (usePhysics())
	{
		// 최소 높이
		if (m_pParent->GetPosition().y < m_mapHeight)
		{
			isGround(true);
			m_pParent->SetPositionY(m_mapHeight);
			m_pPhysics->m_force -= m_pPhysics->m_force * m_pPhysics->m_drag;
			m_pPhysics->m_force.y *= -m_pPhysics->m_repulsion;
			if (CollisionEvent != nullptr)
				CollisionEvent(this, nullptr);
		}
	}
	return true;
	accTime;
}

bool Collider::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool Collider::Release()	noexcept
{
	return true;
}


bool Collider::CollisionAllCheck(const float& spf) noexcept
{
	// 제외할 충돌체들
	for (auto& iter : m_IgnoreList)
	{
		m_CollisionList.push_front(iter);
	} 
	m_CollisionList.push_front(this);

	for (auto& iter : ObjectManager::Get().GetColliderList())
	{
		if (iter->m_pParent == nullptr)
		{
			ErrorMessage(__FUNCTION__ + " -> Parent is Null!"s);
			continue;
		}
		// 자신 또는 처리된 객체 제외
		auto finder = std::find(m_CollisionList.begin(), m_CollisionList.end(), iter);
		if(finder != m_CollisionList.end())
			continue;

		if (m_eTagArray[iter->m_eTag] && iter->m_eTagArray[m_eTag])
		{
			if (CollisionCheck(iter))
			{
				// 상대 충돌 목록 추가
				iter->m_CollisionList.push_front(this);

				/////D3DXVECTOR3 vDirection		= Vector3::Zero;
				////D3DXVECTOR3 vForceDis		= Vector3::Zero;
				////D3DXVECTOR3 vForceDisOther	= Vector3::Zero;
				/////// 반작용
				/////vForceDis = iter->GetTotalForce();
				/////
				/////vForceDisOther = GetTotalForce();
				///////vDirection = iter->GetCenter() - GetCenter();
				///////D3DXVec3Normalize(&vDirection, &vDirection);
				///////vForceDisOther = -iter->GetTotalForce();
				///////vForceDisOther = D3DXVec3Dot(&vForceDis, &vDirection) * 2.0f * vDirection - vForceDis;
				/////
				/////
				///////vDirection = iter->GetCenter() - GetCenter();
				///////D3DXVec3Normalize(&vDirection, &vDirection);
				///////vForceDisOther = m_force - iter->m_force;
				///////vForceDisOther = vDirection * (m_mass / iter->m_mass) * D3DXVec3Length(&vForceDisOther);
				////vForceDis = iter->GetTotalForce();
				////vForceDisOther = GetTotalForce();
				////if (m_pPhysics->m_usePhysics)
				////{
				////	//m_force += vForceDis;
				////	//D3DXVec3Normalize(&vForceDis, &m_force);
				////	// 반발력
				////	m_pPhysics->m_force = /*vForceDis +*/ -m_pPhysics->m_force * (m_pPhysics->m_repulsion + iter->m_pPhysics->m_repulsion) * 0.5f;
				////	//m_pParent->Translate(-m_force * 0.001f);
				////	//m_pParent->Translate(Vector3::Up * spf);
				////}
				////if (iter->m_pPhysics->m_usePhysics)
				////{
				////	//iter->m_force += vForceDisOther;
				////	//D3DXVec3Normalize(&vForceDisOther, &iter->m_force);
				////	iter->m_pPhysics->m_force = /*vForceDisOther +*/ -iter->m_pPhysics->m_force * (m_pPhysics->m_repulsion + iter->m_pPhysics->m_repulsion) * 0.5f;
				////	//iter->m_pParent->Translate(-iter->m_force * 0.001f);
				////	//iter->m_pParent->Translate(Vector3::Up * spf);
				////}
				////// 마찰력
				////m_pPhysics->m_force		  -= m_pPhysics->m_force		* (m_pPhysics->m_drag + iter->m_pPhysics->m_drag) * 0.5f  * spf;
				////iter->m_pPhysics->m_force -= iter->m_pPhysics->m_force	* (m_pPhysics->m_drag + iter->m_pPhysics->m_drag) * 0.5f  * spf;
				////
				//////m_force = Vector3::Zero;
				//////iter->m_force = Vector3::Zero;

				if (usePhysics())// && iter->m_eTag != ETag::Dummy)
				{
					m_pPhysics->m_force = Normalize(GetCenter() - iter->GetCenter()) * (m_pPhysics->m_repulsion + iter->m_pPhysics->m_repulsion) * PushPower;
					if (m_pPhysics->m_isMoving)
						m_pParent->Translate(-m_pPhysics->m_direction * spf);
				}
				if (iter->usePhysics())// && m_eTag != ETag::Dummy)
				{
					iter->m_pPhysics->m_force = Normalize(iter->GetCenter() - GetCenter()) * (m_pPhysics->m_repulsion + iter->m_pPhysics->m_repulsion) * PushPower;
					if (m_pPhysics->m_isMoving)
						iter->m_pParent->Translate(-iter->m_pPhysics->m_direction * spf);
				}
				// 충돌 이벤트
				if (CollisionEvent != nullptr)
					CollisionEvent(this, iter);
				if (iter->CollisionEvent != nullptr)
					iter->CollisionEvent(iter, this);
			}
		}
	}
	return true;
	spf;
}


bool Collider::CollisionCheck(Collider* pCollider) noexcept
{
	if (!SphereToSphere(this, pCollider))
		return false;
	switch (pCollider->m_eCollider)
	{
	case ECollider::AABB:
	{
		if (SphereToAABB(this, (ColliderAABB*)pCollider))
			return true;
	}	break;
	case ECollider::OBB:
	{
		if (SphereToOBB(this, (ColliderOBB*)pCollider))
			return true;
	}	break;
	case ECollider::Sphere:
	{
		return true;
	}	break;
	}

	return false;
}


bool Collider::SphereToSphere(Collider* pA, Collider* pB) 	noexcept
{
	auto vRange = pA->GetCenter() - pB->GetCenter();
	float distance = D3DXVec3Length(&vRange);
	// 충돌
	if (distance < (pA->GetWorldRadius() + pB->GetWorldRadius()))
	{
		return true;
	}
	return false;
}

bool Collider::SphereToAABB(Collider* pA, ColliderAABB* pB)	 noexcept
{
	D3DXVECTOR3 nearPoint = Vector3::Zero;

	// x 비교
	if (pA->GetCenter().x < (nearPoint.x = pB->GetCenter().x - pB->GetLength().x * 0.5f))
	{	}
	else if (pA->GetCenter().x > (nearPoint.x = pB->GetCenter().x + pB->GetLength().x * 0.5f))
	{	}
	else
	{
		nearPoint.x = pA->GetCenter().x;
	}
	// y 비교
	if (pA->GetCenter().y < (nearPoint.y = pB->GetCenter().y - pB->GetLength().y * 0.5f))
	{	}
	else if (pA->GetCenter().y > (nearPoint.y = pB->GetCenter().y + pB->GetLength().y * 0.5f))
	{	}
	else
	{
		nearPoint.y = pA->GetCenter().y;
	}
	// z 비교
	if (pA->GetCenter().z < (nearPoint.z = pB->GetCenter().z - pB->GetLength().z * 0.5f))
	{	}
	else if (pA->GetCenter().z > (nearPoint.z = pB->GetCenter().z + pB->GetLength().z * 0.5f))
	{	}
	else
	{
		nearPoint.z = pA->GetCenter().z;
	}

	auto vDistance = pA->GetCenter() - nearPoint;
	if ((D3DXVec3Length(&vDistance) - pA->GetWorldRadius()) <= 0)
	{
		return true;
	}
	return false;
}

bool Collider::SphereToOBB(Collider* pA, ColliderOBB* pB)  noexcept
{
	// 최단점
	D3DXVECTOR3 nearPoint = Vector3::Zero;
	// 구 -> OBB 박스 좌표계 변환
	D3DXVECTOR3 tempCenter = pA->GetCenter() - pB->GetCenter();
	D3DXVECTOR3 newCenter = { D3DXVec3Dot(&tempCenter, &pB->m_rotate[0]),
							  D3DXVec3Dot(&tempCenter, &pB->m_rotate[1]),
							  D3DXVec3Dot(&tempCenter, &pB->m_rotate[2]) };
	newCenter += pB->GetCenter();

	// x 비교
	if (newCenter.x < (nearPoint.x = pB->GetCenter().x - pB->GetExtents().x))
	{	}
	else if (newCenter.x > (nearPoint.x = pB->GetCenter().x + pB->GetExtents().x))
	{	}
	else
	{
		nearPoint.x = newCenter.x;
	}
	// y 비교
	if (newCenter.y < (nearPoint.y = pB->GetCenter().y - pB->GetExtents().y))
	{	}
	else if (newCenter.y > (nearPoint.y = pB->GetCenter().y + pB->GetExtents().y))
	{	}
	else
	{
		nearPoint.y = newCenter.y;
	}
	// z 비교
	if (newCenter.z < (nearPoint.z = pB->GetCenter().z - pB->GetExtents().z))
	{	}
	else if (newCenter.z > (nearPoint.z = pB->GetCenter().z + pB->GetExtents().z))
	{	}
	else
	{
		nearPoint.z = newCenter.z;
	}

	auto vDistance = newCenter - nearPoint;
	if ((D3DXVec3Length(&vDistance) - pA->GetWorldRadius()) <= 0)
	{
		return true;
	}
	return false;
}

bool Collider::AABBToAABB(ColliderAABB* pA, ColliderAABB* pB)  noexcept
{
	// 충돌
	auto&& thisMin = pA->GetCenter() + pA->GetMin();
	auto&& thisMax = pA->GetCenter() + pA->GetMax();
	auto&& otherMin = pB->GetCenter() + pB->GetMin();
	auto&& otherMax = pB->GetCenter() + pB->GetMax();
	if (std::max<float>(thisMin.x, otherMin.x) < std::min<float>(thisMax.x, otherMax.x) &&
		std::max<float>(thisMin.y, otherMin.y) < std::min<float>(thisMax.y, otherMax.y) &&
		std::max<float>(thisMin.z, otherMin.z) < std::min<float>(thisMax.z, otherMax.z))
	{
		return true;
	}
	return false;
}

bool Collider::AABBToOBB(ColliderAABB* pA, ColliderOBB* pB)  noexcept
{
	auto CenterA = pA->GetCenter();
	auto CenterB = pB->GetCenter();
	auto ExtentsB = pB->GetExtents();
	D3DXVECTOR3 preMin = CenterA + pA->GetMin();
	D3DXVECTOR3 preMax = CenterA + pA->GetMax();
	// AABB -> OBB 좌표 변환(min)
	D3DXVECTOR3 tempMin = preMin - CenterB;
	D3DXVECTOR3 thisMin = {	  D3DXVec3Dot(&tempMin, &pB->m_pParent->GetRight()),
							  D3DXVec3Dot(&tempMin, &pB->m_pParent->GetUp()),
							  D3DXVec3Dot(&tempMin, &pB->m_pParent->GetForward()) };
	thisMin += CenterB;
	// AABB -> OBB 좌표 변환(max)
	D3DXVECTOR3 tempMax = preMax - CenterB;
	D3DXVECTOR3 thisMax = {   D3DXVec3Dot(&tempMax, &pB->m_pParent->GetRight()),
							  D3DXVec3Dot(&tempMax, &pB->m_pParent->GetUp()),
							  D3DXVec3Dot(&tempMax, &pB->m_pParent->GetForward()) };
	thisMax += CenterB;
	//
	auto&& otherMin = CenterB - ExtentsB;
	auto&& otherMax = CenterB + ExtentsB;
	if (std::max<float>(thisMin.x, otherMin.x) < std::min<float>(thisMax.x, otherMax.x) &&
		std::max<float>(thisMin.y, otherMin.y) < std::min<float>(thisMax.y, otherMax.y) &&
		std::max<float>(thisMin.z, otherMin.z) < std::min<float>(thisMax.z, otherMax.z))
	{
		return true;
	}
	return false;
}

bool Collider::OBBToOBB(ColliderOBB* pA, ColliderOBB* pB)  noexcept
{
	D3DXVECTOR3 v = pB->GetCenter() - pA->GetCenter();
	D3DXVECTOR3 T = { D3DXVec3Dot(&v, &pA->m_rotate[0]),
					  D3DXVec3Dot(&v, &pA->m_rotate[1]),
					  D3DXVec3Dot(&v, &pA->m_rotate[2]) };
	float R[3][3];
	float FR[3][3];
	float ra, rb, t;

	// 회전 행렬 계산
	for (int out = 0; out < 3; ++out)
	{
		for (int in = 0; in < 3; ++in)
		{
			R[out][in] = D3DXVec3Dot(&pA->m_rotate[out], &pB->m_rotate[in]);
			FR[out][in] = 1e-6f + abs(R[out][in]);
		}
	}

	// 모든 분리 축 검사
	D3DXVECTOR3 a = pA->GetExtents();
	D3DXVECTOR3 b = pB->GetExtents();

	//A's basis vectors
	for (int i = 0; i < 3; i++)
	{
		ra = a[i];
		rb = b.x * FR[i][0] + b.y * FR[i][1] + b.z * FR[i][2];
		t = abs(T[i]);

		if (t > ra + rb) return false;
	}

	//B's basis vectors
	for (int i = 0; i < 3; i++)
	{
		ra = a.x * FR[0][i] + a.y * FR[1][i] + a.z * FR[2][i];
		rb = b[i];
		t = abs(T[0] * R[0][i] + T[1] * R[1][i] + T[2] * R[2][i]);

		if (t > ra + rb) return false;
	}

	//L = A0 x B0
	ra = a[1] * FR[2][0] + a[2] * FR[1][0];
	rb = b[1] * FR[0][2] + b[2] * FR[0][1];
	t = abs(T[2] * R[1][0] - T[1] * R[2][0]);
	if (t > ra + rb) return false;

	//L = A0 x B1
	ra = a[1] * FR[2][1] + a[2] * FR[1][1];
	rb = b[0] * FR[0][2] + b[2] * FR[0][0];
	t = abs(T[2] * R[1][1] - T[1] * R[2][1]);
	if (t > ra + rb) return false;

	//L = A0 x B2
	ra = a[1] * FR[2][2] + a[2] * FR[1][2];
	rb = b[0] * FR[0][1] + b[1] * FR[0][0];
	t = abs(T[2] * R[1][2] - T[1] * R[2][2]);
	if (t > ra + rb) return false;

	//L = A1 x B0
	ra = a[0] * FR[2][0] + a[2] * FR[0][0];
	rb = b[1] * FR[1][2] + b[2] * FR[1][1];
	t = abs(T[0] * R[2][0] - T[2] * R[0][0]);
	if (t > ra + rb) return false;

	//L = A1 x B1
	ra = a[0] * FR[2][1] + a[2] * FR[0][1];
	rb = b[0] * FR[1][2] + b[2] * FR[1][0];
	t = abs(T[0] * R[2][1] - T[2] * R[0][1]);
	if (t > ra + rb) return false;

	//L = A1 x B2
	ra = a[0] * FR[2][2] + a[2] * FR[0][2];
	rb = b[0] * FR[1][1] + b[1] * FR[1][0];
	t = abs(T[0] * R[2][2] - T[2] * R[0][2]);
	if (t > ra + rb) return false;

	//L = A2 x B0
	ra = a[0] * FR[1][0] + a[1] * FR[0][0];
	rb = b[1] * FR[2][2] + b[2] * FR[2][1];
	t = abs(T[1] * R[0][0] - T[0] * R[1][0]);
	if (t > ra + rb) return false;

	//L = A2 x B1
	ra = a[0] * FR[1][1] + a[1] * FR[0][1];
	rb = b[0] * FR[2][2] + b[2] * FR[2][0];
	t = abs(T[1] * R[0][1] - T[0] * R[1][1]);
	if (t > ra + rb) return false;

	//L = A2 x B2
	ra = a[0] * FR[1][2] + a[1] * FR[0][2];
	rb = b[0] * FR[2][1] + b[1] * FR[2][0];
	t = abs(T[1] * R[0][2] - T[0] * R[1][2]);
	if (t > ra + rb)	return false;

	// 충돌
	return true;
}

void Collider::AddIgnoreList(Collider* pCollider) noexcept
{
	m_IgnoreList.push_front(pCollider);
	pCollider->m_IgnoreList.push_front(this);
}

void Collider::ClearIgnoreList(const bool& isPostEvent) noexcept
{
	if (isPostEvent)
	{
		static auto pEvent = [](void* pVoid, void*) {
			auto pCollider = (Collider*)pVoid;
			for (auto& iter : pCollider->m_IgnoreList)
			{
				iter->m_IgnoreList.remove(pCollider);
			}
			pCollider->m_IgnoreList.clear();
		};

		ObjectManager::PostFrameEvent.emplace(pEvent, this, nullptr);
	}
	else
	{
		for (auto& iter : m_IgnoreList)
		{
			iter->m_IgnoreList.remove(this);
		}
		m_IgnoreList.clear();
	}
}

void Collider::ClearCollisionList() noexcept
{
	m_CollisionList.clear();
}

void Collider::SetRadius(const float& radius) noexcept
{
	m_radius = radius;
}

const float Collider::GetRadius() const noexcept
{
	return m_radius;
}

const float Collider::GetWorldRadius() const noexcept
{
	return m_radius * m_pParent->GetScaleAverage();
}

const D3DXVECTOR3 Collider::GetCenter() const noexcept
{
	return m_pParent->GetWorldPosition() + m_pivot;
}


Component* Collider::clone() noexcept
{
	auto pCollider = new Collider(*this);
	ObjectManager::Get().PushCollider(pCollider);
	return (Component*)pCollider;
}