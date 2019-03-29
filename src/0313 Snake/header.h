#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <process.h>
#include <Windows.h>

#define _CRT_SECURE_NO_WARNINGS
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13

#define WIDTH 26
#define HEIGHT 26

#define BUFFER_WIDTH 100
#define BUFFER_HEIGHT 100


void Initialize();
void DrawScreen();
void KeyCheck();
void Process(void* p);
void ColisionCheck();
void Move();
void CreateItem(int type);
void GameOver();
void AddTail();
void DeleteTail();

void GotoXY(short x, short y);
void BufferInit();
void BufferWrite(char* string, short x, short y);
void BufferFlipping();
void BufferClear();
void BufferDelete();