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

// 리스트 초기화
bool InitNode();
// 리스트 끝에 노드 추가
void NewNode(char* pName, int Age, int Tel, char* pAddr);
// 대상 위치 뒤에 노드 삽입
bool InsertNode(TNode* pFindNode, char* pName, int Age, int Tel, char* pAddr);
// 대상 노드 수정
bool UpdateNode(TNode* pFindNode, char* pName, int Age, int Tel, char* pAddr);
// 대상 노드 노드 삭제
TNode* DeleteNode(TNode* pDelNode);
// 대상 노드 출력
bool PrintData(TNode* pFindNode);
// 전체 노드 출력
void PrintAllData();
// 노드 검색
bool SearchNode(char* name);
