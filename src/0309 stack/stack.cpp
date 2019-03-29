#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#define MAX 10
#define OVERFLOW -1


int g_top = -1;
int g_stack[MAX] = { 0 };

// 초기화
void InitStack() {
	for(int i = 0; i < MAX; i++)
	{
		g_stack[i] = 0;
	}
	g_top = -1;
}
// 출력
void PrintStack() {
	for (int i = 0; i <= g_top; i++)
		printf("[%d]\t", g_stack[i]);
	printf("\n");
}
// 푸쉬
int Push(int num) {
	if (++g_top >= MAX)
	{
		g_top--;
		printf("Stack Overflow~!\n");
		return OVERFLOW;
	}
	g_stack[g_top] = num;
	return num;
}
// 팝
int Pop() {
	int temp = 0;
	if (g_top < 0)
	{
		printf("Stack Underflow~!\n");
		return OVERFLOW;
	}
	temp = g_stack[g_top--] = 0;
	return temp;
}

void main() {
	InitStack();


	while (1)
	{
		char* pInput = new char[50];
		char* pToken = 0;
		char* pContext = 0;

		printf("명령을 입력하셈(push {value}, pop, print, end) : ");
		scanf_s("%[^\n]s", pInput, 50);

		pToken = strtok_s(pInput, " ", &pContext);
		if (pToken != NULL)
		{
			if (!strcmp(pToken, "push"))
			{
				pToken = strtok_s(NULL, " ", &pContext);
				while (pToken != NULL)
				{
					Push(atoi(pToken));
					pToken = strtok_s(NULL, " ", &pContext);
				}
				PrintStack();
			}
			else if (!strcmp(pToken, "pop"))
			{
				Pop();
				PrintStack();
			}
			else if (!strcmp(pToken, "print"))
			{
				PrintStack();
			}
			else if (!strcmp(pToken, "end"))
			{
				printf("ㅂㅂ~\n");
				getchar();
				return;
			}
			else
			{
				printf("잘못된 명령 입니다.\n");
			}

			getchar();
			delete[] pInput;
			pToken = NULL;
			pContext = NULL;
		}

	}

}





