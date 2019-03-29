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

// 화면 출력
void DrawScreen();
// 블록 생성
void CreateBlock(int type);
// 블록 이동
bool MoveBlock(int dir);
// 블록 하강(1=드랍)
bool DownBlock(bool drop);
// 블록 돌리기
void ChangeBlock(int x, int y);
// 줄 파괴
void DestroyLine(int line, int combo);
// 줄 체크
void LineCheck();
// 키 입력 체크
void KeyCheck();
// 게임 오버 처리
void OverCheck();
// 블록 하강 처리
void Process(void* p);

void GotoXY(short x, short y);
void BufferInit();
void BufferWrite(char* string, short x, short y);
void BufferFlipping();
void BufferClear();
void BufferDelete();