// ��Ʈ�� Ž��, ����, ���� ���α׷�
// �Է�: 2 ���� ȸ��� ���� - Com_names1.txt,  Com_names2.txt
//       (����: ȸ����� �߰��� space ���ڸ� ������ �� �� ����.)
// ���� �� ���� ���� ��� ȸ����� �� ���ڵ�� �Ͽ� �ְ�,
//  �� ����  ��ɹ� ���� ������ ������.

#define   _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXK 2            // MAXK��  2d   ��
#define HALFK MAXK/2        // d (capacity order) �̴�.
#define MAX 100            // ���� �ִ� ���Ҽ�.
#define false 0
#define true 1

typedef struct element {      // ���ڵ� ����.  ȸ���� �� ���̷� ������
	char name[100];            // ȸ���
	int nleng;               // ȸ��� ����
}ele;

typedef struct node* nodeptr;   // *nodeptr�� node�� ���¸� ���� �������̴�. 
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

nodeptr root = NULL;      // root�� ���� ������ ����Ѵ�. 
//nodeptr temp;
nodeptr stack[MAX];         //stack�� max���� 100
int top = -1;

void push(nodeptr node);
nodeptr pop();

void insert_btree();               // file ��ü�� ���� �� �Լ��� insert_arec�� ȣ���Ѵ�. 
int insert_arec(ele in_rec);         // ���ڵ带 �ϳ� �����Ѵ�. 
nodeptr retrieve(char*, int*);         // Ű���� �־� �˻��Ѵ�. [���̿� �Բ� ���] 
int seq_scan_btree(nodeptr curr);      // ��ü ���ڵ���� ����Ѵ�. 
int B_tree_deletion(char*);            // Function to delete a record with a given key.
void  print_level(char* skey);

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
		fflush(stdin);   // buffer clear 
		printf("\n����� �����ÿ�\n");
		printf("insert: i �̸�  / delete : d �̸�  /  retrieve : r �̸�  /  ��ü���: p   /  ����: e  >> ");
		res_gets = gets(line);
		if (!res_gets)
			break;      // no input.

		lleng = strlen(line);
		if (lleng <= 0)
			continue;   // empty line input. try again.

		i = 1;
		if (line[0] == 'E' || line[0] == 'e') {      // exit program
			printf("�������� ������!\n\n");   return 0;
		}

		else if (line[0] == 'I' || line[0] == 'i') { // ���ڵ� �Ѱ� ���� ���.

			// ���� ȸ����� ������.
			k = 0;
			while (line[i] == ' ')   i++;
			while (i < lleng) {
				name_s[k] = line[i]; k++; i++;
			}
			name_s[k] = '\0';

			if (strlen(name_s) == 0)
				continue;

			strcpy(a_insert.name, name_s);
			a_insert.nleng = strlen(name_s);

			top = -1;
			r = insert_arec(a_insert);   // ���ڵ� �ϳ��� �����ϴ� �Լ��� ȣ�� 
			if (r == 0)
				printf("���� ����.\n", r);
			else
				printf("���� ����.\n", r);

		}

		else if (line[0] == 'D' || line[0] == 'd') {  // ���� ��� ����
			k = 0;
			while (line[i] == ' ')   i++;
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

		}
		else if (line[0] == 'R' || line[0] == 'r') {   // Ž�� ��� ����
			k = 0;
			while (line[i] == ' ')   i++;
			while (i < lleng) {
				name_s[k] = line[i]; k++; i++;
			}
			name_s[k] = '\0';

			if (strlen(name_s) == 0)
				continue;
			tp = retrieve(name_s, &i);
			if (tp) {
				printf("Ž������.  Name: %s,  ����: %d\n", tp->rec[i].name, tp->rec[i].nleng);
				print_level(name_s);
			}
			else
				printf("Ž����û ���� Ű�� ���� ���ڵ尡 �������� ����.\n");

		}
		else if (line[0] == 'P' || line[0] == 'p') // ����Ʈ ��� ����. ��� ���ڵ带 Ű ������ ���.
			seq_scan_btree(root);
		else
			printf("input error \n");
	}   //while
}   //main

/**************** stack ���� �Լ��� ********************/
void push(nodeptr node) {
	if (top >= MAX-1) /// >>>  MAX-1 �̻�( >= ) �̾�� ���� �ʳ�?
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

/*******************************************************/
// ���ڵ� �ϳ��� �����ϴ� �Լ��̴�.
// ��ȯ��: 0: ���Խ���,  1, 2: ���Լ��� (1: ������ ����, 2:�� �� �� �þ� ��.)

int insert_arec(ele in_rec) {   //�ϳ��� ���ڵ带 ����  key = ȸ���  

	int i, j;
	nodeptr curr, child, new_ptr, tptr = NULL;
	ele empty = { "\0",0 };
	big_node bnode;

	if (!root) {   // root�� NULL�̸� btree�� �������. �� ù ��带 ����� ���⿡ �ִ´�.
		root = (nodeptr)malloc(sizeof(node));       /* Fill your code */   // nodeptr���·� nodeũ�⸸ŭ �Ҵ�޾� �����ּҴ� root�� ����   
		root->rec[0] = in_rec;               // key���� root->rec[0]�� �ִ´�. 
		root->ptr[0] = root->ptr[1] = root->ptr[2] = NULL;      // p0�� p1�� NULL�� �ִ´�. 
		root->rec[1] = empty;
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
	child = NULL;

	int index = 0;
	nodeptr tp;
	tp = retrieve(in_rec.name, &in_rec.nleng);
	if (tp) {
		return 0;
	}

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
				curr->ptr[j + 1] = curr->ptr[j] /* Fill your code */;
				curr->rec[j] = curr->rec[j - 1]/* Fill your code */;
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

			bnode.ptr[0] = curr->ptr[0] /* Fill your code */;
			for (j = 0; j < i; j++) {
				bnode.rec[j] = curr->rec[j]/* Fill your code */;
				bnode.ptr[j + 1] = curr->ptr[j + 1]/* Fill your code */;
			}

			bnode.rec[j] = in_rec /* Fill your code */;
			bnode.ptr[j + 1] = child/* Fill your code */;
			j++;

			while (i < MAXK) {
				bnode.rec[j] = curr->rec[i]/* Fill your code */;
				bnode.ptr[j + 1] = curr->ptr[i + 1]/* Fill your code */;

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


			int k = 0;
			curr->ptr[0] = bnode.ptr[0];
			for (k = 0; k < 1; k++) {
				curr->rec[k] = bnode.rec[k];
				curr->ptr[k + 1] = bnode.ptr[k + 1];
			}
			for (k = 1; k < MAXK; k++) {
				strcpy(curr->rec[k].name, "");
				curr->ptr[k + 1] = NULL;
			}
			curr->fill_cnt = 1;

			new_ptr = (nodeptr)malloc(sizeof(node));
			for (k = 0; k < 1; k++) {
				new_ptr->rec[k] = bnode.rec[1 + k + 1];
				new_ptr->ptr[k] = bnode.ptr[1 + k + 1];
			}
			new_ptr->ptr[k] = bnode.ptr[1 + k + 1];
			for (k = 1; k < MAXK; k++) {
				strcpy(new_ptr->rec[k].name, "");
				new_ptr->ptr[k + 1] = NULL;
			}
			new_ptr->fill_cnt = 1;



			child = new_ptr;
			in_rec = bnode.rec[1];

			if (top >= 0) {		// ������ emtpy �� �ƴ� ��� 
				curr = pop();	// curr �� �θ�� curr�� ������.
			}
			else { // ������ empty �� (�� curr �� root �����.) �� root ��带 ����� curr �� �θ�� ��.  
				tptr = (nodeptr)malloc(sizeof(node));/* Fill your code */;
				tptr->rec[0] = in_rec/* Fill your code */;
				tptr->ptr[0] = curr/* Fill your code */;
				tptr->ptr[1] = new_ptr/* Fill your code */;
				tptr->fill_cnt = 1/* Fill your code */;
				root = tptr/* Fill your code */;
				total_height++;
				return 2; // ���� ���� (���� 2: �� ��Ʈ�� ����)
			} // else.
		} // else.
	} while (1);


	return 0; // �� ������ ������ ���� ����.
} //�Լ� insert_arec

void insert_btree() {   //������ü�� ���ڵ带 ���� ->insert_arec �� ȣ�� 
	FILE* fp;
	ele data;
	char name_i[20], line[200];
	char* ret_fgets;
	int num, r;
	double score;
	int n = 0, lineleng;
	int check, count = 0;
	fp = fopen("Com_names1.txt", "r");
	if (fp == NULL) {
		printf("Cannot open this file :  Com_names1.txt\n");
		scanf("%d", &check);
	} //if 

	root = NULL;
	while (1) {

		ret_fgets = fgets(line, 200, fp);
		if (!ret_fgets)
			break;  // ���� ��� ����.

		lineleng = strlen(line);  // line �� ������ ���ڴ� newline ������.
		if (lineleng - 1 >= 100)
			continue; // ȸ����� �ʹ� �� ����
		line[lineleng - 1] = '\0'; //  ������ newline ���� ����.

		strcpy(data.name, line);  // ������ ���ڵ� �غ�.
		data.nleng = strlen(line);

		top = -1;  // ������ ����·� �ʱ�ȭ.
		r = insert_arec(data);   // �� ���ڵ带 ��Ʈ���� �����Ѵ�.
		if (r != 0)
			count++; // ���Լ��� ī��Ʈ ����.
	} //while 

	fp = fopen("Com_names2.txt", "r");
	if (fp == NULL) {
		printf("Cannot open this file :  Com_names2.txt\n");
		scanf("%d", &check);
	} //if 

	while (1) {
		ret_fgets = fgets(line, 200, fp);
		if (!ret_fgets)
			break;  // ���� ��� ����.

		lineleng = strlen(line);  // line �� ������ ���ڴ� newline ������.
		if (lineleng - 1 >= 100)
			continue; // ȸ����� �ʹ� �� ����
		line[lineleng - 1] = '\0'; //  ������ newline ���� ����.

		strcpy(data.name, line);  // ������ ���ڵ� �غ�.
		data.nleng = strlen(line);

		top = -1;  // ������ ����·� �ʱ�ȭ.
		r = insert_arec(data);   // �� ���ڵ带 ��Ʈ���� �����Ѵ�.
		if (r != 0)
			count++; // ���Լ��� ī��Ʈ ����.

	} //while 


	printf("���� ������ ���ڵ� �� = %d \n\n", count);
	fclose(fp);
}   //  �Լ� insert_btree

nodeptr  retrieve(char* skey, int* idx_found) {   //�˻� �Լ� 
	nodeptr curr = root;
	nodeptr P;
	int i; int level = 0;
	do {
		for (i = 0; i < curr->fill_cnt; i++) {
			if (strcmp(skey, curr->rec[i].name) < 0)
				break;
			else if (strcmp(skey, curr->rec[i].name) == 0) {
				//printf("ã�� ����� ����: %d , ����� ���ڵ� �ε��� ��ȣ: %d\n", level, i);
				*idx_found = i;
				return curr;
			}
			else
				; // do next i.
		} // for i= 
		P = curr->ptr[i];
		level++;
		if (P) {
			curr = P;
		}
	} while (P);



	return NULL;

}//retrieve

void  print_level(char* skey) {   //�˻� �Լ� 
	nodeptr curr = root;
	nodeptr P;
	int i; int level = 0;
	do {
		for (i = 0; i < curr->fill_cnt; i++) {
			if (strcmp(skey, curr->rec[i].name) < 0)
				break;
			else if (strcmp(skey, curr->rec[i].name) == 0) {
				printf("ã�� ����� ����: %d , ����� ���ڵ� �ε��� ��ȣ: %d\n", level, i);

				return 0;
			}
			else
				; // do next i.
		} // for i= 
		P = curr->ptr[i];
		level++;
		if (P) {
			curr = P;
		}
	} while (P);



	return 0;

}//retrieve



int seq_scan_btree(nodeptr curr) {
	int check_stack = 0;
	int i;
	int n;
	if (curr)
	{
		n = curr->fill_cnt;
		for (i = 0; i < n; i++) {

			seq_scan_btree(curr->ptr[i]);
			printf("Name : %s\n", curr->rec[i].name);

		}
		seq_scan_btree(curr->ptr[i]);
	} //if(curr)
	else if (!curr)
	{
		return 0;
	}

	return 0;
} //seq_scan_btree

// ����������� ����, �θ��� ���ڵ�, �������� ������ �޾Ƽ� ��й踦 �ϴ� �Լ�
//  wcase: curr �� ���������� ��й��̸� ��L��, curr �� ���������� ��й��̸� ��R��.
//  j :  father �ȿ��� curr�� ����Ű�� �������� �ε�����.
void redistribution(nodeptr father, nodeptr l_sibling, nodeptr r_sibling, char wcase, int j) {
	int i, k, m, n, h;
	two_Bnode twoB; //  twobnode(bnode�� 2���� ����)

	if (wcase == 'L')
		j = j - 1;
	else if (wcase == 'R')
		j = j;

	//copy l_sibling's content, intermediate key in father, r_sibling's content to twobnode;
	for (i = 0; i < l_sibling->fill_cnt/* Fill your code */; i++) {
		twoB.ptr[i] = l_sibling->ptr[i]/* Fill your code /*/;
		twoB.rec[i] = l_sibling->rec[i]/* Fill your code */;
	}
	twoB.ptr[i] = l_sibling->ptr[2]/* Fill your code */;

	// ����:  j �� father ������ l_sibling �� ���� index �� ��� ����.
	twoB.rec[i] = father->rec[j]/* Fill your code */; // �θ𿡼��� �߰� Ű�� ������.
	i++;

	for (k = 0; k < r_sibling->fill_cnt; k++, i++) {
		twoB.ptr[i] = r_sibling->ptr[k];
		twoB.rec[i] = r_sibling->rec[k]/* Fill your code */;
	}
	twoB.ptr[i] = r_sibling->ptr[2]/* Fill your code */;


	//Split twobnode into first half, middle record, second half;
	h = i / 2;  // h is the index of middle record.

	//copy first half to left node;
	for (n = 0; n < h; n++) {
		l_sibling->ptr[n] = twoB.ptr[n]/* Fill your code */;
		l_sibling->rec[n] = twoB.rec[n]/* Fill your code */;
	}
	l_sibling->ptr[n] = twoB.ptr[n]/* Fill your code */;
	l_sibling->fill_cnt = h/* Fill your code */;

	//copy second half to r_sibling;
	n++;
	for (m = 0; m < (i - h - 1); m++, n++) {
		r_sibling->ptr[m] = twoB.ptr[n]/* Fill your code */;
		r_sibling->rec[m] = twoB.rec[n]/* Fill your code */;
	}
	r_sibling->ptr[m] = twoB.ptr[n]/* Fill your code */;
	r_sibling->fill_cnt = m/* Fill your code */;

	//move the middle record to father ;
	father->rec[j] = twoB.rec[h]/* Fill your code */;
} // end of redistribution


int B_tree_deletion(char* out_key) {
	nodeptr  curr, r_sibling, l_sibling, father, Pt, leftptr, rightptr,successor;
	int i, j, k, r_OK, l_OK, found = 0, finished = 0;
	curr = root;

	// Step (0): search for a record (to be deleted) whose key equals out_key.
	do {
		for (i = 0; i < curr->fill_cnt; i++)
			if (strcmp(out_key, curr/* Fill your code */->rec[i].name) < 0)
				break;
			else if (strcmp(out_key, curr /* Fill your code */->rec[i].name) == 0) {
				found = 1; break;
			}
		if (found == 1)
			break;  // ����: ���� i�� ã�� ��ġ�� ��� ����.
		else {		// curr�� ����. child�� ���� ���� �Ѵ�.
			Pt = curr->ptr[i] /* Fill your code */;
			if (Pt) {
				push(curr/* Fill your code */);
				curr = Pt;
			}
			else
				break;
		}
	} while (!found);
	if (!found) {
		return 0;
	}

	// Comes here when the key is found. It is in curr's node. i has index of rec to delete.
	// Step (1):  find successor of d_rec.

	Pt = curr;
	if (curr->ptr[0]) {   // curr node is not a leaf node  
		// We need to find successor of out_key ;
		//Pt = curr /* Fill your code */;
		push(curr/* Fill your code */);
		curr = curr->ptr[i + 1];
		// ���� ���� �����͸� ���󳻷� ����.
		while (curr->ptr[0]/* Fill your code */) {
			push(curr/* Fill your code */);
			curr = curr->ptr[0];
			 /* Fill your code */;
		}
		successor = curr;
		Pt->rec[i] = curr->rec[0];
		out_key = curr->rec[0].name;
		//curr->rec[i] = Pt->rec[0];
		//curr = Pt;
		//i = 0;
	} //end if

	// curr ��忡�� index �� i �� ���ڵ�� �� ���� �����͸� �����Ͽ��� �Ѵ�.
	finished = false;
	do {
		// Step (2):
		//Remove record of index i and a pointer to its right from curr's node; 
		for (j = i + 1; j < curr->fill_cnt /* Fill your code */; j++) {
			curr->rec[j - 1] = curr->rec[j]/* Fill your code */;
			/* Fill your code */curr->ptr[j] = curr->ptr[j + 1];
		}
		curr->fill_cnt = curr->fill_cnt - 1;

		// Step (3):
		if (curr == root) {
			if (curr->fill_cnt == 0/* Fill your code */) {
				root = curr->ptr[0]/* Fill your code */;
				free(curr);
			}
			return 1; // deletion succeeded.
		}

		// Step (4):
		// curr is not the root. 
		if (curr->fill_cnt >= HALFK) { return 2; } // Finish deletion with success.

		// Now, curr violates minimum capacity constraint.
		// Step (5):
		father = pop(); // bring father of curr.
		// r-sibling = pointer to right sibling of curr' node; 
		// l-sibling = pointer to left sibling of curr's node;
		for (j = 0; j <= father->fill_cnt; j++)
			if (father->ptr[j] == curr) // find ptr of father which goes down to curr.
				break;
		if (j >= 1)
			l_sibling = father->ptr[j - 1];
		else
			l_sibling = NULL;
		if (j < father->fill_cnt)
			r_sibling = father->ptr[j + 1];
		else
			r_sibling = NULL;

		// ����: father �� ptr[j] �� curr �� ����
		//  r_sibling or l_sibling  �� �ϳ��� d ���� ���� ���ڵ� ������ ��й� ������!
		r_OK = 0;  l_OK = 0;
		if (r_sibling && r_sibling->fill_cnt > HALFK)
			r_OK = 1;
		else if (l_sibling && l_sibling->fill_cnt > HALFK)
			l_OK = 1;

		if (r_OK || l_OK) {
			//if (r_sibling has more than d keys) {
			if (r_OK) {
				redistribution(father, curr, r_sibling, 'R', j);
			}
			else if (l_OK) {
				redistribution(father, l_sibling, curr, 'L', j);
			}
			printf("Redistribution has been done.\n");
			return 3; // Deletion succeeded with redistribution.
		}
		else {   //  Step 6: merging (�պ��� �ʿ���)
			//  Let leftptr be a pointer to left one of curr and sibling chosen to merge ;
			//  Let rightptr point to the right one of curr and sibling chosen to merge ;
			if (r_sibling) {
				leftptr = curr; rightptr = r_sibling /* Fill your code */;
			} // r_sibling exists.
			else {
				leftptr = l_sibling; rightptr = curr/* Fill your code */;
			} // surely l_sibling exists.

			//   �Ʒ� �� ���� leftptr, rightptr �� ������ leftptr ������ �պ��ϴ� �κ��� �;� ��!! 
			//   ����: ���� i �� �� ���� ������ father ���� �߰� ���ڵ带 ����Ű�� �� ���ƾ� ��.
			/*
			Fill
			your
			code
			*/
			if (j != 0)
				leftptr->rec[leftptr->fill_cnt] = father->rec[j - 1];
			else
				leftptr->rec[leftptr->fill_cnt] = father->rec[j];
			leftptr->fill_cnt++;


			for (i = 0; i < rightptr->fill_cnt; i++) {
				leftptr->rec[leftptr->fill_cnt] = rightptr->rec[i];
				leftptr->ptr[leftptr->fill_cnt] = rightptr->ptr[i];
				leftptr->fill_cnt++;
			}

			leftptr->ptr[leftptr->fill_cnt] = rightptr->ptr[i];


			if (r_sibling)
				out_key = father->rec[j].name;
			else
				out_key = father->rec[j - 1].name;

			free(rightptr);
			printf("Merging has been done.\n");
			curr = father;
			// Note that i has index of record in father to be deleted. 
			// Deletion of this record and pointer to its right will be done at start of next iteration.
		} // else.
	} while (!finished);  // end of do-while ��.

} // B_tree_deletion
