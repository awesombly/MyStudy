#include "header.h"

int g_Board[ROWS][COLS];
int g_iCursorX = COLS, g_iCursorY = 9;	// 커서 위치
int g_bPlayer = true;					// 플레이어
int g_iGameLoop = PLAYING;				// 게임 진행 판정
int g_iGameState = GAME_INIT;
bool g_bRestart = false;				// 재시작

// 초기화
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
// 키 입력
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
				printf("그곳엔 놓을 수 없습니다.");
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

// 좌표 이동
void GotoXY(short x, short y) {
	COORD Pos = { x - 1, y - 1 };			// x, y를 가지는 구조체
	// 콘솔창 커서 위치를 설정(표준입출력핸들을 가져옴, 위치);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
// 출력
void DrawScreen() {
	system("cls");

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			switch (g_Board[row][col])
			{
			case 1:
				printf("○");
				break;
			case 2:
				printf("●");
				break;
			default:
				switch (row)
				{
				case 0:
					if (col == 0)
						printf("┌");
					else if (col == COLS - 1)
						printf("┐");
					else
						printf("┬");
					break;
				case ROWS - 1:
					if (col == 0)
						printf("└");
					else if (col == COLS - 1)
						printf("┘");
					else
						printf("┴");
					break;
				default:
					if (col == 0)
						printf("├");
					else if (col == COLS - 1)
						printf("┤");
					else
						printf("┼");
					break;
				}
				break;
			}
		}
		printf("\n");
	}
	GotoXY(2, ROWS + 1);
	printf("방향키 - 이동, 엔터 - 돌 놓기");
	GotoXY(2, ROWS + 2);
	printf("1p = ○\t2p = ● \n 현재 좌표 : [ %d, %d ]", g_iCursorX / 2 + 1, g_iCursorY);
	GotoXY(g_iCursorX, g_iCursorY);
}

/* 판정
void Judgment() {
	g_bPlayer = !g_bPlayer;

	int cal[8][4] = { 0, };
	int total = 0;
	g_iGameLoop = PLAYING;

	//  보드판 칸 만큼 반복 //
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			// 해당 플레이어의
			for (int player = 1; player <= 2; player++)
			{
				// 해당 돌이 있다면,
				if (player == g_Board[row][col])
				{
					// 해당 돌을 중심으로 k칸만큼 떨어진 상하좌우 대각선 칸에
					for (int k = 1; k < 5; k++)
					{
						// 해당 플레이어의 돌이 있다면 배열에 체크
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
					// 오목 체크
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
					// 초기화
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

	// 게임중
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
		printf("%s이 승리!!.", g_iGameLoop == WHITEWIN ? "백" : "흑");
		getchar();
	}

}