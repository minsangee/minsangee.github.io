#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1; // -1 리턴하면 메인함수에서 처리하고 종료
	}
	// 플레이어 데이터 load
	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		// 아직 안 배운 문법(구조체 포인터, 간접참조연산자)
		PLAYER* p = &player[i];
		// 파일에서 각 스탯 최댓값 읽기
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel), &(p->str));
		p->id = i;
		p->stamina = 100; // 100%
		// 현재 상태
		p->is_alive = true;
		p->hasitem = false;
	}
	// 아이템 데이터 load
	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d",
			item[i].name, (unsigned int)sizeof(item[i].name),
			&(item[i].intel_buf),
			&(item[i].str_buf),
			&(item[i].stamina_buf));
	}
	fclose(fp);
	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		player[i].is_alive = true;
	}
	return 0;
}

int intro() {
	printf("  .／  ⌒   ＼ \n");
	printf("  ／　　  　＼		쭈 \n");
	printf("  /         |＼		꾸 \n");
	printf(" (/         | )		미 \n");
	printf(" /          |ㅣ		게 \n");
	printf(" f　　　　   ㅣ		임 \n");
	printf(" | ●　　●    ｜ \n");
	printf(" |    ▽   　 ｜ \n");
	printf(" ㄴ＿ ＿  　ノ \n");
	printf(" J丁丁丁丁￣l＼   \n");
	printf("く(_(_(＿L＿)ノ  \n");


	Sleep(500);
}

int ending() {
	printf("         *the end*\n");
	printf("         *the end*\n");
	printf("         *the end*\n");
	printf("         *the end*\n");
	printf("	    ㅅ_ㅅ  \n");
	printf("	    ('ω')\n");
	printf("	／ Ｕ ∽ Ｕ＼\n");
	printf("	│＊　게　＊│\n");
	printf("	│＊　임　＊│\n");
	printf("	│＊　종　＊│\n");
	printf("	│＊　료　＊│\n");
	printf("	│＊　☆　＊ │\n");
	printf("	￣￣￣￣￣￣ \n");
	if (n_alive == 1) {
		// 우승자 출력
		printf(" 우승자: ");
		for (int i = 0; i < n_player; i++) {
			if (player[i].is_alive ==true) {
				printf("플레이어 %d", i);
				break;
			}
		}
	}
}

int main(void) {
	jjuggumi_init();
	intro();
	mugunghwa();
	heal_stamina();
	nightgame();
	heal_stamina();
	juldarigi();
	heal_stamina();
	jebi();
	ending();
	return 0;
}
