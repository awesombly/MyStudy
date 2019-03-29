#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <memory>

class TreeNode {
public:
	int value;
	TreeNode* parentNode;
	TreeNode* leftNode;
	TreeNode* rightNode;

public:
	TreeNode() noexcept : value(0), leftNode(nullptr), rightNode(nullptr), parentNode(nullptr) { };
};



class Tree {
public:
	TreeNode* m_root;

public:
	Tree() noexcept;

	TreeNode** cutParent(TreeNode* node) noexcept;
	bool DeleteNode(TreeNode* node, const int& value) noexcept;
	TreeNode* AddNode(TreeNode* node, const int& value) noexcept;
	TreeNode* SearchNode(TreeNode* node, const int& value, int&& index = 0) const noexcept;
	void PrintTree(TreeNode* node, int&& index = 0) const noexcept;
};


Tree::Tree() noexcept
{
	m_root = new TreeNode();
	m_root->value = 50;
}

// 부모 연결 끊고 반환
TreeNode** Tree::cutParent(TreeNode* node) noexcept
{
	if (node->parentNode != nullptr)
	{
		auto parentNode = node->parentNode;

		if (parentNode->leftNode == node)
		{
			return &(parentNode->leftNode = nullptr);
		}
		else
		{
			return &(parentNode->rightNode = nullptr);
		}
	}
	return nullptr;
}

bool Tree::DeleteNode(TreeNode* node, const int& value) noexcept
{
	TreeNode* delnode = SearchNode(m_root, value);
	if (delnode == nullptr)
	{
		std::cout << "해당 노드가 없습니다." << std::endl;
		return false;
	}

	TreeNode* changeNode = nullptr;
	if (delnode->leftNode != nullptr)
	{
		changeNode = delnode->leftNode;
		while (changeNode->rightNode != nullptr)
		{
			changeNode = changeNode->rightNode;
		}
		cutParent(changeNode);
		
		if (changeNode->leftNode != nullptr)
		{
			changeNode->leftNode->parentNode = changeNode->parentNode;
			changeNode->parentNode->leftNode = changeNode->leftNode;
		}
	}
	else if (delnode->rightNode != nullptr)
	{
		changeNode = delnode->rightNode;
		while (changeNode->leftNode != nullptr)
		{
			changeNode = changeNode->leftNode;
		}
		cutParent(changeNode);

		if (changeNode->rightNode != nullptr)
		{
			changeNode->rightNode->parentNode = changeNode->parentNode;
			changeNode->parentNode->rightNode = changeNode->rightNode;
		}
	}
	changeNode->parentNode = delnode->parentNode;
	changeNode->leftNode = delnode->leftNode;
	changeNode->rightNode = delnode->rightNode;

	if(changeNode->leftNode != nullptr)
		changeNode->leftNode->parentNode = changeNode;
	if(changeNode->rightNode != nullptr)
		changeNode->rightNode->parentNode = changeNode;

	*cutParent(delnode) = changeNode;
	delete delnode;
	return true;
}

TreeNode* Tree::AddNode(TreeNode* node, const int& value) noexcept
{
	TreeNode* newnode = new TreeNode();
	TreeNode* comnode = node;

	newnode->value = value;
	if (newnode->value == comnode->value)
	{
		std::cout << "중복된 입력 입니다." << std::endl;
		return nullptr;
	}
	else if (newnode->value < comnode->value)
	{
		if (comnode->leftNode == nullptr)
		{
			newnode->parentNode = comnode;
			return comnode->leftNode = newnode;
		}
		else
		{
			return AddNode(comnode->leftNode, value);
		}
	}
	else
	{
		if (comnode->rightNode == nullptr)
		{
			newnode->parentNode = comnode;
			return comnode->rightNode = newnode;
		}
		else
		{
			return AddNode(comnode->rightNode, value);
		}
	}
}

TreeNode* Tree::SearchNode(TreeNode* node, const int& value, int&& index) const noexcept
{
	TreeNode* comnode = node;

	if (value == comnode->value)
	{
		printf("%d, %d층\n", value, index);
		return comnode;
	}
	else if (value < comnode->value)
	{
		if (comnode->leftNode == nullptr)
		{
			printf("찾는 노드가 없습니다.\n");
			return nullptr;
		}
		else
		{
			return SearchNode(comnode->leftNode, value, std::move(++index));
		}
	}
	else
	{
		if (comnode->rightNode == nullptr)
		{
			printf("찾는 노드가 없습니다.\n");
			return nullptr;
		}
		else
		{
			return SearchNode(comnode->rightNode, value, std::move(++index));
		}
	}

}

void Tree::PrintTree(TreeNode* node, int&& index) const noexcept
{
	if (node == nullptr) return;

	TreeNode* printNode = node;

	for (int i = index; i > 0; i--)
		std::cout << "  ";
	std::cout << printNode->value << std::endl;
	if (node == m_root)
		printf("\n");

	PrintTree(printNode->leftNode, std::move(++index));
	PrintTree(printNode->rightNode, std::move(++index));
}
