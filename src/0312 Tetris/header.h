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


void DrawScreen();						// ȭ�� ���
void CreateBlock(int type);				// ��� ����
bool MoveBlock(int dir);				// ��� �̵�
bool DownBlock(bool isDrop);			// ��� �ϰ�(1=���)
void ChangeBlock(int x, int y);			// ��� ������
void DestroyLine(int line, int combo);	// �� �ı�
void LineCheck();						// �� üũ
void KeyCheck();						// Ű �Է� üũ
void OverCheck();						// ���� ���� ó��
void Process(void* p);					// ��� �ϰ� ó��

void GotoXY(short x, short y);
// ���� ó��
void BufferInit();
void BufferWrite(char* string, short x, short y);
void BufferFlipping();
void BufferClear();
void BufferDelete();