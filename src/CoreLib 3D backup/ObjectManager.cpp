#include "ObjectManager.h"
//#include "Worker.h"

Camera ObjectManager::MainCamera;
MyRect* ObjectManager::MapRect = new MyRect();
int ObjectManager::MapWallY = 460;

bool ObjectManager::Init() noexcept
{
	MainCamera.Init();
	ReadSpriteScript();
	return true;
}


bool ObjectManager::Frame(const float& spf, const float& accTime) noexcept
{
	MainCamera.Frame(spf, accTime);
	for (auto& iter : m_ObjectList[EObjType::Image])
		iter->Frame(spf, accTime);
	for (auto& iter : m_ObjectList[EObjType::Collider])
		iter->Frame(spf, accTime);
	for (auto& iter : m_ObjectList[EObjType::Player])
		iter->Frame(spf, accTime);
	for (auto& iter : m_ObjectList[EObjType::Enemy])
		iter->Frame(spf, accTime);
	for (auto& iter : m_ObjectList[EObjType::Item])
		iter->Frame(spf, accTime);
	for (auto& iter : m_ObjectList[EObjType::Effect])
		iter->Frame(spf, accTime);
	for (auto& iter : m_ObjectList[EObjType::PlayerAttack])
		iter->Frame(spf, accTime);
	for (auto& iter : m_ObjectList[EObjType::EnemyAttack])
		iter->Frame(spf, accTime);
	for (auto& iter : m_ObjectList[EObjType::Score])
		iter->Frame(spf, accTime);
	for (auto iter = m_ObjectList[EObjType::UI].rbegin(); iter != m_ObjectList[EObjType::UI].rend(); iter++)
		(*iter)->Frame(spf, accTime);

	return true;
}

bool ObjectManager::Render(ID3D11DeviceContext* pDContext) noexcept
{
	MainCamera.Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::Image])
		iter->Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::Collider])
		iter->Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::Player])
		iter->Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::Enemy])
		iter->Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::Item])
		iter->Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::Effect])
		iter->Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::PlayerAttack])
		iter->Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::EnemyAttack])
		iter->Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::Score])
		iter->Render(pDContext);
	for (auto& iter : m_ObjectList[EObjType::UI])
		iter->Render(pDContext);

	return true;
}

// 스트라이트 리스트
bool ObjectManager::ReadSpriteScript()
{
	FILE* fp;
	_wfopen_s(&fp, L"./srcData.txt", L"rt");
	if (fp == nullptr)
	{
		ErrorMessage(L""s + __FUNCTIONW__ + L" -> 파일 읽기 실패!");
		return false;
	}

	TCHAR _buffer[100] = { 0, };
	TCHAR _objName[25] = { 0, };
	TCHAR _bitName[25] = { 0, };
	TCHAR _extName[10] = { 0, };
	TCHAR _stateName[25] = { 0, };

	MyVector4 _vector4;
	float _frame = 0.0f;
	int   _stateCount = 0;
	Texture* _pTexture = nullptr;

	_fgetts(_buffer, _countof(_buffer), fp);					// 한줄 읽기
	while (wcscmp(_buffer, L"end\n"))
	{
		_stscanf_s(_buffer, L"%s %d", _objName, 25, &_stateCount);	// 객체 이름, 갯수, 마스크컬러

		while (_stateCount--)
		{
			_fgetts(_buffer, _countof(_buffer), fp);
			ZeroMemory(_extName, sizeof(_extName));
			_stscanf_s(_buffer, L"%s %s %s", _stateName, 25, _bitName, 25, _extName, 10);			// 스프라이트 이름, 비트맵파일, 확장자

			_fgetts(_buffer, _countof(_buffer), fp);
			while (wcscmp(_buffer, L"\n"))
			{
				_stscanf_s(_buffer, L"%f %f %f %f %f",								// 스프라이트 정보(프레임, 좌표)
						   &_frame,
						   &_vector4.x, &_vector4.y,
						   &_vector4.z, &_vector4.w);
				if (wcscmp(_extName, L""))
					_pTexture = DxManager::Instance->getTexture(_bitName, _extName);
				else
					_pTexture = DxManager::Instance->getTexture(_bitName);

				m_SpriteList[_objName][_stateName].emplace_back(_vector4, _frame, _pTexture);
				_fgetts(_buffer, _countof(_buffer), fp);
			}
		}
		_fgetts(_buffer, _countof(_buffer), fp);
	}

	fclose(fp);


	return true;
}

// 맵 데이터
bool ObjectManager::ReadMapScript()
{
	FILE* fp;
	_wfopen_s(&fp, L"./mapData.txt", L"rt");
	if (fp == nullptr)
	{
		ErrorMessage(L""s + __FUNCTIONW__ + L" -> 파일 읽기 실패!");
		return false;
	}
	TCHAR _buffer[60] = { 0, };
	TCHAR _objName[25] = { 0, };
	TCHAR _extName[10] = { 0, };
	int   _count = 0;

	MyRect _rect;
	//Texture* _pTexture = nullptr;

	// 맵 좌 벽
	TakeObject(L"None", { -100, 0, 100, MapRect->getHeight() })
		->setEnable(false);

	_fgetts(_buffer, _countof(_buffer), fp);					// 한줄 읽기
	while (wcscmp(_buffer, L"end\n"))
	{
		_stscanf_s(_buffer, L"%s %d %s", _objName, 25, &_count, _extName, 10);
		// 투명 벽
		if (!wcscmp(_objName, L"None"))
		{
			while (_count--)
			{
				_fgetts(_buffer, _countof(_buffer), fp);
				_stscanf_s(_buffer, L"%f %f %d %d", &_rect.m_point.x, &_rect.m_point.y, &_rect.m_width, &_rect.m_height);
				TakeObject(L"None", _rect)->setEnable(false);
			}
		}
		// 정적 객체
		else
		{
			while (_count--)
			{
				_fgetts(_buffer, _countof(_buffer), fp);
				_stscanf_s(_buffer, L"%f %f %d %d", &_rect.m_point.x, &_rect.m_point.y, &_rect.m_width, &_rect.m_height);

				PushObject(new MyActor(_objName, EObjType::Collider))
					->setRect(_rect);
			}
		}
		_fgetts(_buffer, _countof(_buffer), fp);
	}

	fclose(fp);
	return true;
}

bool ObjectManager::Release() noexcept
{
	MainCamera.Release();
	/*for (auto& outIter : m_ObjectList)
	{
		for (auto& inIter : outIter.second)
		{
			inIter->Release();
			delete inIter;
		}
	}*/
	return true;
}


list<Sprite>* ObjectManager::getSpriteList(const wstring_view& objName, const wstring_view& stateName)
{
	auto iter = m_SpriteList[objName.data()].find(stateName.data());
	if (iter == m_SpriteList[objName.data()].end())
	{
		while (1)
		{
			ErrorMessage(L""s + __FUNCTIONW__ + L" -> Sprite Null Point!");
			Sleep(1000);
		}
	}
	return &iter->second;
} 

MyActor* ObjectManager::TakeObject(const wstring_view& objName)
{
	MyActor* myObj = nullptr;
	if (m_DisabledPull[objName].empty())
	{
		 //대기 풀이 비었다면 복사 생성
		auto& iter = m_ProtoPull.find(objName)->second;

		if (iter == nullptr)
			return nullptr;

		myObj = iter->clone();			// 복사
		m_ObjectList[myObj->m_objType].push_back(myObj);
	}
	else
	{
		// 대기 풀이 있다면 꺼내옴
		myObj = m_DisabledPull[objName].top();
		m_DisabledPull[objName].pop();
	}
	myObj->Init();
	myObj->setEnable(true);
	return myObj;
}

MyActor* ObjectManager::TakeObject(const wstring_view& objName, const MyRect& rect)
{
	auto myObj = TakeObject(objName);
	myObj->setRect(rect);
	return myObj;
}

MyActor* ObjectManager::TakeObject(const wstring_view& objName, const int& width, const int& height)
{
	auto myObj = TakeObject(objName);
	myObj->setWH(width, height);
	return myObj;
}

MyActor* ObjectManager::TakeObject(const wstring_view& objName, const MyVector2& center)
{
	auto myObj = TakeObject(objName);
	myObj->setCenter(center);
	return myObj;
}

MyActor* ObjectManager::PushObject(MyActor* object)
{
	m_ObjectList[object->m_objType].push_back(object);
	object->setEnable(true);
	return object;
}

MyActor* ObjectManager::SetProtoObject(MyActor* object)
{
	if (m_ProtoPull.find(object->m_myName) != m_ProtoPull.end())
	{
		ErrorMessage(""s + __FUNCTION__ + " -> 중복된 이름!");
		return nullptr;
	}
	m_ProtoPull[object->m_myName] = object;
	object->Init();
	object->setEnable(false);
	return object;
}

void ObjectManager::DisableObject(MyActor* object)
{
	object->setEnable(false);
	m_DisabledPull[object->m_myName].push(object);
}

void ObjectManager::DeleteObject(MyActor* object)
{
	for (auto outIter = m_ObjectList.begin(); outIter != m_ObjectList.end(); outIter++)
	{
		auto iter = find(outIter->second.begin(), outIter->second.end(), object);
		if (iter != outIter->second.end())
		{
			delete *iter;
			outIter->second.erase(iter);
			return;
		}
	}
	while (1)
	{
		ErrorMessage(L""s + __FUNCTIONW__ + L" -> " + object->m_myName + L", Not Found!" );
		Sleep(1000);
	}
}