#pragma once
#include "Timer.h"
#include "Sprite.h"

enum class EObjType : char {
	Player,
	Enemy,
	Effect,
	PlayerAttack,
	EnemyAttack,
	Wall,
	Bitmap,
};

// 비트맵 파일에 대한 정보를 가지는 오브젝트
class MyBitMap : public Basis, public MyRect
{
public:
	friend class ObjectManager;
	MyBitMap();
	MyBitMap(const wstring& objName, const bool& insertFront = false);
	virtual ~MyBitMap();
protected:
	HDC			m_hRotateDC;						// 회전시 쓸 DC?
	//HBITMAP		m_hRotateBitmap;				// 회전시 비트맵
	//HBITMAP		m_hRotateMaskBitmap;			// 회전시 마스크비트맵
	//HBITMAP		m_hRotateOldBitmap;				// 회전시 이전 비트맵
	list<Sprite>* m_pSpriteList;				// 스프라이트(좌표, 프레임) 리스트

	float		m_crossLength;						// 대각선 길이
	float		m_frameCount;					// 스프라이트 프레임 계산
	bool		m_isEnable;						// 활성화값, 직접 제어 No!
public:
	list<Sprite>::iterator m_curSprite;			// 현재 스프라이트 이터레이터
	EObjType     m_objType;						// 객체 타입
	wstring      m_myName;						// 객체별 이름
	int			 m_damage;								// 기본 데미지

	static MyRect* MapRect;						// 맵 전체 크기
public:
	// 현재 대각선 구하는 기능 외 없음..
	void setCrossLength();
	void setSrcRect(const MyRect& rect);
	MyRect& getSrcRect() const;
	virtual bool isEnable();

	virtual void setTarget(const Point& target) { target; };				// 상속용
	virtual void setSpriteList(list<Sprite>* sprite);
	virtual MyBitMap* setSpriteList(const wstring& stateName);
	virtual void setEnable(const bool& isEnable);
	virtual MyBitMap* clone();					// 자신 복사
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};

