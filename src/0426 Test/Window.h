#pragma once
#include "Basis.h"

class Window : public Basis
{
public:
	Window();
	virtual ~Window();
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
};

