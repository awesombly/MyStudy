#pragma once
#include "BitMapManager.h"
#include "SoundManager.h"

// 오브젝트(MyBitMap) 전체를 관리할 매니저(메모리 풀)
class ObjectManager : public Singleton<ObjectManager>, public Basis
{
private:
	map<wstring, stack<MyBitMap*> > m_DisabledPull;		// 대기상태 풀<이름>
	map<wstring, MyBitMap*>			m_ProtoPull;		// 복사용 풀<이름>
public:
	map<EObjType, list<MyBitMap*> >  m_ObjectList;		// 전체 순회용 리스트<타입>
	list<MyRect*>					m_WallList;			// 벽 리스트
private:
	friend class Singleton<ObjectManager>;
	ObjectManager() = default;
public:
	virtual ~ObjectManager();

	MyBitMap* TakeObject(const wstring& objName);		// 오브젝트 가져오기(+비트맵 등록)
	MyBitMap* TakeObject(const wstring& objName, const int& width, const int& height);	// " + 가로세로 설정
	void DisableObject(MyBitMap* object);				// 오브젝트 비활성화
	void InsertObject(MyBitMap* object);				// 오브젝트->리스트 추가
	void AddWall(MyRect* pRect);
	void ClearWall();
	bool ReadMapData(const MyBitMap& mapBitmap);

	bool Init()	   override;
	bool Frame()   override;
	bool Render()  override;
	bool Release() override;
};