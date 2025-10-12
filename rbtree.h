#ifndef RBTREE_H
#define RBTREE_H

#include "structures.h"
using namespace std;
RBTree* createEmptyRBTree();
void leftRotate(RBTree* rbt, NodeRB* x);
void rightRotate(RBTree* rbt, NodeRB* x);
void insertNode(RBTree* rbt, int data);
void fixInsert(RBTree* rbt, NodeRB* node);
NodeRB* searchNode(RBTree* rbt, int data);
NodeRB* findMin(RBTree* rbt, NodeRB* node);
void fixDelete(RBTree* rbt, NodeRB* x);
void deleteNode(RBTree* rbt, int data);
void printRBTreeHelper(NodeRB* node, RBTree* rbt, string prefix, bool isLeft);
void printRBTree(RBTree* rbt);
void loadRBTreeFromStream(RBTree* rbt, istream& is);
void loadRBTreeHelper(RBTree* rbt, NodeRB*& node, NodeRB* parent, istream& is);
void saveRBTreeToStream(RBTree* rbt, ostream& os);
void saveRBTreeHelper(NodeRB* node, RBTree* rbt, ostream& os);
#endif
