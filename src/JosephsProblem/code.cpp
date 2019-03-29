#include "header.h"

// ����Ʈ �ʱ�ȭ
bool InitList() {
	g_pNodeHead = new CNode;
	strcpy_s(g_pNodeHead->name, "Null");
	g_pNodeHead->pPrevNode = g_pNodeHead;
	g_pNodeHead->pNextNode = g_pNodeHead;

	if (g_pNodeHead == NULL) return false;
	return true;
}

// ��� ����
void InsertNode(char* Name) {
	CNode* pNewNode = new CNode;
	strcpy_s(pNewNode->name, Name);

	pNewNode->pPrevNode = g_pNodeHead->pPrevNode;
	g_pNodeHead->pPrevNode = pNewNode;
	pNewNode->pNextNode = g_pNodeHead;
	pNewNode->pPrevNode->pNextNode = pNewNode;
}

// ��� ����
void DeleteNode(CNode* pDelNode) {
	CNode* pDelPrevNode = pDelNode->pPrevNode;
	CNode* pDelNextNode = pDelNode->pNextNode;

	pDelPrevNode->pNextNode = pDelNextNode;
	pDelNextNode->pPrevNode = pDelPrevNode;

	delete pDelNode;
}

// ����Ʈ ���
void PrintList() {
	CNode* pPrintNode = g_pNodeHead->pNextNode;

	while ((pPrintNode != NULL) && (pPrintNode != g_pNodeHead))
	{
		printf("\(%s\),\t", pPrintNode->name);
		pPrintNode = pPrintNode->pNextNode;
	}
}

// �˰��� ����
void JosephsFunction(int JumpCnt) {
	CNode* pCurrentNode = g_pNodeHead->pNextNode;
	CNode* pTempNode = 0;

	printf("%s ", pCurrentNode->name);
	while (g_pNodeHead->pNextNode->pNextNode != g_pNodeHead)
	{
		for (int i = 0; i < JumpCnt; i++)
		{
			pCurrentNode = pCurrentNode->pNextNode;
			if (pCurrentNode == g_pNodeHead)
				pCurrentNode = pCurrentNode->pNextNode;
			printf(" -> %s", pCurrentNode->name);
		}
		printf("(die)");
		pTempNode = pCurrentNode->pPrevNode;
		if (pTempNode == g_pNodeHead)
			pTempNode = pTempNode->pPrevNode;
		DeleteNode(pCurrentNode);
		pCurrentNode = pTempNode;
		getchar();
	}
	printf("������ : %s", pCurrentNode->name);
}





void main() {
	if (!InitList()) {
		printf("����~\n");
		getchar(); getchar();
		return;
	}

	int nodeCnt = 0, jumpCnt;
	char name[20];

	do
	{
		printf("������ ��� ������ �Է� : ");
		scanf_s("%d", &nodeCnt);
	} while (nodeCnt <= 0);

	do
	{
		printf("�ǳʶ� ��� ������ �Է� : ");
		scanf_s("%d", &jumpCnt);
	} while (jumpCnt <= 0);

	printf("���� ��� �̸��� �Է� : ");
	getchar();
	do
	{
		if (!strcmp(gets_s(name), "\0"))
		{
			printf("�ٽ� �Է��ϼ��� : ");
			nodeCnt++;
			continue;
		}
		InsertNode(name);
	} while (--nodeCnt > 0);

	printf("\n");
	PrintList();
	printf("\n");

	JosephsFunction(jumpCnt);

	getchar();
}