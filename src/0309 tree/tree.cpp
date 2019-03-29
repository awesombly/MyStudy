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

		printf("����� �Է��ϼ�( add {value}, delete {value}, search {value}, print, end) : ");
		scanf_s("%[^\n]s", pInput, 50);

		pToken = strtok_s(pInput, " ", &pContext);					// �������� �ɰ�
		if (pToken != NULL)
		{
			if (!strcmp(pToken, "add") || !strcmp(pToken, "a"))		// ��ū�� �´�?
			{
				pToken = strtok_s(NULL, " ", &pContext);			// ���� ��ū��
				while (pToken != NULL)								// ��� ������
				{
					tree.AddNode(tree.m_root, atoi(pToken));		// ����
					pToken = strtok_s(NULL, " ", &pContext);		// ��� �� �ֳ�
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