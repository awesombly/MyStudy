#pragma once

template <class Type>
class Singleton
{
public:
	Singleton()					= default;
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&)		= delete;
	virtual ~Singleton()		= default;
public:
	static Type* Instance;
	static Type& GetInstance();
};

template<class Type>
Type* Singleton<Type>::Instance = nullptr;

template<class Type>
Type& Singleton<Type>::GetInstance()
{
	if (Instance == nullptr)
		Instance = new Type();
	return *Instance;
}