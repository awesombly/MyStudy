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

// ��Ʈ�� ���Ͽ� ���� ������ ������ ������Ʈ
class MyBitMap : public Basis, public MyRect
{
public:
	friend class ObjectManager;
	MyBitMap();
	MyBitMap(const wstring& objName, const bool& insertFront = false);
	virtual ~MyBitMap();
protected:
	HDC			m_hRotateDC;						// ȸ���� �� DC?
	//HBITMAP		m_hRotateBitmap;				// ȸ���� ��Ʈ��
	//HBITMAP		m_hRotateMaskBitmap;			// ȸ���� ����ũ��Ʈ��
	//HBITMAP		m_hRotateOldBitmap;				// ȸ���� ���� ��Ʈ��
	list<Sprite>* m_pSpriteList;				// ��������Ʈ(��ǥ, ������) ����Ʈ

	float		m_crossLength;						// �밢�� ����
	float		m_frameCount;					// ��������Ʈ ������ ���
	bool		m_isEnable;						// Ȱ��ȭ��, ���� ���� No!
public:
	list<Sprite>::iterator m_curSprite;			// ���� ��������Ʈ ���ͷ�����
	EObjType     m_objType;						// ��ü Ÿ��
	wstring      m_myName;						// ��ü�� �̸�
	int			 m_damage;								// �⺻ ������

	static MyRect* MapRect;						// �� ��ü ũ��
public:
	// ���� �밢�� ���ϴ� ��� �� ����..
	void setCrossLength();
	void setSrcRect(const MyRect& rect);
	MyRect& getSrcRect() const;
	virtual bool isEnable();

	virtual void setTarget(const Point& target) { target; };				// ��ӿ�
	virtual void setSpriteList(list<Sprite>* sprite);
	virtual MyBitMap* setSpriteList(const wstring& stateName);
	virtual void setEnable(const bool& isEnable);
	virtual MyBitMap* clone();					// �ڽ� ����
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};

