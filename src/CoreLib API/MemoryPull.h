#pragma once
#include "header.h"

// �̻��

template <class object>
class MemoryPull
{
public:
	friend class MyObject;
	MemoryPull() = default;
	virtual ~MemoryPull();
public:
	list<object*> m_ObjectList;			// ��ü ��ȸ�� ����Ʈ
protected:
	//stack<object*> m_DisabledPull;		// ������ Ǯ
	map<wstring, stack<object*> > m_DisabledPull;		// ������ Ǯ
public:
	virtual object* TakeObject(const wstring& objName);
	virtual void    DisableObject(object* obj);
	virtual void	InsertObject(object* obj);
	virtual void    Release();
};


template<class object>
MemoryPull<object>::~MemoryPull() {
	for (auto& iter : m_ObjectList)
		delete iter;
}

template<class object>
object* MemoryPull<object>::TakeObject(const wstring& objName)
{
	object* myObj = nullptr;
	if (m_DisabledPull[objName].empty())
	{
		myObj = new object();
		m_ObjectList.push_back(myObj);
	}
	else
	{
		myObj = m_DisabledPull.top();
		m_DisabledPull.pop();
	}
	return myObj;
}

template<class object>
void MemoryPull<object>::DisableObject(object* obj)
{
	m_DisabledPull.push(obj);
}

template<class object>
void MemoryPull<object>::InsertObject(object* obj)
{
	m_ObjectList.push_back(obj);
}

template<class object>
void MemoryPull<object>::Release()
{
	/*for (auto& iter : m_ObjectList)
	{
		delete iter;
	}*/
	m_ObjectList.clear();
}