#pragma once

template <typename T>
class MyNode
{
public:
	T* m_pData;
	MyNode<T>* m_pNext;
	MyNode<T>* m_pPrev;
public:
	MyNode() 
	{
		m_pData = new T();
		m_pNext = nullptr;
		m_pPrev = nullptr;
	};

	MyNode(T* data)
	{
		m_pData = data;
		m_pNext = nullptr;
		m_pPrev = nullptr;
	}

	virtual ~MyNode() 
	{	
		delete m_pData;	
	};

	MyNode& operator = (const MyNode& node)
	{
		m_pData = node.m_pData;
		return *this;
	}
};