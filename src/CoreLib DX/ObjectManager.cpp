#include "ObjectManager.h"
//#include "Worker.h"

MyRect* ObjectManager::MapRect = new MyRect();
int ObjectManager::MapWallY = 460;

bool ObjectManager::Init() noexcept
{
	ReadSpriteScript();
	return true;
}


bool ObjectManager::Frame(const float& spf, const float& accTime) noexcept
{
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

// ��Ʈ����Ʈ ����Ʈ
bool ObjectManager::ReadSpriteScript()
{
	FILE* fp;
	_wfopen_s(&fp, L"./srcData.txt", L"rt");
	if (fp == nullptr)
	{
		wostringstream ostr;
		ostr << __FUNCTION__ << ", ���� �б� ����!" << endl;
		OutputDebugString(ostr.str().c_str());
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

	_fgetts(_buffer, _countof(_buffer), fp);					// ���� �б�
	while (wcscmp(_buffer, L"end\n"))
	{
		_stscanf_s(_buffer, L"%s %d", _objName, 25, &_stateCount);	// ��ü �̸�, ����, ����ũ�÷�

		while (_stateCount--)
		{
			_fgetts(_buffer, _countof(_buffer), fp);
			ZeroMemory(_extName, sizeof(_extName));
			_stscanf_s(_buffer, L"%s %s %s", _stateName, 25, _bitName, 25, _extName, 10);			// ��������Ʈ �̸�, ��Ʈ������, Ȯ����

			_fgetts(_buffer, _countof(_buffer), fp);
			while (wcscmp(_buffer, L"\n"))
			{
				_stscanf_s(_buffer, L"%f %f %f %f %f",								// ��������Ʈ ����(������, ��ǥ)
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

// �� ������
bool ObjectManager::ReadMapScript()
{
	FILE* fp;
	_wfopen_s(&fp, L"./mapData.txt", L"rt");
	if (fp == nullptr)
	{
		wostringstream ostr;
		ostr << __FUNCTION__ << ", ���� �б� ����!" << endl;
		OutputDebugString(ostr.str().c_str());
		return false;
	}
	TCHAR _buffer[60] = { 0, };
	TCHAR _objName[25] = { 0, };
	TCHAR _extName[10] = { 0, };
	int   _count = 0;

	MyRect _rect;
	//Texture* _pTexture = nullptr;

	// �� �� ��
	TakeObject(L"None", { -100, 0, 100, MapRect->getHeight() })
		->setEnable(false);

	_fgetts(_buffer, _countof(_buffer), fp);					// ���� �б�
	while (wcscmp(_buffer, L"end\n"))
	{
		_stscanf_s(_buffer, L"%s %d %s", _objName, 25, &_count, _extName, 10);
		// ���� ��
		if (!wcscmp(_objName, L"None"))
		{
			while (_count--)
			{
				_fgetts(_buffer, _countof(_buffer), fp);
				_stscanf_s(_buffer, L"%f %f %d %d", &_rect.m_point.x, &_rect.m_point.y, &_rect.m_width, &_rect.m_height);
				TakeObject(L"None", _rect)->setEnable(false);
			}
		}
		// ���� ��ü
		else
		{
			while (_count--)
			{
				_fgetts(_buffer, _countof(_buffer), fp);
				_stscanf_s(_buffer, L"%f %f %d %d", &_rect.m_point.x, &_rect.m_point.y, &_rect.m_width, &_rect.m_height);

			/*	if (wcscmp(_extName, L""))
					_pTexture = DxManager::Instance->getTexture(L"map/mapObject", _extName);
				else
					_pTexture = DxManager::Instance->getTexture(L"map/mapObject");*/
		 		//m_SpriteList[_objName][L"basic"].emplace_back(MyVector4(_objRect), 0.0f, _pTexture);
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
		wostringstream ostr;
		ostr << __FUNCTION__ << L", " << objName << L", " << stateName << L", Sprite nullptr!" << endl;
		while (1)
		{
			OutputDebugString(ostr.str().c_str());
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
		 //��� Ǯ�� ����ٸ� ���� ����
		auto& iter = m_ProtoPull.find(objName)->second;

		if (iter == nullptr)
			return nullptr;

		myObj = iter->clone();			// ����
		m_ObjectList[myObj->m_objType].push_back(myObj);
	}
	else
	{
		// ��� Ǯ�� �ִٸ� ������
		myObj = m_DisabledPull[objName].top();
		m_DisabledPull[objName].pop();
		myObj->Init();
	}
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
		wostringstream ostr;
		ostr << __FUNCTION__ << ", �ߺ��� �̸�!" << endl;
		OutputDebugString(ostr.str().c_str());
		return nullptr;
	}
	m_ProtoPull[object->m_myName] = object;
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
	wostringstream ostr;
	ostr << __FUNCTION__ << ", " << object->m_myName << ", Not Found!" << endl;
	while (1)
	{
		OutputDebugString(ostr.str().c_str());
		Sleep(1000);
	}
}