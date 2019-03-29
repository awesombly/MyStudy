#include "BitMapManager.h"

HBRUSH BitMapManager::m_backBrush = CreateSolidBrush(RGB(255, 255, 255));

// 스프라이트 얻기
list<Sprite>* BitMapManager::getSpriteList(const wstring& objName, const wstring& stateName)
{
	return &m_srcRectMap[objName][stateName];
}

// 비트맵 얻고, 없으면 로드
BitMapFile* BitMapManager::getBitMapFile(wstring bitName, const bool& isMask)
{
	if (isMask)
		bitName += L"mask";
	if (m_bitMapFile.find(bitName) == m_bitMapFile.end())
		return AddBitMapFile(bitName);
	return &m_bitMapFile[bitName];
}

// 오브젝트 등록
MyBitMap* BitMapManager::AddObjBitMap(const wstring& objName, MyBitMap* bitMap, const bool& insertFront)
{
	bitMap->m_myName = objName;
	bitMap->setSpriteList(&m_srcRectMap[objName][L"basic"]);
	bitMap->setWH(bitMap->m_curSprite->m_srcRect.getWidth(), bitMap->m_curSprite->m_srcRect.getHeight());
	bitMap->setEnable(true);
	if(insertFront)
		m_objBitMap.insert(++++m_objBitMap.begin(), bitMap);
	else
		m_objBitMap.push_back(bitMap);
	return m_objBitMap.back();
}

// 비트맵 로드, 추가
BitMapFile* BitMapManager::AddBitMapFile(const wstring& bitName)
{
	if (!m_bitMapFile[bitName].Load(bitName))
	{
		m_bitMapFile.erase(bitName);
		return nullptr;
	}
	return &m_bitMapFile[bitName];
}

void BitMapManager::RemoveBitMap(MyBitMap* bitMap)
{
	auto iter = find(m_objBitMap.begin(), m_objBitMap.end(), bitMap);
	//delete *iter;
	m_objBitMap.erase(iter);
}

void BitMapManager::RemoveBitMapFile(const wstring& bitmapFile)
{
	m_bitMapFile.erase(bitmapFile);
}

bool BitMapManager::Init()
{
	/*MyEnemy::StateToString[EState::Basic] = L"basic";
	MyEnemy::StateToString[EState::Attack] = L"attack";
	MyEnemy::StateToString[EState::Move] = L"move";
	MyEnemy::StateToString[EState::Hit] = L"hit";
	MyEnemy::StateToString[EState::Runaway] = L"";*/


	FILE* fp;
	_wfopen_s(&fp, L"./bitmap.txt", L"rt");
	if (fp == nullptr)
	{
		OutputDebugString(L"\n\nbitmap.txt를 읽을 수 없다~\n\n\n");
		return false;
	}

	TCHAR _buffer[100] = { 0, };
	TCHAR _objName[25] = { 0, };
	TCHAR _bitName[25] = { 0, };
	TCHAR _stateName[25] = { 0, };
	
	MyRect* _rect = nullptr;
	float _frame = 0.0f;
	int   _spriteCount = 0;
	int   _stateCount = 0;
	int  _RGB[3] = { 0, };

	_fgetts(_buffer, _countof(_buffer), fp);					// 한줄 읽기
	while (wcscmp(_buffer, L"end"))
	{
		ZeroMemory(_RGB, sizeof(int) * 3);
		_stscanf_s(_buffer, L"%s %d %d %d %d", _objName, 25, &_stateCount, &_RGB[0], &_RGB[1], &_RGB[2]);	// 객체 이름, 갯수, 마스크컬러

		while (_stateCount--)
		{
			_fgetts(_buffer, _countof(_buffer), fp);
			_stscanf_s(_buffer, L"%s %s", _stateName, 25, _bitName, 25);			// 스프라이트 이름, 비트맵파일

			_fgetts(_buffer, _countof(_buffer), fp);								
			while (wcscmp(_buffer, L"\n"))
			{
				_rect = new MyRect();
				_stscanf_s(_buffer, L"%f %f %f %d %d",								// 스프라이트 정보(프레임, 좌표)
						   &_frame,
						   &_rect->m_point.x, &_rect->m_point.y,
						   &_rect->m_width, &_rect->m_height);
				m_srcRectMap[_objName][_stateName].emplace_back(*_rect, _frame, RGB(_RGB[0], _RGB[1], _RGB[2]));
				m_srcRectMap[_objName][_stateName].back().m_mainBit = getBitMapFile(_bitName);			// mainBit
				m_srcRectMap[_objName][_stateName].back().m_maskBit = getBitMapFile(_bitName, true);	// maskBit
				_fgetts(_buffer, _countof(_buffer), fp);
			}
		}
		_fgetts(_buffer, _countof(_buffer), fp);
	}

	fclose(fp);

	AddObjBitMap(L"hpBarBack", &m_hpBarBack)
		->setRect(Window::getClientRect().right - 412, 16, 350, 30);
	AddObjBitMap(L"hpBar", &m_hpBar)
		->setRect(Window::getClientRect().right - 369, 16, 297, 30);
	m_hpOriginWH = { 297, m_hpBar.m_curSprite->m_srcRect.getWidth() };
	RemoveBitMap(&m_hpBarBack);
	RemoveBitMap(&m_hpBar);

	return true;
}

bool BitMapManager::Frame()
{
	return true;
}

// 비트맵 전체 드로우
bool BitMapManager::Render()
{
	for (auto& iter : m_objBitMap)
	{
		iter->Render();
	}
	DrawUI();
	return true;
}

bool BitMapManager::Release()
{
	for (auto& iter : m_objBitMap)
	{
		//delete iter;
	}
	m_objBitMap.clear();
	m_bitMapFile.clear();
	return true;
}

void BitMapManager::DrawUI()
{
	m_hpBar.m_width = (int)m_hpOriginWH.x * MyPlayer::Instance->getHealth() / 100;
	m_hpBar.m_curSprite->m_srcRect.m_width = (int)m_hpOriginWH.y * MyPlayer::Instance->getHealth() / 100;
	m_hpBarBack.m_curSprite->m_mainBit->DrawUI(m_hpBarBack, m_hpBarBack.m_curSprite->m_srcRect, 99999);
	m_hpBar.m_curSprite->m_mainBit->	DrawUI(m_hpBar,		m_hpBar.m_curSprite->m_srcRect,		99999);
}