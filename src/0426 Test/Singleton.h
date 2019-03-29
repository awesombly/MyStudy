#pragma once

template <class Type>
class Singleton
{
public:
	Singleton() = default;
	virtual ~Singleton() = default;
public:
	static Type& GetInstance()
	{
		static Type Instance;
		return Instance;
	}
};

