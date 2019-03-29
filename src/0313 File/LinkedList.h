#pragma once
#include "MyNode.h"

template <typename T>
class LinkedList
{
public:
	LinkedList();
	~LinkedList();
public:
	void Clear();
	void AddNode(MyNode<T>* pNode = nullptr);
	void AddNode(T* pNode);
	bool DeleteIndex(int index = -1);
	MyNode<T>& FindIndex(int index) const;
	MyNode<T>* Begin() const;
	MyNode<T>* End() const;
	bool isEmpty() const;
	int getCount() const;
private:
	MyNode<T>* m_pHead;
	MyNode<T>* m_pTail;
	int m_dataCounter;
};


template <typename T>
LinkedList<T>::LinkedList()
{
	m_pHead = new MyNode<T>();
	m_pHead->m_pNext = nullptr;
	m_pTail = m_pHead;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
	Clear();
	delete m_pHead;
}

template <typename T>
void LinkedList<T>::Clear()
{
	while (DeleteIndex(1));
	
}

template <typename T>
void LinkedList<T>::AddNode(MyNode<T>* pNode)
{
	if (pNode == nullptr)
	{
		pNode = new MyNode<T>();
	}
	m_pTail->m_pNext = pNode;
	m_pTail = m_pTail->m_pNext;
	m_dataCounter++;
}

template <typename T>
void LinkedList<T>::AddNode(T* tNode)
{
	MyNode<T>* pNode = new MyNode<T>(tNode);

	m_pTail->m_pNext = pNode;
	m_pTail = m_pTail->m_pNext;
	m_dataCounter++;
}

template <typename T>
bool LinkedList<T>::DeleteIndex(int index)
{
	MyNode<T>* pNode = m_pHead;
	if (pNode->m_pNext == nullptr)
		return false;
	while (--index > 0)
	{
		if ((pNode = pNode->m_pNext) == nullptr)
			return false;
	}

	MyNode<T>* delNode = pNode->m_pNext;
	if (delNode == m_pTail)
		m_pTail = pNode;
	pNode->m_pNext = delNode->m_pNext;

	delete delNode;
	--m_dataCounter;

	return true;
}

template <typename T>
MyNode<T>& LinkedList<T>::FindIndex(int index) const
{
	MyNode<T>* pNode = m_pHead;
	while (index-- > 0)
	{
		if ((pNode = pNode->m_pNext) == nullptr)	
			throw exception("해당 노드가 없습니다.");
	}
	
	return *pNode;
}

template <typename T>
bool LinkedList<T>::isEmpty() const
{
	return m_dataCounter == 0 ? true : false;
}

template <typename T>
int LinkedList<T>::getCount() const 
{
	return m_dataCounter;
}

template <typename T>
MyNode<T>* LinkedList<T>::Begin() const
{
	return m_pHead->m_pNext == nullptr ? m_pHead : m_pHead->m_pNext;
}

template <typename T>
MyNode<T>* LinkedList<T>::End() const
{
	return nullptr;
}