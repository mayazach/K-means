#include <iostream>
#include <cmath>
#include "binaryTree.h"
#include "curve.h"
#include "curveList.h"
#include "optimal_traversal.h"

using namespace std;

BinTree::TreeNode::TreeNode(TreeNode *a=NULL,TreeNode *b=NULL){
	left = a;
	right = b;
	leaf = false;
	//cout << "Creating tree node." << endl;
}

BinTree::TreeNode::TreeNode(Curve c,TreeNode *a=NULL,TreeNode *b=NULL){
	left = a;
	right = b;
	elem = c;
	leaf = true;
	//cout << "Creating tree node with curve." << endl;
}

BinTree::TreeNode::~TreeNode(){
	if(left!=NULL)
		delete left;
	if(right!=NULL)
		delete right;
	//cout << "Deleting tree node" << endl;
}

void BinTree::TreeNode::printLeaf(){
	if(left!=NULL)
		left->printLeaf();
	if(right!=NULL)
		right->printLeaf();
	if(leaf)
		curvePrint(elem);
}

Curve BinTree::TreeNode::meanFrechet(){
	Curve a,b,c;
	int i;
	if(left == NULL)
		a.id = "NULL";
	else if(left->leaf)
		a = left->elem;
	else
		a = left->meanFrechet();
	if(right == NULL)
		b.id = "NULL";
	else if(right->leaf)
		b = right->elem;
	else
		b = right->meanFrechet();
	if((a.id == "NULL") && (b.id == "NULL")){
		//cout << "no curves" << endl;
		return a;
	}
	else if(a.id == "NULL"){
		//cout << "Right curve" << endl;
		return b;
	}
	else if(b.id == "NULL"){
		//cout << "Left curve" << endl;
		return a;
	}
	else{
		//cout << "Two curves" << endl;
		c = get_mean_discrete(&a,&b);
		c.inDataset = false;
		if(!left->leaf && !a.inDataset){
			if(a.inDataset)
				cout << "Deleting dataset curve!" << endl;
			for(i=0;i<a.m;i++)
				delete [] a.points[i];
			delete [] a.points;
		}
		if(!right->leaf && !b.inDataset){
			if(b.inDataset)
				cout << "Deleting dataset curve!" << endl;
			for(i=0;i<b.m;i++)
				delete [] b.points[i];
			delete [] b.points;
		}
		return c;
	}
}

BinTree::BinTree(){
	root = NULL;
	height = 0;
	//cout << "Creating binary tree" << endl;
}

BinTree::~BinTree(){
	if(root!=NULL)
		delete root;
	//cout << "Deleting binary tree" << endl;
}

void BinTree::insertCurve(Curve c,int i,TreeNode *temp,int index){
	int divide;
	if(i >= height-1){
		if(temp->left == NULL)
			temp->left = new TreeNode(c);
		else
			temp->right = new TreeNode(c);
	}
	else{
		divide = pow(2,height-i-1);
		if(index < divide)
			insertCurve(c,i+1,temp->left,index);
		else
			insertCurve(c,i+1,temp->right,index-divide);
	}
}

void BinTree::createBranches(TreeNode *temp,int i){
	if(i >= height-1)
		return;
	else{
		temp->left = new TreeNode();
		createBranches(temp->left,i+1);
		temp->right = new TreeNode();
		createBranches(temp->right,i+1);
	}
}

void BinTree::constructTree(Curve curves[],int size){
	int i;
	height = (int) ceil(log2(size));
	root = new TreeNode();
	createBranches(root,0);
	for(i=0;i<size;i++){
		insertCurve(curves[i],0,root,i);
	}
}

void BinTree::printLeaves(){
	if(root!=NULL)
		root->printLeaf();
}

Curve BinTree::meanFrechet(){
	Curve a;
	if(root != NULL)
		a = root->meanFrechet();
	//cout << "Got here safely" << endl;
	a.id = "Frechet result!";
	return a;
}
