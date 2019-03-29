#include "header.h"



Buffer::Buffer()
{
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
	m_hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(m_hBuffer[0], size);			// 버퍼 사이즈 설정
	SetConsoleWindowInfo(m_hBuffer[0], true, &rect);		// 윈도우 버퍼 설정
	SetConsoleCursorInfo(m_hBuffer[0], &cursorInfo);		// 콘솔 커서 설정

															// 두번째 버퍼
	m_hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(m_hBuffer[1], size);
	SetConsoleWindowInfo(m_hBuffer[1], true, &rect);
	SetConsoleCursorInfo(m_hBuffer[1], &cursorInfo);
}


Buffer::~Buffer()
{
	// 버퍼 닫음
	CloseHandle(m_hBuffer[0]);
	CloseHandle(m_hBuffer[1]);
}

/*
Buffer* Buffer::getInstance() {
if (instance == NULL) instance = new Buffer();
return instance;
}
*/

// 버퍼 입력
void Buffer::BufferWrite(const char* string, const short& x, const short& y)
{
	DWORD dw;					// 좌표 저장 구조체
	COORD CursorPos = { x, y };	// 좌표 설정
	SetConsoleCursorPosition(m_hBuffer[m_nScreenIndex], CursorPos);				// 좌표 이동
	WriteFile(m_hBuffer[m_nScreenIndex], string, strlen(string), &dw, NULL);	// 버퍼 쓰기
}

// 버퍼 출력, 전환
void Buffer::BufferFlipping()
{
	// 계속 화면을 전환한다면 Sleep이 필요하겠지만, 여기선 움직일때만 전환하므로 없는게 나을듯
	Sleep(33);				// 전환 딜레이(프레임)
	SetConsoleActiveScreenBuffer(m_hBuffer[m_nScreenIndex]);		// 버퍼 활성화->버퍼 내용 출력
	m_nScreenIndex = !m_nScreenIndex;								// 다음 버퍼 선택
}

// 버퍼 클리어
void Buffer::BufferClear()
{
	COORD coord = { 0, 0 };
	DWORD dw;
	// 버퍼의 모든 공간을 스페이스바로 채움
	FillConsoleOutputCharacter(m_hBuffer[m_nScreenIndex], ' ', BUFFER_WIDTH * BUFFER_HEIGHT, coord, &dw);
}

// 커서 이동
void Buffer::GotoXY(const short& x, const short& y)
{
	COORD CursorPos = { x, y };
	SetConsoleCursorPosition(m_hBuffer[m_nScreenIndex], CursorPos);				// 좌표 이동
}