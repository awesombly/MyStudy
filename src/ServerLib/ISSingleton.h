#pragma once
#include <mutex>


template <class Type>
class ISSingleton
{
public:
	ISSingleton()					= default;
	ISSingleton(const ISSingleton&) = delete;
	ISSingleton(ISSingleton&&)		= delete;
	virtual ~ISSingleton()		= default;
public:
	static Type* Instance;
	static Type& GetInstance();
};

template<class Type>
Type* ISSingleton<Type>::Instance = nullptr;

template<class Type>
Type& ISSingleton<Type>::GetInstance()
{
	// 한번만 실행
	static std::once_flag flag;
	std::call_once(flag,
				   []() {
		Instance = new Type();
	});

	return *Instance;
}
