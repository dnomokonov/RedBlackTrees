#include <iostream>

enum Color {RED, BLACK};

typedef struct rbtree {
	int key;
	Color color;
	rbtree* parent, * left, * right;
} rbtree;

//main function

rbtree* rbtree_add(rbtree* root, int key);
rbtree* rbtree_lookup(rbtree* root, int key);
rbtree* rbtree_delete(rbtree* root, int key);
rbtree* rbtree_min(rbtree* root);
rbtree* rbtree_max(rbtree* root);
void rbtree_free(rbtree* root);
void rbtree_print_dfs(rbtree* root, int level);

//additional function

rbtree* rbtree_create(int key);
void rbtree_addFixup(rbtree* &root, rbtree* z);
void RBTLeftRotate(rbtree* &root, rbtree* x);
void RBTRightRotate(rbtree* &root, rbtree* x);
void RBTtransplant(rbtree* &root, rbtree* u, rbtree* v);
void RBTdeleteFixup(rbtree* &root, rbtree* x);

//iu menu - test fuction v1.0

void start_menu(rbtree* root);