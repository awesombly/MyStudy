#include "header.h"
#define OVERFLOW -999

// �ʱ�ȭ
void InitQueue() {
	g_front = new Queue();
	g_rear = new Queue();
	g_front->PrevNode = NULL;
	g_front->NextNode = g_rear;
	g_rear->PrevNode = g_front;
	g_rear->NextNode = NULL;

	g_front->value = 0;
	g_rear->value = 0;
}
// ����
int Enqueue(int value) {
	Queue* enQueue = new Queue();

	enQueue->value = value;
	g_rear->PrevNode->NextNode = enQueue;
	enQueue->PrevNode = g_rear->PrevNode;
	enQueue->NextNode = g_rear;
	g_rear->PrevNode = enQueue;

	return value;
}
// ����
int Dequeue() {
	Queue* deQueue = g_front->NextNode;

	if (deQueue == NULL || deQueue == g_rear)
	{
		printf("������ ť�� �����ϴ�.\n");
		return OVERFLOW;
	}
	g_front->NextNode = g_front->NextNode->NextNode;
	g_front->NextNode->NextNode->PrevNode = g_front;
	int value = deQueue->value;

	delete deQueue;

	return value;
}
// ���
void PrintQueue() {
	Queue* pQueue = g_front->NextNode;

	while (pQueue != NULL && pQueue != g_rear)
	{
		printf("[%d]\t", pQueue->value);
		pQueue = pQueue->NextNode;
	}
	printf("\n");
}


void main() {
	InitQueue();

	while (1)
	{
		char* pInput = new char[50];
		char* pToken = 0;
		char* pContext = 0;

		printf("����� �Է��ϼ�( enqueue {value}, dequeue, print, end) : ");
		scanf_s("%[^\n]s", pInput, 50);

		pToken = strtok_s(pInput, " ", &pContext);
		if (pToken != NULL)
		{
			if (!strcmp(pToken, "enqueue"))
			{
				pToken = strtok_s(NULL, " ", &pContext);
				while (pToken != NULL)
				{
					Enqueue(atoi(pToken));
					pToken = strtok_s(NULL, " ", &pContext);
				}
				PrintQueue();
			}
			else if (!strcmp(pToken, "dequeue"))
			{
				Dequeue();
				PrintQueue();
			}
			else if (!strcmp(pToken, "print"))
			{
				PrintQueue();
			}
			else if (!strcmp(pToken, "end"))
			{
				printf("����~\n");
				getchar();
				return;
			}
			else
			{
				printf("�߸��� ��� �Դϴ�.\n");
			}

			getchar();
			delete[] pInput;
			pToken = NULL;
			pContext = NULL;
		}

	}



}