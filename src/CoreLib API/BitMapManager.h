#pragma once
//#include "Singleton.h"
#include "MyPlayer.h"
#include "MyEnemy.h"
#include "MyEffect.h"
#include "Sprite.h"


// ȭ�鿡 �Ѹ��� ���� ��Ʈ�� �������� ����, ����
class BitMapManager final : public Singleton<BitMapManager>, public Basis
{
private:
	friend class Singleton<BitMapManager>;
	BitMapManager() = default;
public:
	virtual ~BitMapManager() = default;
private:
	map<wstring, BitMapFile>					  m_bitMapFile;	// ���� ��Ʈ�� ����
	map<wstring, map<wstring, list<Sprite> > >	  m_srcRectMap;	// �� ��ü�� ���� ���� �� ��������Ʈ
public:
	list<MyBitMap*>								  m_objBitMap;	// BitMapFile�� �����ϴ� ������Ʈ��
	static HBRUSH m_backBrush;									// 255, 255, 255 �귯��

	MyBitMap m_hpBar;												// ü�¹�
	MyBitMap m_hpBarBack;
	Point	 m_hpOriginWH;
private:
	BitMapFile*  AddBitMapFile(const wstring& bitName);
	void		 RemoveBitMapFile(const wstring& bitName);
public:
	//MyBitMap*  getBitMap(wstring bitName, MyRect* srcRect);
	list<Sprite>* getSpriteList(const wstring& objName, const wstring& stateName);
	BitMapFile * getBitMapFile(wstring bitName, const bool& isMask = false);
	MyBitMap*	 AddObjBitMap(const wstring& objName, MyBitMap* bitMap, const bool& insertFront = false);	// ��Ʈ�� ��ü ���
	void		 RemoveBitMap(MyBitMap* bitMap);
	void		 DrawUI();

	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};

