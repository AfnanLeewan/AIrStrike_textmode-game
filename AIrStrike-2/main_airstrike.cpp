#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> 
#include<windows.h> 
#include<time.h> 
#include<conio.h> 
#include<stdlib.h>
#include<time.h> 
#include <thread>
#include <mutex>
#include <string.h>
#include "ScoreBoard.h"
struct player { int x = 45; int y = 65; int shield = 20; int stbullet[3] = { 0,0,0 }; int x_bullet[3]; int y_bullet[3]; int* bulletform = stbullet; char name[1000]; };
struct Aenmemy { int x; int y; int shield = 3; int stbullet=0; int x_bullet=0; int y_bullet=0; int status; int Fr = 1; int mode = 0; };
struct Benmemy { int x; int y; int shield = 3; int stbullet = 0; int x_bullet = 0; int y_bullet = 0; int status; int Fr = 1; int mode = 0; };
struct Hitem { int x; int y; int status = 0; int Fr = 1; }h;
struct wave { int status = 0; int level = 1; int win = 0; }wave;
player p;
Aenmemy Aen[10];
int scorewave1 = 50;
int scorewave2 = 150;
int score = 0, sheild = 20;
void setcolor(int fg, int bg);
//void gotoxy(int x, int y);
void clear_map();
void draw_ship(int x, int y);
void draw_bullet(int x, int y);
void clear_bullet(int x, int y);
void clear_ship(int x, int y);
void clear_enemy(int x, int y);
void setcursor(bool visible);
void draw_map();
void draw_itemH(int x, int y);
void clear_item(int x, int y);
void draw_enemy(int x, int y);
void draw_kaboom(int x, int y);
void gameover(int x, int y);
void startgame(int x, int y);
void clearstrat(int x, int y);
void scoreboard();
char cursor(int x, int y);
void Aenemy(int x);
void ASenemy(int x);
void bullet_shoot(int x);
void enem_shoot(int x);
void itemfall();
void savescore(int x,char y);
std::mutex mtx;
int main()
{
	srand(time(NULL));
	setcursor(0);

	gotoxy(14, 4);

	char ch = '.', menselect = '.', s = '.';
	int  select = 0, mselect = 0;																						// 0=MENU 1=GAMEPLAY 2=SCORE 3=DEV NAME 4=PAUSE 5 =Gameover	
	int px = 45, py = 65, pbullet[3] = { 0,0,0 }, pxbullet[3], pybullet[3], pshield = 20;										//player
	int statAe[10] = { 0,0,0,0,0,0,0,0,0,0 }, Aex[10], Aey[10], ebullet[10] = { 0,0,0,0,0,0,0,0,0,0 }, exbullet[10], eybullet[10], eshield[10] = { 3,3,3,3,3,3,3,3,3,3 }, Fr1 = 1, Fr2 = 1, Fr3 = 1, Fr[10] = { 1,1,1,1,1,1,1,1,1,1 };				//enemy type A MAX 10 unit per window

	do {
		draw_map();
		clear_map();
		while (select == 0) {



			setcolor(2, 0); gotoxy(43, 32); printf("press E to start game");
			setcolor(2, 0); gotoxy(43, 33); printf("        score        ");
			setcolor(2, 0); gotoxy(43, 34); printf("      Developer      ");
			setcolor(3, 0); startgame(15, 20);
			if (_kbhit()) {
				s = _getch();
				if (s == 's') { if (mselect != 3) { Beep(700, 100); mselect += 1; } }
				if (s == 'w') { if (mselect != 1) { Beep(700, 100); mselect -= 1; } }
				if (mselect == 1) { gotoxy(42, 32); setcolor(5, 0); printf(">");  gotoxy(42, 33); printf(" "); gotoxy(42, 34); printf(" "); }
				if (mselect == 2) { gotoxy(42, 33); setcolor(5, 0); printf(">"); gotoxy(42, 32); printf(" "); gotoxy(42, 34); printf(" "); }
				if (mselect == 3) { gotoxy(42, 34); setcolor(5, 0); printf(">"); gotoxy(42, 32); printf(" "); gotoxy(42, 33); printf(" "); }
				if (s == ' ') { select = mselect; Beep(500, 100); Beep(500, 100);  Beep(700, 100);  break; }
				fflush(stdin);
			}

		}
		clear_map();
		//-----------------------------------------------------------------------------------------//GAMEPLA
		
		while (select == 1) {
			if (wave.status == 0) { gotoxy(50, 15); setcolor(2, 0); printf("WAVE %d", wave.level); Sleep(2000);  gotoxy(50, 15); printf("         "); wave.status = 1; }
			gotoxy(89, 4); setcolor(7, 0); printf("Score : %d", score);
			gotoxy(8, 4); setcolor(7, 0);
			printf("shield :%d", p.shield);
			printf("                                                              ");
			draw_ship(p.x, p.y);
			for (int i = 0; i < p.shield; i++)
			{
				setcolor(7, 6);
				gotoxy(14 + i, 4);
				printf(" ");
			}
			if (_kbhit()) {
				ch = _getch();
				if (ch == 'a' && p.x != 5) { clear_ship(p.x, p.y); draw_ship(--p.x, p.y); }
				if (ch == 'd' && p.x != 94) { clear_ship(p.x, p.y); draw_ship(++p.x, p.y); }
				if (ch == 'w' && p.y != 7) { clear_ship(p.x, p.y); draw_ship(p.x, --p.y); }
				if (ch == 's' && p.y != 65) { clear_ship(p.x, p.y); draw_ship(p.x, ++p.y); }
				if (ch == ' ') { select = 4; }
				if (ch == 'j' && (p.stbullet[0] == 0 || p.stbullet[1] == 0 || p.stbullet[2] == 0))

				{
					if (p.stbullet[0] == 0) { p.stbullet[0] = 1; p.x_bullet[0] = p.x + 4; p.y_bullet[0] = p.y - 1; }
					else if (p.stbullet[1] == 0) { p.stbullet[1] = 1; p.x_bullet[1] = p.x + 4; p.y_bullet[1] = p.y - 1; }
					else if (p.stbullet[2] == 0) { p.stbullet[2] = 1; p.x_bullet[2] = p.x + 4; p.y_bullet[2] = p.y - 1; }
				}
				fflush(stdin);

			}

			for (int i = 0; i < 3; i++)
			{
				bullet_shoot(i);
			}
			//------------------------------------------------wave1
			if (wave.level == 1) {
				for (int i = 0; i < 10; i++)
				{
					Aenemy(i);
					
				}
				if (score >= scorewave1) { wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave2
			if (wave.level == 2) {
				for (int i = 0; i < 5; i++)
				{
					Aenemy(i);
					
				}
				for (int i = 5; i < 10; i++)
				{
					Aenemy(i);
					enem_shoot(i);

				}
				if (score >= scorewave2) { wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave3

			//------------------------------------------------wave4

			//------------------------------------------------wave5

			itemfall();
			if (h.y == p.y && h.x == p.x + 2) {
				p.shield += 5; h.status = 0;	std::thread q(Beep, 700, 500); q.detach(); clear_item(h.x, h.y); h.x == NULL; h.y = NULL;
			}














			// if (p.x == h[0].x && p.y == h[0].y) { std::thread q(Beep, 900, 500); q.detach(); p.shield += 5; }






			/*
			if (p.stbullet[0] == 1) {
				char bs1 = cursor(p.x_bullet[0], p.y_bullet[0] - 1);
				clear_bullet(p.x_bullet[0], p.y_bullet[0]);

				if (p.y_bullet[0] == 6) { p.stbullet[0] = 0; }
				else if (bs1 != ' ') { p.stbullet[0] = 0; clear_bullet(p.x_bullet[0], pybullet[0]);}
				else { draw_bullet(pxbullet[0], --pybullet[0]); }
			}
			if (pbullet[1] == 1) {

				char bs1 = cursor(pxbullet[0], pybullet[0] - 1);
				clear_bullet(pxbullet[1], pybullet[1]);
				if (pybullet[1] == 6) { pbullet[1] = 0; }
				else if (bs1 != ' ') { pbullet[0] = 0;  }
				else { draw_bullet(pxbullet[1], --pybullet[1]); }
			}
			if (pbullet[2] == 1) {
				char bs1 = cursor(pxbullet[0], pybullet[0] - 1);
				clear_bullet(pxbullet[2], pybullet[2]);
				if (pybullet[2] == 6) { pbullet[2] = 0; }
				else if (bs1 != ' ') { pbullet[0] = 0;  }
				else { draw_bullet(pxbullet[2], --pybullet[2]); }
			}


			if (statAe[0] == 0) { Aex[0] = 6 + rand() % 80; Aey[0] = 7 + rand() % 40; draw_enemy(Aex[0], Aey[0]); statAe[0] = 1; }
			if (statAe[0] == 1) {
				char bs1 = cursor(Aex[0], Aey[0] + 1);
				clear_enemy(Aex[0], Aey[0]);
				if (Aey[0] == 69) { statAe[0] = 0; }
				else { if (Fr[0] != 10) { ++Fr[0]; draw_enemy(Aex[0], Aey[0]); } else {
					if (Aex[0] < px) { ++Aex[0]; } if (Aex[0] > px) { --Aex[0]; }draw_enemy(Aex[0], ++Aey[0]); Fr[0] = 1; } }
			}
			if (pxbullet[0] >= Aex[0] && pxbullet[0] <= Aex[0] + 6 && pybullet[0] == Aey[0] + 1) {/* std::thread q(draw_kaboom, Aex[0], Aey[0]); q.detach();*//*score += 10; clear_enemy(Aex[0], Aey[0]); statAe[0] = 0; clear_bullet(pxbullet[0], pybullet[0]); pbullet[0] = 0; pxbullet[0] = NULL; pybullet[0] = NULL; }
			if ( px == Aex[0] && Aey[0] == py) {std::thread q(Beep, 700, 500);q.detach(); pshield -= 5; statAe[0] = 0; clear_enemy(Aex[0], Aey[0]); }





			if (statAe[1] == 0) { Aex[1] = 6 + rand() % 80; Aey[1] = 7 + rand() % 40; draw_enemy(Aex[1], Aey[1]); statAe[1] = 1; }
			if (statAe[1] == 1) {
				char bs1 = cursor(Aex[1], Aey[1] + 1);
				clear_enemy(Aex[1], Aey[1]);
				if (Aey[1] == 69) { statAe[1] = 0; }
				else {
					if (Fr2 != 10) { ++Fr2; draw_enemy(Aex[1], Aey[1]); }
					else {
						if (Aex[1] < px) { ++Aex[1]; } if (Aex[1] > px) { --Aex[1]; }draw_enemy(Aex[1], ++Aey[1]); Fr2 = 1;
					}
				}
			}
			if (pxbullet[0] >= Aex[1] && pxbullet[0] <= Aex[1] + 6 && pybullet[0] == Aey[1] + 1) {  /*std::thread q(draw_kaboom, Aex[1], Aey[1]); q.detach();*//* score += 10; clear_enemy(Aex[1], Aey[1]); statAe[1] = 0; clear_bullet(pxbullet[0], pybullet[0]); pbullet[0] = 0; pxbullet[0] = NULL; pybullet[0] = NULL; }
			if (px == Aex[1] && Aey[1] == py) { std::thread q(Beep, 700, 500); q.detach(); pshield -= 5; statAe[1] = 0; clear_enemy(Aex[1], Aey[1]); }




			if (statAe[2] == 0) { Aex[2] = 6 + rand() % 80; Aey[2] = 7 + rand() % 40; draw_enemy(Aex[2], Aey[2]); statAe[2] = 1; }
			if (statAe[2] == 1) {
				char bs1 = cursor(Aex[2], Aey[2] + 1);
				clear_enemy(Aex[2], Aey[2]);
				if (Aey[2] == 69) { statAe[2] = 0; }
				else {
					if (Fr3 != 10) { ++Fr3; draw_enemy(Aex[2], Aey[2]); }
					else {
						if (Aex[2] < px) { ++Aex[2]; } if (Aex[2] > px) { --Aex[1]; }draw_enemy(Aex[2], ++Aey[2]); Fr3 = 1;
					}
				}
			}
			if (pxbullet[0] >= Aex[2] && pxbullet[0] <= Aex[2] + 6 && pybullet[0] == Aey[2] + 1) { /*std::thread q(draw_kaboom, Aex[2], Aey[2]); q.detach();*//* score += 10; clear_enemy(Aex[2], Aey[2]); statAe[2] = 0; clear_bullet(pxbullet[0], pybullet[0]); pbullet[0] = 0; pxbullet[0] = NULL; pybullet[0] = NULL; }
			if (px == Aex[2] && Aey[2] == py) { std::thread q(Beep, 700, 500); q.detach(); pshield -= 5; statAe[2] = 0; clear_enemy(Aex[2], Aey[2]); }

			*/



			if (p.shield <= 0) {
				gameover(18, 20);
				gotoxy(47, 32); scanf("%s",p.name);
				int len = strlen(p.name);
				if (len <= 10) { ScoreBoard(p.name, score); select = 0; }
				else
				{
					gotoxy(45, 35); printf("YOUR NAME IS TOO LONG !!!");
					 gameover(18, 20);
				}
				 
			}
			//std::thread q(Beep, 700, 500);
			//q.detach();

			Sleep(10);

		}
		while (select == 2) {
			setcolor(7, 0); gotoxy(51, 28); printf("SCORE"); gotoxy(25, 20); Read(41, 30);; gotoxy(40, 40); printf("PRESS SPACEBAR TO BACK TO MENU ");
			if (_kbhit()) {
				char s;
				s = _getch();
				if (s == ' ') { select = 0; }
			}
		}








	} while (ch != 'x' || sheild == 0);


	return 0;
}


void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
/*
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
*/
void draw_ship(int x, int y)
{
	setcolor(1, 0);
	gotoxy(x, y++); printf("    ^");
	gotoxy(x, y++); printf("   | |");
	gotoxy(x, y++); printf("  ||0||");
	gotoxy(x, y++); printf("|--| |--| ");
	gotoxy(x, y++); printf("  /|||\\");


}
void draw_bullet(int x, int y)
{
	setcolor(4, 0);
	gotoxy(x, y); printf("O");

}
void clear_bullet(int x, int y)
{
	setcolor(0, 0);
	gotoxy(x, y); printf(" ");


}
void clear_ship(int x, int y) {
	setcolor(4, 0);
	gotoxy(x, y); printf("          ");
	gotoxy(x, y++); printf("          ");
	gotoxy(x, y++); printf("          ");
	gotoxy(x, y++); printf("          ");
	gotoxy(x, y++); printf("          ");
	gotoxy(x, y++); printf("          ");
}
void clear_enemy(int x, int y) {
	setcolor(0, 0); gotoxy(x, y); printf("        ");

}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void draw_map() {
	setcolor(1, 2);
	gotoxy(4, 5);
	for (int i = 0; i <= 100; i++)
	{
		printf(" ");
	}
	int y = 6;
	gotoxy(4, y);
	for (int i = 6; i < 70; i++)
	{
		gotoxy(4, i);
		printf(" ");
		y++;

	}

	for (int i = 4; i < 104; i++)
	{
		gotoxy(i, 70);
		printf(" ");
		y++;

	}

	for (int i = 6; i <= 70; i++)
	{
		gotoxy(104, i);
		printf(" ");
	}

}
void draw_itemH(int x, int y) {
	setcolor(2, 0);
	gotoxy(x, y); printf("|<H>|");
}
void clear_item(int x, int y) {
	setcolor(2, 0);
	gotoxy(x, y); printf("     ");
}



void draw_enemy(int x, int y)
{
	setcolor(5, 0);
	gotoxy(x, y); printf("<<+O+>>");

}
void draw_kaboom(int x, int y) {
	int i = y;
	setcolor(2, 0);

	gotoxy(x, y);   printf("'.\\|/.'");
	gotoxy(x, y++); printf("(\\   /)");
	gotoxy(x, y++); printf("- -O- -");
	gotoxy(x, y++); printf("(/   \\)");
	gotoxy(x, y++); printf(",'/|\\'.");

	Sleep(1000);

	setcolor(0, 0);
	gotoxy(x, i);	printf("           ");
	gotoxy(x, i++); printf("           ");
	gotoxy(x, i++); printf("           ");
	gotoxy(x, i++); printf("           ");
	gotoxy(x, i++); printf("           ");
	gotoxy(x, i++); printf("           ");

}
void gameover(int x, int y) {
	for (int i = 0; i < 10; i++) {
		Aen[i].status = 2;
		clear_enemy(Aen[i].x, Aen[i].y);
	}
	setcolor(4, 0);
	gotoxy(x, y++); printf(" _______  _______  _______  _______  _______           _______  _______ ");
	gotoxy(x, y++); printf("(  ____ \\(  ___  )(       )(  ____ \\(  ___  )|\\     /|(  ____ \\(  ____ )");
	gotoxy(x, y++); printf("| (    \\/| (   ) || () () || (    \\/| (   ) || )   ( || (    \\/| (    )|");
	gotoxy(x, y++); printf("| |      | (___) || || || || (__    | |   | || |   | || (__    | (____)|");
	gotoxy(x, y++); printf("| | ____ |  ___  || |(_)| ||  __)   | |   | |( (   ) )|  __)   |     __)");
	gotoxy(x, y++); printf("| | \\_  )| (   ) || |   | || (      | |   | | \\ \\_/ / | (      | (\\ (   ");
	gotoxy(x, y++); printf("| (___) || )   ( || )   ( || (____/\\| (___) |  \\   /  | (____/\\| ) \\ \\_");
	gotoxy(x, y++); printf("(_______)|/     \\||/     \\|(_______/(_______)   \\_/   (_______/|/   \\__/");
	setcolor(2, 0);
	gotoxy(45, 29); printf("---------------------");
	gotoxy(45, 30); printf("|  enter your name  |");
	gotoxy(45, 31); printf("|                   |");
	gotoxy(45, 32); printf("|                   |");
	gotoxy(45, 33); printf("---------------------");
	
	
	
	
	

}
void startgame(int x, int y) {
	setcolor(4, 0);
	gotoxy(x, y++); printf(" _______ _________ _______  _______ _________ _______ _________ _        _______ ");
	gotoxy(x, y++); printf("(  ___  )\\__   __/(  ____ )(  ____ \\\\__   __/(  ____ )\\__   __/| \\    /\\(  ____ \\");
	gotoxy(x, y++); printf("| (   ) |   ) (   | (    )|| (    \\/   ) (   | (    )|   ) (   |  \\  / /| (    \\/");
	gotoxy(x, y++); printf("| (___) |   | |   | (____)|| (_____    | |   | (____)|   | |   |  (_/ / | (__    ");
	gotoxy(x, y++); printf("|  ___  |   | |   |     __)(_____  )   | |   |     __)   | |   |   _ (  |  __)   ");
	gotoxy(x, y++); printf("| (   ) |   | |   | (\\ (         ) |   | |   | (\\ (      | |   |  ( \\ \\ | (      ");
	gotoxy(x, y++); printf("| )   ( |___) (___| ) \\ \\__/\\____) |   | |   | ) \\ \\_____) (__ |   / \\ \\| (____/\\");
	gotoxy(x, y++); printf("|/     \\|\\_______/|/   \\__ /\\_______)  )_(   |/   \\__ /\\______/|_ /   \\/(_______ / ");

}
void clearstrat(int x, int y) {
	gotoxy(x, y++); printf("                                                                                          ");
	gotoxy(x, y++); printf("                                                                                          ");
	gotoxy(x, y++); printf("                                                                                          ");
	gotoxy(x, y++); printf("                                                                                          ");
	gotoxy(x, y++); printf("                                                                                          ");
	gotoxy(x, y++); printf("                                                                                          ");
	gotoxy(x, y++); printf("                                                                                          ");
	gotoxy(x, y++); printf("                                                                                          ");
}
void scoreboard() {
	gotoxy(25, 20); setcolor(4, 5);
	printf("                                                           ");
	gotoxy(25, 21); printf("                          SCORE                            ");
	gotoxy(25, 22); printf("                                                           ");
	gotoxy(25, 23); setcolor(2, 0); printf("#1");
	gotoxy(25, 24); setcolor(0, 5); printf("                                                           ");
	gotoxy(25, 25); setcolor(2, 0); printf("#2");
	gotoxy(25, 26); setcolor(0, 5); printf("                                                           ");
	gotoxy(25, 27); setcolor(2, 0); printf("#3");
	gotoxy(25, 28); setcolor(0, 5); printf("                                                           ");
	gotoxy(25, 29); setcolor(2, 0); printf("#4");
	gotoxy(25, 30); setcolor(0, 5); printf("                                                           ");
	gotoxy(25, 31); setcolor(2, 0); printf("#5");
	gotoxy(25, 32); setcolor(0, 5);
	printf("                                                           ");
}
void clear_map() {

	gotoxy(5, 6);
	for (int j = 0; j < 64; j++) {
		gotoxy(5, 6 + j);
		for (int i = 0; i < 99; i++)
		{
			setcolor(0, 0); printf(" ");
		}
	}
}

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read)) return '\0';
	else
		return buf[0];
}
void Aenemy(int x) {
	if (Aen[x].status == 0 && wave.status == 1) {
		Aen[x].x = 6 + rand() % 80; Aen[x].y = 7 + rand() % 10; draw_enemy(Aen[x].x, Aen[x].y); Aen[x].status = 1; Aen[x].mode = 1 + rand() % 3;
	}


	if (Aen[x].mode == 1) {
		if (Aen[x].status == 1) {
			char bs1 = cursor(Aen[x].x, Aen[x].y + 1);
			clear_enemy(Aen[x].x, Aen[x].y);
			if (Aen[x].y == 69) { Aen[x].status = 0; }
			else {
				if (Aen[x].Fr != 10) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); }
				else {
					if (Aen[x].x < p.x) { ++Aen[x].x; } if (Aen[x].x > p.x) { --Aen[x].x; }draw_enemy(Aen[x].x, ++Aen[x].y); Aen[x].Fr = 1;
				}
			}
		}
	}

	if (Aen[x].mode == 2) {
		if (Aen[x].status == 1) {
			char bs1 = cursor(Aen[x].x, Aen[x].y + 1);
			clear_enemy(Aen[x].x, Aen[x].y);
			if (Aen[x].y == 69 ) { Aen[x].status = 0; }
			else {
				if (Aen[x].Fr != 10) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); }
				else {
					draw_enemy(Aen[x].x, ++Aen[x].y); Aen[x].Fr = 1;
				}
			}
		}
	}

	if (Aen[x].mode == 3) {
		if (Aen[x].status == 1) {
			char bs1 = cursor(Aen[x].x, Aen[x].y + 1);
			clear_enemy(Aen[x].x, Aen[x].y);
			if (Aen[x].y == 69 ) { Aen[x].status = 0; }
			else {
				if (Aen[x].Fr != 10) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); }
				else {
					int i = rand() % 2;
					if (i == 0) { if (Aen[x].x <= 86) { Aen[x].x++; } }if (i == 1) { if (Aen[x].x >= 10) { Aen[x].x--; } }
					draw_enemy(Aen[x].x, ++Aen[x].y); Aen[x].Fr = 1;
				}
			}
		}
	}


	
	/*
	clear_bullet(Aen[x].x_bullet, Aen[x].y_bullet);
	if (Aen[x].y_bullet == 69) { Aen[x].stbullet = 0;  Aen[x].x_bullet = Aen[x].x; Aen[x].y_bullet = Aen[x].y; }
	else { draw_bullet(Aen[x].x_bullet, ++Aen[x].y_bullet); }
	*/

	for (int i = 0; i < 3; i++)
	{
		if (p.x_bullet[i] >= Aen[x].x && p.x_bullet[i] <= Aen[x].x + 6 && p.y_bullet[i] == Aen[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); Aen[x].shield--; clear_bullet(p.x_bullet[i], p.y_bullet[i]); p.stbullet[i] = 0; p.x_bullet[i] = NULL; p.y_bullet[i] = NULL;
		}
	}


	
	if (p.x == Aen[x].x && Aen[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; Aen[x].status = 0; clear_enemy(Aen[x].x, Aen[x].y); }
	if (Aen[x].shield == 0) {/*std::thread q(draw_kaboom, Aen[x].x, Aen[x].y); q.detach(); */score += 10; clear_enemy(Aen[x].x, Aen[x].y); Aen[x].status = 0; Aen[x].shield = 3;  int i = rand() % 2; Aen[x].x = NULL;Aen[x].y=NULL;
	if (i == 1 && h.status == 0) {
		h.status = 1;
		h.x = Aen[x].x; h.y = Aen[x].y;
		draw_itemH(h.x, h.y);
	}
	}









}
void ASenemy(int x) {
	if (Aen[x].status == 0 && wave.status == 1) {
		Aen[x].x = 6 + rand() % 80; Aen[x].y = 7 + rand() % 10; draw_enemy(Aen[x].x, Aen[x].y); Aen[x].status = 1; Aen[x].mode = 1 + rand() % 3;
	}


	if (Aen[x].mode == 1) {
		if (Aen[x].status == 1) {
			char bs1 = cursor(Aen[x].x, Aen[x].y + 1);
			clear_enemy(Aen[x].x, Aen[x].y);
			if (Aen[x].y == 69) { Aen[x].status = 0; }
			else {
				if (Aen[x].Fr != 10) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); }
				else {
					if (Aen[x].x < p.x) { ++Aen[x].x; } if (Aen[x].x > p.x) { --Aen[x].x; }draw_enemy(Aen[x].x, ++Aen[x].y); Aen[x].Fr = 1;
				}
			}
		}
	}

	if (Aen[x].mode == 2) {
		if (Aen[x].status == 1) {
			char bs1 = cursor(Aen[x].x, Aen[x].y + 1);
			clear_enemy(Aen[x].x, Aen[x].y);
			if (Aen[x].y == 69) { Aen[x].status = 0; }
			else {
				if (Aen[x].Fr != 10) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); }
				else {
					draw_enemy(Aen[x].x, ++Aen[x].y); Aen[x].Fr = 1;
				}
			}
		}
	}

	if (Aen[x].mode == 3) {
		if (Aen[x].status == 1) {
			char bs1 = cursor(Aen[x].x, Aen[x].y + 1);
			clear_enemy(Aen[x].x, Aen[x].y);
			if (Aen[x].y == 69) { Aen[x].status = 0; }
			else {
				if (Aen[x].Fr != 10) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); }
				else {
					int i = rand() % 2;
					if (i == 0) { if (Aen[x].x <= 86) { Aen[x].x++; } }if (i == 1) { if (Aen[x].x >= 10) { Aen[x].x--; } }
					draw_enemy(Aen[x].x, ++Aen[x].y); Aen[x].Fr = 1;
				}
			}
		}
	}


		char bs1 = cursor(Aen[x].x_bullet, Aen[x].y_bullet - 1);
		clear_bullet(Aen[x].x_bullet, Aen[x].y_bullet);

		if (Aen[x].y_bullet == 69) { Aen[x].stbullet = 0;  Aen[x].x_bullet = Aen[x].x; Aen[x].y_bullet = Aen[x].y;}
		else if (bs1 != ' ') { Aen[x].stbullet = 0; clear_bullet(Aen[x].x_bullet, Aen[x].y_bullet); }
		else { draw_bullet(Aen[x].x_bullet, ++Aen[x].y_bullet); }
	






	if (p.x_bullet[0] >= Aen[x].x && p.x_bullet[0] <= Aen[x].x + 6 && p.y_bullet[0] == Aen[x].y + 1) {

		std::thread q(Beep, 700, 500); q.detach(); Aen[x].shield--; clear_bullet(p.x_bullet[0], p.y_bullet[0]); p.stbullet[0] = 0; p.x_bullet[0] = NULL; p.y_bullet[0] = NULL;
	}
	if (p.x == Aen[x].x && Aen[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; Aen[x].status = 0; clear_enemy(Aen[x].x, Aen[x].y); }
	if (Aen[x].shield == 0) {/*std::thread q(draw_kaboom, Aen[x].x, Aen[x].y); q.detach(); */score += 10; clear_enemy(Aen[x].x, Aen[x].y); Aen[x].status = 0; Aen[x].shield = 3;  int i = rand() % 2; Aen[x].x = NULL; Aen[x].y = NULL;
	if (i == 1 && h.status == 0) {
		h.status = 1;
		h.x = Aen[x].x; h.y = Aen[x].y;
		draw_itemH(h.x, h.y);
	}
	}









}

void enem_shoot(int x) {
	
	clear_bullet(Aen[x].x_bullet, Aen[x].y_bullet);
	if (Aen[x].y_bullet == 69) { Aen[x].stbullet = 0;  Aen[x].x_bullet = Aen[x].x; Aen[x].y_bullet = Aen[x].y; }
	else { draw_bullet(Aen[x].x_bullet, ++Aen[x].y_bullet); }

	
}
void bullet_shoot(int x) {
	if (p.stbullet[x] == 1) {
		char bs1 = cursor(p.x_bullet[x], p.y_bullet[x] - 1);
		clear_bullet(p.x_bullet[x], p.y_bullet[x]);

		if (p.y_bullet[x] == 6) { p.stbullet[x] = 0; }
		else if (bs1 != ' ') { p.stbullet[x] = 0; clear_bullet(p.x_bullet[x], p.y_bullet[x]); }
		else { draw_bullet(p.x_bullet[x], --p.y_bullet[x]); }
	}
}


void itemfall() {

	if (h.status == 1) {
		char bs1 = cursor(h.x, h.y - 1);
		clear_item(h.x, h.y);

		if (h.y == 69) { h.status = 0; }
		else if (bs1 != ' ') { h.status = 0; clear_item(h.x, h.y); }
		else {
			if (h.Fr != 15) { h.Fr++;  draw_itemH(h.x, h.y); }
			else { draw_itemH(h.x, ++h.y); h.Fr = 1; }
		}
	}
}
void savescore(int x, char y) {



}
