#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>


#define TEAM_SIZE 3
#define TOTAL_TEAMS 2

typedef struct _point
{
    int x, y;
}point;


PLAYER *teams[TOTAL_TEAMS][TEAM_SIZE];
point pointarr[TOTAL_TEAMS][TEAM_SIZE];
int linex;
double str_move = 0;
bool running_j = true;
bool L_check_lie = false;
bool R_check_lie = false;

void initializeGame() {
    // 초기화 로직을 추가
    
    
    for (int i = 0; i < TEAM_SIZE * TOTAL_TEAMS; i++) {
        if (player[i].is_alive == false)
            player[i].hasitem = false;

    }

    for(int i = 0; i < TEAM_SIZE * TOTAL_TEAMS; i++){
        teams[i % 2][i / 2] = &player[i];
    }

    // 맵 초기화
    map_init(3, 50);

    for (int i = 0; i < TOTAL_TEAMS; i++) {
        for (int j = 0; j < TEAM_SIZE; j++) {
            pointarr[i][j].x = (N_COL / 2) + ((i % 2) * 2 - 1) * (j + 2);
            pointarr[i][j].y = N_ROW / 2;
        }
    }

    linex = N_COL / 2 ;
}



void handleInput() {
    // 입력 처리 코드를 추가
    key_t key = get_key();
    if (key != K_UNDEFINED)
    {
        switch (key) {
        case K_QUIT:
            running_j = false;
            break;
        case K_Z:
            // 왼쪽으로 당김
            str_move -= 1;
            break;
        case K_SLASH:
            // 오른쪽으로 당김
            str_move += 1;
            break;
        case K_X:
            if (L_check_lie)
            {
                break;
            }

            // 왼쪽 팀이 눕기 사용
            bool L_canlie = true;

            for (int i = 0; i < TEAM_SIZE; i++)
            {
                if (teams[0][i] == NULL)
                    continue;
                if (teams[0][i]->stamina < 30)
                {
                    L_canlie = false;
                }
            }

            if (L_canlie)
            {
                L_check_lie = true;
                
                for (int i = 0; i < TEAM_SIZE; i++)
                {

                    if (teams[0][i] == NULL)
                        continue;
                    str_move -= teams[0][i]->str + teams[0][i]->item.str_buf;
                    teams[0][i]->stamina -= 30;
                }
            }
            break;
        case K_DOT:
            if (R_check_lie)
            {
                break;
            }

            // 오른쪽 팀이 눕기 사용
            bool R_canlie = true;

            for (int i = 0; i < TEAM_SIZE; i++)
            {
                if (teams[1][i] == NULL)
                    continue;
                if (teams[1][i]->stamina < 30)
                {
                    R_canlie = false;
                }
            }

            if (R_canlie)
            {
                R_check_lie = true;
                for (int i = 0; i < TEAM_SIZE; i++)
                {
                    if (teams[1][i] == NULL)
                        continue;

                    str_move += teams[1][i]->str + teams[1][i]->item.str_buf;
                    teams[1][i]->stamina -= 30;
                }
            }
            break;
        }
    }
}

void checkPlayerFall() {
    // 플레이어가 구멍에 떨어졌는지 확인하는 코드를 추가
    for (int i = 0; i < TEAM_SIZE; i++) {
        if (pointarr[0][i].x >= N_COL / 2) {
            // 구멍에 떨어짐
            teams[0][i] = NULL;
            // 떨어진 플레이어의 힘 맵 값을 초기화
        }
    }
    for (int i = 0; i < TEAM_SIZE; i++) {
        if (pointarr[1][i].x <= N_COL / 2) {
            // 구멍에 떨어짐
            teams[1][i] = NULL;
            // 떨어진 플레이어의 힘 맵 값을 초기화
        }
    }
}


void draw_map() {
    for (int i = 1; i < N_COL - 2; i++)
    {
        back_buf[1][i] = ' ';
    }

    for (int i = linex -1; i <= linex +1; i++)
    {
        back_buf[1][i] = '-';
    }

    for (int i = 0; i < TEAM_SIZE * TOTAL_TEAMS; i++)
    {

        if (teams[i / TEAM_SIZE][i % TEAM_SIZE] == NULL)
            continue;
        back_buf[pointarr[i / TEAM_SIZE][i % TEAM_SIZE].y][pointarr[i / TEAM_SIZE][i % TEAM_SIZE].x] = '0' + teams[i / TEAM_SIZE][i % TEAM_SIZE]->id;
    }
}

bool check_alive(int team) {

    for (int i = 0; i < TEAM_SIZE; i++)
    {
        if (teams[team][i] != NULL)
        {
            return true;
        }
    }
    return false;
}

void juldarigi() {
    initializeGame();
    system("cls");

    back_buf[0][N_COL / 2] = ' ';
    back_buf[2][N_COL / 2] = ' ';

    double is_one_sec = 0;
    double timer = getTick();
    // 왼쪽 팀 str합 추가하기
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        str_move -= teams[0][i]->str + teams[0][i]->item.str_buf;
    }
    // 오른쪽 팀 str합 추가하기
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        str_move += teams[1][i]->str + teams[1][i]->item.str_buf;
    }

    display();
    while (running_j){
        handleInput();

        is_one_sec += getTick() - timer;
        timer = getTick();
        
        if (is_one_sec >= 1000)
        {
            if (str_move < 0) {
                linex -= L_check_lie ? 2 : 1;
                for (int i = 0; i < TOTAL_TEAMS; i++) {
                    for (int j = 0; j < TEAM_SIZE; j++) {
                        pointarr[i][j].x -= L_check_lie ? 2 : 1;
                    }
                }
            }
            else if (str_move > 0) {
                linex += R_check_lie ? 2 : 1;
                for (int i = 0; i < TOTAL_TEAMS; i++) {
                    for (int j = 0; j < TEAM_SIZE; j++) {
                        pointarr[i][j].x += R_check_lie ? 2 : 1;
                    }
                }
            }

            checkPlayerFall();

            is_one_sec = 0;
            str_move = 0;
            for (int i = 0; i < TEAM_SIZE; i++)
            {
                if (teams[0][i] == NULL)
                    continue;
                str_move -= teams[0][i]->str + teams[0][i]->item.str_buf;
            }
            for (int i = 0; i < TEAM_SIZE; i++)
            {

                if (teams[1][i] == NULL)
                    continue;
                str_move += teams[1][i]->str + teams[1][i]->item.str_buf;
            }

            L_check_lie = false;
            R_check_lie = false;

            if (!check_alive(1))
            {
                for (int i = 0; i < TEAM_SIZE * TOTAL_TEAMS; i++) {
                    teams[i % 2][i / 2] = &player[i];
                }

                for (int i = 0; i < TEAM_SIZE; i++) {
                    teams[0][i]->is_alive = true;
                }

                for (int i = 0; i < TEAM_SIZE; i++) {
                    teams[1][i]->hasitem = false;
                    teams[1][i]->intel /= 2;
                    teams[1][i]->str /= 2;
                }

                running_j = false;
            }
            
            if (!check_alive(0))
            {
                for (int i = 0; i < TEAM_SIZE * TOTAL_TEAMS; i++) {
                    teams[i % 2][i / 2] = &player[i];
                }

                for (int i = 0; i < TEAM_SIZE; i++) {
                    teams[1][i]->is_alive = true;
                }

                for (int i = 0; i < TEAM_SIZE; i++) {
                    teams[0][i]->hasitem = false;
                    teams[0][i]->intel /= 2;
                    teams[0][i]->str /= 2;
                }
                
                running_j = false;

            }

            
        }

        draw_map();
        display();
        gotoxy(N_ROW + 1, 0);

        printf("str : %lf", str_move);
        gotoxy(N_ROW + 2, 0);
        if (R_check_lie && L_check_lie)
            printf("양쪽이 누웠습니다");
        else if (R_check_lie)
            printf("오른쪽이 누웠습니다");
        else if (L_check_lie)
            printf("왼쪽이 누웠습니다");
        else
            printf("                  ");

    }

}


// 나머지 부분에 필요한 함수 및 변수를 추가하세요.
