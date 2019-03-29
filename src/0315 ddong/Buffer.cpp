#include "header.h"



Buffer::Buffer()
{
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
	m_hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(m_hBuffer[0], size);			// ���� ������ ����
	SetConsoleWindowInfo(m_hBuffer[0], true, &rect);		// ������ ���� ����
	SetConsoleCursorInfo(m_hBuffer[0], &cursorInfo);		// �ܼ� Ŀ�� ����

															// �ι�° ����
	m_hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(m_hBuffer[1], size);
	SetConsoleWindowInfo(m_hBuffer[1], true, &rect);
	SetConsoleCursorInfo(m_hBuffer[1], &cursorInfo);
}


Buffer::~Buffer()
{
	// ���� ����
	CloseHandle(m_hBuffer[0]);
	CloseHandle(m_hBuffer[1]);
}

/*
Buffer* Buffer::getInstance() {
if (instance == NULL) instance = new Buffer();
return instance;
}
*/

// ���� �Է�
void Buffer::BufferWrite(const char* string, const short& x, const short& y)
{
	DWORD dw;					// ��ǥ ���� ����ü
	COORD CursorPos = { x, y };	// ��ǥ ����
	SetConsoleCursorPosition(m_hBuffer[m_nScreenIndex], CursorPos);				// ��ǥ �̵�
	WriteFile(m_hBuffer[m_nScreenIndex], string, strlen(string), &dw, NULL);	// ���� ����
}

// ���� ���, ��ȯ
void Buffer::BufferFlipping()
{
	// ��� ȭ���� ��ȯ�Ѵٸ� Sleep�� �ʿ��ϰ�����, ���⼱ �����϶��� ��ȯ�ϹǷ� ���°� ������
	Sleep(33);				// ��ȯ ������(������)
	SetConsoleActiveScreenBuffer(m_hBuffer[m_nScreenIndex]);		// ���� Ȱ��ȭ->���� ���� ���
	m_nScreenIndex = !m_nScreenIndex;								// ���� ���� ����
}

// ���� Ŭ����
void Buffer::BufferClear()
{
	COORD coord = { 0, 0 };
	DWORD dw;
	// ������ ��� ������ �����̽��ٷ� ä��
	FillConsoleOutputCharacter(m_hBuffer[m_nScreenIndex], ' ', BUFFER_WIDTH * BUFFER_HEIGHT, coord, &dw);
}

// Ŀ�� �̵�
void Buffer::GotoXY(const short& x, const short& y)
{
	COORD CursorPos = { x, y };
	SetConsoleCursorPosition(m_hBuffer[m_nScreenIndex], CursorPos);				// ��ǥ �̵�
}