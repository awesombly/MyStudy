#pragma once
#include "MyPlayer.h"


class PlayerServant : public MyPlayer
{
protected:
public:
	//MyVector2  MyCenter;
	PlayerServant() = default;
public:
	PlayerServant(const wstring_view& myName, const EObjType& objType,
				  const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	virtual ~PlayerServant() = default;

	static void SynchPosition() {};							// 스크롤 위치 동기화
	/*MyVector2 getCenter() { 
		return MyRect::getCenter();
	}*/

	// 아이템류
	virtual void AddUsingItem(Item* pItem) {return; pItem; };
	virtual void RemoveUsingItem(Item* pItem) { return; pItem; };
	virtual void UpdateItemState() {};
	virtual bool isEquipItem(const wstring_view& itemName) { return false; itemName; };
	virtual void UseFlamePotion() {};

	virtual void DeadEvent(MyActor* target) override { target; };
	//virtual bool setState(const EPlayerState state);		// 상태 전이
	virtual bool HeadCheck() { return false; }
	//virtual bool Collision(const float& spf);
	virtual bool MouseCheck() { return true; }
	//virtual bool setSpriteState(const wstring& stateName, const bool& isRepeat = true);
	virtual void setDelayHit(const float& delay, const bool& useHalfAlpha = true) { delay; useHalfAlpha; };
	virtual void setHp(const float& value) override { value; };
	virtual void operHp(const float& value) override { value; };
	virtual void operScore(const int& value) override { value; };

	//virtual bool Init(const wstring_view& myName, const EObjType& objType,
					  //const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader") noexcept;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;

	virtual void Move(const int& x, const int& y)				noexcept override { x; y; };
	virtual void Move(const float& x, const float& y)			noexcept override { x; y; };
	virtual void Move(const MyVector2& point)					noexcept override { point; };
	virtual void Move(const POINT& point)						noexcept override { point; };
};

