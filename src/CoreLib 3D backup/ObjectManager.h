#pragma once
#include "DxManager.h"
#include "MyUI.h"
//#include "MyObject.h"


class ObjectManager : public IDxBasis, public ISingleton<ObjectManager>
{
private:
	map<wstring_view, stack<MyActor*> > m_DisabledPull;			// 대기상태 풀<이름>
	map<wstring_view, MyActor*>		    m_ProtoPull;			// 복사용 풀<이름>
	map<wstring, map<wstring, list<Sprite> > >	  m_SpriteList;	// 각 객체에 대한 상태 별 스프라이트
public:
	map<EObjType, list<MyActor*> >		m_ObjectList;			// 전체 순회용 리스트<타입>
	static MyRect*  MapRect;									// 맵 전체 크기
	static int		MapWallY;									// 하단 이동용 바닥 위치

	static Camera MainCamera;									// 메인 카메라
private:
	bool ReadSpriteScript();
public:
	list<Sprite>*	getSpriteList(const wstring_view& objName, const wstring_view& stateName);
	bool ReadMapScript();
	// 오브젝트 가져오기
	MyActor* TakeObject(const wstring_view& objName);		
	MyActor* TakeObject(const wstring_view& objName, const MyRect& rect);
	MyActor* TakeObject(const wstring_view& objName, const MyVector2& center);
	MyActor* TakeObject(const wstring_view& objName, const int& width, const int& height);
	MyActor* SetProtoObject(MyActor* object);				// 복사용 오브젝트 추가
	MyActor* PushObject(MyActor* object);					// 오브젝트->리스트 추가
	void DisableObject(MyActor* object);					// 오브젝트 비활성화
	void DeleteObject(MyActor* object);						// delete후 리스트 제거

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