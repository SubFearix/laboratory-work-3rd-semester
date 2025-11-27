#include <iostream>
#include "structures.h"

using namespace std;

void Queue::remFrontQueue()
{
	const Node* temp = this->head;
	this->head = this->head -> next;
	delete temp;
	
	if (this->head == nullptr) this->tail = nullptr;
}

void Queue::addBackQueue(const string& data)
{
    Node* node = createNodeQueue(data);
    if (this->head == nullptr){
    	this->head = node;
    	this->tail = node;
    	return;
    }
    this->tail->next = node;
    this->tail = node;
}

void Queue::printQueue() const
{
    const Node* curr = this->head;
    while (curr != nullptr) {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << endl;
}

