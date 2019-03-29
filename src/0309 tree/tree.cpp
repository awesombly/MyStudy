#include "header.h"
//#define MAX_NODE





void main() {
	//srand((unsigned)time(NULL));
	Tree tree;

	while (1)
	{
		char* pInput = new char[50];
		char* pToken = 0;
		char* pContext = 0;

		printf("명령을 입력하셈( add {value}, delete {value}, search {value}, print, end) : ");
		scanf_s("%[^\n]s", pInput, 50);

		pToken = strtok_s(pInput, " ", &pContext);					// 공백으로 쪼갬
		if (pToken != NULL)
		{
			if (!strcmp(pToken, "add") || !strcmp(pToken, "a"))		// 토큰이 맞니?
			{
				pToken = strtok_s(NULL, " ", &pContext);			// 다음 토큰줘
				while (pToken != NULL)								// 명령 있으면
				{
					tree.AddNode(tree.m_root, atoi(pToken));		// 실행
					pToken = strtok_s(NULL, " ", &pContext);		// 명령 더 있냐
				}
				tree.PrintTree(tree.m_root);
			}
			else if (!strcmp(pToken, "delete") || !strcmp(pToken, "d"))
			{
				pToken = strtok_s(NULL, " ", &pContext);
				while (pToken != NULL)
				{
					tree.DeleteNode(tree.m_root, atoi(pToken));
					pToken = strtok_s(NULL, " ", &pContext);
				}
			}
			else if (!strcmp(pToken, "search") || !strcmp(pToken, "s"))
			{
				pToken = strtok_s(NULL, " ", &pContext);
				while (pToken != NULL)
				{
					tree.SearchNode(tree.m_root, atoi(pToken));
					pToken = strtok_s(NULL, " ", &pContext);
				}
			}
			else if (!strcmp(pToken, "print") || !strcmp(pToken, "p"))
			{
				tree.PrintTree(tree.m_root);
			}
			else if (!strcmp(pToken, "end") || !strcmp(pToken, "e"))
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