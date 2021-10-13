#pragma once
#include<stdio.h> 
#include<windows.h> 
#include<time.h> 
#include<conio.h> 
#include<stdlib.h>

void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void draw_Player(int x, int y)
{
	setcolor(1, 0);
	gotoxy(x, y++); printf("    ^");
	gotoxy(x, y++); printf("   | |");
	gotoxy(x, y++); printf("  ||0||");
	gotoxy(x, y++); printf("|--| |--| ");
	gotoxy(x, y++); printf("  /|||\\");
}
void clear_Player(int x, int y) {
	setcolor(4, 0);
	gotoxy(x, y); printf("          ");
	gotoxy(x, y++); printf("          ");
	gotoxy(x, y++); printf("          ");
	gotoxy(x, y++); printf("          ");
	gotoxy(x, y++); printf("          ");
	gotoxy(x, y++); printf("          ");
}


