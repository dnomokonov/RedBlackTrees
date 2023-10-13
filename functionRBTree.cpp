#include "LibRBTree.h"

rbtree* rbtree_create(int key) {
	rbtree* node = new rbtree;

	node->key = key;
	node->color = RED;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;

	return node;
}

void RBTLeftRotate(rbtree* &root, rbtree* x) {
	rbtree* y = x->right;
	x->right = y->left;

	if (y->left != NULL) {
		y->left->parent = x;
	}

	y->parent = x->parent;

	if (x->parent == NULL) {
		root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

void RBTRightRotate(rbtree* &root, rbtree* x) {
	rbtree* y = x->left;
	x->left = y->right;

	if (y->right != NULL) {
		y->right->parent = x;
	}

	y->parent = x->parent;

	if (x->parent == NULL) {
		root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}

	y->right = x;
	x->parent = y;
}

void rbtree_addFixup(rbtree* &root, rbtree* z) {
	while (z != root && z->parent->color == RED) {
		if (z->parent == z->parent->parent->left) {
			rbtree* u = z->parent->parent->right;
			if (u->color == RED) {
				z->parent->color = BLACK;
				u->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) {
					z = z->parent;
					RBTLeftRotate(root, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				RBTRightRotate(root, z->parent->parent);
			}
		}
		else {
			rbtree* u = z->parent->parent->left;
			if (u != NULL && u->color == RED) {
				z->parent->color = BLACK;
				u->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					RBTRightRotate(root, z);
				}

				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				RBTLeftRotate(root, z->parent->parent);
			}
		}
	}

	root->color = BLACK;
}

rbtree* rbtree_add(struct rbtree* root, int key) {
	rbtree* node = rbtree_create(key);
	rbtree* y = NULL;
	rbtree* x = root;

	while (x != NULL) {
		y = x;
		if (node->key < x->key) {
			x = x->left;
		}
		else if (node->key > x->key) {
			x = x->right;
		}
		else {
			delete node;
			return root;
		}
	}

	node->parent = y;

	if (y == NULL) {
		root = node;
	}
	else if (node->key < y->key) {
		y->left = node;
	}
	else {
		y->right = node;
	}

	rbtree_addFixup(root, node);

	return root;
}

rbtree* rbtree_lookup(struct rbtree* root, int key) {
	while (root != NULL) {
		if (key < root->key) {
			root = root->left;
		}
		else if (key > root->key) {
			root = root->right;
		}
		else {
			return root;
		}
	}

	return NULL;
}

void RBTtransplant(rbtree* &root, rbtree* u, rbtree* v) {
	if (u->parent == NULL) {
		root = v;
	}
	else if (u == u->parent->left) {
		u->parent->left = v;
	}
	else {
		u->parent->right = v;
	}

	if (v != NULL) {
		v->parent = u->parent;
	}
}

void RBTdeleteFixup(rbtree* &root, rbtree* x) {
	while (x != NULL && x != root && x->color == BLACK) {
		if (x == x->parent->left) {
			rbtree* w = x->parent->right;
			
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				RBTLeftRotate(root, x->parent);
				w = x->parent->right;
			}

			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					RBTRightRotate(root, w);
					w = x->parent->right;
				}
				else {
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					RBTLeftRotate(root, x->parent);
					x = root;
				}
			}
		}
		else {
			rbtree* w = x->parent->left;
			if (x->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				RBTRightRotate(root, x->parent);
				w = x->parent->left;
			}

			if ((w->right == nullptr || w->right->color == BLACK) &&
				(w->left == nullptr || w->left->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->left == NULL || w->left->color == BLACK) {
					if (w->right != NULL) {
						w->right->color = BLACK;
					}
					w->color = RED;
					RBTLeftRotate(root, w);
					w = x->parent->left;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;

				if (w->left != NULL) {
					w->left->color = BLACK;
				}
				
				RBTRightRotate(root, x->parent);
				x = root;
			}
		}
	}

	if (x != NULL) {
		x->color = BLACK;
	}
}

rbtree* rbtree_delete(struct rbtree* root, int key) {
	rbtree* z = rbtree_lookup(root, key);

	if (z == NULL) {
		return root;
	}

	rbtree* y = z;
	Color ycolor = y->color;
	rbtree* x;

	if (z->left == NULL) {
		x = z->right;
		RBTtransplant(root, z, z->right);
	}
	else if (z->right == NULL) {
		x = z->left;
		RBTtransplant(root, z, z->left);
	}
	else {
		y = rbtree_min(z->right);
		ycolor = y->color;
		x = y->right;

		if (y->parent == z) {
			x->parent = y;
		}
		else {
			RBTtransplant(root, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}

		RBTtransplant(root, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}

	if (ycolor == BLACK) {
		RBTdeleteFixup(root, x);
	}

	delete z;

	return root;
}

rbtree* rbtree_min(struct rbtree* root) {
	while (root->left != NULL) {
		root = root->left;
	}
	return root;
}

rbtree* rbtree_max(struct rbtree* root) {
	while (root->right != NULL) {
		root = root->right;
	}
	return root;
}

void rbtree_free(struct rbtree* root) {
	if (root == NULL) {
		return;
	}

	rbtree_free(root->left);
	rbtree_free(root->right);
	delete(root);
}

void rbtree_print_dfs(struct rbtree* root, int level) {
	if (root == NULL) {
		return;
	}

	for (int i = 0; i < level; i++) {
		std::cout << " ";
	}

	std::cout << root->key << "(" << (root->color == RED ? "R" : "B") << ")" << std::endl;

	rbtree_print_dfs(root->right, level++);
	rbtree_print_dfs(root->left, level++);
}


void start_menu(rbtree* root) {
	std::cout << R"(   ____    ____    ____        ____     __       ______  ____     __  __      )" << std::endl;
	std::cout << R"(  /\  _`\ /\  _`\ /\  _`\     /\  _`\  /\ \     /\  _  \/\  _`\  /\ \/\ \     )" << std::endl;
	std::cout << R"(  \ \ \L\ \ \ \L\_\ \ \/\ \   \ \ \L\ \\ \ \    \ \ \L\ \ \ \/\_\\ \ \/'/'    )" << std::endl;
	std::cout << R"(   \ \ ,  /\ \  _\L\ \ \ \ \   \ \  _ <'\ \ \  __\ \  __ \ \ \/_/_\ \ , <     )" << std::endl;
	std::cout << R"(    \ \ \\ \\ \ \L\ \ \ \_\ \   \ \ \L\ \\ \ \L\ \\ \ \/\ \ \ \L\ \\ \ \\`\   )" << std::endl;
	std::cout << R"(     \ \_\ \_\ \____/\ \____/    \ \____/ \ \____/ \ \_\ \_\ \____/ \ \_\ \_\ )" << std::endl;
	std::cout << R"(      \/_/\/ /\/___/  \/___/      \/___/   \/___/   \/_/\/_/\/___/   \/_/\/_/ )" << std::endl;
	std::cout << "\n" << std::endl;
	std::cout << R"(                       ______  ____    ____    ____        )" << std::endl;
	std::cout << R"(                      /\__  _\/\  _`\ /\  _`\ /\  _`\      )" << std::endl;
	std::cout << R"(                      \/_/\ \/\ \ \L\ \ \ \L\_\ \ \L\_\    )" << std::endl;
	std::cout << R"(                         \ \ \ \ \ ,  /\ \  _\L\ \  _\L    )" << std::endl;
	std::cout << R"(                          \ \ \ \ \ \\ \\ \ \L\ \ \ \L\ \  )" << std::endl;
	std::cout << R"(                           \ \_\ \ \_\ \_\ \____/\ \____/  )" << std::endl;
	std::cout << R"(                            \/_/  \/_/\/ /\/___/  \/___/   )" << std::endl;

	int point;
	while (true) {
		std::cout << std::endl;
		std::cout << "[1] RBtree output" << std::endl;
		std::cout << "[2] Delete a node" << std::endl;
		std::cout << "[3] Exit" << std::endl;

		std::cout << std::endl;
		std::cout << "[Input number]  ";
		std::cin >> point;
		std::cout << std::endl;
		if (point == 1) {
			rbtree_print_dfs(root, 0);
		}
		else if (point == 2) {
			int numNode;
			std::cout << "[Number key] ";
			std::cin >> numNode;
			rbtree_delete(root, numNode);
		}
		else if (point == 3) {
			std::cout << "Good bye >.< !" << std::endl;
			return;
		}
	}
}