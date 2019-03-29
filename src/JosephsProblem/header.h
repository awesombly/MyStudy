#pragma once

#include <stdio.h>
#include <string.h>


typedef struct CircularNode {

	char name[20];
	CircularNode* pPrevNode;
	CircularNode* pNextNode;

} CNode;

CNode* g_pNodeHead = 0;

// 리스트 초기화
bool InitList();
// 노드 삽입
void InsertNode(char* Name);
// 노드 삭제
void DeleteNode(CNode* pDelNode);
// 리스트 출력
void PrintList();
// 알고리즘 실행
void JosephsFunction(int JumpCnt);