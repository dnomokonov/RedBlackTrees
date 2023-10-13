#include "LibRBTree.h"
#include <fstream>

using namespace std;

int dataSize = 40;

int main()
{
	rbtree* root = NULL;

	ifstream fileIn("data.txt");
	int number;
	for (int i = 0; i < dataSize; i++) {
		fileIn >> number;
		root = rbtree_add(root, number);
	}

	start_menu(root);

	rbtree_free(root);
	return 0;
}
