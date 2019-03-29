#include "header.h"

bool InitNode() {
	g_pNodeHead = new TNode;
	strcpy_s(g_pNodeHead->name, "NULL");
	g_pNodeHead->pNextNode = g_pNodeHead;
	g_pNodeTail = g_pNodeHead;
	
	if (g_pNodeHead == NULL)
		return false;
	return true;
}

void NewRandomNode() {
	TNode* newNode = new TNode;

	char name[4] = { (int)rand() % 26 + 97 , (int)rand() % 26 + 65 , (int)rand() % 26 + 65, '\0' };
	char addr[6] = { (int)rand() % 26 + 65, (int)rand() % 26 + 65, (int)rand() % 26 + 65, (int)rand() % 26 + 65, (int)rand() % 26 + 65, '\0' };

	strcpy_s(newNode->name, name);
	newNode->age = rand() % 25 + 10 ;
	newNode->tel = rand() % 100000000 + 100000000;
	strcpy_s(newNode->addr, addr);
	newNode->pPrevNode = 0;
	newNode->pNextNode = 0;

	g_pNodeTail->pNextNode = newNode;
	newNode->pPrevNode = g_pNodeTail;
	g_pNodeTail = newNode;

	g_NodeCounter++;
}

void NewNode(char* pName, int Age, int Tel, char* pAddr) {
	TNode* newNode = new TNode;
	
	strcpy_s(newNode->name, pName);
	newNode->age = Age;
	newNode->tel = Tel;
	strcpy_s(newNode->addr, pAddr);
	newNode->pPrevNode = 0;
	newNode->pNextNode = 0;

	g_pNodeTail->pNextNode = newNode;
	newNode->pPrevNode = g_pNodeTail;
	g_pNodeTail = newNode;

	g_NodeCounter++;
}

bool InsertNode(TNode* pFindNode, char* pName, int Age, int Tel, char* pAddr) {
	if (pFindNode == NULL) return false;
	TNode* newNode = new TNode;

	strcpy_s(newNode->name, pName);
	newNode->age = Age;
	newNode->tel = Tel;
	strcpy_s(newNode->addr, pAddr);
	newNode->pPrevNode = 0;
	newNode->pNextNode = 0;

	TNode* tempNode = pFindNode->pNextNode;
	pFindNode->pNextNode = newNode;
	newNode->pPrevNode = pFindNode;
	newNode->pNextNode = tempNode;
	tempNode->pPrevNode = newNode;

	g_pFindNode = g_pFindNode->pNextNode;
	g_NodeCounter++;

	return true;
}

bool UpdateNode(TNode* pFindNode, char* pName = 0, int Age = 0, int Tel = 0, char* pAddr = 0) {
	if (pFindNode == NULL) return false;

	printf("#수정 전 데이터\n");
	PrintData(pFindNode);

	if(pName != NULL)
		strcpy_s(pFindNode->name, pName);
	if (Age != NULL)
		pFindNode->age = Age;
	if (Tel != NULL)
		pFindNode->tel = Tel;
	if (pAddr != NULL)
		strcpy_s(pFindNode->addr, pAddr);
	printf("#수정 후 데이터\n");
	PrintData(pFindNode);

	return true;
}

TNode* DeleteNode(TNode* pDelNode) {
	g_pFindNode = g_pFindNode->pNextNode;

	TNode* pDelPrevNode = pDelNode->pPrevNode;
	TNode* pDelNextNode = pDelNode->pNextNode;

	pDelPrevNode->pNextNode = pDelNextNode;
	pDelNextNode->pPrevNode = pDelPrevNode;

	delete pDelNode;
	

	return pDelNextNode;
}

bool SearchNode(char* name) {
	TNode* searchNode = g_pNodeHead->pNextNode;
	while (searchNode != NULL)
	{
		if (!strcmp(searchNode->name, name))
		{
			g_pFindNode = searchNode;
			return true;
		}
		searchNode = searchNode->pNextNode;
	}
	return false;
}

bool PrintData(TNode* pFindNode) {
	if (pFindNode == NULL) return false;

	printf("#%d 이름 : %s, 나이 : %d, 전번 : %d, 주소 : %s\n", 1,
		pFindNode->name, pFindNode->age, 
		pFindNode->tel, pFindNode->addr);

	return true;
}

void PrintAllData() {
	TNode* pPrintNode = g_pNodeHead->pNextNode;
	int cnt = 0;
	while (pPrintNode != g_pNodeHead && pPrintNode != NULL) {
		printf("#%d 이름 : %s, 나이 : %d, 전번 : %d, 주소 : %s\n", ++cnt,
			pPrintNode->name, pPrintNode->age,
			pPrintNode->tel, pPrintNode->addr);
		pPrintNode = pPrintNode->pNextNode;
	}
	printf("\n총 %d건의 노드가 있음다.\n", cnt);
}


void main() {


	if (!InitNode()) {
		printf("에러~");
		return;
	}

	srand((unsigned)time(0));
	int index = 0;
	char* name = new char[5];
	while(1)
	{
		printf("0 - 검색, 1 - 랜덤 삽입, 2 - 전체 출력 : ");
		scanf_s("%d", &index);

		switch (index)
		{
		case 0:
			do
			{
				printf("검색할 이름을 입력하세요 : ");
				scanf_s("%s", name, 5);
			} while (!strcmp(name, "\0"));
			if (!SearchNode(name))
			{
				printf("잘못된 입력 입니다.");
				continue;
			}
			PrintData(g_pFindNode);
			break;
		case 1:
			NewRandomNode();
			break;
		case 2:
			PrintAllData();
			//DeleteNode(g_pFindNode);
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 99:
			printf("ㅂㅂ~");
			getchar();	return;
			break;
		default:
			printf("다시 쓰셈\n");
			break;
		}
	}
	delete(name);

	/*
	NewNode("aa", 11, 000123456, "aa");
	NewNode("bbb", 22, 000000654, "aa123");
	NewNode("cc", 33, 01023654, "aa321");
	NewNode("dddd", 44, 010150, "aaaaa");
	NewNode("eee", 55, 010235645, "aaddasdda");
	NewNode("fff", 66, 0101010101, "aaqweweq123");

	PrintAllData();
	printf("\n\n");
	
	g_pFindNode = g_pNodeHead->pNextNode->pNextNode->pNextNode;
	InsertNode(g_pFindNode, "Insert Node", 00, 123123, "ajfkajfk");
	PrintData(g_pFindNode);
	printf("\n\n");

	DeleteNode(g_pFindNode);

	UpdateNode(g_pFindNode, "UpdateName", NULL, 123123132, NULL);
	printf("\n\n");
	PrintAllData();
	*/

	getchar();
}