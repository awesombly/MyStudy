#pragma once
//#include "Singleton.h"
#include "MyPlayer.h"
#include "MyEnemy.h"
#include "MyEffect.h"
#include "Sprite.h"


// 화면에 뿌리기 위한 비트맵 정보들을 저장, 관리
class BitMapManager final : public Singleton<BitMapManager>, public Basis
{
private:
	friend class Singleton<BitMapManager>;
	BitMapManager() = default;
public:
	virtual ~BitMapManager() = default;
private:
	map<wstring, BitMapFile>					  m_bitMapFile;	// 원본 비트맵 파일
	map<wstring, map<wstring, list<Sprite> > >	  m_srcRectMap;	// 각 객체에 대한 상태 별 스프라이트
public:
	list<MyBitMap*>								  m_objBitMap;	// BitMapFile을 참조하는 오브젝트들
	static HBRUSH m_backBrush;									// 255, 255, 255 브러쉬

	MyBitMap m_hpBar;												// 체력바
	MyBitMap m_hpBarBack;
	Point	 m_hpOriginWH;
private:
	BitMapFile*  AddBitMapFile(const wstring& bitName);
	void		 RemoveBitMapFile(const wstring& bitName);
public:
	//MyBitMap*  getBitMap(wstring bitName, MyRect* srcRect);
	list<Sprite>* getSpriteList(const wstring& objName, const wstring& stateName);
	BitMapFile * getBitMapFile(wstring bitName, const bool& isMask = false);
	MyBitMap*	 AddObjBitMap(const wstring& objName, MyBitMap* bitMap, const bool& insertFront = false);	// 비트맵 객체 등록
	void		 RemoveBitMap(MyBitMap* bitMap);
	void		 DrawUI();

	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};

