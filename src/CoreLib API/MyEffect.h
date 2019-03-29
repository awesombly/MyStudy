#pragma once
#include "MyBitMap.h"

class MyEffect : public MyBitMap
{
public:
	MyEffect(const wstring& objName, const EObjType& type, const int& damage = 0);
	virtual ~MyEffect() = default;
public:
	//virtual void	  setEnable(const bool& isEnable);
	void setEnable(const bool& isEnable);
	virtual MyBitMap* clone()  override;					// �ڽ� ����
	virtual bool	  Render() override;
};