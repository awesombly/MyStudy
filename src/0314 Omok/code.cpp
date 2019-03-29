#include "header.h"

int g_Board[ROWS][COLS];
int g_iCursorX = COLS, g_iCursorY = 9;	// Ŀ�� ��ġ
int g_bPlayer = true;					// �÷��̾�
int g_iGameLoop = PLAYING;				// ���� ���� ����
int g_iGameState = GAME_INIT;
bool g_bRestart = false;				// �����

// �ʱ�ȭ
void Initialize() {
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			g_Board[row][col] = 0;
		}
	}
	DrawScreen();
};
// Ű �Է�
void KeyCheck() {
	bool isTurn = true;
	while (isTurn)
	{
		switch (_getch())
		{
		case LEFT:
			if (g_iCursorX - 2 < 0)
				continue;
			g_iCursorX -= 2;
			break;
		case RIGHT:
			if (g_iCursorX + 2 >= COLS * 2)
				continue;
			g_iCursorX += 2;
			break;
		case UP:
			if (g_iCursorY - 1 <= 0)
				continue;
			g_iCursorY--;
			break;
		case DOWN:
			if (g_iCursorY + 1 > ROWS)
				continue;
			g_iCursorY++;
			break;
		case ENTER:
			if (g_Board[g_iCursorY - 1][g_iCursorX / 2] != 0)
			{
				GotoXY(2, ROWS + 3);
				printf("�װ��� ���� �� �����ϴ�.");
				break;
			}

			if (g_bPlayer)
				g_Board[g_iCursorY - 1][g_iCursorX / 2] = 1;
			else
				g_Board[g_iCursorY - 1][g_iCursorX / 2] = 2;

			isTurn = false;
			break;
		}
		GotoXY(g_iCursorX, g_iCursorY);
	}
}

// ��ǥ �̵�
void GotoXY(short x, short y) {
	COORD Pos = { x - 1, y - 1 };			// x, y�� ������ ����ü
	// �ܼ�â Ŀ�� ��ġ�� ����(ǥ��������ڵ��� ������, ��ġ);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
// ���
void DrawScreen() {
	system("cls");

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			switch (g_Board[row][col])
			{
			case 1:
				printf("��");
				break;
			case 2:
				printf("��");
				break;
			default:
				switch (row)
				{
				case 0:
					if (col == 0)
						printf("��");
					else if (col == COLS - 1)
						printf("��");
					else
						printf("��");
					break;
				case ROWS - 1:
					if (col == 0)
						printf("��");
					else if (col == COLS - 1)
						printf("��");
					else
						printf("��");
					break;
				default:
					if (col == 0)
						printf("��");
					else if (col == COLS - 1)
						printf("��");
					else
						printf("��");
					break;
				}
				break;
			}
		}
		printf("\n");
	}
	GotoXY(2, ROWS + 1);
	printf("����Ű - �̵�, ���� - �� ����");
	GotoXY(2, ROWS + 2);
	printf("1p = ��\t2p = �� \n ���� ��ǥ : [ %d, %d ]", g_iCursorX / 2 + 1, g_iCursorY);
	GotoXY(g_iCursorX, g_iCursorY);
}

/* ����
void Judgment() {
	g_bPlayer = !g_bPlayer;

	int cal[8][4] = { 0, };
	int total = 0;
	g_iGameLoop = PLAYING;

	//  ������ ĭ ��ŭ �ݺ� //
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			// �ش� �÷��̾���
			for (int player = 1; player <= 2; player++)
			{
				// �ش� ���� �ִٸ�,
				if (player == g_Board[row][col])
				{
					// �ش� ���� �߽����� kĭ��ŭ ������ �����¿� �밢�� ĭ��
					for (int k = 1; k < 5; k++)
					{
						// �ش� �÷��̾��� ���� �ִٸ� �迭�� üũ
						int cnt = 0;
						for (int rowI = -1; rowI <= 1; rowI++)
						{
							for (int colI = -1; colI <= 1; colI++)
							{
								if (rowI == 0 && colI == 0)	continue;
								if (player == g_Board[row - (rowI * k)][col - (colI * k)])
									cal[cnt++][k - 1] = 1;
							}
						}
					}
					// ���� üũ
					for (int i = 0; i < 8; i++)
					{
						for (int k = 0; k < 4; k++)
						{
							total += cal[i][k];
						}

						if (total == 4)
						{
							if (player == 1)
								g_iGameLoop = BLACKWIN;
							else
								g_iGameLoop = WHITEWIN;
						}
						total = 0;
					}
					// �ʱ�ȭ
					memset(cal, 0, sizeof(cal));
				}
			}
		}
	}

}
*/

void Judgment() {
	g_bPlayer = !g_bPlayer;
	g_iGameLoop = PLAYING;

	for (int col = 0; col < COLS; col++)
	{
		for (int row = 0; row < ROWS; row++)
		{
			for (int dir = 0; dir < 8; dir++)
			{
				board[]
			}
		}
	}














}

void main() {
	g_iGameState = GAME_INIT;

	// ������
	while (g_iGameLoop == PLAYING)
	{
		// (Init)->Key->Draw->Judgment->Key...
		switch (g_iGameState)
		{
		case GAME_INIT:
			Initialize();
			g_iGameState = KEY_CHECK;
			g_bRestart = false;
			g_bPlayer = true;
			break;
		case KEY_CHECK:
			KeyCheck();
			g_iGameState = GAME_DRAW;
			break;
		case GAME_DRAW:
			DrawScreen();
			g_iGameState = GAME_CHECK;
			break;
		case GAME_CHECK:
			Judgment();
			g_iGameState = KEY_CHECK;
			break;
		}
	}

	if (g_iGameLoop == WHITEWIN || g_iGameLoop == BLACKWIN)
	{
		DrawScreen();
		GotoXY(3, 23);
		printf("%s�� �¸�!!.", g_iGameLoop == WHITEWIN ? "��" : "��");
		getchar();
	}

}