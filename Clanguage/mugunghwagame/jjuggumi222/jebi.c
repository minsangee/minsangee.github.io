#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>



int surPlayers = 0;
int round = 1;
void question(void);
void randq(void);
int cursorRow = 2;   // �ʱ� Ŀ�� �� ��ġ
int cursorCol = 1;   // �ʱ� Ŀ�� �� ��ġ
bool start = true;
char log[50];
void jebi() {

	srand((unsigned int)time(NULL));
	system("cls");
	if (5 < n_player) {
		map_init(5, 18 + n_player);
	}
	else {
		map_init(5, 18);
	}
	// map ��� �Լ� ����


	for (int i = 0; i < n_player; i++) {   //���� �÷��̾� �� ����
		if (player[i].is_alive == true)
			surPlayers++;
	}
	int start = true;

	while (start) {
		if (5 < n_player) {
			map_init(5, 18 + n_player);
		}
		else {
			map_init(5, 18);
		}
		gotoxy(6, 0);
		printf("Round %d,", round);  //���� ī��Ʈ
		round++;
		gotoxy(6, 10);

		int turnPlayer = -1;

		for (int i = 0; i < n_player; i++) {
			if (player[i].is_alive == true) {
				turnPlayer = i;

				break;
			}
		}
		if (turnPlayer != -1) {
			printf("turn: player %d\n", turnPlayer);   //�÷��̾� ����ǥ��
		}

		display();
		question();
		randq();


		int continueGame = 1;
		while (continueGame) {
			draw();

			gotoxy(cursorRow, cursorCol);
			key_t key = get_key();
			if (key != K_UNDEFINED)
			{
				if (key == K_QUIT) {
					continueGame = false;
					start = false;
					if (surPlayers >= 2) {
						system("cls");
						printf("����ڸ� ������ ���߽��ϴ�\n");
						printf("--��Ƴ��� �÷��̾�--\n");
						for (int i = 0; i < n_player; i++) {
							if (player[i].is_alive == true) {
								turnPlayer = i;
								printf("%d player\n", i);
							}
						}
					}
				}
				switch (key) {
				case 75:
					if (cursorCol > 1) {

						if (back_buf[cursorRow][cursorCol - 2] != ' ') {
							gotoxy(cursorRow, cursorCol); {

								printf("?");
							}
							cursorCol -= 2;
							gotoxy(cursorRow, cursorCol); {
								printf("@");
							}
						}


					}
					break;
				case 77:
					if (cursorCol < 2 * surPlayers - 2) {

						if (back_buf[cursorRow][cursorCol + 2] != ' ') {
							gotoxy(cursorRow, cursorCol); {

								printf("?");
							}
							cursorCol += 2;
							gotoxy(cursorRow, cursorCol); {
								printf("@");
							}
						}


					}
					break;
				case 32:
					gotoxy(cursorRow, cursorCol); {

						printf("?");
					}
					if (back_buf[cursorRow][cursorCol] == false) {

						snprintf(log, sizeof(log), "player %d fail", turnPlayer);

						dialog(log);
						player[turnPlayer].is_alive = false;
						surPlayers--;
						n_alive--;
						cursorRow = 2;
						cursorCol = 1;
						system("cls");
						continueGame = 0;

					}
					else {
						snprintf(log, sizeof(log), "player %d pass", turnPlayer);

						dialog(log);
						back_buf[cursorRow][cursorCol] = ' ';//?�� �������� ����
						// ���� ��ġ �����ʿ� �ִ� ���ڵ��� �������� �̵� (����)
						cursorCol += 2;

						while (cursorCol < 20 && (back_buf[cursorRow][cursorCol] == false || back_buf[cursorRow][cursorCol] == '?')) {
							back_buf[cursorRow][cursorCol - 2] = back_buf[cursorRow][cursorCol];
							back_buf[cursorRow][cursorCol] = ' ';
							cursorCol += 2;
						}
						cursorRow = 2;
						cursorCol = 1;


						do {
							turnPlayer = (turnPlayer + 1) % n_player;
						} while (!player[turnPlayer].is_alive);



						gotoxy(6, 10);

						if (turnPlayer != -1) {
							printf("turn: player %d\n", turnPlayer);
						}



					}
				}





			}

		}

		for (int i = 0; i < n_player; i++) {
			if (player[i].is_alive == true) {
				player[i].is_alive = true;
			}
		}
		if (n_alive == 1) {
			n_player = 1;
			start = false;
		}
	}


}




void question(void) {     //�÷��̾� �� ��� ? �����
	for (int i = 1; i < N_ROW + 5; i++) {
		for (int j = 0; j < N_COL + 18 + n_player; j++) {
			if (i == 2 && j % 2 == 1) {
				if (j / 2 < surPlayers) {
					back_buf[i][j] = '?';

				}
			}
		}
	}

}


void randq(void) {
	// ���� '?'�� ���� ����
	int totalQuestions = 0;
	for (int i = 1; i < N_ROW + 5; i++) {
		for (int j = 0; j < N_COL + 18 + n_player; j++) {
			if (i == 2 && j % 2 == 1) {
				if (j / 2 < surPlayers && back_buf[i][j] == '?') {
					totalQuestions++;
				}
			}
		}
	}

	if (totalQuestions > 0) {
		// �������� ���õ� '?' ã��
		int randomIndex = rand() % totalQuestions;
		int currentCount = 0;

		for (int i = 1; i < N_ROW + 5; i++) {
			for (int j = 0; j < N_COL + 18 + n_player; j++) {
				if (i == 2 && j % 2 == 1) {
					if (j / 2 < surPlayers) {
						if (back_buf[i][j] == '?') {
							if (currentCount == randomIndex) {


								back_buf[i][j] = false;

								// ����� ���� ��� (false�� ��쿡�� '?'�� ���)
								if (back_buf[i][j] == false) {
									// ȭ�鿡 '?' ���
									gotoxy(i, j);
									printf("?");
								}

								return;
							}
							currentCount++;
						}
					}
				}
			}
		}
	}
}