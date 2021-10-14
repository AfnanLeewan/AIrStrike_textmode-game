#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <utility>
#include <algorithm>
#include <string.h>
#include <vector>
#include <iostream>
using namespace std;
void gotoxy(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
inline void ScoreBoard(string PlayerName, unsigned int PlayerScore)
{
    FILE* fp;
    char temp[255];
    unsigned int score[6];
    string name[6];
    vector<pair<unsigned int, string>> userScore;
    fp = fopen("./score.txt", "r");
    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%s", &temp);
        name[i] = temp;
        fscanf(fp, "%d", &score[i]);
        userScore.push_back(make_pair(score[i], name[i]));
    }
    name[5] = PlayerName;
    score[5] = PlayerScore;
    userScore.push_back(make_pair(score[5], name[5]));
    sort(userScore.begin(), userScore.end());
    fclose(fp);
    fopen("./score.txt", "w");
    for (int i = 5; i >= 1; i--)
    {
        strcpy(temp, userScore[i].second.c_str());
        fprintf(fp, "%s %d\n", temp, userScore[i].first);
    }
    fclose(fp);
}
inline void Read(int x,int y)
{
    FILE* fp;
    char temp[255];
    unsigned int score[5];
    string name[5];
    fp = fopen("./score.txt", "r");
    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%s", &temp);
        name[i] = temp;
        fscanf(fp, "%d", &score[i]);
    }
    fclose(fp);
    for (size_t i = 0; i < 5; i++)
    {
        gotoxy(x, y + i); cout << name[i];
        gotoxy(x+25, y + i); cout << score[i] << endl;                                      //<< "      " << score[i] << endl;
      

    }
}