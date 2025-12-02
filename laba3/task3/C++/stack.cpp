#include <iostream>
#include "structures.h"

using namespace std;

void Stack::popStack()
{
	if (this->top == nullptr) return;
	const Node* temp = this->top;
	this->top = this->top -> next;
	delete temp;
}


void Stack::printStack() const
{
    const Node* curr = this->top;
    while (curr != nullptr) {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << endl;
}


void Stack::pushStack(const string& data){
	Node* node = createNodeSt(data);
	if (this->top == nullptr){
    	this->top = node;
    	return;
    }
	node->next = this->top;
	this->top = node;
}

