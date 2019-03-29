#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <process.h>
#include <Windows.h>


#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13

#define WIDTH 10
#define HEIGHT 24

#define BUFFER_WIDTH 80
#define BUFFER_HEIGHT 80

// ȭ�� ���
void DrawScreen();
// ��� ����
void CreateBlock(int type);
// ��� �̵�
bool MoveBlock(int dir);
// ��� �ϰ�(1=���)
bool DownBlock(bool drop);
// ��� ������
void ChangeBlock(int x, int y);
// �� �ı�
void DestroyLine(int line, int combo);
// �� üũ
void LineCheck();
// Ű �Է� üũ
void KeyCheck();
// ���� ���� ó��
void OverCheck();
// ��� �ϰ� ó��
void Process(void* p);

void GotoXY(short x, short y);
void BufferInit();
void BufferWrite(char* string, short x, short y);
void BufferFlipping();
void BufferClear();
void BufferDelete();