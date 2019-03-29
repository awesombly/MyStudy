#pragma once

#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <conio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13

#define COLS 19
#define ROWS 19

enum GAME { PLAYING = 1, WHITEWIN, BLACKWIN };
enum STATE { GAME_INIT = 0, KEY_CHECK, GAME_DRAW, GAME_CHECK };



void Initialize();
void DrawScreen();
void Judgment();
void GotoXY(short x, short y);
void KeyCheck();

