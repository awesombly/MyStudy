#pragma once
#include "ServerBasic.h"

class Acceptor : public ServerBasic
{
protected:
	virtual bool Run() override;
public:
	Acceptor() = default;
	virtual ~Acceptor() = default;
};

