// ��Ʈ�� Ž��, ����, ���� ���α׷�
// �Է�: 2 ���� ȸ��� ���� - Com_names1.txt,  Com_names2.txt
//       (����: ȸ����� �߰��� space ���ڸ� ������ �� �� ����.)
// ���� �� ���� ���� ��� ȸ����� �� ���ڵ�� �Ͽ� �ְ�,
//  �� ����  ��ɹ� ���� ������ ������.

#define	_CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXK 2				// MAXK��  2d   ��
#define HALFK MAXK/2        // d (capacity order) �̴�.
#define MAX 100				// ���� �ִ� ���Ҽ�.
#define false 0
#define true 1

typedef struct element {		// ���ڵ� ����.  ȸ���� �� ���̷� ������
	char name[100];				// ȸ���
	int nleng;					// ȸ��� ����
}ele;

typedef struct node* nodeptr;	// *nodeptr�� node�� ���¸� ���� �������̴�. 
typedef struct node {           // �Ϲ� ��� 
	int fill_cnt;
	ele rec[MAXK];
	nodeptr ptr[MAXK + 1];
}node;

typedef struct big_node {       // �Ϲݳ�庸�� ���ڵ�� �����Ͱ� �ϳ��� �� ū ���� [�պ��� ���] 
	nodeptr ptr[MAXK + 2];
	ele rec[MAXK + 1];
}big_node;

typedef struct two_Bn {
	nodeptr ptr[2 * MAXK + 1];
	ele rec[2 * MAXK];
} two_Bnode;

nodeptr root = NULL;		// root�� ���� ������ ����Ѵ�. 
//nodeptr temp;
nodeptr stack[MAX];			//stack�� max���� 100
int top = -1;

void push(nodeptr node);
nodeptr pop();

void insert_btree();					// file ��ü�� ���� �� �Լ��� insert_arec�� ȣ���Ѵ�. 
int insert_arec(ele in_rec);			// ���ڵ带 �ϳ� �����Ѵ�. 
nodeptr retrieve(char*, int*);			// Ű���� �־� �˻��Ѵ�. [���̿� �Բ� ���] 
int seq_scan_btree(nodeptr curr);		// ��ü ���ڵ���� ����Ѵ�. 
int B_tree_deletion(char*);				// Function to delete a record with a given key.

int total_height = 0;
// ��ü ���� 
int main(void) {

	char input, line[200], * res_gets;
	ele a_insert;
	char name_s[100];
	int num, lleng;
	int i, k, r, find;
	nodeptr tp;

	insert_btree();

	// ��ɹ� ���� ����.
	while (1) {
		fflush(stdin);	// buffer clear 
		printf("\n����� �����ÿ�\n");
		printf("insert: i �̸�  / delete : d �̸�  /  retrieve : r �̸�  /  ��ü���: p   /  ����: e  >> ");
		res_gets = gets(line);
		if (!res_gets)
			break;		// no input.

		lleng = strlen(line);
		if (lleng <= 0)
			continue;	// empty line input. try again.

		i = 1;
		if (line[0] == 'E' || line[0] == 'e') {		// exit program
			printf("�������� ������!\n\n");   return 0;
		}

		else if (line[0] == 'I' || line[0] == 'i') { // ���ڵ� �Ѱ� ���� ���.

			// ���� ȸ����� ������.
			k = 0;
			while (line[i] == ' ')	i++;
			while (i < lleng) {
				name_s[k] = line[i]; k++; i++;
			}
			name_s[k] = '\0';

			if (strlen(name_s) == 0)
				continue;

			strcpy(a_insert.name, name_s);
			a_insert.nleng = strlen(name_s);

			top = -1;
			r = insert_arec(a_insert);	// ���ڵ� �ϳ��� �����ϴ� �Լ��� ȣ�� 
			if (r == 0)
				printf("���� ����.\n", r);
			else
				printf("���� ����.\n", r);

		}
		/*
		else if (line[0] == 'D' || line[0] == 'd') {  // ���� ��� ����
			k = 0;
			while (line[i] == ' ')	i++;
			while (i < lleng) {
				name_s[k] = line[i]; k++; i++;
			}
			name_s[k] = '\0';

			if (strlen(name_s) == 0)
				continue;

			r = B_tree_deletion(name_s);
			if (r == 0)
				printf("������û ���� Ű�� ���ڵ尡 �������� ����.\n", r);
			else
				printf("���� ����.\n");

		}*/
		else if (line[0] == 'R' || line[0] == 'r') {	// Ž�� ��� ����
			k = 0;
			while (line[i] == ' ')	i++;
			while (i < lleng) {
				name_s[k] = line[i]; k++; i++;
			}
			name_s[k] = '\0';

			if (strlen(name_s) == 0)
				continue;
			tp = retrieve(name_s, &i);
			if (tp)
				printf("Ž������.  Name: %s,  ����: %d\n", tp->rec[i].name, tp->rec[i].nleng);
			else
				printf("Ž����û ���� Ű�� ���� ���ڵ尡 �������� ����.\n");

		}
		else if (line[0] == 'P' || line[0] == 'p') // ����Ʈ ��� ����. ��� ���ڵ带 Ű ������ ���.
			seq_scan_btree(root);
		else
			printf("input error \n");
	}	//while
}	//main

/**************** stack ���� �Լ��� ********************/
void push(nodeptr node) {
	if (top > MAX) /// >>>  MAX-1 �̻�( >= ) �̾�� ���� �ʳ�?
	{
		printf("stack is full\n");
		return;
	}
	top++;
	stack[top] = node;

}

nodeptr pop() {
	int temp = 0;
	if (top < 0)
	{
		printf("stack is empty.\n");
		return 0;
	}

	temp = top; ///>>> ���� top �� ����Ű�� ���� ���� �ֱ��� ����Ÿ�� ����Ǿ� �ִ�. ����,
	top--;
	return stack[temp];
}


int insert_arec(ele in_rec) {	//�ϳ��� ���ڵ带 ����  key = ȸ���  

	int i, j;
	nodeptr curr, child, new_ptr, tptr = NULL;
	ele empty = { "\0",0 };
	big_node bnode;

	if (!root) {	// root�� NULL�̸� btree�� �������. �� ù ��带 ����� ���⿡ �ִ´�.
		root = (nodeptr)malloc(sizeof(node)); 		/* Fill your code */	// nodeptr���·� nodeũ�⸸ŭ �Ҵ�޾� �����ּҴ� root�� ����   
		root->rec[0] = in_rec;					// key���� root->rec[0]�� �ִ´�. 
		root->ptr[0] = root->ptr[1] = NULL;		// p0�� p1�� NULL�� �ִ´�. 
		root->fill_cnt = 1;
		return 1;  // ù ��带 ����� �ְ� ������. 
	}

	//root is not null
	curr = root;

	//   �Ʒ� �� ���� in_rec �� ���� ���� ������带 ã�� curr�� ����Ű�� �ϴ� �κ��� �;� ��!! check
	/*
	Fill
	your
	code
	*/


	int index = 0;
	while (curr->ptr[0] != NULL || curr->ptr[1] != NULL) {


		if (strcmp(curr->rec[index].name, in_rec.name) == 0) {
			return 0;
		}
		//curr < input
		else if (strcmp(curr->rec[index].name, in_rec.name) < 0) {
			if (curr->fill_cnt == 2 && strcmp(curr->rec[index + 1].name, in_rec.name) < 0) {
				push(curr);
				curr = curr->ptr[index + 2];
				index = 0;
			}
			else {
				push(curr);
				curr = curr->ptr[index + 1];
				index = 0;
			}
		}
		else {//curr > input
			push(curr);
			curr = curr->ptr[index];
			index = 0;
		}


	}
	for (int index = 0; index < 2; index++) {
		if (strcmp(curr->rec[index].name, in_rec.name) == 0) {
			return 0;
		}
	}
	do {

		// curr node is not full
		if (curr->fill_cnt < MAXK) {
			for (i = 0; i < curr->fill_cnt; i++)
				if (strcmp(in_rec.name, curr->rec[i].name)/* Fill your code */ < 0)
					break;
			for (j = curr->fill_cnt; j > i; j--) {
				curr->ptr[j + 1] =curr->ptr[j] /* Fill your code */;
				curr->rec[j] = curr->rec[j-1]/* Fill your code */;
			}
			//i=0
			curr->rec[i] = in_rec/* Fill your code */;
			curr->ptr[i + 1] = child/* Fill your code */;
			curr->fill_cnt++;

			return 1; // ���Լ��� (���� 1: ��Ʈ�� �߰� ���� ������).
		}
		else {
			// curr node is full
			for (i = 0; i < MAXK; i++) {
				if (strcmp(in_rec.name, curr->rec[i].name/* Fill your code */) < 0)
					break;
			}
			if (i == 0) {
				bnode.ptr[0]=
			}
			bnode.ptr[0] = /* Fill your code */;
			for (j = 0; j < i; j++) {
				bnode.rec[j] = curr->rec[j]/* Fill your code */;
				bnode.ptr[j + 1] = curr->ptr[j+1]/* Fill your code */;
			}

			bnode.rec[j] =in_rec /* Fill your code */;
			bnode.ptr[j + 1] = /* Fill your code */;
			j++;

			while (i < MAXK) {
				bnode.rec[j] = /* Fill your code */;
				bnode.ptr[j + 1] = /* Fill your code */;

				j++;
				i++;
			}

			//   �Ʒ� �� ���� big node �� 3 �κ����� ������ ���ݺδ� curr ��, ��� ���ڵ�� in_rec��, 
			//   �Ĺݺδ� �� ��忡 �ְ�, child�� �� �� ��带 ����Ű�� �ϴ� �κ��� �;� ��!!
			/*
			Fill
			your
			code
			*/

			if (top >= 0) {		// ������ emtpy �� �ƴ� ��� 
				curr = pop();	// curr �� �θ�� curr�� ������.
			}
			else { // ������ empty �� (�� curr �� root �����.) �� root ��带 ����� curr �� �θ�� ��.  
				tptr = /* Fill your code */;
				tptr->rec[0] = /* Fill your code */;
				tptr->ptr[0] = /* Fill your code */;
				tptr->ptr[1] = /* Fill your code */;
				tptr->fill_cnt = /* Fill your code */;
				root = /* Fill your code */;
				total_height++;
				return 2; // ���� ���� (���� 2: �� ��Ʈ�� ����)
			} // else.
		} // else.
	} while (1);

	return 0; // �� ������ ������ ���� ����.
} //�Լ� insert_arec
