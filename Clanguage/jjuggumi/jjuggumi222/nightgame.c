#define _CRT_SECURE_NO_WARNINGS
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3


void init_nightgame();
void item_spawn();
void chocie(int, int);
void depredation(int, int);
void conciliate(int, int);
void hasitem_choice(int, int, int);
void move_manual(key_t key);
void move_random(int i, int dir);
void move_tail(int i, int nx, int ny);

int item_buf[ROW_MAX][COL_MAX];
int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];
int ix[ITEM_MAX], iy[ITEM_MAX];
char strn[] = { "0이" };
char strn2[] = { "0번에게 시도" };
void init_nightgame() {
	map_init(8, 23);
	int x, y;
	for (int i = 0; i < n_player; i++) {
		if (player[i].is_alive == true) {
			do {
				x = randint(1, N_ROW - 2);
				y = randint(1, N_COL - 2);
			} while (!placable(x, y));
			px[i] = x;
			py[i] = y;
			period[i] = randint(100, 200);
			back_buf[px[i]][py[i]] = '0' + i;
		}
	}
	item_spawn();
}

void item_spawn() {
	srand((unsigned int)time(NULL));
	int n_item = 2;
	int x, y;
	for (int i = 0; i < n_item; i++) {
		do {
			x = randint(2, N_ROW - 3);
			y = randint(2, N_COL - 3);
		} while (!placable(x, y));
		ix[i] = x;
		iy[i] = y;
		back_buf[ix[i]][iy[i]] = 'I';
		item_buf[ix[i]][iy[i]] = rand() % 7;
	}
}

void chocie(int p1, int p2) {
	srand((unsigned int)time(NULL));
	int number;
	int save_p;
	int i = 0;
	if (p1 == 0) {
		gotoxy(N_ROW + 1, 0);
		printf("선택지(1.강탈시도(str) /2.회유시도(int) /3.무시 ) : ");
		while (i == 0) {
			scanf_s("%d", &number);
			if (number > 0 && number < 4) {
				if (player[p1].stamina > 0) {
					switch (number) {
					case 1: depredation(p1, p2); i++; break;
					case 2: conciliate(p1, p2); i++; break;
					case 3: printf("무시했다"); Sleep(1000); i++; break;
					}
				}
				else {
					if (number == 3) {
						printf("무시했다"); Sleep(1000); i++;
					}
					else {
						printf("스테미나 부족");
						Sleep(1000);
						gotoxy(N_ROW + 1, strlen("선택지(1.강탈시도(str) /2.회유시도(int) /3.무시 ) : "));
						printf("                                                     ");
						gotoxy(N_ROW + 2, 0);
						printf("                                                     ");
						gotoxy(N_ROW + 1, strlen("선택지(1.강탈시도(str) /2.회유시도(int) /3.무시 ) : "));
						continue;
					}
				}
			}
			else {
				gotoxy(N_ROW + 1, strlen("선택지(1.강탈시도(str) /2.회유시도(int) /3.무시 ) : "));
				printf("                                                     ");
				gotoxy(N_ROW + 1, strlen("선택지(1.강탈시도(str) /2.회유시도(int) /3.무시 ) : "));
				continue;
			}
		}
		gotoxy(N_ROW + 1, 0);
		printf("                                                               ");
		gotoxy(N_ROW + 2, 0);
		printf("                                                               ");
	}
	else {
		if (player[p1].hasitem == false && player[p2].hasitem == false) {

		}
		else {
			if (player[p1].hasitem == false && player[p2].hasitem == true) {
				number = rand() % 3 + 1;
				switch (number) {
				case 1: depredation(p1, p2); break;
				case 2: conciliate(p1, p2); break;
				case 3: break;
				}
				gotoxy(N_ROW + 1, 0);
				printf("                                                     ");
				gotoxy(N_ROW + 1, 0);
				strn[0] = p1 + '0';
				printf("%s", strn);
				strn2[0] = p2 + '0';
				printf(" %s", strn2);
				Sleep(1000);
				gotoxy(N_ROW + 1, 0);
				printf("                                                     ");
			}
			else {
				number = rand() % 3 + 1;
				save_p = p1;
				p1 = p2;
				p2 = save_p;
				if (player[p1].stamina > 0) {
					switch (number) {
					case 1: depredation(p1, p2); break;
					case 2: conciliate(p1, p2); break;
					case 3: break;
					}
					gotoxy(N_ROW + 1, 0);
					printf("                                                     ");
					gotoxy(N_ROW + 1, 0);
					strn[0] = p1 + '0';
					printf("%s", strn);
					strn2[0] = p2 + '0';
					printf(" %s", strn2);
					Sleep(1000);
					gotoxy(N_ROW + 1, 0);
					printf("                                                     ");
				}
			}
		}
	}
}

void depredation(int p1, int p2) {
	int p1_str = player[p1].str + player[p1].item.str_buf;
	int p2_str = player[p2].str + player[p2].item.str_buf;
	int dummy = ITEM_MAX - 1;
	if (p1 == 0) {
		if (p1_str > p2_str) {
			if (player[p1].hasitem == true) {
				if (player[p2].hasitem == true) {
					strncpy(player[dummy].item.name, player[p1].item.name, 100);
					player[dummy].item.intel_buf = player[p1].item.intel_buf;
					player[dummy].item.str_buf = player[p1].item.str_buf;
					player[dummy].item.stamina_buf = player[p1].item.stamina_buf;

					strncpy(player[p1].item.name, player[p2].item.name, 100);
					player[p1].item.intel_buf = player[p2].item.intel_buf;
					player[p1].item.str_buf = player[p2].item.str_buf;
					player[p1].item.stamina_buf = player[p2].item.stamina_buf;

					strncpy(player[p2].item.name, player[dummy].item.name, 100);
					player[p2].item.intel_buf = player[dummy].item.intel_buf;
					player[p2].item.str_buf = player[dummy].item.str_buf;
					player[p2].item.stamina_buf = player[dummy].item.stamina_buf;
					printf("강탈 성공 %d 스테미나 : -40%%", p1);
					player[p1].stamina -= 40;
				}
				else {
					printf("상대가 아이템을 갖고있지않다.");
					player[p1].stamina -= 40;
				}
			}
			else if (player[p1].hasitem == false) {
				player[p1].hasitem = true;
				strncpy(player[dummy].item.name, player[p1].item.name, 100);
				player[dummy].item.intel_buf = player[p1].item.intel_buf;
				player[dummy].item.str_buf = player[p1].item.str_buf;
				player[dummy].item.stamina_buf = player[p1].item.stamina_buf;

				strncpy(player[p1].item.name, player[p2].item.name, 100);
				player[p1].item.intel_buf = player[p2].item.intel_buf;
				player[p1].item.str_buf = player[p2].item.str_buf;
				player[p1].item.stamina_buf = player[p2].item.stamina_buf;

				strncpy(player[p2].item.name, player[dummy].item.name, 100);
				player[p2].item.intel_buf = player[dummy].item.intel_buf;
				player[p2].item.str_buf = player[dummy].item.str_buf;
				player[p2].item.stamina_buf = player[dummy].item.stamina_buf;
				printf("강탈 성공 %d 스테미나 : -40%%", p1);
				player[p1].stamina -= 40;
			}
		}
		else if (p1_str < p2_str) {
			printf("강탈 실패 %d 스테미나 : -60%%", p1);
			player[p1].stamina -= 60;
		}
		Sleep(1000);
	}
	else {
		if (p1_str > p2_str) {
			if (player[p1].hasitem == true) {
				if (player[p2].hasitem == true) {
					strncpy(player[dummy].item.name, player[p1].item.name, 100);
					player[dummy].item.intel_buf = player[p1].item.intel_buf;
					player[dummy].item.str_buf = player[p1].item.str_buf;
					player[dummy].item.stamina_buf = player[p1].item.stamina_buf;

					strncpy(player[p1].item.name, player[p2].item.name, 100);
					player[p1].item.intel_buf = player[p2].item.intel_buf;
					player[p1].item.str_buf = player[p2].item.str_buf;
					player[p1].item.stamina_buf = player[p2].item.stamina_buf;

					strncpy(player[p2].item.name, player[dummy].item.name, 100);
					player[p2].item.intel_buf = player[dummy].item.intel_buf;
					player[p2].item.str_buf = player[dummy].item.str_buf;
					player[p2].item.stamina_buf = player[dummy].item.stamina_buf;
					player[p1].stamina -= 40;
				}
				else {
					player[p1].stamina -= 40;
				}
			}
			else if (player[p1].hasitem == false) {
				player[p1].hasitem = true;
				strncpy(player[dummy].item.name, player[p1].item.name, 100);
				player[dummy].item.intel_buf = player[p1].item.intel_buf;
				player[dummy].item.str_buf = player[p1].item.str_buf;
				player[dummy].item.stamina_buf = player[p1].item.stamina_buf;

				strncpy(player[p1].item.name, player[p2].item.name, 100);
				player[p1].item.intel_buf = player[p2].item.intel_buf;
				player[p1].item.str_buf = player[p2].item.str_buf;
				player[p1].item.stamina_buf = player[p2].item.stamina_buf;

				strncpy(player[p2].item.name, player[dummy].item.name, 100);
				player[p2].item.intel_buf = player[dummy].item.intel_buf;
				player[p2].item.str_buf = player[dummy].item.str_buf;
				player[p2].item.stamina_buf = player[dummy].item.stamina_buf;
				player[p1].stamina -= 40;
			}
		}
		else if (p1_str < p2_str) {
			player[p1].stamina -= 60;
		}
	}
}

void conciliate(int p1, int p2) {
	int p1_int = player[p1].intel + player[p1].item.intel_buf;
	int p2_int = player[p2].intel + player[p2].item.intel_buf;
	int dummy = ITEM_MAX - 1;
	if (p1 == 0) {
		if (p1_int > p2_int) {
			if (player[p1].hasitem == true) {
				if (player[p2].hasitem == true) {
					strncpy(player[dummy].item.name, player[p1].item.name, 100);
					player[dummy].item.intel_buf = player[p1].item.intel_buf;
					player[dummy].item.str_buf = player[p1].item.str_buf;
					player[dummy].item.stamina_buf = player[p1].item.stamina_buf;

					strncpy(player[p1].item.name, player[p2].item.name, 100);
					player[p1].item.intel_buf = player[p2].item.intel_buf;
					player[p1].item.str_buf = player[p2].item.str_buf;
					player[p1].item.stamina_buf = player[p2].item.stamina_buf;

					strncpy(player[p2].item.name, player[dummy].item.name, 100);
					player[p2].item.intel_buf = player[dummy].item.intel_buf;
					player[p2].item.str_buf = player[dummy].item.str_buf;
					player[p2].item.stamina_buf = player[dummy].item.stamina_buf;
					printf("회유 성공 %d 스테미나 : -20%%", p1);
					player[p1].stamina -= 20;
				}
				else {
					printf("상대가 아이템을 갖고있지않다.");
					player[p1].stamina -= 20;
				}
			}
			else if (player[p1].hasitem == false) {
				player[p1].hasitem = true;
				strncpy(player[dummy].item.name, player[p1].item.name, 100);
				player[dummy].item.intel_buf = player[p1].item.intel_buf;
				player[dummy].item.str_buf = player[p1].item.str_buf;
				player[dummy].item.stamina_buf = player[p1].item.stamina_buf;

				strncpy(player[p1].item.name, player[p2].item.name, 100);
				player[p1].item.intel_buf = player[p2].item.intel_buf;
				player[p1].item.str_buf = player[p2].item.str_buf;
				player[p1].item.stamina_buf = player[p2].item.stamina_buf;

				strncpy(player[p2].item.name, player[dummy].item.name, 100);
				player[p2].item.intel_buf = player[dummy].item.intel_buf;
				player[p2].item.str_buf = player[dummy].item.str_buf;
				player[p2].item.stamina_buf = player[dummy].item.stamina_buf;
				printf("회유 성공 %d 스테미나 : -20%%", p1);
				player[p1].stamina -= 20;
			}
		}
		else if (p1_int < p2_int) {
			printf("회유 실패 %d 스테미나 : -40%%", p1);
			player[p1].stamina -= 40;
		}
		Sleep(1000);
	}
	else {
		if (p1_int > p2_int) {
			if (player[p1].hasitem == true) {
				if (player[p2].hasitem == true) {
					strncpy(player[dummy].item.name, player[p1].item.name, 100);
					player[dummy].item.intel_buf = player[p1].item.intel_buf;
					player[dummy].item.str_buf = player[p1].item.str_buf;
					player[dummy].item.stamina_buf = player[p1].item.stamina_buf;

					strncpy(player[p1].item.name, player[p2].item.name, 100);
					player[p1].item.intel_buf = player[p2].item.intel_buf;
					player[p1].item.str_buf = player[p2].item.str_buf;
					player[p1].item.stamina_buf = player[p2].item.stamina_buf;

					strncpy(player[p2].item.name, player[dummy].item.name, 100);
					player[p2].item.intel_buf = player[dummy].item.intel_buf;
					player[p2].item.str_buf = player[dummy].item.str_buf;
					player[p2].item.stamina_buf = player[dummy].item.stamina_buf;
					player[p1].stamina -= 20;
				}
				else {
					player[p1].stamina -= 20;
				}
			}
			else if (player[p1].hasitem == false) {
				player[p1].hasitem = true;
				strncpy(player[dummy].item.name, player[p1].item.name, 100);
				player[dummy].item.intel_buf = player[p1].item.intel_buf;
				player[dummy].item.str_buf = player[p1].item.str_buf;
				player[dummy].item.stamina_buf = player[p1].item.stamina_buf;

				strncpy(player[p1].item.name, player[p2].item.name, 100);
				player[p1].item.intel_buf = player[p2].item.intel_buf;
				player[p1].item.str_buf = player[p2].item.str_buf;
				player[p1].item.stamina_buf = player[p2].item.stamina_buf;

				strncpy(player[p2].item.name, player[dummy].item.name, 100);
				player[p2].item.intel_buf = player[dummy].item.intel_buf;
				player[p2].item.str_buf = player[dummy].item.str_buf;
				player[p2].item.stamina_buf = player[dummy].item.stamina_buf;
				player[p1].stamina -= 20;
			}
		}
		else if (p1_int < p2_int) {
			player[p1].stamina -= 40;
		}
	}
}

void hasitem_choice(int p1, int nx, int ny) {
	srand((unsigned int)time(NULL));
	int number;
	int dummy = player[p1].n;
	if (p1 == 0) {
		gotoxy(N_ROW + 1, 0);
		printf("선택지(1.아이템 교체 /2.지나간다 ) : ");
		scanf_s("%d", &number);
		switch (number)
		{
		case 1:strncpy(player[p1].item.name, item[item_buf[nx][ny]].name, 100);
			player[p1].item.intel_buf = item[item_buf[nx][ny]].intel_buf;
			player[p1].item.str_buf = item[item_buf[nx][ny]].str_buf;
			player[p1].item.stamina_buf = item[item_buf[nx][ny]].stamina_buf;
			player[p1].n = item_buf[nx][ny];
			item_buf[nx][ny] = dummy;
			printf("교체했다");
			Sleep(500);
			break;
		case 2:
			printf("지나갔다");
			Sleep(500);
			break;
		}
		gotoxy(N_ROW + 1, 0);
		printf("                                                     ");
		gotoxy(N_ROW + 2, 0);
		printf("                                                     ");
	}
	else {
		number = rand() % 2 + 1;
		switch (number)
		{
		case 1:strncpy(player[p1].item.name, item[item_buf[nx][ny]].name, 100);
			player[p1].item.intel_buf = item[item_buf[nx][ny]].intel_buf;
			player[p1].item.str_buf = item[item_buf[nx][ny]].str_buf;
			player[p1].item.stamina_buf = item[item_buf[nx][ny]].stamina_buf;
			player[p1].n = item_buf[nx][ny];
			item_buf[nx][ny] = dummy;
			break;
		case 2:
			break;
		}
	}
}


void move_manual(key_t key) {
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break;
	case K_DOWN: dir = DIR_DOWN; break;
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// 움직여서 놓일 자리
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (back_buf[nx][ny] == '1') {
		chocie(0, 1);
		if (!placable(nx, ny)) {
			return;
		}
	}
	else if (back_buf[nx][ny] == '2') {
		chocie(0, 2);
		if (!placable(nx, ny)) {
			return;
		}
	}
	else if (back_buf[nx][ny] == '3') {
		chocie(0, 3);
		if (!placable(nx, ny)) {
			return;
		}
	}
	else if (back_buf[nx][ny] == '4') {
		chocie(0, 4);
		if (!placable(nx, ny)) {
			return;
		}
	}
	else if (back_buf[nx][ny] == '5') {
		chocie(0, 5);
		if (!placable(nx, ny)) {
			return;
		}
	}
	else if (back_buf[nx][ny] == '6') {
		chocie(0, 6);
		if (!placable(nx, ny)) {
			return;
		}
	}
	else if (back_buf[nx][ny] == '7') {
		chocie(0, 7);
		if (!placable(nx, ny)) {
			return;
		}
	}
	else if (back_buf[nx][ny] == '8') {
		chocie(0, 8);
		if (!placable(nx, ny)) {
			return;
		}
	}
	else if (back_buf[nx][ny] == '9') {
		chocie(0, 9);
		if (!placable(nx, ny)) {
			return;
		}
	}
	else if (back_buf[nx][ny] == 'I') {
		if (player[0].hasitem == false) {
			player[0].hasitem = true;
			strncpy(player[0].item.name, item[item_buf[nx][ny]].name, 100);
			player[0].item.intel_buf = item[item_buf[nx][ny]].intel_buf;
			player[0].item.str_buf = item[item_buf[nx][ny]].str_buf;
			player[0].item.stamina_buf = item[item_buf[nx][ny]].stamina_buf;
			player[0].n = item_buf[nx][ny];
			item_buf[nx][ny] = NULL;
		}
		else if (player[0].hasitem = true) {
			hasitem_choice(0, nx, ny);
			if (!placable(nx, ny)) {
				return;
			}
		}
	}
	else {
		if (!placable(nx, ny)) {
			return;
		}
	}

	move_tail(0, nx, ny);
}

void move_random(int p, int dir) {
	int nx, ny;
	int p2;
	do {
		nx = px[p] + randint(-1, 1);
		ny = py[p] + randint(-1, 1);
		if (player[p].stamina > 0) {
			if (back_buf[nx][ny] == '1') {
				p2 = 1;
				if (p != p2) {
					chocie(p, p2);
				}
			}
			else if (back_buf[nx][ny] == '2') {
				p2 = 2;
				if (p != p2) {
					chocie(p, p2);
				}
			}
			else if (back_buf[nx][ny] == '3') {
				p2 = 3;
				if (p != p2) {
					chocie(p, p2);
				}
			}
			else if (back_buf[nx][ny] == '4') {
				p2 = 4;
				if (p != p2) {
					chocie(p, p2);
				}
			}
			else if (back_buf[nx][ny] == '5') {
				p2 = 5;
				if (p != p2) {
					chocie(p, p2);
				}
			}
			else if (back_buf[nx][ny] == '6') {
				p2 = 6;
				if (p != p2) {
					chocie(p, p2);
				}
			}
			else if (back_buf[nx][ny] == '7') {
				p2 = 7;
				if (p != p2) {
					chocie(p, p2);
				}
			}
			else if (back_buf[nx][ny] == '8') {
				p2 = 8;
				if (p != p2) {
					chocie(p, p2);
				}
			}
			else if (back_buf[nx][ny] == '9') {
				p2 = 9;
				if (p != p2) {
					chocie(p, p2);
				}
			}
		}
		if (back_buf[nx][ny] == 'I') {
			if (player[p].hasitem == false) {
				player[p].hasitem = true;
				strncpy(player[p].item.name, item[item_buf[nx][ny]].name, 100);
				player[p].item.intel_buf = item[item_buf[nx][ny]].intel_buf;
				player[p].item.str_buf = item[item_buf[nx][ny]].str_buf;
				player[p].item.stamina_buf = item[item_buf[nx][ny]].stamina_buf;
				player[p].n = item_buf[nx][ny];
				item_buf[nx][ny] = NULL;
				move_tail(p, nx, ny);
			}
			else if (player[p].hasitem = true) {
				hasitem_choice(p, nx, ny);
				if (!placable(nx, ny)) {
					return;
				}
			}
		}
	} while (!placable(nx, ny));
	move_tail(p, nx, ny);
}


void move_tail(int player, int nx, int ny) {
	int p = player;
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void nightgame() {
	srand((unsigned int)time(NULL));
	init_nightgame();

	system("cls");
	display();
	while (1) {
		// player 0만 손으로 움직임(4방향)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual(key);
		}
		for (int p = 0; p < n_player; p++) {
			if (player[p].is_alive) {
				if (player[p].stamina < 0) {
					player[p].stamina = 0;
				}
			}
		}
		// player 1 부터는 랜덤으로 움직임(8방향)
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				move_random(i, -1);
			}
		}

		display();
		Sleep(10);
		tick += 10;
	}
}