#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>



typedef struct _point
{
	int x, y;
}point;
typedef enum _direction
{
	IDLE = 1, LEFT, UP, DOWN, RIGHT
}direction;
typedef enum _state {
	alive, dead, finished
}state;		// �� �÷��̾���� ���¸� ��Ÿ���� �ڷ���
typedef struct _tickState
{
	double goalTick;	// Ʈ����ƽ
	double cntTick;		// ���� ƽ
}tickState;		// �̺�Ʈ�� �߻���Ű�� �ֱ⸦ �����ϱ� ���� �ڷ���

bool moveOn(point*, direction);	// �÷��̾� �̵�
double getTick();		// Tick���
int SayFlower();		// ����ȭ�����Ǿ����ϴ�.  ���
void checkFinished();	// �÷��̾ ���������� �����ϸ� finised ó��
void killPlayer();		// ���� �ڵ��ƺ��� �� �÷��̾ �����̸� dead ó��

bool running = true;	// ���� �ݺ��� ���� ����
point* pl;				// �÷��̾� ��ġ��
tickState* plTicks;		// �÷��̾� �̵� �ֱ�
state* states;			// �÷��̾� ���� (alive, dead, finished)
state* fstates;			// ���� üũ
int taggerY;			// ���� ���� ��ġ
char str[] = { "0�� ���" };

void mugunghwa() {
	srand((unsigned int)time(NULL));
	////////////////////  ���� �ʱ�ȭ  ////////////////////////////////////////////////////////////////////////////////
	system("cls");
	if (5 < n_player) {
		map_init(4 + n_player, 40);
	}
	else {
		map_init(9, 40);
	}

	// ���� ����
	taggerY = N_ROW / 2;
	back_buf[taggerY - 1][1] = '#';
	back_buf[taggerY][1] = '#';
	back_buf[taggerY + 1][1] = '#';
	tickState taggerSaying = { 100,0 };	// ���� ���ϰ� �ִ� �ð� ����
	tickState taggerWatching = { 3000,0 }; // ���� �ٶ󺸰� �ִ� �ð� ����
	bool isWatching = false;		// ����� ó���� ���� ���� �ִ�

	// �÷��̾� ��ġ �� �̵��ֱ� �� ���� ����
	pl = (point*)malloc(sizeof(point) * n_player);
	plTicks = (tickState*)malloc(sizeof(tickState) * n_player);
	states = (state*)malloc(sizeof(state) * n_player);
	fstates = (state*)malloc(sizeof(state) * n_player);
	for (int i = 0; i < n_player; i++)
	{
		// �÷��̾�� ��ġ ����
		pl[i].x = 38;
		pl[i].y = 2 + i;
		back_buf[pl[i].y][pl[i].x] = '0' + i;

		// �÷��̾�� �̵� �ֱ� ����
		if (i == 0)
		{
			plTicks[0].goalTick = 0;
			plTicks[0].cntTick = 0;
		}
		else
		{
			plTicks[i].goalTick = randint(100, 400); //ai ������ �ӵ�
			plTicks[i].cntTick = 0;
		}

		// �÷��̾� ���� ����
		states[i] = alive;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ���� ���� ������
	while (running) {
		display();
		//����� �Է� ó��
		{
			key_t key = get_key();
			if (key != K_UNDEFINED)
			{
				if (key == K_QUIT)
					running = false;
				else if (states[0] == alive && plTicks[0].goalTick <= plTicks[0].cntTick)
					switch (key)
					{
					case K_LEFT:
						moveOn(pl + 0, LEFT);
						break;
					case K_UP:
						moveOn(pl + 0, UP);
						break;
					case K_DOWN:
						moveOn(pl + 0, DOWN);
						break;
					case K_RIGHT:
						moveOn(pl + 0, RIGHT);
						break;
					}
			}

		}
		// AI �̵� ó��
		for (int i = 1; i < n_player; i++)
		{
			// ĳ���Ͱ� ����ִ� ���°�, ���� ƽ�� �̺�Ʈ �߻� ƽ�� �Ѿ��...
			if (states[i] == alive && plTicks[i].goalTick <= plTicks[i].cntTick)
			{
				plTicks[i].cntTick = 0;
				if (isWatching == false || (rand() % 10 == 0))	// ���� �ٶ󺸸� 10�� 1Ȯ���� �����δ�. ���� 10%
				{
					int random = rand() % 10;
					if (random == 0)
						moveOn(pl + i, IDLE);
					else if (random == 1)
						moveOn(pl + i, UP);
					else if (random == 2)
						moveOn(pl + i, DOWN);
					else
					{
						moveOn(pl + i, LEFT);
					}
				}
			}


		}


		// ���� ���ϱ�&�ڵ��ƺ��� ����
		if (isWatching)
		{
			if (taggerWatching.goalTick <= taggerWatching.cntTick)
			{
				isWatching = false;
				taggerWatching.cntTick = 0;
				back_buf[N_ROW / 2 - 1][1] = '#';
				back_buf[N_ROW / 2][1] = '#';
				back_buf[N_ROW / 2 + 1][1] = '#';
			}
			else
			{
				back_buf[N_ROW / 2 - 1][1] = '@';
				back_buf[N_ROW / 2][1] = '@';
				back_buf[N_ROW / 2 + 1][1] = '@';
				killPlayer();
			}
		}
		else
		{
			if (taggerSaying.goalTick <= taggerSaying.cntTick)
			{
				int cnt = SayFlower();
				if (cnt < 10) {
					if (cnt == 0)
						taggerSaying.goalTick = 1000;
					taggerSaying.goalTick += rand() % 200;
				}
				else
				{
					if (cnt == 10)
						taggerSaying.goalTick = 1000;
					taggerSaying.goalTick -= rand() % 300;
				}
				taggerSaying.cntTick = 0;
				if (20 <= cnt)
					isWatching = true;
			}

		}
		for (int i = 0; i < n_player; i++) {
			if (fstates[i] != states[i] && states[i] == dead) {
				str[0] = i + '0';
				dialog(str);
				fstates[i] = states[i];
			}
		}

		// �÷��̾� ���� ���� Ȯ��
		checkFinished();

		// �� �÷��̾���� tick ����
		double tick = getTick();
		for (int i = 0; i < n_player; i++) plTicks[i].cntTick += tick;
		taggerSaying.cntTick += tick;
		if (isWatching) taggerWatching.cntTick += tick;



		// ���� ���� ���� ���� Ȯ��
		int finishedN = 0;
		for (int i = 0; i < n_player; i++)
			if (states[i] == finished)
				finishedN++;
		if (finishedN == n_alive)	// Ŭ������ ��� ���� ����ִ� �÷��̾� ���� ������ ���� ����
			running = false;
		else if (n_alive <= 1 && finishedN == 0) // �ƹ��� Ŭ�������� ���߰�, �÷��̾� ���� 1�� ���ϸ� ���� ����
			running = false;
		else if (finishedN == n_player)	// �� �������� ���, ���� ����
			running = false;


	}
	display();
	gotoxy(ROW_MAX + 3, 0);
}




bool moveOn(point* pt, direction dir)
{
	switch (dir)
	{
	case LEFT:
		if (back_buf[pt->y][pt->x - 1] != ' ')
			return false;
		back_buf[pt->y][pt->x - 1] = back_buf[pt->y][pt->x];
		back_buf[pt->y][pt->x] = ' ';
		pt->x -= 1;
		break;

	case UP:
		if (back_buf[pt->y - 1][pt->x] != ' ')
			return false;
		back_buf[pt->y - 1][pt->x] = back_buf[pt->y][pt->x];
		back_buf[pt->y][pt->x] = ' ';
		pt->y -= 1;
		break;

	case DOWN:
		if (back_buf[pt->y + 1][pt->x] != ' ')
			return false;
		back_buf[pt->y + 1][pt->x] = back_buf[pt->y][pt->x];
		back_buf[pt->y][pt->x] = ' ';
		pt->y += 1;
		break;

	case RIGHT:
		if (back_buf[pt->y][pt->x + 1] != ' ')
			return false;
		back_buf[pt->y][pt->x + 1] = back_buf[pt->y][pt->x];
		back_buf[pt->y][pt->x] = ' ';
		pt->x += 1;
		break;
	}
	return true;
}


double getTick()
{
	static double beforeClock = 0;
	double temp = beforeClock;
	beforeClock = clock();
	return clock() - temp;

}

int SayFlower()
{
	static int cnt = 0;
	const char* sentence = "����ȭ�����Ǿ����ϴ�";
	if (cnt == 0)	// ����ȭ�����Ǿ����ϴ� ����� �� �����
	{
		gotoxy(N_ROW + 1, 0);
		printf("                                                               ");
	}
	while (cnt < strlen(sentence))
	{
		gotoxy(N_ROW + 1, cnt);
		printf("%c%c", sentence[cnt], sentence[cnt + 1]);
		cnt += 2;
		return cnt;
	}
	cnt = 0;
	return cnt;
}

void checkFinished()
{
	for (int i = 0; i < n_player; i++)
		if (states[i] == alive && ((pl[i].x == 1) || (pl[i].x == 2 && taggerY - 1 <= pl[i].y && pl[i].y <= taggerY + 1)))
		{
			back_buf[pl[i].y][pl[i].x] = ' ';
			states[i] = finished;
		}
}
void killPlayer()
{
	for (int i = 0; i < N_ROW; i++)
		for (int j = 0; j < N_COL; j++)
			if (front_buf[i][j] != back_buf[i][j] && back_buf[i][j] != '#' && back_buf[i][j] != '@')
			{
				if ('0' <= back_buf[i][j] && back_buf[i][j] <= '9')
				{
					// ��ġ�� �ٲ� �÷��̾�� dead���·� ��ȯ
					states[back_buf[i][j] - '0'] = dead;

					for (int k = j - 1; 1 < k; k--)
					{
						// ��, �տ� ����� ������ alive���·� �ٽ� ��ȯ
						if ('0' <= back_buf[i][k] && back_buf[i][k] <= '9')
							states[back_buf[i][j] - '0'] = alive;
					}
					if (states[back_buf[i][j] - '0'] == dead)
					{
						player[back_buf[i][j] - '0'].is_alive = false;
						back_buf[i][j] = ' ';
						n_alive--;
					}
				}
			}
}