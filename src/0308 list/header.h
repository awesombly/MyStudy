#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // rand
#include <conio.h>   // getchar?
#include <time.h>

typedef struct TNodeData {
	char name[20];
	int age;
	int tel;
	char addr[20];
	TNodeData* pPrevNode;
	TNodeData* pNextNode;
} TNode;

TNode* g_pNodeHead = 0;
TNode* g_pNodeTail = 0;
TNode* g_pFindNode = 0;
int g_NodeCounter = 0;

// ����Ʈ �ʱ�ȭ
bool InitNode();
// ����Ʈ ���� ��� �߰�
void NewNode(char* pName, int Age, int Tel, char* pAddr);
// ��� ��ġ �ڿ� ��� ����
bool InsertNode(TNode* pFindNode, char* pName, int Age, int Tel, char* pAddr);
// ��� ��� ����
bool UpdateNode(TNode* pFindNode, char* pName, int Age, int Tel, char* pAddr);
// ��� ��� ��� ����
TNode* DeleteNode(TNode* pDelNode);
// ��� ��� ���
bool PrintData(TNode* pFindNode);
// ��ü ��� ���
void PrintAllData();
// ��� �˻�
bool SearchNode(char* name);
