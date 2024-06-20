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
	// 능력치: 지능, 힘, 스태미나
	int intel, str, stamina;
	// 현재 상태
	int n;
	bool is_alive; // 탈락했으면 false
	bool hasitem; // 아이템이 있으면 true
	ITEM item; // 아이템 1개 장착 가능
} PLAYER;


PLAYER player[PLAYER_MAX];
ITEM item[ITEM_MAX];

int n_player, n_alive, n_item;
int tick;  // 시계

// 미니게임

void mugunghwa(void);
void nightgame(void);
void juldarigi(void);
void jebi(void);

int randint(int low, int high);

#endif