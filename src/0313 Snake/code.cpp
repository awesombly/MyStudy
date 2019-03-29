#include "header.h"

typedef struct Snake {
	int x, y;
	Snake* pNext;
	Snake* pPrev;
} Node;

Node* pHead = NULL;
Node* pTail = NULL;
int nodeCount = 0;
int dirX = 0, dirY = 0;

int board[WIDTH][HEIGHT] = { 0, };
int score = 0;
int itemCounter = 0;
int foodCount = 0;
bool isPlaying = false;


time_t start = clock(), end = 0;

HANDLE hBuffer[2];		// ���� �ڵ�
bool nScreenIndex;		// ���� ������ ����

void Initialize() {
	// ĳ���� �ʱ�ȭ
	pHead = new Node();
	pTail = new Node();
	
	pHead->x = pTail->x = rand() % (WIDTH - 4) + 2;
	pHead->y = pTail->y = rand() % (HEIGHT - 4) + 2;
	board[pHead->x][pHead->y] = -1;

	pHead->pNext = pTail;
	pHead->pPrev = NULL;
	pTail->pNext = NULL;
	pTail->pPrev = pHead;
	nodeCount += 2;
	for(int i = 0; i < 4; i++)
		AddTail();

	// ���� �ʱ�ȭ
	for (int height = 0; height < HEIGHT; height++)
	{
		for (int width = 0; width < WIDTH; width++)
		{
			if (height == 0 || height == HEIGHT - 1 || width == 0 || width == WIDTH - 1)
				board[width][height] = 10;
		}
	}

	// �ʱ� ������ ����
	for (int i = 0; i < 7; i++)
		CreateItem(rand() % 11);
}

/*
void DrawScreen() {
	system("cls");
	printf("\n��");

	for (int height = 0; height < HEIGHT; height++)
	{
		for (int width = 0; width < WIDTH; width++)
		{
			switch (board[width][height])
			{
			case 0:
				printf("��");
				break;
			case -1:
				printf("��");
				break;
			case -2:
				printf("��");
				break;
			case -3:
				printf("��");
				break;
			case -10:
				printf("��");
				break;
			case 1:
				printf("��");
				break;
			case 2:
				printf("��");
				break;
			case 3:
				printf("��");
				break;
			case 4:
				printf("��");
				break;
			case 5:
				printf("��");
				break;
			case 6:
				printf("��");
				break;
			case 7:
				printf("��");
				break;
			case 8:
				printf("��");
				break;
			case 9:
				printf("��");
				break;
			case 10:
				printf("��");
				break;
			}
		}
		printf("\n��");
	}
	printf("\t���� : %d,\t\t���� %d��", score, nodeCount);
}
*/
void DrawScreen() {
	int x = 0, y = 0;
	
	BufferClear();
	for (int height = 0; height < HEIGHT; height++)
	{
		for (int width = 0; width < WIDTH; width++)
		{
			switch (board[width][height])
			{
			case 0:
				//BufferWrite("��", x, y);
				break;
			case -1:
				BufferWrite("��", x, y);
				break;
			case -2:
				BufferWrite("��", x, y);
				break;
			case -3:
				BufferWrite("��", x, y);
				break;
			case -10:
				BufferWrite("��", x, y);
				break;
			case 1:
				BufferWrite("��", x, y);
				break;
			case 2:
				BufferWrite("��", x, y);
				break;
			case 3:
				BufferWrite("��", x, y);
				break;
			case 4:
				BufferWrite("��", x, y);
				break;
			case 5:
				BufferWrite("��", x, y);
				break;
			case 6:
				BufferWrite("��", x, y);
				break;
			case 7:
				BufferWrite("��", x, y);
				break;
			case 8:
				BufferWrite("��", x, y);
				break;
			case 9:
				BufferWrite("��", x, y);
				break;
			case 10:
				BufferWrite("��", x, y);
				break;
			}
			x += 2;
		}
		x = 0;
		y++;
	}
	char* str = new char[10];
	sprintf_s(str, 10, "%d", score);

	BufferWrite("���� :", x, y);
	BufferWrite(str, x + 8, y);

	BufferFlipping();
}

void KeyCheck() {
	while (isPlaying)
	{
		switch (_getch())
		{
		case LEFT:
			if (dirX <= 0)
			{
				dirX = -1;
				dirY = 0;
				Move();
			}
			break;
		case RIGHT:
			if (dirX >= 0)
			{
				dirX = 1;
				dirY = 0;
				Move();
			}
			break;
		case UP:
			if (dirY <= 0)
			{
				dirX = 0;
				dirY = -1;
				Move();
			}
			break;
		case DOWN:
			if (dirY >= 0)
			{
				dirX = 0;
				dirY = 1;
				Move();
			}
			break;
		default:
			break;
		}
	}
}

void Process(void* p) {
	int timer = 650;				// ���� ����
	while (isPlaying)
	{
		end = clock();
		if (end - start > timer)
		{
			if(timer > 150) timer -= 2;
			Move();
			if (itemCounter > 20)
			{
				itemCounter = 0;
				CreateItem(rand() % 11);
			}
			// ������ �ϳ��� ������ ����
			if (foodCount == 0)
				CreateItem(rand() % 6);
		}
	}
}

void ColisionCheck() { 
	switch(board[pHead->x][pHead->y])
	{
	case -2:		// ����
	case -3:
		GameOver();
		break;
	case 1:			// ����
		score += 200;
	case 2:
		score += 200;
		AddTail();
	case 3:
		score += 200;
	case 4:
		score += 200;
	case 5:
		score += 500;
		foodCount--;
		AddTail();
		board[pHead->x][pHead->y] = 0;
		break;
	case 6:			// ��
		score -= 100;
	case 7:
		score -= 100;
		DeleteTail();
	case 8:
		score -= 100;
	case 9:
		score -= 500;
		DeleteTail();
		board[pHead->x][pHead->y] = 0;
		break;
	case 10:
		GameOver();
		break;
	}
}

void Move() {
	if (dirX == 0 && dirY == 0)	return;
	start = clock();					// ������ ó�� ����

	// �������� ���󰡱�
	Node* body = pTail;
	int index = nodeCount;

	board[pTail->x][pTail->y] = 0;
	while (body != pHead)
	{
		body->x = body->pPrev->x;
		body->y = body->pPrev->y;
	
		body = body->pPrev;
	}
	// �Ӹ� �̵�
	board[pHead->x][pHead->y] = 0;
	pHead->x += dirX;
	pHead->y += dirY;
	ColisionCheck();

	// �����ǿ� ĳ���� �Է�
	body = pHead->pNext;
	while (body != NULL)
	{
		board[body->x][body->y] = -(index-- % 2 + 2);
		body = body->pNext;
	}
	// ��Ұų� �׾��ų�
	board[pHead->x][pHead->y] = isPlaying ? -1 : -10;
	
	itemCounter++;
	DrawScreen();
}

void CreateItem(int type) {
	int x = (rand() % WIDTH - 2) + 1;
	int y = (rand() % HEIGHT - 2) + 1;

	if (x == pHead->x || y == pHead->y || board[x][y] != 0)
		return CreateItem(type);

	if (type >= 1 && type <= 5)	foodCount++;
	board[x][y] = type;
}

void GameOver() {
	isPlaying = false;
	printf("\n\tGame Over!!\n");
}

void AddTail() { 
	Node* body = new Node();
	body->x = pHead->x;
	body->y = pHead->y;
	body->pNext = pTail;
	body->pPrev = pTail->pPrev;
	pTail->pPrev->pNext = body;
	pTail->pPrev = body;
	nodeCount++;
}

void DeleteTail() {
	Node* body = pTail->pPrev;
	body->pPrev->pNext = pTail;
	pTail->pPrev = body->pPrev;

	board[pTail->x][pTail->y] = 0;
	pTail->x = body->x;
	pTail->y = body->y;
	
	delete body;
	nodeCount--;

	if (nodeCount <= 2)
		GameOver();
}

// Ŀ�� �̵�
void GotoXY(short x, short y) {
	COORD CursorPos = { x, y };
	SetConsoleCursorPosition(hBuffer[nScreenIndex], CursorPos);				// ��ǥ �̵�
}

// ���� �ʱ�ȭ
void BufferInit() {
	COORD size = { BUFFER_WIDTH, BUFFER_HEIGHT };
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Right = BUFFER_WIDTH - 1;
	rect.Top = 0;
	rect.Bottom = BUFFER_HEIGHT - 1;
	// �ܼ� Ŀ�� ����
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = false;

	// ����[0] = ���� ���� �Լ�
	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[0], size);		// ���� ������ ����
	SetConsoleWindowInfo(hBuffer[0], true, &rect);		// ������ ���� ����
	SetConsoleCursorInfo(hBuffer[0], &cursorInfo);		// �ܼ� Ŀ�� ����
	
	// �ι�° ����
	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[1], size);
	SetConsoleWindowInfo(hBuffer[1], true, &rect);
	SetConsoleCursorInfo(hBuffer[1], &cursorInfo);
}

// ���� �Է�
void BufferWrite(char* string, short x, short y) {
	DWORD dw;					// ��ǥ ���� ����ü
	COORD CursorPos = { x, y };	// ��ǥ ����
	SetConsoleCursorPosition(hBuffer[nScreenIndex], CursorPos);				// ��ǥ �̵�
	WriteFile(hBuffer[nScreenIndex], string, (DWORD)strlen(string), &dw, NULL);	// ���� ����
}

// ���� ���, ��ȯ
void BufferFlipping() {
	// ��� ȭ���� ��ȯ�Ѵٸ� Sleep�� �ʿ��ϰ�����, ���⼱ �����϶��� ��ȯ�ϹǷ� ���°� ������
	//Sleep(33);				// ��ȯ ������(������)
	SetConsoleActiveScreenBuffer(hBuffer[nScreenIndex]);		// ���� Ȱ��ȭ->���� ���� ���
	nScreenIndex = !nScreenIndex;								// ���� ���� ����
}

// ���� Ŭ����
void BufferClear() {
	COORD coord = { 0, 0 };
	DWORD dw;
	// ������ ��� ������ �����̽��ٷ� ä��
	FillConsoleOutputCharacter(hBuffer[nScreenIndex], ' ', BUFFER_WIDTH * BUFFER_HEIGHT, coord, &dw);
}

// ���� ����
void BufferDelete() {
	// ���� ����
	CloseHandle(hBuffer[0]);
	CloseHandle(hBuffer[1]);
}


void main() {
	
	BufferInit();


	isPlaying = true;
	srand((unsigned)time(NULL));
	Initialize();


	_beginthread(Process, 0, NULL);
	DrawScreen();
	KeyCheck();
	
	while (1);
	BufferDelete();
}