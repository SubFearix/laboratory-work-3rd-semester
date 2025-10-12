#include <iostream>
#include "structures.h"
#include "queue.h"
using namespace std;


Node* createNodeQueue(string data)
{
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    return node;
}

Queue* createEmptyQueue() {
    Queue* que = new Queue;
    que->head = nullptr;
    que->tail = nullptr;
    return que;
}

void remFrontQueue(Queue* que)
{
	Node* temp = que->head;
	que->head = que->head -> next;
	delete temp;
	
	if (que->head == nullptr) que->tail = nullptr;
}

void addBackQueue(Queue* que, string data)
{
    Node* node = createNodeQueue(data);
    if (que->head == nullptr){
    	que->head = node;
    	que->tail = node;
    	return;
    }
    que->tail->next = node;
    que->tail = node;
}

void printQueue(Queue* que) {
    Node* curr = que->head;
    while (curr != nullptr) {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << endl;
}

