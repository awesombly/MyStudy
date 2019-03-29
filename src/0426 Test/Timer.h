#pragma once
#include "Basis.h"

class Timer : public Basis
{
public:
	Timer();
	virtual ~Timer();
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
};

