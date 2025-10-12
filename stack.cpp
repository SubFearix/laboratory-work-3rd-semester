#include <iostream>
#include "structures.h"
#include "stack.h"
using namespace std;


Node* createNodeStack(string data)
{
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    return node;
}

Stack* createEmptyStack() {
    Stack* st = new Stack;
    st->top = nullptr;
    return st;
}

void popStack(Stack* st)
{
	if (st->top == nullptr) return;
	Node* temp = st->top;
	st->top = st->top -> next;
	delete temp;
}


void printStack(Stack* st) {
    Node* curr = st->top;
    while (curr != nullptr) {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << endl;
}


void pushStack(Stack* st, string data){
	Node* node = createNodeStack(data);
	if (st->top == nullptr){
    	st->top = node;
    	return;
    }
	node->next = st->top;
	st->top = node;
}

