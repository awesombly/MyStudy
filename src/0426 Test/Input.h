#pragma once
#include "Singleton.h"
#include "Basis.h"

class Input : public Singleton<Input>, public Basis
{
private:
	Input();
public:
	friend class Singleton<Input>;
	virtual ~Input();
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
};

#define I_Input  Input::GetInstance()