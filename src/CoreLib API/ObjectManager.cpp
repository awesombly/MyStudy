#include "ObjectManager.h"

//ObjectManager& g_objectManager = ObjectManager::GetInstance();

bool ObjectManager::Init()
{
	m_ProtoPull.emplace(L"EnemyBigRed",		new MyEnemy(L"EnemyBigRed"));
	m_ProtoPull.emplace(L"EnemySmallRed",	new MyEnemy(L"EnemySmallRed"));
	m_ProtoPull.emplace(L"Fairy",			new MyEnemy(L"Fairy"));

	m_ProtoPull.emplace(L"Explosion",		new MyEffect(L"Explosion", EObjType::Effect));
	m_ProtoPull.emplace(L"Bomb",			new MyEffect(L"Bomb", EObjType::Effect));
	m_ProtoPull.emplace(L"Clash",			new MyEffect(L"Clash", EObjType::Effect));

	m_ProtoPull.emplace(L"EffectSlash",		new MyEffect(L"EffectSlash", EObjType::PlayerAttack, 50));
	m_ProtoPull.emplace(L"EffectChargeSlash", new MyEffect(L"EffectChargeSlash", EObjType::PlayerAttack, 100));

	m_ProtoPull.emplace(L"FairyClash",	new MyObject(L"FairyClash", EObjType::EnemyAttack, 10));
	m_ProtoPull.emplace(L"PlayerBomb",	new MyObject(L"PlayerBomb", EObjType::PlayerAttack, 50));
	

	for (auto& iter : m_ProtoPull)
	{
		iter.second->setCenter(0, 0);
		iter.second->m_isEnable = false;
		BitMapManager::Instance->RemoveBitMap(iter.second);
	}
	return true;
}

ObjectManager::~ObjectManager()
{
	Release();
}

MyBitMap* ObjectManager::TakeObject(const wstring& objName)
{
	MyBitMap* myObj = nullptr;
	if (m_DisabledPull[objName].empty())
	{
		// 대기 풀이 비었다면 복사 생성
		auto& iter = m_ProtoPull.find(objName)->second;

		if (iter == nullptr)	
			return nullptr;
		
		myObj = iter->clone();			// 복사
		BitMapManager::Instance->AddObjBitMap(objName, myObj);
		m_ObjectList[myObj->m_objType].push_back(myObj);
	}
	else
	{
		// 대기 풀이 있다면 꺼내옴
		myObj = m_DisabledPull[objName].top();
		m_DisabledPull[objName].pop();
	}
	myObj->m_isEnable = true;
	myObj->Init();
	return myObj;
}

MyBitMap* ObjectManager::TakeObject(const wstring& objName, const int& width, const int& height)
{
	auto myObj = TakeObject(objName);
	myObj->setWH(width, height);
	return myObj;
}

void ObjectManager::DisableObject(MyBitMap* object)
{
	object->m_isEnable = false;
	m_DisabledPull[object->m_myName].push(object);
}

void ObjectManager::InsertObject(MyBitMap* object)
{
	m_ObjectList[object->m_objType].push_back(object);
	object->m_isEnable = true;
}

void ObjectManager::AddWall(MyRect* pRect)
{
	m_WallList.push_back(pRect);
}

void ObjectManager::ClearWall()
{
	//for (auto iter : m_WallList)
		//delete iter;
	m_WallList.clear();
}

bool ObjectManager::ReadMapData(const MyBitMap& mapBitmap)
{
	FILE* fp;
	_wfopen_s(&fp, L"./mapData.txt", L"rt");
	if (fp == nullptr)
	{
		OutputDebugString(L"\n\nmapData.txt를 읽을 수 없다~\n\n\n");
		return false;
	}

	TCHAR _buffer[60] = { 0, };
	TCHAR _objName[25] = { 0, };
	int   _count = 0;

	MyRect* _rect = nullptr;

	_fgetts(_buffer, _countof(_buffer), fp);					// 한줄 읽기
	while (wcscmp(_buffer, L"end"))
	{
		_stscanf_s(_buffer, L"%s %d", _objName, 25, &_count);
		// 투명 벽
		if (!wcscmp(_objName, L"None"))
		{
			while (_count--)
			{
				_rect = new MyRect();
				_fgetts(_buffer, _countof(_buffer), fp);
				_stscanf_s(_buffer, L"%f %f %d %d", &_rect->m_point.x, &_rect->m_point.y, &_rect->m_width, &_rect->m_height);

				_rect->m_point = { _rect->m_point.x, _rect->m_point.y };
				_rect->m_width = _rect->m_width;
				_rect->m_height = _rect->m_height;
				AddWall(_rect);
			}
		}
		// 정적 객체
		else
		{
			while (_count--)
			{
				_rect = (MyRect*)new MyBitMap(_objName, true);
				_fgetts(_buffer, _countof(_buffer), fp);
				_stscanf_s(_buffer, L"%f %f %d %d", &_rect->m_point.x, &_rect->m_point.y, &_rect->m_width, &_rect->m_height);
				AddWall(_rect);
			}
		}
		_fgetts(_buffer, _countof(_buffer), fp);
	}

	// 맵 좌우 벽
	_rect = new MyRect();
	_rect->setRect(-100, 0, 100, mapBitmap.getHeight());
	AddWall(_rect);

	/*_rect = new MyRect();
	_rect->setRect(mapBitmap.getRight(), 0, 100, mapBitmap.getHeight());
	AddWall(_rect);*/

	fclose(fp);
	return true;
}



bool  ObjectManager::Frame()
{
	// 플레이어 처리
	MyPlayer::Instance->Frame();

	// 적 처리
	for (auto& iter : m_ObjectList[EObjType::Enemy])
	{
		iter->Frame();
	}
	for (auto& iter : m_ObjectList[EObjType::EnemyAttack])
	{
		iter->Frame();
	}
	for (auto& iter : m_ObjectList[EObjType::PlayerAttack])
	{
		iter->Frame();
	}
	
	
	// 
	return true;
}


bool  ObjectManager::Render()
{
	return true;
}

bool  ObjectManager::Release()
{
	for (auto iter = m_ObjectList.begin(); iter != m_ObjectList.end(); iter++)
	{
		for (auto& inter : iter->second)
			delete inter;
	}
	m_ObjectList.clear();
	ClearWall();
	return true;
}