#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

typedef struct QueueNode {
	int value;
	QueueNode* PrevNode;
	QueueNode* NextNode;
} Queue;

Queue* g_front = 0;
Queue* g_rear = 0;

int g_nodeCount = 0;

// 초기화
void InitQueue();
// 삽입
int Enqueue(int value);
// 삭제
int Dequeue();
// 출력
void PrintQueue();