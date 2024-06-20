#ifndef _JJUGGUMI_H_
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX		11
#define ITEM_MAX		11

typedef struct {
	char name[100];
	int intel_buf, str_buf, stamina_buf;
} ITEM;

typedef struct {
	int id;
	char name[100];
	// �ɷ�ġ: ����, ��, ���¹̳�
	int intel, str, stamina;
	// ���� ����
	int n;
	bool is_alive; // Ż�������� false
	bool hasitem; // �������� ������ true
	ITEM item; // ������ 1�� ���� ����
} PLAYER;


PLAYER player[PLAYER_MAX];
ITEM item[ITEM_MAX];

int n_player, n_alive, n_item;
int tick;  // �ð�

// �̴ϰ���

void mugunghwa(void);
void nightgame(void);
void juldarigi(void);
void jebi(void);

int randint(int low, int high);

#endif