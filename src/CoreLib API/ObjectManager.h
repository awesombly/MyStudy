#pragma once
#include "BitMapManager.h"
#include "SoundManager.h"

// ������Ʈ(MyBitMap) ��ü�� ������ �Ŵ���(�޸� Ǯ)
class ObjectManager : public Singleton<ObjectManager>, public Basis
{
private:
	map<wstring, stack<MyBitMap*> > m_DisabledPull;		// ������ Ǯ<�̸�>
	map<wstring, MyBitMap*>			m_ProtoPull;		// ����� Ǯ<�̸�>
public:
	map<EObjType, list<MyBitMap*> >  m_ObjectList;		// ��ü ��ȸ�� ����Ʈ<Ÿ��>
	list<MyRect*>					m_WallList;			// �� ����Ʈ
private:
	friend class Singleton<ObjectManager>;
	ObjectManager() = default;
public:
	virtual ~ObjectManager();

	MyBitMap* TakeObject(const wstring& objName);		// ������Ʈ ��������(+��Ʈ�� ���)
	MyBitMap* TakeObject(const wstring& objName, const int& width, const int& height);	// " + ���μ��� ����
	void DisableObject(MyBitMap* object);				// ������Ʈ ��Ȱ��ȭ
	void InsertObject(MyBitMap* object);				// ������Ʈ->����Ʈ �߰�
	void AddWall(MyRect* pRect);
	void ClearWall();
	bool ReadMapData(const MyBitMap& mapBitmap);

	bool Init()	   override;
	bool Frame()   override;
	bool Render()  override;
	bool Release() override;
};