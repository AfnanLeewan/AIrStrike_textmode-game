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
struct Aenmemy { int x; int y; int shield = 3; int stbullet = 0; int x_bullet; int y_bullet; int status; int Fr = 1; int mode = 0; int Fr_b = 1; int drop = 0; int shoot; };
struct Benmemy { int x; int y; int shield = 3; int stbullet = 0; int x_bullet ; int y_bullet ; int status; int Fr = 1; int mode = 0; int Fr_b = 1; int drop = 0; };
struct Cenmemy { int x; int y; int shield = 5; int stbullet[3] = { 0,0,0 }; int x1_bullet; int y1_bullet; int x2_bullet; int y2_bullet; int x3_bullet; int y3_bullet; int status; int Fr = 1; int mode = 0; int Fr_b[3] = { 1,1,1 };  int drop = 0; int shoot = 0; };
struct Healing { int x; int y; int heal = 5; int status = 0; int Fr = 1; }h;
struct wave { int status = 0; int level = 1; int win = 0; }wave;
player p;
Aenmemy Aen[10];
Benmemy Ben[10];
Cenmemy Cen[10];
int playtime = 1;
int scorewave1 = 100;
int scorewave2 = 150;
int scorewave3 = 250;
int scorewave4 = 350;
int scorewave5 = 450;
int score = 0, sheild = 20;
void setcolor(int fg, int bg);
//void gotoxy(int x, int y);
void clear_map();
void draw_ship(int x, int y);
void draw_bullet(int x, int y);
void draw_enbullet(int x,int y);
void clear_bullet(int x, int y);
void clear_ship(int x, int y);
void clear_enemy(int x, int y);
void setcursor(bool visible);
void draw_map();
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
void draw_Benemy(int x, int y);
void draw_Cenemy(int x, int y);
void Benemy(int x);
void clear_Benemy(int x, int y);
void Benemy(int x);
void Cenemy(int x);
void Cenem_shoot(int x);
void draw_itemH(int x,int y);
void clear_item(int x, int y);
void itemH(int x);


std::mutex mtx;
int main()
{
	srand(time(NULL));
	setcursor(0);

	gotoxy(14, 4);

	char ch = '.', menselect = '.', s = '.';
	int  select = 0, mselect = 0;																						// 0=MENU 1=GAMEPLAY 2=SCORE 3=DEV NAME 4=PAUSE 5 =Gameover	

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
					itemH(Aen[i].drop); Aen[i].drop = 0;
					

				}
				if (score >= scorewave1) { wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave2
			if (wave.level == 2) {
				for (int i = 0; i < 2; i++)
				{
					Cenemy(i);
				
				}
			




				if (score >= scorewave2) { wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave3
			if (wave.level == 3) {
				for (int i = 0; i < 3; i++)
				{
					Benemy(i);

				}
				for (int i = 0; i < 5; i++)
				{
					Aenemy(i);

				}
				for (int i = 5; i < 10; i++)
				{
					Aenemy(i);
					enem_shoot(i);

				}
				if (score >= scorewave3) { wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}

			//------------------------------------------------wave4

			//------------------------------------------------wave5

	




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
	setcolor(11, 0);
	gotoxy(x, y++); printf("    ^");
	gotoxy(x, y++); printf("   | |");
	gotoxy(x, y++); printf("  ||0||");
	gotoxy(x, y++); printf("|--| |--| ");
	gotoxy(x, y++); printf("  /|||\\");


}
void draw_bullet(int x, int y)
{
	setcolor(2, 0);
	gotoxy(x, y); printf("^");

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


void draw_enbullet(int x, int y) {
	
		setcolor(4, 0);
		gotoxy(x, y); printf("O");

	


}

void draw_enemy(int x, int y)
{
	setcolor(13, 0);
	gotoxy(x, y); printf("<<+O+>>");

}
void draw_Cenemy(int x, int y) {
	setcolor(14, 0);
	gotoxy(x, y); printf("/~~~\\");
	gotoxy(x, ++y); printf("\\-V-/");
	

}
void clear_Cenemy(int x, int y) {
	setcolor(8, 0);
	gotoxy(x, y); printf("     ");
	gotoxy(x, ++y); printf("     ");

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



void enem_shoot(int x) {
	if (Aen[x].stbullet == 0) { Aen[x].x_bullet = Aen[x].x; Aen[x].y_bullet = Aen[x].y; Aen[x].stbullet = 1; }
	clear_bullet(Aen[x].x_bullet, Aen[x].y_bullet);
	if (Aen[x].y_bullet == 69) { Aen[x].stbullet = 0;  Aen[x].x_bullet = Aen[x].x; Aen[x].y_bullet = Aen[x].y; }
	else {

		if (Aen[x].Fr_b != 5) { Aen[x].Fr_b++; draw_enbullet(Aen[x].x_bullet, Aen[x].y_bullet); }

		else { draw_enbullet(Aen[x].x_bullet, ++Aen[x].y_bullet); Aen[x].Fr_b = 1; }
	}

	if (Aen[x].y_bullet == p.y&&Aen[x].x_bullet>=p.x-3&&Aen[x].x_bullet<=p.x+4) { p.shield -= 2; std::thread q(Beep, 700, 500); q.detach(); clear_bullet(Aen[x].x_bullet, Aen[x].y_bullet); Aen[x].stbullet = 0; }
	
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




void draw_Benemy(int x, int y) {
	setcolor(10, 0);
	gotoxy(x, y);   printf("'   '");
	gotoxy(x, ++y); printf("|.-.|");
	gotoxy(x, ++y); printf(".'o'.");
	gotoxy(x, ++y); printf(" |o| ");
	gotoxy(x, ++y); printf("  |  ");
}
void clear_Benemy(int x, int y) {
	setcolor(5, 0);
	gotoxy(x, y);   printf("     ");
	gotoxy(x, ++y); printf("     ");
	gotoxy(x, ++y); printf("     ");					
	gotoxy(x, ++y); printf("     ");						
	gotoxy(x, ++y); printf("     ");						


}


void Cenem_shoot(int x) {
	if (Cen[x].stbullet[0] == 0) { Cen[x].x1_bullet = Cen[x].x+2; Cen[x].y1_bullet = Cen[x].y+2; Cen[x].stbullet[0] = 1; }
	clear_bullet(Cen[x].x1_bullet, Cen[x].y1_bullet);
	if (Cen[x].y1_bullet == 69) { Cen[x].stbullet[0] = 0;  Cen[x].x1_bullet = Cen[x].x; Cen[x].y1_bullet = Cen[x].y; }
	else {

		if (Cen[x].Fr_b[0]!= 5) { Cen[x].Fr_b[0]++; draw_enbullet(Cen[x].x1_bullet, Cen[x].y1_bullet); }

		else { draw_enbullet(Cen[x].x1_bullet, ++Cen[x].y1_bullet); Cen[x].Fr_b[0] = 1; }
	}


	if (Cen[x].stbullet[1] == 0) { Cen[x].x2_bullet = Cen[x].x + 2; Cen[x].y2_bullet = Cen[x].y + 2; Cen[x].stbullet[1] = 1; }
	clear_bullet(Cen[x].x2_bullet, Cen[x].y2_bullet);
	if (Cen[x].y2_bullet == 69||Cen[x].x2_bullet == 103) { Cen[x].stbullet[1] = 0;  Cen[x].x2_bullet = Cen[x].x; Cen[x].y2_bullet = Cen[x].y; }
	else {

		if (Cen[x].Fr_b[1] != 5) { Cen[x].Fr_b[1]++; draw_enbullet(Cen[x].x2_bullet, Cen[x].y2_bullet); }

		else { draw_enbullet(++Cen[x].x2_bullet, ++Cen[x].y2_bullet); Cen[x].Fr_b[1] = 1; }
	}


	if (Cen[x].stbullet[2] == 0) { Cen[x].x3_bullet = Cen[x].x + 2; Cen[x].y3_bullet = Cen[x].y + 2; Cen[x].stbullet[2] = 1; }
	clear_bullet(Cen[x].x3_bullet, Cen[x].y3_bullet);
	if (Cen[x].y3_bullet == 69 || Cen[x].x3_bullet == 5) { Cen[x].stbullet[2] = 0;  Cen[x].x3_bullet = Cen[x].x; Cen[x].y3_bullet = Cen[x].y; }
	else {

		if (Cen[x].Fr_b[2] != 5) { Cen[x].Fr_b[2]++; draw_enbullet(Cen[x].x3_bullet, Cen[x].y3_bullet); }

		else { draw_enbullet(--Cen[x].x3_bullet, ++Cen[x].y3_bullet); Cen[x].Fr_b[2] = 1; }
	}






	if (Cen[x].y1_bullet == p.y && Cen[x].x1_bullet >= p.x - 3 && Cen[x].x1_bullet <= p.x + 4) { p.shield -= 2; std::thread q(Beep, 700, 500); q.detach(); clear_bullet(Cen[x].x1_bullet, Cen[x].y1_bullet); Cen[x].stbullet[0] = 0; }
	if (Cen[x].y2_bullet == p.y && Cen[x].x2_bullet >= p.x - 3 && Cen[x].x2_bullet <= p.x + 4) { p.shield -= 2; std::thread q(Beep, 700, 500); q.detach(); clear_bullet(Cen[x].x2_bullet, Cen[x].y2_bullet); Cen[x].stbullet[1] = 0; }
	if (Cen[x].y3_bullet == p.y && Cen[x].x3_bullet >= p.x - 3 && Cen[x].x3_bullet <= p.x + 4) { p.shield -= 2; std::thread q(Beep, 700, 500); q.detach(); clear_bullet(Cen[x].x3_bullet, Cen[x].y3_bullet); Cen[x].stbullet[2] = 0; }
}
void draw_itemH(int x,int y) {
	setcolor(2, 0);
	gotoxy(x, y); printf("|H|");
}
void clear_item(int x, int y) {
	setcolor(2, 0);
	gotoxy(x, y); printf("    ");
}
void itemH(int x) {

	if (x == 1) {
		int i = rand() % 3;
		gotoxy(10, 10); setcolor(2,0); printf("%d", i);
		if (h.status == 0&&i==0) {

			h.status = 1;
		}
	}
	if (h.status == 1) {
			clear_item(h.x, h.y);
			if (h.y == 65) { h.status = 0; }
			else {
				if (h.Fr != 200) { ++h.Fr; draw_itemH(h.x, h.y); }
				else {
					draw_itemH(h.x, ++h.y); h.Fr = 0;
				}
			}
		}
	
	
	if (h.y == p.y) {
		p.shield += h.heal;
		h.status = 0;
		clear_item(h.x, h.y); 
		h.x = NULL; h.y = NULL;
	}
	





	


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





	for (int i = 0; i < 3; i++)
	{
		if (p.x_bullet[i] >= Aen[x].x && p.x_bullet[i] <= Aen[x].x + 6 && p.y_bullet[i] == Aen[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); Aen[x].shield--; clear_bullet(p.x_bullet[i], p.y_bullet[i]); p.stbullet[i] = 0; p.x_bullet[i] = NULL; p.y_bullet[i] = NULL;
		}
	}



	if (p.x == Aen[x].x && Aen[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; Aen[x].status = 0; clear_enemy(Aen[x].x, Aen[x].y); }
	if (Aen[x].shield == 0) {
		Aen[x].drop = 1;
		if (h.status == 0) { h.x = Aen[x].x; h.y = Aen[x].y; }
		/*std::thread q(draw_kaboom, Aen[x].x, Aen[x].y); q.detach(); */score += 10; clear_enemy(Aen[x].x, Aen[x].y); Aen[x].status = 0; Aen[x].shield = 3;  Aen[x].x = NULL; Aen[x].y = NULL;
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
				if (Aen[x].Fr != 10) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); Aen[x].shoot = 1; }
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
				if (Aen[x].Fr != 10) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y);; Aen[x].shoot = 1; }
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
				if (Aen[x].Fr != 10) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); ; Aen[x].shoot = 1;}
				else {
					int i = rand() % 2;
					if (i == 0) { if (Aen[x].x <= 86) { Aen[x].x++; } }if (i == 1) { if (Aen[x].x >= 10) { Aen[x].x--; } }
					draw_enemy(Aen[x].x, ++Aen[x].y); Aen[x].Fr = 1;
				}
			}
		}
	}


	if (Aen[x].shoot == 1) { enem_shoot(x); }






	if (p.x_bullet[0] >= Aen[x].x && p.x_bullet[0] <= Aen[x].x + 6 && p.y_bullet[0] == Aen[x].y + 1) {

		std::thread q(Beep, 700, 500); q.detach(); Aen[x].shield--; clear_bullet(p.x_bullet[0], p.y_bullet[0]); p.stbullet[0] = 0; p.x_bullet[0] = NULL; p.y_bullet[0] = NULL;
	}
	if (p.x == Aen[x].x && Aen[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; Aen[x].status = 0; clear_enemy(Aen[x].x, Aen[x].y); }
	if (Aen[x].shield == 0) {/*std::thread q(draw_kaboom, Aen[x].x, Aen[x].y); q.detach(); */score += 10; clear_enemy(Aen[x].x, Aen[x].y); Aen[x].status = 0; Aen[x].shield = 3;  int i = rand() % 2; Aen[x].x = NULL; Aen[x].y = NULL;
	}









}
void Benemy(int x) {
	if (Ben[x].status == 0 && wave.status == 1) {
		Ben[x].x = 6 + rand() % 80; Ben[x].y = 7 + rand() % 10; draw_Benemy(Ben[x].x, Ben[x].y); Ben[x].status = 1; Ben[x].mode = 1;//+ rand() % 3;
	}


	if (Ben[x].mode == 1) {
		if (Ben[x].status == 1) {
			char bs1 = cursor(Ben[x].x, Ben[x].y + 1);
			clear_Benemy(Ben[x].x, Ben[x].y);
			if (Ben[x].y == 65) { Ben[x].status = 0; }
			else {
				if (Ben[x].Fr != 3) { ++Ben[x].Fr; draw_Benemy(Ben[x].x, Ben[x].y); }
				else {
					if (Ben[x].x < p.x) { ++Ben[x].x; } if (Ben[x].x > p.x) { --Ben[x].x; }draw_Benemy(Ben[x].x, ++Ben[x].y); Ben[x].Fr = 1;
				}
			}
		}
	}




	for (int i = 0; i < 3; i++)
	{
		if (p.x_bullet[i] >= Ben[x].x && p.x_bullet[i] <= Ben[x].x + 6 && p.y_bullet[i] == Ben[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); Ben[x].shield--; clear_bullet(p.x_bullet[i], p.y_bullet[i]); p.stbullet[i] = 0; p.x_bullet[i] = NULL; p.y_bullet[i] = NULL;
		}
	}



	if (p.x == Ben[x].x && Ben[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; Ben[x].status = 0; clear_Benemy(Ben[x].x, Ben[x].y); }
	if (Ben[x].shield == 0) {
		Ben[x].drop = 1;
		if (h.status == 0) { h.x = Ben[x].x; h.y = Ben[x].y; }
		/*std::thread q(draw_kaboom, Aen[x].x, Aen[x].y); q.detach(); */score += 20; clear_Benemy(Ben[x].x, Ben[x].y); Ben[x].status = 0; Ben[x].shield = 3;  Ben[x].x = NULL; Ben[x].y = NULL;

	}






}
void Cenemy(int x) {
	if (Cen[x].status == 0 && wave.status == 1) {
		Cen[x].x = 6 + rand() % 80; Cen[x].y = 7 + rand() % 10; draw_Cenemy(Cen[x].x, Cen[x].y); Cen[x].status = 1; Cen[x].mode = 1;
	}


	if (Cen[x].mode == 1) {
		if (Cen[x].status == 1) {
			char bs1 = cursor(Cen[x].x, Cen[x].y + 1);
			clear_Cenemy(Cen[x].x, Cen[x].y);
			if (Cen[x].y == 65) { Cen[x].status = 0; }
			else {
				if (Cen[x].Fr != 30) { ++Cen[x].Fr; draw_Cenemy(Cen[x].x, Cen[x].y); Cen[x].shoot = 1; }
				else {
					draw_Cenemy(Cen[x].x, ++Cen[x].y); Cen[x].Fr = 1;
				}
			}
		}
	}




	for (int i = 0; i < 3; i++)
	{
		if (p.x_bullet[i] >= Cen[x].x && p.x_bullet[i] <= Cen[x].x + 5 && p.y_bullet[i] == Cen[x].y) {

			std::thread q(Beep, 700, 500); q.detach(); Cen[x].shield--; clear_bullet(p.x_bullet[i], p.y_bullet[i]); p.stbullet[i] = 0; p.x_bullet[i] = NULL; p.y_bullet[i] = NULL;
		}
	}



	if (p.x == Cen[x].x && Cen[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; Cen[x].status = 0; clear_Cenemy(Cen[x].x, Cen[x].y); }
	if (Cen[x].shield == 0) {
		Cen[x].drop = 1;
		if (h.status == 0) { h.x = Cen[x].x; h.y = Cen[x].y; }
		/*std::thread q(draw_kaboom, Aen[x].x, Aen[x].y); q.detach(); */score += 30; clear_Cenemy(Cen[x].x, Cen[x].y); Cen[x].status = 0; Cen[x].shield = 5;  Cen[x].x = NULL; Cen[x].y = NULL;

	}

	if (Cen[x].shoot == 1) { Cenem_shoot(x); }
	



}