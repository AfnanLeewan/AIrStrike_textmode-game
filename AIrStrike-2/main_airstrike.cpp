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
#define _WIN32_WINNT 0x500
#define screen_x 75
#define screen_y 143
#define scount 80

HANDLE wHnd;
HANDLE rHnd;
DWORD fdwMode;
COORD bufferSize = { screen_x,screen_y };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD characterPos = { 0,0 };
struct star { int x; float y; int status=0; int fr=0; }star[80];
struct player { int x = 45; int y = 65; int shield = 40; int stbullet[3] = { 0,0,0 }; int x_bullet[3]; int y_bullet[3]; int* bulletform = stbullet; char name[1000]; int color= 11; };
struct Aenmemy { int x; int y; int shield = 3; int stbullet = 0; int x_bullet; int y_bullet; int status; int Fr = 1; int mode = 0; int Fr_b = 1; int drop = 0; int shoot; int frl = 10; };
struct ASenmemy { int x; int y; int shield = 3; int stbullet = 0; int x_bullet; int y_bullet; int status; int Fr = 1; int mode = 0; int Fr_b = 1; int drop = 0; int shoot; };
struct Benmemy { int x; int y; int shield = 3; int stbullet = 0; int x_bullet ; int y_bullet ; int status; int Fr = 1; int mode = 0; int Fr_b = 1; int drop = 0; };
struct Cenmemy { int x; int y; int shield = 5; int stbullet[3] = { 0,0,0 }; int x1_bullet; int y1_bullet; int x2_bullet; int y2_bullet; int x3_bullet; int y3_bullet; int status; int Fr = 1; int mode = 0; int Fr_b[3] = { 1,1,1 };  int drop = 0; int shoot = 0; };
struct Item { int x; int y; int fr = 1; int ch = 0; int status = 0; int R = 0; int C = 0; int M = 0; int sR = 0; int sM = 0; int sC = 0; }Itm;
struct item_Rocket { int stbullet = 0; int x_bullet; int y_bullet; int n = 0; int fr = 0;}R;
struct item_Cbullet { int stbullet = 0; int st_bullet[5] = { 0,0,0,0,0 }; int x_bullet[5]; int y_bullet[5]; int n = 30; int fr = 0; }C;
struct item_copilot { int status = 0; int x; int y; int stbullet = 0; int x_bullet; int y_bullet; int fr=0; int n = 20; };
struct Healing { int x; int y; int heal = 5; int status = 0; int Fr = 1; }h;
struct wave { int status = 0; int level = 1; int win = 0; }wave;
struct boom { int x; int y; int status = 0; int fr = 0; }boom;
struct enemy_wave { int score[10] = { 100,500,700,900,1100,1600,2200,3000,4000,0 };
int An[10] = { 5,7,5,5,7,7,7,5,2,5};
int ASen[10]={ 5,5,5,5,7,5,6,7,3,4};
int Bn[10] = { 0,2,2,2,2,3,2,3,4,4 };
int Cn[10] = { 0,1,2,3,3,3,4,3,3,3 };

}ew;
player p;
Aenmemy Aen[10];
ASenmemy ASn[10];
Benmemy Ben[10];
Cenmemy Cen[10];
item_copilot M[2];
int playtime = 1;
int score = 0, sheild = 20;
int pselect = 1;
int shfr = 0;
int r = 2, m = 7;
int boomfr = 0;
int oldhpbar = 0, hpbar = 0;
int oldcbar = 0, cbar = 0;
int oldmbar = 0, mbar = 0;
int oldrbar = 0, rbar = 0;
void setcolor(int fg, int bg);
void clear_map();
int setConsole(int x, int y);
void clear_game();
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
void draw_item(int x,int y);
void clear_item(int x, int y);
void Item(int x);
void draw_rocket(int x, int y);
void clear_rocket(int x,int y);
void Rocket();
void Cbullet();
void draw_copilot(int x, int y);
void copilot();
void clear_copilot(int x, int y);
void clear_kaboom(int x, int y);
void init_star();
void star_fall();
void star_fallll();
void clear_window();
void draw_frame();
void dashboard();
void draw_shield();
void hp_bar();
void C_bar();
void M_bar();
void R_bar();
void draw_cbullet(int x, int y);
void clear_cbullet(int x, int y);
void clear_bar();
void changecolor();
void htplay();
std::mutex mtx;
int main()
{
	HWND consoleWindow = GetConsoleWindow();
	
	setConsole(screen_x,screen_y);
	srand(time(NULL));
	setcursor(0);
	init_star();
	gotoxy(14, 4);

	char ch = '.', menselect = '.', s = '.';
	int  select = 0, mselect = 1;																						// 0=MENU 1=GAMEPLAY 2=SCORE 3=DEV NAME 4=PAUSE 5 =Gameover	

	do {
		setConsole(screen_x, screen_y);
		draw_frame();
		draw_map();
		clear_map();
		while (select == 0) {
			star_fall();
			setcolor(7, 0); gotoxy(112, 18); printf("Press J to change color");
			draw_ship(119, 13);
			setcolor(2, 0); gotoxy(43, 32); printf("      START GAME    ");
			setcolor(2, 0); gotoxy(43, 33); printf("        SCORE        ");
			setcolor(2, 0); gotoxy(43, 34); printf("        EXIT         ");
			setcolor(7, 0); gotoxy(43, 37); printf("PRESS SPACEBAR TO SELECT");
			setcolor(3, 0); startgame(15, 20);
			if (_kbhit()) {
				s = _getch();
				if (s == 's') { if (mselect != 3) { Beep(700, 100); mselect += 1; } }
				if (s == 'w') { if (mselect != 1) { Beep(700, 100); mselect -= 1; } }
				if (s == 'j') { changecolor(); }

				if (s == ' ') { select = mselect; Beep(500, 100); Beep(500, 100);  Beep(700, 100);  break;  }
				fflush(stdin);
			}
			if (mselect == 1) { gotoxy(42, 32); setcolor(12, 0); printf(">");  gotoxy(42, 33); printf(" "); gotoxy(42, 34); printf(" "); }
			if (mselect == 2) { gotoxy(42, 33); setcolor(12, 0); printf(">"); gotoxy(42, 32); printf(" "); gotoxy(42, 34); printf(" "); }
			if (mselect == 3) { gotoxy(42, 34); setcolor(12, 0); printf(">"); gotoxy(42, 32); printf(" "); gotoxy(42, 33); printf(" "); }
		}
		clear_map(); clear_game(); setcolor(7, 0); gotoxy(112, 18); printf("                      ");
		//-----------------------------------------------------------------------------------------//GAMEPLA
		Itm.C = 0; Itm.R = 0; Itm.M = 0;
		Itm.status = 0;
		if (select == 1) { setcolor(10, 0); printf("      ", score); score = 0; }
		while (select == 1) {
			
			dashboard();
			star_fall();
			hp_bar();
			if (wave.status == 0) {  gotoxy(45, 14); setcolor(0, 0); printf("                               "); gotoxy(50, 15); setcolor(2, 0); printf("WAVE %d", wave.level); Sleep(2000);  gotoxy(50, 15); printf("         "); wave.status = 1; }




			draw_ship(p.x, p.y);

			if (_kbhit()) {
				ch = _getch();
				if (ch == 'a' && p.x != 5 && M[0].x != 5) { clear_ship(p.x, p.y); draw_ship(--p.x, p.y); }
				if (ch == 'd' && p.x != 94&&M[1].x!=97) { clear_ship(p.x, p.y); draw_ship(++p.x, p.y); }
				if (ch == 'w' && p.y != 7) { clear_ship(p.x, p.y); draw_ship(p.x, --p.y); }
				if (ch == 's' && p.y != 65) { clear_ship(p.x, p.y); draw_ship(p.x, ++p.y); }
				if (ch == ' ') { select = 4; }
				if (ch == 'j' && (p.stbullet[0] == 0 || p.stbullet[1] == 0 || p.stbullet[2] == 0))

				{
					if (p.stbullet[0] == 0) { p.stbullet[0] = 1; p.x_bullet[0] = p.x + 4; p.y_bullet[0] = p.y - 1; }
					else if (p.stbullet[1] == 0) { p.stbullet[1] = 1; p.x_bullet[1] = p.x + 4; p.y_bullet[1] = p.y - 1; }
					else if (p.stbullet[2] == 0) { p.stbullet[2] = 1; p.x_bullet[2] = p.x + 4; p.y_bullet[2] = p.y - 1; }
				}
				if (ch == 'k' &&( R.stbullet == 0&&C.stbullet==0)&&(Itm.R==1||Itm.C==1)) { 
					if (Itm.R == 1) { R.stbullet = 1; R.x_bullet = p.x + 4; R.y_bullet = p.y - 1; }
					if (Itm.C == 1) { C.stbullet = 1; for (int i = 0; i < 5; i++) { C.st_bullet[i] = 1; C.x_bullet[i] = p.x + 4; C.y_bullet[i] = p.y - 1; } }

					
				}
				fflush(stdin);

			}

			for (int i = 0; i < 3; i++)
			{
				bullet_shoot(i);
			}
			Rocket();
			Cbullet();
			copilot();














			
			//------------------------------------------------wave1
			if (wave.level == 1) {

				for (int i=0;i<ew.ASen[wave.level-1]; i++)
				{
					ASenemy(i);
				}
				for (int i = 0; i < ew.An[wave.level-1]; i++)
				{
					Aenemy(i);
				}
				for (int i = 0; i < ew.Bn[wave.level-1]; i++)
				{
					Benemy(i);
				}
				for (int i = 0; i < ew.Cn[wave.level-1]; i++)
				{
					Cenemy(i);
				}


				if (score >= ew.score[wave.level-1]) { gotoxy(47, 14); setcolor(2, 0); printf("PREPARE FOR WAVE 2"); wave.status = 2; wave.win++; }
				if (wave.win == 200) { wave.status = 0; wave.level++; wave.win = 0; }
				gotoxy(10, 10); printf("%d",wave.win);
			}
			//------------------------------------------------wave2
			if (wave.level == 2) {
				
				for (int i = 0; i < ew.ASen[wave.level - 1]; i++)
				{
					ASenemy(i);

				}
				for (int i = 0; i < ew.An[wave.level-1]; i++)
				{
					Aenemy(i);
					
				}
				for (int i = 0; i < ew.Bn[wave.level - 1]; i++)
				{
					Benemy(i);

				}
				for (int i = 0; i < ew.Cn[wave.level - 1]; i++)
				{
					Cenemy(i);

				}
				if (score >= ew.score[1]) { gotoxy(47, 14); setcolor(2, 0); printf("PREPARE FOR WAVE 3"); wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave3
			if (wave.level == 3) {

				for (int i = 0; i < ew.ASen[wave.level - 1]; i++)
				{
					ASenemy(i);

				}
				for (int i = 0; i < ew.An[wave.level - 1]; i++)
				{
					Aenemy(i);

				}
				for (int i = 0; i < ew.Bn[wave.level - 1]; i++)
				{
					Benemy(i);

				}
				for (int i = 0; i < ew.Cn[wave.level - 1]; i++)
				{
					Cenemy(i);

				}
				if (score >= ew.score[2]) { gotoxy(47, 14); setcolor(2, 0); printf("PREPARE FOR WAVE 4"); wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave4
			if (wave.level == 4) {

				for (int i = 0; i < ew.ASen[wave.level - 1]; i++)
				{
					ASenemy(i);
				}
				for (int i = 0; i < ew.An[wave.level - 1]; i++)
				{
					Aenemy(i);
				}
				for (int i = 0; i < ew.Bn[wave.level - 1]; i++)
				{
					Benemy(i);
				}
				for (int i = 0; i < ew.Cn[wave.level - 1]; i++)
				{
					Cenemy(i);
				}


				if (score >= ew.score[wave.level - 1]) { gotoxy(47, 14); setcolor(2, 0); printf("PREPARE FOR WAVE 5"); wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave5
			if (wave.level == 5) {

				for (int i = 0; i < ew.ASen[wave.level - 1]; i++)
				{
					ASenemy(i);
				}
				for (int i = 0; i < ew.An[wave.level - 1]; i++)
				{
					Aenemy(i);
				}
				for (int i = 0; i < ew.Bn[wave.level - 1]; i++)
				{
					Benemy(i);
				}
				for (int i = 0; i < ew.Cn[wave.level - 1]; i++)
				{
					Cenemy(i);
				}


				if (score >= ew.score[wave.level - 1]) { gotoxy(47, 14); setcolor(2, 0); printf("PREPARE FOR WAVE 6"); wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave6
			if (wave.level == 5) {

				for (int i = 0; i < ew.ASen[wave.level - 1]; i++)
				{
					ASenemy(i);
				}
				for (int i = 0; i < ew.An[wave.level - 1]; i++)
				{
					Aenemy(i);
				}
				for (int i = 0; i < ew.Bn[wave.level - 1]; i++)
				{
					Benemy(i);
				}
				for (int i = 0; i < ew.Cn[wave.level - 1]; i++)
				{
					Cenemy(i);
				}


				if (score >= ew.score[wave.level - 1]) { gotoxy(47, 14); setcolor(2, 0); printf("PREPARE FOR WAVE 7"); wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave7
			if (wave.level == 5) {

				for (int i = 0; i < ew.ASen[wave.level - 1]; i++)
				{
					ASenemy(i);
				}
				for (int i = 0; i < ew.An[wave.level - 1]; i++)
				{
					Aenemy(i);
				}
				for (int i = 0; i < ew.Bn[wave.level - 1]; i++)
				{
					Benemy(i);
				}
				for (int i = 0; i < ew.Cn[wave.level - 1]; i++)
				{
					Cenemy(i);
				}


				if (score >= ew.score[wave.level - 1]) { gotoxy(47, 14); setcolor(2, 0); printf("PREPARE FOR WAVE 8"); wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave8
			if (wave.level == 5) {

				for (int i = 0; i < ew.ASen[wave.level - 1]; i++)
				{
					ASenemy(i);
				}
				for (int i = 0; i < ew.An[wave.level - 1]; i++)
				{
					Aenemy(i);
				}
				for (int i = 0; i < ew.Bn[wave.level - 1]; i++)
				{
					Benemy(i);
				}
				for (int i = 0; i < ew.Cn[wave.level - 1]; i++)
				{
					Cenemy(i);
				}


				if (score >= ew.score[wave.level - 1]) { gotoxy(47, 14); setcolor(2, 0); printf("PREPARE FOR WAVE 9"); wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave9
			if (wave.level == 5) {

				for (int i = 0; i < ew.ASen[wave.level - 1]; i++)
				{
					ASenemy(i);
				}
				for (int i = 0; i < ew.An[wave.level - 1]; i++)
				{
					Aenemy(i);
				}
				for (int i = 0; i < ew.Bn[wave.level - 1]; i++)
				{
					Benemy(i);
				}
				for (int i = 0; i < ew.Cn[wave.level - 1]; i++)
				{
					Cenemy(i);
				}


				if (score >= ew.score[wave.level - 1]) { gotoxy(47, 14); setcolor(2, 0); printf("PREPARE FOR WAVE 10"); wave.status = 2; wave.win++; }
				if (wave.win == 700) { wave.status = 0; wave.level++; wave.win = 0; }
			}
			//------------------------------------------------wave10
			if (wave.level == 5) {

				for (int i = 0; i < ew.ASen[wave.level - 1]; i++)
				{
					ASenemy(i);
				}
				for (int i = 0; i < ew.An[wave.level - 1]; i++)
				{
					Aenemy(i);
				}
				for (int i = 0; i < ew.Bn[wave.level - 1]; i++)
				{
					Benemy(i);
				}
				for (int i = 0; i < ew.Cn[wave.level - 1]; i++)
				{
					Cenemy(i);
				}

			}

			while (select == 4) {
				
				setcolor(7, 0); gotoxy(43, 31); printf("----------------------");
				setcolor(7, 0); gotoxy(43, 32); printf("|        PAUSE       | ");
				setcolor(r, 0); gotoxy(43, 33); printf("|       RESUME       | ");
				setcolor(m, 0); gotoxy(43, 34); printf("|      MAIN MANU     | ");
				setcolor(7, 0); gotoxy(43, 35); printf("----------------------");
			

				if (_kbhit()) {
					char q;
					q = _getch();

					if (q == 's') { if (pselect != 2) { Beep(700, 100); pselect += 1; } }
					if (q == 'w') { if (pselect != 1) { Beep(700, 100); pselect -= 1; } }
					if (pselect == 1) { r = 2; m = 7; } 
					if (pselect == 2) { r = 7; m = 2; }
					if (q == ' ') {
						if (pselect == 1) { select = 1;		
						setcolor(5, 0); gotoxy(40, 31); printf("                           ");
						setcolor(5, 0); gotoxy(40, 32); printf("                           ");
						setcolor(2, 0); gotoxy(40, 33); printf("                           ");
						setcolor(2, 0); gotoxy(40, 34); printf("                           ");
						setcolor(2, 0); gotoxy(40, 35); printf("                           ");

						}
						else
						{
							select = 0;
						}
					}
				}




			}


			if (p.shield <= 0) {
				gotoxy(108, 26); printf("                              ");
				gameover(18, 20);
				gotoxy(47, 32); scanf("%s",p.name);
				int len = strlen(p.name);
				if (len <= 10) { ScoreBoard(p.name, score); select = 0; }
				else
				{
					
					gotoxy(45, 35); printf("YOUR NAME IS TOO LONG !!!");
					 gameover(18, 20);
				}
				Itm.C = 0; Itm.R = 0; Itm.M = 0;
			}
			//std::thread q(Beep, 700, 500);
			//q.detach();

			Sleep(10);

		}
		while (select == 2) {
			star_fall();
			setcolor(12, 0); gotoxy(51, 28); printf("SCORE"); setcolor(15, 0); gotoxy(25, 20); Read(41, 30); setcolor(8, 0); gotoxy(40, 40); printf("PRESS SPACEBAR TO BACK TO MENU ");
			if (_kbhit()) {
				char s;
				s = _getch();
				if (s == ' ') { select = 0; }
			}
		}


		
		if(select==3){ PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0); }

		



	} while (ch != 'x' || sheild == 0);


	return 0;
}


void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void draw_ship(int x, int y)
{
	setcolor(p.color, 0);
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
	for (int i = 3; i < 106; i++)
	{
		setcolor(0, 7);
		gotoxy(i, 5); printf(" ");
		gotoxy(i, 70); printf(" ");
	}
	for (int i = 6; i < 70; i++)
	{
		setcolor(0, 7);
		gotoxy(3, i); printf("  ");
		gotoxy(104, i); printf("  ");
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
	
	setcolor(2, 0);

	gotoxy(x, y);   printf("'.\\|/.'");
	gotoxy(x, y++); printf("(\\   /)");
	gotoxy(x, y++); printf("- -O- -");
	gotoxy(x, y++); printf("(/   \\)");
	gotoxy(x, y++); printf(",'/|\\'.");
}


void clear_kaboom(int x, int y){
	setcolor(0, 2);
	gotoxy(x, y);	printf("           ");
	gotoxy(x, y++); printf("           ");
	gotoxy(x, y++); printf("           ");
	gotoxy(x, y++); printf("           ");
	gotoxy(x, y++); printf("           ");
	gotoxy(x, y++); printf("           ");
	


}
void gameover(int x, int y) {
	
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
	setcolor(12, 0);
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
	if (ASn[x].stbullet == 0) { ASn[x].x_bullet = ASn[x].x; ASn[x].y_bullet = ASn[x].y; ASn[x].stbullet = 1; }
	clear_bullet(ASn[x].x_bullet, ASn[x].y_bullet);
	if (ASn[x].y_bullet == 69) { ASn[x].stbullet = 0;  ASn[x].x_bullet = ASn[x].x; ASn[x].y_bullet = ASn[x].y; }
	else {

		if (ASn[x].Fr_b != 5) { ASn[x].Fr_b++; draw_enbullet(ASn[x].x_bullet, ASn[x].y_bullet); }

		else { draw_enbullet(ASn[x].x_bullet, ++ASn[x].y_bullet); ASn[x].Fr_b = 1; }
	}

	if (ASn[x].y_bullet == p.y&&ASn[x].x_bullet>=p.x-3&&ASn[x].x_bullet<=p.x+4) { p.shield -= 2; std::thread q(Beep, 700, 500); q.detach(); clear_bullet(ASn[x].x_bullet, ASn[x].y_bullet); ASn[x].stbullet = 0; }
	
}
void bullet_shoot(int x) {
	if (p.stbullet[x] == 1) {
		
		clear_bullet(p.x_bullet[x], p.y_bullet[x]);

		if (p.y_bullet[x] == 6) { p.stbullet[x] = 0; }
	
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
	if (Cen[x].shoot == 1 || Cen[x].stbullet[0] == 1) {
		if (Cen[x].stbullet[0] == 0) { Cen[x].x1_bullet = Cen[x].x + 2; Cen[x].y1_bullet = Cen[x].y + 2; Cen[x].stbullet[0] = 1; }
		clear_bullet(Cen[x].x1_bullet, Cen[x].y1_bullet);
		if (Cen[x].y1_bullet == 69) { Cen[x].stbullet[0] = 0;  Cen[x].x1_bullet = Cen[x].x; Cen[x].y1_bullet = Cen[x].y; }
		else {

			if (Cen[x].Fr_b[0] != 5) { Cen[x].Fr_b[0]++; draw_enbullet(Cen[x].x1_bullet, Cen[x].y1_bullet); }

			else { draw_enbullet(Cen[x].x1_bullet, ++Cen[x].y1_bullet); Cen[x].Fr_b[0] = 1; }
		}
	}
	if (Cen[x].shoot == 1 || Cen[x].stbullet[1] == 1) {
		if (Cen[x].stbullet[1] == 0) { Cen[x].x2_bullet = Cen[x].x + 2; Cen[x].y2_bullet = Cen[x].y + 2; Cen[x].stbullet[1] = 1; }
		clear_bullet(Cen[x].x2_bullet, Cen[x].y2_bullet);
		if (Cen[x].y2_bullet == 69 || Cen[x].x2_bullet == 103) { Cen[x].stbullet[1] = 0;  Cen[x].x2_bullet = Cen[x].x; Cen[x].y2_bullet = Cen[x].y; }
		else {

			if (Cen[x].Fr_b[1] != 5) { Cen[x].Fr_b[1]++; draw_enbullet(Cen[x].x2_bullet, Cen[x].y2_bullet); }

			else { draw_enbullet(++Cen[x].x2_bullet, ++Cen[x].y2_bullet); Cen[x].Fr_b[1] = 1; }
		}
	}
	if (Cen[x].shoot == 1 || Cen[x].stbullet[2] == 1) {
		if (Cen[x].stbullet[2] == 0) { Cen[x].x3_bullet = Cen[x].x + 2; Cen[x].y3_bullet = Cen[x].y + 2; Cen[x].stbullet[2] = 1; }
		clear_bullet(Cen[x].x3_bullet, Cen[x].y3_bullet);
		if (Cen[x].y3_bullet == 69 || Cen[x].x3_bullet == 5) { Cen[x].stbullet[2] = 0;  Cen[x].x3_bullet = Cen[x].x; Cen[x].y3_bullet = Cen[x].y; }
		else {

			if (Cen[x].Fr_b[2] != 5) { Cen[x].Fr_b[2]++; draw_enbullet(Cen[x].x3_bullet, Cen[x].y3_bullet); }

			else { draw_enbullet(--Cen[x].x3_bullet, ++Cen[x].y3_bullet); Cen[x].Fr_b[2] = 1; }
		}

	}




	if (Cen[x].y1_bullet == p.y && Cen[x].x1_bullet >= p.x - 3 && Cen[x].x1_bullet <= p.x + 4) { p.shield -= 2; std::thread q(Beep, 700, 500); q.detach(); clear_bullet(Cen[x].x1_bullet, Cen[x].y1_bullet); Cen[x].stbullet[0] = 0; }
	if (Cen[x].y2_bullet == p.y && Cen[x].x2_bullet >= p.x - 3 && Cen[x].x2_bullet <= p.x + 4) { p.shield -= 2; std::thread q(Beep, 700, 500); q.detach(); clear_bullet(Cen[x].x2_bullet, Cen[x].y2_bullet); Cen[x].stbullet[1] = 0; }
	if (Cen[x].y3_bullet == p.y && Cen[x].x3_bullet >= p.x - 3 && Cen[x].x3_bullet <= p.x + 4) { p.shield -= 2; std::thread q(Beep, 700, 500); q.detach(); clear_bullet(Cen[x].x3_bullet, Cen[x].y3_bullet); Cen[x].stbullet[2] = 0; }
}
void draw_item(int x,int y) {
	
	gotoxy(x, y);
	if (Itm.ch == 0) { setcolor(2, 0);  printf("|H|"); }
	if (Itm.ch == 1) { setcolor(3, 0);  printf("|R|"); }
	if (Itm.ch == 2) { setcolor(4, 0);  printf("|C|"); }
	if (Itm.ch == 3) { setcolor(5, 0);  printf("|M|"); }
	if (Itm.ch == 4) { setcolor(6, 0);  printf("|X|"); }
}
void clear_item(int x, int y) {
	setcolor(2, 0);
	gotoxy(x, y); printf("    ");
}
void Item(int x) {

	if (x == 1) {
		
		int i =  rand() % 3;
		

		if (Itm.status == 0&&i==0&&Itm.x>25&&Itm.x<80) {
			Itm.ch =  rand() % 4;
			Itm.status = 1;
		}
	}

	if (Itm.status == 1) {

			
			clear_item(Itm.x, Itm.y);
			if (Itm.y == 65) { Itm.status = 0; }
			else {
				if (Itm.fr != 200) { ++Itm.fr; draw_item(Itm.x, Itm.y); }
				else {
					draw_item(Itm.x, ++Itm.y); Itm.fr = 0;
				}
			}
			}
			if (Itm.y == p.y&&Itm.x>p.x&&Itm.x<p.x+4) {
				if (Itm.ch == 0 && p.shield < 30) {
					switch (p.shield)
					{
					case 29:
						p.shield += 1;
						break;

					case 28:
						p.shield += 2;
						break;

					case 27:
						p.shield += 3;
						break;

					case 26:
						p.shield+=4;
						break;

				

					}
					if (p.shield <= 25) { p.shield += h.heal; }
				
				}
				if (Itm.ch == 1) { Itm.R = 1; Itm.C = 0; Itm.M = 0; R.n = 30;  }
				if (Itm.ch == 2) { Itm.C = 1; Itm.R = 0; Itm.M = 0; C.n = 30;  }
				if (Itm.ch == 3) { Itm.C = 0; Itm.R = 0; Itm.M = 1;M[0].n = 30; }
				
			Itm.status = 0;
			clear_item(Itm.x, Itm.y); 
			Itm.x = NULL; Itm.y = NULL;
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
				if (Aen[x].Fr != Aen[x].frl) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); }
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
				if (Aen[x].Fr != Aen[x].frl) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); }
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
				if (Aen[x].Fr != Aen[x].frl) { ++Aen[x].Fr; draw_enemy(Aen[x].x, Aen[x].y); }
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

	for (int i = 0; i < 5; i++)
	{
		if (C.x_bullet[i] >= Aen[x].x && C.x_bullet[i] <= Aen[x].x + 6 && C.y_bullet[i] == Aen[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); Aen[x].shield--; clear_bullet(C.x_bullet[i], C.y_bullet[i]); C.st_bullet[i] = 0; C.x_bullet[i] = NULL; C.y_bullet[i] = NULL;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (M[i].x_bullet >= Aen[x].x && M[i].x_bullet <= Aen[x].x + 6 && M[i].y_bullet == Aen[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); Aen[x].shield--; clear_bullet(M[i].x_bullet, M[i].y_bullet); M[i].stbullet = 0; M[i].x_bullet = NULL; M[i].y_bullet = NULL;
		}
	}



	if (R.x_bullet >= Aen[x].x && R.x_bullet <= Aen[x].x + 6 && R.y_bullet == Aen[x].y + 1) { Aen[x].shield = 0; }

	if (p.x == Aen[x].x && Aen[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; Aen[x].status = 0; clear_enemy(Aen[x].x, Aen[x].y); }
	if (Aen[x].shield == 0) {
		Aen[x].drop = 1;
		if (Itm.status == 0) { Itm.x = Aen[x].x; Itm.y = Aen[x].y; }

		


		score += 10; clear_enemy(Aen[x].x, Aen[x].y); Aen[x].status = 0; Aen[x].shield = 3;  Aen[x].x = NULL; Aen[x].y = NULL;
	}

	Item(Aen[x].drop); Aen[x].drop = 0;
	


}
void ASenemy(int x) {

	if (ASn[x].status == 0 && wave.status == 1) {
		ASn[x].x = 6 + rand() % 80; ASn[x].y = 7 + rand() % 10; draw_enemy(ASn[x].x, ASn[x].y); ASn[x].status = 1; ASn[x].mode = 1 + rand() % 3;
	}


	if (ASn[x].mode == 1) {
		if (ASn[x].status == 1) {
			char bs1 = cursor(ASn[x].x, ASn[x].y + 1);
			clear_enemy(ASn[x].x, ASn[x].y);
			if (ASn[x].y == 69) { ASn[x].status = 0; }
			else {
				if (ASn[x].Fr != 10) {
					++ASn[x].Fr; draw_enemy(ASn[x].x, ASn[x].y);
				}
				else {
					if (ASn[x].x < p.x) { ++ASn[x].x; } if (ASn[x].x > p.x) { --ASn[x].x; }draw_enemy(ASn[x].x, ++ASn[x].y); ASn[x].Fr = 1;
				}
			}
		}
	}

	if (ASn[x].mode == 2) {
		if (ASn[x].status == 1) {
			char bs1 = cursor(ASn[x].x, ASn[x].y + 1);
			clear_enemy(ASn[x].x, ASn[x].y);
			if (ASn[x].y == 69) { ASn[x].status = 0; }
			else {
				if (ASn[x].Fr != 10) { ++ASn[x].Fr; draw_enemy(ASn[x].x, ASn[x].y); 
				}
				else {
					draw_enemy(ASn[x].x, ++ASn[x].y); ASn[x].Fr = 1;
				}
			}
		}
	}

	if (ASn[x].mode == 3) {
		if (ASn[x].status == 1) {
			char bs1 = cursor(ASn[x].x, ASn[x].y + 1);
			clear_enemy(ASn[x].x, ASn[x].y);
			if (ASn[x].y == 69) { ASn[x].status = 0; }
			else {
				if (ASn[x].Fr != 10) { ++ASn[x].Fr; draw_enemy(ASn[x].x, ASn[x].y); }
				else {
					int i = rand() % 2;
					if (i == 0) { if (ASn[x].x <= 86) { ASn[x].x++; } }if (i == 1) { if (ASn[x].x >= 10) { ASn[x].x--; } }
					draw_enemy(ASn[x].x, ++ASn[x].y); ASn[x].Fr = 1;
				}
			}
		}
	}





	for (int i = 0; i < 3; i++)
	{
		if (p.x_bullet[i] >= ASn[x].x && p.x_bullet[i] <= ASn[x].x + 6 && p.y_bullet[i] == ASn[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); ASn[x].shield--; clear_bullet(p.x_bullet[i], p.y_bullet[i]); p.stbullet[i] = 0; p.x_bullet[i] = NULL; p.y_bullet[i] = NULL;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		if (C.x_bullet[i] >= ASn[x].x && C.x_bullet[i] <= ASn[x].x + 6 && C.y_bullet[i] == ASn[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); ASn[x].shield--; clear_bullet(C.x_bullet[i], C.y_bullet[i]); C.st_bullet[i] = 0; C.x_bullet[i] = NULL; C.y_bullet[i] = NULL;
		}
	}



	if (R.x_bullet >= ASn[x].x && R.x_bullet <= ASn[x].x + 6 && R.y_bullet == ASn[x].y + 1) { ASn[x].shield = 0; }


	for (int i = 0; i < 2; i++)
	{
		if (M[i].x_bullet >= ASn[x].x && M[i].x_bullet <= ASn[x].x + 6 && M[i].y_bullet == ASn[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); ASn[x].shield--; clear_bullet(M[i].x_bullet, M[i].y_bullet); M[i].stbullet = 0; M[i].x_bullet = NULL; M[i].y_bullet = NULL;
		}
	}



	if (p.x == ASn[x].x && ASn[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; ASn[x].status = 0; clear_enemy(ASn[x].x, ASn[x].y); }
	if (ASn[x].shield == 0) {
		ASn[x].drop = 1;
		if (Itm.status == 0) { Itm.x = ASn[x].x; Itm.y = ASn[x].y; }
		/*std::thread q(draw_kaboom, Aen[x].x, Aen[x].y); q.detach(); */score += 10; clear_enemy(ASn[x].x, ASn[x].y); ASn[x].status = 0; ASn[x].shield = 3;  ASn[x].x = NULL; ASn[x].y = NULL;
	}


	if (ASn[x].shoot == 1||ASn[x].stbullet==1) {

		if (ASn[x].stbullet == 0) { ASn[x].x_bullet = ASn[x].x; ASn[x].y_bullet = ASn[x].y; ASn[x].stbullet = 1; }
		if (ASn[x].stbullet == 0 && wave.status == 2) { ASn[x].x_bullet = NULL; ASn[x].y_bullet = NULL; }
		clear_bullet(ASn[x].x_bullet, ASn[x].y_bullet);
		if (ASn[x].y_bullet == 69) { ASn[x].stbullet = 0;  ASn[x].x_bullet = ASn[x].x; ASn[x].y_bullet = ASn[x].y; }
		else {

			if (ASn[x].Fr_b != 5) { ASn[x].Fr_b++; draw_enbullet(ASn[x].x_bullet, ASn[x].y_bullet); }

			else { draw_enbullet(ASn[x].x_bullet, ++ASn[x].y_bullet); ASn[x].Fr_b = 1; }
		}
	}
		if (ASn[x].y_bullet == p.y && ASn[x].x_bullet >= p.x - 3 && ASn[x].x_bullet <= p.x + 4) { p.shield -= 2; std::thread q(Beep, 700, 500); q.detach(); clear_bullet(ASn[x].x_bullet, ASn[x].y_bullet); ASn[x].stbullet = 0; }
		
		
		if (wave.status == 1) { ASn[x].shoot = 1; }
		if (wave.status == 2 && ASn[x].status == 0) { ASn[x].shoot = 0; }
		
	Item(ASn[x].drop); ASn[x].drop = 0;
}
void Benemy(int x) {
		if (Ben[x].status == 0 && wave.status == 1) {
			Ben[x].x = 6 + rand() % 80; Ben[x].y = 7 + rand() % 10; draw_Benemy(Ben[x].x, Ben[x].y); Ben[x].status = 1; Ben[x].mode = 1;
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


		for (int i = 0; i < 5; i++)
		{
			if (C.x_bullet[i] >= Ben[x].x && C.x_bullet[i] <= Ben[x].x + 6 && C.y_bullet[i] == Ben[x].y + 1) {

				std::thread q(Beep, 700, 500); q.detach(); Ben[x].shield--; clear_bullet(C.x_bullet[i], C.y_bullet[i]); C.st_bullet[i] = 0; C.x_bullet[i] = NULL; C.y_bullet[i] = NULL;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if (M[i].x_bullet >= Ben[x].x && M[i].x_bullet <= Ben[x].x + 6 && M[i].y_bullet == Ben[x].y + 1) {

				std::thread q(Beep, 700, 500); q.detach(); Ben[x].shield--; clear_bullet(M[i].x_bullet, M[i].y_bullet); M[i].stbullet = 0; M[i].x_bullet = NULL; M[i].y_bullet = NULL;
			}
		}



		if (R.x_bullet >= Ben[x].x && R.x_bullet <= Ben[x].x + 6 && R.y_bullet == Ben[x].y + 1) { Ben[x].shield = 0; }

		if (p.x == Ben[x].x && Ben[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; Ben[x].status = 0; clear_Benemy(Ben[x].x, Ben[x].y); }
		if (Ben[x].shield == 0) {
			Ben[x].drop = 1;
			if (Itm.status == 0) { Itm.x = Ben[x].x; Itm.y = Ben[x].y; }
			/*std::thread q(draw_kaboom, Aen[x].x, Aen[x].y); q.detach(); */score += 20; clear_Benemy(Ben[x].x, Ben[x].y); Ben[x].status = 0; Ben[x].shield = 3;  Ben[x].x = NULL; Ben[x].y = NULL;

		}
		Item(Ben[x].drop); Ben[x].drop = 0;





	}
void Cenemy(int x) {





	if (Cen[x].status == 0 && wave.status == 1) {
		Cen[x].x = 6 + rand() % 80; Cen[x].y = 7 + rand() % 10; draw_Cenemy(Cen[x].x, Cen[x].y); Cen[x].status = 1; Cen[x].mode = 1; Cen[x].shield = 5;
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
		if (p.x_bullet[i] >= Cen[x].x && p.x_bullet[i] <= Cen[x].x + 4  && p.y_bullet[i] == Cen[x].y + 1) {

			std::thread q(Beep, 900, 500); q.detach(); Cen[x].shield--; clear_bullet(p.x_bullet[i], p.y_bullet[i]); p.stbullet[i] = 0; p.x_bullet[i] = NULL; p.y_bullet[i] = NULL;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (C.x_bullet[i] >= Cen[x].x && C.x_bullet[i] <= Cen[x].x + 6 && C.y_bullet[i] == Cen[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); Cen[x].shield--; clear_bullet(C.x_bullet[i], C.y_bullet[i]); C.st_bullet[i] = 0; C.x_bullet[i] = NULL; C.y_bullet[i] = NULL;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (M[i].x_bullet >= Cen[x].x && M[i].x_bullet <= Cen[x].x + 6 && M[i].y_bullet == Cen[x].y + 1) {

			std::thread q(Beep, 700, 500); q.detach(); Cen[x].shield--; clear_bullet(M[i].x_bullet, M[i].y_bullet); M[i].stbullet = 0; M[i].x_bullet = NULL; M[i].y_bullet = NULL;
		}
	}

	if (R.x_bullet >= Cen[x].x && R.x_bullet <= Cen[x].x + 6 && R.y_bullet == Cen[x].y + 1) { Cen[x].shield = 0; }

	if (Cen[x].x==p.x&&Cen[x].y == p.y) { std::thread q(Beep, 700, 500); q.detach(); p.shield -= 5; Cen[x].status = 0; clear_Cenemy(Cen[x].x, Cen[x].y); }
	if (Cen[x].shield == 0) {
		Cen[x].drop = 1;
		
		if (Itm.status == 0) { Itm.x = Cen[x].x; Itm.y = Cen[x].y; }
		/*std::thread q(draw_kaboom, Aen[x].x, Aen[x].y); q.detach(); */score += 30; clear_Cenemy(Cen[x].x, Cen[x].y); Cen[x].shield = 5; Cen[x].status = 0;  Cen[x].x = NULL; Cen[x].y = NULL;

	}
	if (Cen[x].shoot == 1 || Cen[x].stbullet[0]==1|| Cen[x].stbullet[1]==1|| Cen[x].stbullet[2] == 1) { Cenem_shoot(x); }
	if (wave.status == 1) { Cen[x].shoot = 1; }
	if (wave.status == 2 && Cen[x].status == 0) { Cen[x].shoot = 0; }




}
void clear_game() {
	for (int i = 0; i < 10; i++) {
		Aen[i].status = 0;
		clear_enemy(Aen[i].x, Aen[i].y);
		Aen[i].stbullet = 0;
	}
	for (int i = 0; i < 10; i++) {
		ASn[i].status = 0;
		clear_enemy(ASn[i].x, ASn[i].y);
		ASn[i].stbullet = 0;
	}
	for (int i = 0; i < 10; i++) {
		Ben[i].status = 0;
		clear_enemy(Ben[i].x, Ben[i].y);

	}
	for (int i = 0; i < 10; i++) {
		Cen[i].status = 0;
		clear_enemy(Cen[i].x, Cen[i].y);
		Cen[i].stbullet[0] = 0;
		Cen[i].stbullet[1] = 0;
		Cen[i].stbullet[2] = 0;
	}
	p.shield = 30;
	score = 0;
	wave.level = 1;
	wave.status=0;





}
void draw_rocket(int x,int y ){
	setcolor(12, 0);
	gotoxy(x, y);   printf("/\\");
	gotoxy(x, ++y); printf("||");
	gotoxy(x, ++y); printf("MM");
}
void clear_rocket(int x, int y) {
	setcolor(14, 0);
	gotoxy(x, y);   printf("  ");
	gotoxy(x, ++y); printf("  ");
	gotoxy(x, ++y); printf("  ");
}
void Rocket() {

	if (Itm.R == 1) {
		if (R.stbullet == 1) {

			clear_rocket(R.x_bullet, R.y_bullet);
			if (R.y_bullet == 6) { R.stbullet = 0;  }
			else { draw_rocket(R.x_bullet, --R.y_bullet); }

		}
	}
	if (R.n == 0 || Itm.R == 0) {
		Itm.R = 0; R.stbullet = 0; R.n = 0; clear_rocket(R.x_bullet, R.y_bullet); R.y_bullet = NULL; R.x_bullet = NULL;

	
	}
	if (R.fr == 10) { R.n--; R.fr = 0; }
	else { R.fr++; }
}
void Cbullet() {

	if (Itm.C == 1) {

		if (C.stbullet == 1) {


			if (C.st_bullet[0] == 1) {

				clear_bullet(C.x_bullet[0], C.y_bullet[0]);
				if (C.y_bullet[0] == 6) { C.st_bullet[0] = 0; }
				else { draw_bullet(C.x_bullet[0], --C.y_bullet[0]); }

			}

			if (C.st_bullet[1] == 1) {

				clear_bullet(C.x_bullet[1], C.y_bullet[1]);
				if (C.y_bullet[1] == 6 || C.x_bullet[1] == 100) { C.st_bullet[1] = 0; }
				else { draw_bullet(++C.x_bullet[1], --C.y_bullet[1]); }

			}

			if (C.st_bullet[2] == 1) {

				clear_bullet(C.x_bullet[2], C.y_bullet[2]);
				if (C.y_bullet[2] == 6 || C.x_bullet[2] == 7) { C.st_bullet[2] = 0; }
				else { draw_bullet(--C.x_bullet[2], --C.y_bullet[2]); }

			}

			if (C.st_bullet[3] == 1) {

				clear_bullet(C.x_bullet[3], C.y_bullet[3]);
				if (C.y_bullet[3] == 6 || C.x_bullet[3] == 100) { C.st_bullet[3] = 0; }
				else { draw_bullet(++C.x_bullet[3], C.y_bullet[3]); }

			}

			if (C.st_bullet[4] == 1) {

				clear_bullet(C.x_bullet[4], C.y_bullet[4]);
				if (C.y_bullet[4] == 6 || C.x_bullet[4] == 10) { C.st_bullet[4] = 0; }
				else { draw_bullet(--C.x_bullet[4], C.y_bullet[4]); }

			}
		}
		if (C.st_bullet[0] == 0 && C.st_bullet[1] == 0 && C.st_bullet[2] == 0 && C.st_bullet[3] == 0 && C.st_bullet[4] == 0) { C.stbullet = 0; for (int i = 0; i < 5; i++) { C.st_bullet[i] = 1; } }
		if (C.fr == 20) { C.n--; C.fr = 0; }
		else { C.fr++; }



	}
	if (C.n == 0 || Itm.C == 0) { Itm.C = 0; C.n = 20;
	for (int i = 0; i < 5; i++)
	{
		clear_bullet(C.x_bullet[i], C.y_bullet[i]);
		C.x_bullet[i] = NULL;
		C.y_bullet[i] = NULL;
	}
	
	
	}




	

}

void draw_copilot(int x, int y) {
	gotoxy(x, y++); setcolor(12, 0); printf("      ");
	gotoxy(x, y++); setcolor(12, 0); printf("  /\\ ");
	gotoxy(x, y++); setcolor(12, 0); printf(" <  > ");
	gotoxy(x, y++); setcolor(12, 0); printf(" /||\\ ");
	gotoxy(x, y++); setcolor(12, 0); printf("       ");
	
}
void clear_copilot(int x, int y) {
	gotoxy(x, y++); setcolor(5, 0); printf("     ");
	gotoxy(x, y++); setcolor(5, 0); printf("     ");
	gotoxy(x, y++); setcolor(5, 0); printf("     ");
	gotoxy(x, y++); setcolor(5, 0); printf("     ");

}
void copilot() {
	if (Itm.M == 1) {
		M[0].x = p.x - 10; M[0].y = p.y;
		M[1].x = p.x + 13; M[1].y = p.y;
		setcolor(5, 0); draw_copilot(M[0].x, M[0].y);
		setcolor(5, 0); draw_copilot(M[1].x, M[1].y);

		for (int i = 0; i < 2; i++) {
			if (M[i].stbullet == 0) { M[i].x_bullet = M[i].x + 3; M[i].y_bullet = M[i].y; M[i].stbullet = 1; }
			clear_bullet(M[i].x_bullet, M[i].y_bullet);
			if (M[i].y_bullet == 7) { M[i].stbullet = 0;   M[i].x_bullet = M[i].x;  M[i].y_bullet = M[i].y; }
			else {



				draw_bullet(M[i].x_bullet, --M[i].y_bullet);
			}


		}


		if (M[0].fr == 20) { M[0].n--; M[0].fr = 0; }
		else { M[0].fr++; }


	}
	if (M[0].n == 0 || Itm.M == 0) {
		Itm.M = 0; M[0].n = 20;
		clear_copilot(M[0].x, M[0].y); clear_copilot(M[1].x, M[1].y);
		clear_bullet(M[0].x_bullet, M[0].y_bullet); clear_bullet(M[1].x_bullet, M[1].y_bullet);
		M[0].x = NULL; M[1].x = NULL;
		M[0].y = NULL; M[1].y = NULL;


	}

	
}
int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void init_star() {
	for (int i = 0; i < 50; i++) {

		star[i].x = 5 + rand() % 99;
		star[i].y = 6 + rand() % 60;



	}

}
void star_fall() {

	for (int i = 0; i < 50; i++) {
		setcolor(8, 0);
		gotoxy(star[i].x,star[i].y); printf("*");
	
	
	}


}
void clear_window() {
	for (int i = 6; i <= 60; i++) {
		for (int j = 5; i <=104; i++)
		{
			gotoxy(i, j); printf(" ");
		}
	}


}
void draw_frame() {
	for (int i = 105; i < 142; i++)
	{
		setcolor(0, 7);
		gotoxy(i, 5); printf(" ");
		gotoxy(i, 20); printf(" ");
		gotoxy(i, 70); printf(" ");
	}
	for (int i = 6; i < 71; i++)
	{
		setcolor(0, 7);
		gotoxy(105, i); printf("  ");
		gotoxy(140, i); printf("  ");
	}
	gotoxy(108, 65); setcolor(7, 0); printf("Name : AFNAN LEEWAN");
	gotoxy(108, 66); setcolor(7, 0); printf("ID   : 64011011");
	htplay();
}
void dashboard() {
	gotoxy(127,22); setcolor(7, 0); printf("       ");
	gotoxy(119,22); setcolor(7, 0); printf("Score : %d", score);
	gotoxy(127, 23); setcolor(7, 0); printf("       ");
	gotoxy(119, 23); setcolor(7, 0); printf("Level : %d", wave.level);

	gotoxy(108,25); setcolor(7, 0); printf("SHIELD");
	gotoxy(108,26);

	

	if (Itm.R == 1) { draw_rocket(123, 8); Itm.sR = 1; R_bar(); }
	if (Itm.C == 1) { draw_cbullet(121, 11); Itm.sC = 1; C_bar(); }
	if (Itm.M == 1) { draw_copilot(110, 14); draw_copilot(131, 14); Itm.sM = 1; M_bar();}
	if (Itm.R == 0 && Itm.sR == 1) { clear_rocket(123, 8); Itm.sR = 0; clear_cbullet(123, 8); clear_bar();}
	if (Itm.C == 0 && Itm.sC == 1) { clear_cbullet(121, 11); Itm.sC = 0; clear_bar(); }
	if (Itm.M == 0&& Itm.sM==1) { clear_copilot(110, 14); clear_copilot(131, 14); Itm.sM = 0; clear_bar(); clear_cbullet(123, 8);
	}


}


void hp_bar()
{

	hpbar = p.shield;
    
    if (oldhpbar > hpbar)
    {
        for (int i = hpbar; i <= oldhpbar; i++)
        {
            setcolor(0, 0);
            gotoxy(108 + i, 26); printf(" ");
        }
    }
    for (int i = 0; i <= hpbar; i++)
    {
        setcolor(0, 2);
        gotoxy(108 + i, 26); printf(" ");
    }
    oldhpbar = hpbar;
}
void draw_cbullet(int x, int y) {
	setcolor(2, 0);
	gotoxy(x, y);	printf("  ^  ");
	gotoxy(x, ++y); printf("^   ^");
	gotoxy(x-=3, ++y); printf("^");
	gotoxy(x += 10, y); printf("^");
}
void clear_cbullet(int x, int y) {
	setcolor(2, 0);
	gotoxy(x, y);	printf("     ");
	gotoxy(x, ++y); printf("     ");
	gotoxy(x -= 3, ++y); printf(" ");
	gotoxy(x += 10, y); printf(" ");
}
void C_bar()
{

	cbar = C.n;
	gotoxy(108, 27); printf("|C|");
	if (oldcbar > cbar)
	{
		for (int i = cbar; i <= oldcbar; i++)
		{
			setcolor(0, 0);
			gotoxy(108 + i, 28); printf(" ");
		}
	}
	for (int i = 0; i <= cbar; i++)
	{
		setcolor(0, 10);
		gotoxy(108 + i, 28); printf(" ");
	}
	oldcbar = cbar;
	
}
void clear_bar() {
	gotoxy(108, 27); printf("    ");
	gotoxy(108, 28); printf("                              ");


}
void M_bar()
{

	mbar = M[0].n;
	gotoxy(108, 27); printf("|M|");
	if (oldmbar > mbar)
	{
		for (int i = mbar; i <= oldmbar; i++)
		{
			setcolor(0, 0);
			gotoxy(108 + i, 28); printf(" ");
		}
	}
	for (int i = 0; i <= mbar; i++)
	{
		setcolor(0, 11);
		gotoxy(108 + i, 28); printf(" ");
	}
	oldmbar = mbar;
	
}

void R_bar() {
	rbar =R.n;
	gotoxy(108, 27); printf("|R|");
	if (oldrbar > rbar)
	{
		for (int i = rbar; i <= oldrbar; i++)
		{
			setcolor(0, 0);
			gotoxy(108 + i, 28); printf(" ");
		}
	}
	for (int i = 0; i <= rbar; i++)
	{
		setcolor(0, 12);
		gotoxy(108 + i, 28); printf(" ");
	}
	oldrbar = rbar;
}
void changecolor() {
	switch (p.color)
	{
	case 11 :
		p.color = 12;

		break;
	case 12:
		p.color = 9;

		break;
	case 9:
		p.color = 15;

		break;
	case 15:
		p.color = 11;

		break;



	}


}
void htplay() {
	gotoxy(108, 35); printf("HOW TO PLAY :");
	gotoxy(110, 37); setcolor(0, 7); printf(" W ");
	gotoxy(108, 38); setcolor(0, 7); printf("A  S  D");
	gotoxy(117, 38); setcolor(7, 0); printf("Controle your Aircraft");
	gotoxy(115, 40); setcolor(0, 7); printf(" K ");
	gotoxy(122, 40); setcolor(7, 0); printf("Shoot");
	gotoxy(115, 42); setcolor(0, 7); printf(" J ");
	gotoxy(122, 42); setcolor(7, 0); printf("Use Item");
	gotoxy(111, 44); setcolor(0, 7); printf(" SpaceBar ");
	gotoxy(122, 44); setcolor(7, 0); printf("Pause");




}