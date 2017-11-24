#ifndef BINARYTREE
#define BINARYTREE
#include "curve.h"
#include "optimal_traversal.h"

class BinTree{
	struct TreeNode{
		Curve elem;
		bool leaf;
		TreeNode *left;
		TreeNode *right;
		TreeNode(TreeNode *a,TreeNode *b);
		TreeNode(Curve c,TreeNode *a,TreeNode *b);
		~TreeNode();
		void printLeaf();
		Curve meanFrechet();
	};
	TreeNode *root;
	int height;
	void createBranches(TreeNode *temp,int i);
	void insertCurve(Curve c,int i,TreeNode *temp,int index);
public:
	BinTree();
	~BinTree();
	void constructTree(Curve curves[],int size);
	void printLeaves();
	Curve meanFrechet();
};

#endif