#include <stdio.h>

#define MAXD_idx 4
#define MAXD_data 4
#define D_idx 2
#define D_data 2
#define MAX 100

typedef char type_key[100];

typedef struct recore {
	type_key name;
	int leng;
}type_rec;

typedef struct idxnode* type_ptr_idxnode;
typedef struct datanode* type_ptr_datanode;
typedef struct idxnode {
	type_key key[MAXD_idx];
	type_ptr_idxnode ptri[MAXD_idx + 1];
	type_ptr_datanode ptrd[MAXD_idx + 1];
	int fill_cnt;
}type_idx_node;

typedef struct datanode {
	type_rec rec[MAXD_data];
	type_ptr_datanode link;
	int fill_cnt;
}type_data_node;

typedef struct big_node_index {
	type_key key[MAXD_idx + 1];
	type_ptr_idxnode ptri[MAXD_idx + 2];
	type_ptr_datanode ptrd[MAXD_idx + 2];
}big_node_index;

typedef struct big_node_data {
	type_rec rec[MAXD_data + 1];
}big_node_data;

type_ptr_idxnode ROOT = NULL;
type_ptr_datanode HEAD = NULL;

type_ptr_idxnode stack[MAX];
int top = -1;
 
void push(type_ptr_idxnode node) {
	if (top > MAX) {
		printf("stack is full\n");
		return;
	}
	top++;
	stack[top] = node;
}

type_ptr_idxnode pop() {
	int temp = 0;
	if (top < 0) {
		printf("stack is empty\n");
		getchar();
	}
	temp = top;
	top--;
	return stack[temp];
}
