#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <string>
using namespace std;

struct Node {
    string data;
    Node* next;
    Node* prev;
};
enum Color{RED, BLACK};

struct NodeRB{
    int data;
    NodeRB* left;
    NodeRB* right;
    NodeRB* parent;
    Color color;
};

struct RBTree{
	NodeRB* root;
	NodeRB* nilnode;
};

struct Array {
    string* data;
    int capacity;
    int size;
};


struct OLList {
    Node* head;
    Node* tail;
};

struct Queue {
    Node* head;
    Node* tail;
};


struct Stack {
    Node* top;
};

struct TLList {
    Node* head;
    Node* tail;
};


#endif
