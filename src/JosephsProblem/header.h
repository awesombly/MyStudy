#pragma once

#include <stdio.h>
#include <string.h>


typedef struct CircularNode {

	char name[20];
	CircularNode* pPrevNode;
	CircularNode* pNextNode;

} CNode;

CNode* g_pNodeHead = 0;

// ����Ʈ �ʱ�ȭ
bool InitList();
// ��� ����
void InsertNode(char* Name);
// ��� ����
void DeleteNode(CNode* pDelNode);
// ����Ʈ ���
void PrintList();
// �˰��� ����
void JosephsFunction(int JumpCnt);