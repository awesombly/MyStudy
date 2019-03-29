#pragma once

template <class Type>
class ISingleton
{
public:
	ISingleton()					= default;
	ISingleton(const ISingleton&) = delete;
	ISingleton(ISingleton&&)		= delete;
	virtual ~ISingleton()		= default;
public:
	static Type* Instance;
	static Type& GetInstance();
};

template<class Type>
Type* ISingleton<Type>::Instance = nullptr;

template<class Type>
Type& ISingleton<Type>::GetInstance()
{
	if (Instance == nullptr)
		Instance = new Type();
	return *Instance;
}