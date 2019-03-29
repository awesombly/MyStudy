#include "Core.h"

class MainClass : public Core {
public:
	MainClass() = default;
	~MainClass() = default;

public:
	virtual bool Init() override
	{
		return true;
	}
	virtual bool Frame() override
	{
		return true;
	}
	virtual bool Render() override
	{
		return true;
	}
	virtual bool Release() override
	{
		return true;
	}
};

RUNRUN;