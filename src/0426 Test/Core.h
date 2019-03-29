#pragma once
#include "Input.h"
#include "Timer.h"
#include "Window.h"

class Core : public Basis 
{
public:
	Core();
	virtual ~Core();
public:
	bool Run()
	{
		Init();
		Frame();
		Render();
		Release();
		return true;
	}
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
};

