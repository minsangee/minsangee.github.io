#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1; // -1 �����ϸ� �����Լ����� ó���ϰ� ����
	}
	// �÷��̾� ������ load
	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		// ���� �� ��� ����(����ü ������, ��������������)
		PLAYER* p = &player[i];
		// ���Ͽ��� �� ���� �ִ� �б�
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel), &(p->str));
		p->id = i;
		p->stamina = 100; // 100%
		// ���� ����
		p->is_alive = true;
		p->hasitem = false;
	}
	// ������ ������ load
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
	printf("  .��  ��   �� \n");
	printf("  ������  ����		�� \n");
	printf("  /         |��		�� \n");
	printf(" (/         | )		�� \n");
	printf(" /          |��		�� \n");
	printf(" f��������   ��		�� \n");
	printf(" | �ܡ�����    �� \n");
	printf(" |    ��   �� �� \n");
	printf(" ���� ��  ���� \n");
	printf(" J�������ˣ�l��   \n");
	printf("��(_(_(��L��)��  \n");


	Sleep(500);
}

int ending() {
	printf("         *the end*\n");
	printf("         *the end*\n");
	printf("         *the end*\n");
	printf("         *the end*\n");
	printf("	    ��_��  \n");
	printf("	    ('��')\n");
	printf("	�� �� �� �ա�\n");
	printf("	�������ԡ�����\n");
	printf("	�������ӡ�����\n");
	printf("	��������������\n");
	printf("	�������ᡡ����\n");
	printf("	�������١��� ��\n");
	printf("	������������ \n");
	if (n_alive == 1) {
		// ����� ���
		printf(" �����: ");
		for (int i = 0; i < n_player; i++) {
			if (player[i].is_alive == true) {
				printf("�÷��̾� %d", i);
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