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

HANDLE hBuffer[2];		// 버퍼 핸들
bool nScreenIndex;		// 현재 선택한 버퍼

void Initialize() {
	// 캐릭터 초기화
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

	// 보드 초기화
	for (int height = 0; height < HEIGHT; height++)
	{
		for (int width = 0; width < WIDTH; width++)
		{
			if (height == 0 || height == HEIGHT - 1 || width == 0 || width == WIDTH - 1)
				board[width][height] = 10;
		}
	}

	// 초기 아이템 생성
	for (int i = 0; i < 7; i++)
		CreateItem(rand() % 11);
}

/*
void DrawScreen() {
	system("cls");
	printf("\n　");

	for (int height = 0; height < HEIGHT; height++)
	{
		for (int width = 0; width < WIDTH; width++)
		{
			switch (board[width][height])
			{
			case 0:
				printf("　");
				break;
			case -1:
				printf("＠");
				break;
			case -2:
				printf("＊");
				break;
			case -3:
				printf("＊");
				break;
			case -10:
				printf("Ｘ");
				break;
			case 1:
				printf("★");
				break;
			case 2:
				printf("♣");
				break;
			case 3:
				printf("♬");
				break;
			case 4:
				printf("♨");
				break;
			case 5:
				printf("♠");
				break;
			case 6:
				printf("▩");
				break;
			case 7:
				printf("▒");
				break;
			case 8:
				printf("▦");
				break;
			case 9:
				printf("▣");
				break;
			case 10:
				printf("■");
				break;
			}
		}
		printf("\n　");
	}
	printf("\t점수 : %d,\t\t꼬리 %d개", score, nodeCount);
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
				//BufferWrite("　", x, y);
				break;
			case -1:
				BufferWrite("＠", x, y);
				break;
			case -2:
				BufferWrite("＊", x, y);
				break;
			case -3:
				BufferWrite("＊", x, y);
				break;
			case -10:
				BufferWrite("Ｘ", x, y);
				break;
			case 1:
				BufferWrite("★", x, y);
				break;
			case 2:
				BufferWrite("♣", x, y);
				break;
			case 3:
				BufferWrite("♬", x, y);
				break;
			case 4:
				BufferWrite("♨", x, y);
				break;
			case 5:
				BufferWrite("♠", x, y);
				break;
			case 6:
				BufferWrite("▩", x, y);
				break;
			case 7:
				BufferWrite("▒", x, y);
				break;
			case 8:
				BufferWrite("▦", x, y);
				break;
			case 9:
				BufferWrite("▣", x, y);
				break;
			case 10:
				BufferWrite("■", x, y);
				break;
			}
			x += 2;
		}
		x = 0;
		y++;
	}
	char* str = new char[10];
	sprintf_s(str, 10, "%d", score);

	BufferWrite("점수 :", x, y);
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
	int timer = 650;				// 실행 간격
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
			// 음식이 하나도 없을시 생성
			if (foodCount == 0)
				CreateItem(rand() % 6);
		}
	}
}

void ColisionCheck() { 
	switch(board[pHead->x][pHead->y])
	{
	case -2:		// 꼬리
	case -3:
		GameOver();
		break;
	case 1:			// 음식
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
	case 6:			// 벽
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
	start = clock();					// 쓰레드 처리 갱신

	// 꼬리부터 따라가기
	Node* body = pTail;
	int index = nodeCount;

	board[pTail->x][pTail->y] = 0;
	while (body != pHead)
	{
		body->x = body->pPrev->x;
		body->y = body->pPrev->y;
	
		body = body->pPrev;
	}
	// 머리 이동
	board[pHead->x][pHead->y] = 0;
	pHead->x += dirX;
	pHead->y += dirY;
	ColisionCheck();

	// 보드판에 캐릭터 입력
	body = pHead->pNext;
	while (body != NULL)
	{
		board[body->x][body->y] = -(index-- % 2 + 2);
		body = body->pNext;
	}
	// 살았거나 죽었거나
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

// 커서 이동
void GotoXY(short x, short y) {
	COORD CursorPos = { x, y };
	SetConsoleCursorPosition(hBuffer[nScreenIndex], CursorPos);				// 좌표 이동
}

// 버퍼 초기화
void BufferInit() {
	COORD size = { BUFFER_WIDTH, BUFFER_HEIGHT };
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Right = BUFFER_WIDTH - 1;
	rect.Top = 0;
	rect.Bottom = BUFFER_HEIGHT - 1;
	// 콘솔 커서 정보
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = false;

	// 버퍼[0] = 버퍼 생성 함수
	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[0], size);		// 버퍼 사이즈 설정
	SetConsoleWindowInfo(hBuffer[0], true, &rect);		// 윈도우 버퍼 설정
	SetConsoleCursorInfo(hBuffer[0], &cursorInfo);		// 콘솔 커서 설정
	
	// 두번째 버퍼
	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[1], size);
	SetConsoleWindowInfo(hBuffer[1], true, &rect);
	SetConsoleCursorInfo(hBuffer[1], &cursorInfo);
}

// 버퍼 입력
void BufferWrite(char* string, short x, short y) {
	DWORD dw;					// 좌표 저장 구조체
	COORD CursorPos = { x, y };	// 좌표 설정
	SetConsoleCursorPosition(hBuffer[nScreenIndex], CursorPos);				// 좌표 이동
	WriteFile(hBuffer[nScreenIndex], string, (DWORD)strlen(string), &dw, NULL);	// 버퍼 쓰기
}

// 버퍼 출력, 전환
void BufferFlipping() {
	// 계속 화면을 전환한다면 Sleep이 필요하겠지만, 여기선 움직일때만 전환하므로 없는게 나을듯
	//Sleep(33);				// 전환 딜레이(프레임)
	SetConsoleActiveScreenBuffer(hBuffer[nScreenIndex]);		// 버퍼 활성화->버퍼 내용 출력
	nScreenIndex = !nScreenIndex;								// 다음 버퍼 선택
}

// 버퍼 클리어
void BufferClear() {
	COORD coord = { 0, 0 };
	DWORD dw;
	// 버퍼의 모든 공간을 스페이스바로 채움
	FillConsoleOutputCharacter(hBuffer[nScreenIndex], ' ', BUFFER_WIDTH * BUFFER_HEIGHT, coord, &dw);
}

// 버퍼 삭제
void BufferDelete() {
	// 버퍼 닫음
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