#pragma once
#include "DxManager.h"
#include "MyUI.h"
//#include "MyObject.h"


class ObjectManager : public IDxBasis, public ISingleton<ObjectManager>
{
private:
	map<wstring_view, stack<MyActor*> > m_DisabledPull;			// ������ Ǯ<�̸�>
	map<wstring_view, MyActor*>		    m_ProtoPull;			// ����� Ǯ<�̸�>
	map<wstring, map<wstring, list<Sprite> > >	  m_SpriteList;	// �� ��ü�� ���� ���� �� ��������Ʈ
public:
	map<EObjType, list<MyActor*> >		m_ObjectList;			// ��ü ��ȸ�� ����Ʈ<Ÿ��>
	static MyRect*  MapRect;									// �� ��ü ũ��
	static int		MapWallY;									// �ϴ� �̵��� �ٴ� ��ġ

	static Camera MainCamera;									// ���� ī�޶�
private:
	bool ReadSpriteScript();
public:
	list<Sprite>*	getSpriteList(const wstring_view& objName, const wstring_view& stateName);
	bool ReadMapScript();
	// ������Ʈ ��������
	MyActor* TakeObject(const wstring_view& objName);		
	MyActor* TakeObject(const wstring_view& objName, const MyRect& rect);
	MyActor* TakeObject(const wstring_view& objName, const MyVector2& center);
	MyActor* TakeObject(const wstring_view& objName, const int& width, const int& height);
	MyActor* SetProtoObject(MyActor* object);				// ����� ������Ʈ �߰�
	MyActor* PushObject(MyActor* object);					// ������Ʈ->����Ʈ �߰�
	void DisableObject(MyActor* object);					// ������Ʈ ��Ȱ��ȭ
	void DeleteObject(MyActor* object);						// delete�� ����Ʈ ����

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
private:
	friend class ISingleton<ObjectManager>;
	ObjectManager() = default;
public:
	virtual ~ObjectManager() = default;
};