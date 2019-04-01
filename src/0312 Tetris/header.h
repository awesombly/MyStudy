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


void DrawScreen();						// 화면 출력
void CreateBlock(int type);				// 블록 생성
bool MoveBlock(int dir);				// 블록 이동
bool DownBlock(bool isDrop);			// 블록 하강(1=드랍)
void ChangeBlock(int x, int y);			// 블록 돌리기
void DestroyLine(int line, int combo);	// 줄 파괴
void LineCheck();						// 줄 체크
void KeyCheck();						// 키 입력 체크
void OverCheck();						// 게임 오버 처리
void Process(void* p);					// 블록 하강 처리

void GotoXY(short x, short y);
// 버퍼 처리
void BufferInit();
void BufferWrite(char* string, short x, short y);
void BufferFlipping();
void BufferClear();
void BufferDelete();