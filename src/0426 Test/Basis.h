#pragma once
#include "header.h"

class Basis
{
public:
	Basis() = default;
	virtual ~Basis() = default;
public :
	virtual bool Init() = 0;
	virtual bool Frame() = 0;
	virtual bool Render() = 0;
	virtual bool Release() = 0;
};

