#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
struct pcb {  // ���̿��ƿ� 
	int id;
	int status;  // ����״̬ 
	int firstaddr;
	int length;
	int outbufword;
}*PCB[3];
FILE* f;
struct req {  //  ������ƿ� 
	int reqname;
	int length;
	int addr;
}reqblock[10];
int buffer[2][100];  // �����
int l1 = 1, l2 = 1;
int head = 0, tail = 0;
int t1, t2;
void request(int i) {  // ���������Ϣ�� 
	int j, length = 0, m;
	struct req* run;
	if (i == 1) {
		t1--;
	}
	else {
		t2--;
	}
	cout << "�û�" << i << "�������ݣ�\n";
	run = &reqblock[tail % 10];
	run->reqname = i;
	run->length = 0;
	if (tail == 0) {
		run->addr = 0;
	}
	else {
		int index = (tail - 1 % 10);
		run->addr = reqblock[index].addr + reqblock[index].length;
	}
	for (m = 0; m < 100; m++) {
		if (buffer[i - 1][m] == 0) {
			run->addr = m;
			break;
		}
	}
	int s = 0;
	while (1) {
		j = rand() % 10;
		if (j == 0) {
			run->length = length;
			break;
		}
		buffer[i - 1][(run->addr + length)] = s;  // �����������
		cout << s << " ";
		s++;
		length++;
	}
	cout << endl;
	PCB[i - 1]->length += length;
	length = 0;
	if (PCB[2]->status == 2) {
		PCB[2]->status = 0;
	}
	tail++;
}

void spooling() {
	int i, j;
	struct req* run;
	printf("����SPOOLING����������������ݣ�\n");
	fprintf(f, "����SPOOLING����������������ݣ�\n");
	run = &reqblock[head % 10];
	printf("�û�%d\n", run->reqname);
	fprintf(f, "�û�%d\n", run->reqname);
	for (i = 0; i < run->length; i++) {
		printf("%d  ", buffer[run->reqname - 1][run->addr + i]);
		fprintf(f, "%d  ", buffer[run->reqname - 1][run->addr + i]);
	}
	printf("\n");
	fprintf(f, "\n");
	head++;
	for (j = 0; j < 2; j++) {
		if (PCB[j]->status == 1) {
			PCB[j]->status = 0;
		}
	}
}

int main() {
	int i, n;
	f = fopen("result.txt", "w");  // Ϊ"�ļ�ָ����"���Ǳ�˵��ΪFILE���͵�ָ�����
	for (i = 0; i < 2; i++) {
		for (n = 0; n < 100; n++) {
			buffer[i][n] = 0;   // ��ʼ����ά����buffer�����뾮����� 
		}
	}
	for (i = 0; i < 3; i++) {  // ��ʼ�����̿��ƿ飬Ϊ�ṹ�忪�ٿռ� 
		struct pcb* tmPcb = (struct pcb*)malloc(sizeof(struct pcb));
		tmPcb->id = i;
		tmPcb->status = 0;
		tmPcb->firstaddr = 0;
		tmPcb->length = 0;
		tmPcb->outbufword = 1;
		PCB[i] = tmPcb;
	}
	printf("How much work do p1 want to do?");
	fprintf(f, "How much work do p1 want to do?");
	scanf("%d", &t1);
	fprintf(f, "%d\n", t1);
	printf("How much work do p2 want to do?");
	fprintf(f, "How much work do p2 want to do?");
	scanf("%d", &t2);
	fprintf(f, "%d\n", t2);
	srand((unsigned)time(NULL));  // �������������ϵͳʱ�������� 
	while (1) {
		i = rand() % 100;
		if (i <= 45) {
			if ((PCB[0]->status == 0) && (t1 > 0)) {
				request(1);
			}
		}
		else if ((i < 90) && (t2 > 0)) {
			if (PCB[1]->status == 0) {
				request(2);
			}
		}
		else {
			spooling();
		}
		if ((t1 == 0) && (t2 == 0) && (head == tail)) {
			break;
		}
	}
	for (i = 0; i < 3; i++) {
		free(PCB[i]);
		PCB[i] = NULL;
	}
	fclose(f);
	return 0;
}
