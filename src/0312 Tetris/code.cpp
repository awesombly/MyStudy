#include "header.h"

struct Tetromino {
	int x = 0;
	int y = 0;
	int type = 0;
	int rotate = 0;
};
Tetromino position;
int board[WIDTH][HEIGHT] = { 0, };
int block[7][4][4][4] = { // O
						{{{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, -1, -1 },{ 0, 0, -1, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, -1, -1 },{ 0, 0, -1, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, -1, -1 },{ 0, 0, -1, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, -1, -1 },{ 0, 0, -1, -1 } }},
						// L
						{{{ 0, 0, 0, 0 },{ 0, 0, -1, 0 },{ 0, 0, -1, 0 },{ 0, 0, -1, -1 } },
						{ { 0, 0, 0, 0 },{ 0, -1, -1, -1 },{ 0, -1, 0, 0 },{ 0, 0, 0, 0 } },
						{ { 0, 0, 0, 0 },{ 0, 0, -1, -1 },{ 0, 0, 0, -1 },{ 0, 0, 0, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, -1 },{ 0, -1, -1, -1 },{ 0, 0, 0, 0 } }},
						// J
						{{{ 0, 0, 0, 0 },{ 0, 0, 0, -1 },{ 0, 0, 0, -1 },{ 0, 0, -1, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, -1, 0, 0 },{ 0, -1, -1, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, -1, -1 },{ 0, 0, -1, 0 },{ 0, 0, -1, 0 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, -1, -1, -1 },{ 0, 0, 0, -1 } }},
						// T
						{{{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, -1, -1, -1 },{ 0, 0, -1, 0 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, -1 },{ 0, 0, -1, -1 },{ 0, 0, 0, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, -1, 0 },{ 0, -1, -1, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, -1, 0 },{ 0, 0, -1, -1 },{ 0, 0, -1, 0 } }},
						// Z
						{{{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, -1, -1, 0 },{ 0, 0, -1, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, -1 },{ 0, 0, -1, -1 },{ 0, 0, -1, 0 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, -1, -1, 0 },{ 0, 0, -1, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, -1 },{ 0, 0, -1, -1 },{ 0, 0, -1, 0 } } },
						// S
						{{{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, -1, -1 },{ 0, -1, -1, 0 } },
						{ { 0, 0, 0, 0 },{ 0, 0, -1, 0 },{ 0, 0, -1, -1 },{ 0, 0, 0, -1 } },
						{ { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, -1, -1 },{ 0, -1, -1, 0 } },
						{ { 0, 0, 0, 0 },{ 0, 0, -1, 0 },{ 0, 0, -1, -1 },{ 0, 0, 0, -1 } } },
						// I
						{{{ 0, 0, 0, -1 },{ 0, 0, 0, -1 },{ 0, 0, 0, -1 },{ 0, 0, 0, -1 } },
						{ { -1, -1, -1, -1 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 } },
						{ { 0, 0, 0, -1 },{ 0, 0, 0, -1 },{ 0, 0, 0, -1 },{ 0, 0, 0, -1 } },
						{ { -1, -1, -1, -1 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 } } } };
int score = 0;
bool isPlaying = false;

HANDLE hBuffer[2];
bool nScreenIndex;

void DrawScreen() {
	int startX = 3;
	int x = startX, y = 3;
	
	BufferClear();
	for (int i = 4; i <= HEIGHT; i++)
	{
		for (int k = -1; k <= WIDTH; k++)
		{
			x += 2;
			if (i == HEIGHT || k == -1 || k == WIDTH)
			{
				BufferWrite("��", x, y);
				continue;
			}
			switch (board[k][i])
			{
			case 0:
				BufferWrite("��", x, y);
				break;
			case 1:
				BufferWrite("��", x, y);
				break;
			case -1:
				BufferWrite("��", x, y);
				break;
			}
		}
		x = startX;
		y++;
	}
	char* str = new char[10];
	sprintf_s(str, 10, "%d", score);
	
	BufferWrite("Score : ", x + 3, ++y);
	BufferWrite(str, x + 11, y);

	if(!isPlaying)	BufferWrite("Game Over!!", x + 4, ++y);
	BufferFlipping();
}

void CreateBlock(int type) {
	if (type < 0 || type > 7)	type = rand() % 7;

	int center = WIDTH / 2 - 2;
	position.x = center;
	position.y = 0;
	position.type = type;
	position.rotate = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			board[k + center][i] = block[position.type][position.rotate][i][k];
		}
	}
}

bool MoveBlock(int dir) {
	for (int i = HEIGHT - 1; i >= 0; i--)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			if (board[k][i] == -1)
			{
				while ((k + dir < 0) || (k + dir >= WIDTH) || (board[k + dir][i] >= 1))	return false;
			}
		}
	}

	for (int i = HEIGHT - 1; i >= 0; i--)
	{
		if (dir < 0)
		{
			
			for (int k = 0; k < WIDTH; k++)
			{
				if (board[k][i] <= -1)
				{
					board[k][i]++;
					board[k + dir][i]--;
				}
			}
		}
		else
		{
			for (int k = WIDTH - 1; k >= 0; k--)
			{
				if (board[k][i] <= -1)
				{
					board[k][i]++;
					board[k + dir][i]--;
				}
			}
		}
	}

	position.x += dir;
	DrawScreen();
	return true;
}

bool DownBlock(bool isDrop = false) {
	bool isColision = false;

	for (int i = HEIGHT - 1; i >= 0; i--)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			if (board[k][i] == -1)
			{
				while ((i + 1 >= HEIGHT) || (board[k][i + 1] >= 1))
				{
					isColision = true;
					break;
				}
			}
		}
	}

	if (isColision)
	{
		for (int i = HEIGHT - 1; i >= 0; i--)
		{
			for (int k = 0; k < WIDTH; k++)
			{
				if (board[k][i] == -1)
				{
					board[k][i] = 1;
				}
			}
		}
		LineCheck();
		DrawScreen();
		OverCheck();
		CreateBlock(rand() % 7);
		return true;
	}
	else
	{
		for (int i = HEIGHT - 1; i >= 0; i--)
		{
			for (int k = 0; k < WIDTH; k++)
			{
				if (board[k][i] <= -1)
				{
					board[k][i]++;
					board[k][i + 1]--;
				}
			}
		}
		position.y++;
	}
	
	if (isDrop) return DownBlock(true);

	DrawScreen();
	return true;
}

void ChangeBlock(int x, int y) {
	int tempRot = position.rotate >= 3 ? 0 : position.rotate + 1;

	for (int height = 0; height < 4; height++)
	{
		for (int width = 0; width < 4; width++)
		{
			if (block[position.type][tempRot][height][width] == -1)
			{
				if (width + x < 0)	return ChangeBlock(x + 1, y);
				if (width + x >= WIDTH)	return ChangeBlock(x - 1, y);
				if (height + y >= HEIGHT)	return ChangeBlock(x, y - 1);
				if (board[width + x][height + y] >= 1)
				{
					return;
				}
			}
		}
	}

	for (int height = 0; height < 4; height++)
	{
		for (int width = 0; width < 4; width++)
		{
			if (block[position.type][position.rotate][height][width] == -1)
				board[width + position.x][height + position.y] = 0;
		}
	}

	position.x = x;
	position.y = y;
	position.rotate++;
	if (position.rotate > 3)	position.rotate = 0;

	for (int height = 0; height < 4; height++)
	{
		for (int width = 0; width < 4; width++)
		{
			if (block[position.type][position.rotate][height][width] == -1)
				board[width + x][height + y] = -1;
		}
	}

	DrawScreen();
}

void DestroyLine(int line, int combo)
{
	for (int k = 0; k < WIDTH; k++)
	{
		board[k][line] = 0;
	}

	for (int i = line; i > 0; i--)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			board[k][i] = board[k][i - 1];
			board[k][i - 1] = 0;
		}
	}
	score += (1000 + combo * 1000);
}

void LineCheck() {
	int blockCnt = 0, combo = 0;

	for (int i = HEIGHT - 1; i >= 4; i--)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			if (board[k][i] >= 1)
			{
				blockCnt++;
			}
		}
		if (blockCnt >= WIDTH)
		{
			DestroyLine(i, combo);
			i++;
			combo++;
		}
		blockCnt = 0;
	}
}

void KeyCheck() {
	while (isPlaying)
	{
		switch (_getch())
		{
		case LEFT:
			MoveBlock(-1);
			break;
		case RIGHT:
			MoveBlock(1);
			break;
		case UP:
			ChangeBlock(position.x, position.y);
			break;
		case DOWN:
			DownBlock(false);
			break;
		case ENTER:
			DownBlock(true);
			break;
		}
	}
}

void OverCheck() {
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			if (board[k][i] >= 1)
			{
				isPlaying = false;
				DrawScreen();
			}
		}
	}
}

void Process(void* p) {
	int timer = 900;
	clock_t start = clock(), end = 0;
	while (isPlaying)
	{
		end = clock();
		if (end - start > timer)
		{
			if (timer > 250)	timer -= 10;
			start = clock();
			DownBlock();
		}
	}
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
	WriteFile(hBuffer[nScreenIndex], string, strlen(string), &dw, NULL);	// ���� ����
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
	isPlaying = true;
	srand((unsigned)time(NULL));

	BufferInit();

	_beginthread(Process, 0, (void*)NULL);

	DrawScreen();
	CreateBlock(rand() % 7);
	KeyCheck();

	getchar();
	BufferDelete();
}