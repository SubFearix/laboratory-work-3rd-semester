#include <iostream>
#include "structures.h"

using namespace std;

void OLList::addBackOL(const string& data)
{
    Node* node = createNodeOL(data);
    if (this->head == nullptr){
    	this->head = node;
    	this->tail = node;
    	return;
    }
    this->tail->next = node;
    this->tail = node;
}

void OLList::remFrontOL()
{
	const Node* temp = this->head;
	this->head = this->head -> next;
	delete temp;
	
	if (this->head == nullptr) this->tail = nullptr;
}

void OLList::remBackOL()
{
    if (this->head == nullptr) return;
    if (this->head == this->tail) {remFrontOL(); return;}
    Node* curr = this->head;
    while (curr -> next != this->tail){
    	curr = curr -> next;
    }
    delete this->tail;
    this->tail = curr;
    this->tail -> next = nullptr;
}

void OLList::printListOL() const
{
    const Node* curr = this->head;
    while (curr != nullptr) {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << endl;
}


void OLList::addFrontOL(const string& data){
	Node* node = createNodeOL(data);
	if (this->head == nullptr){
    	this->head = node;
    	this->tail = node;
    	return;
    }
	node->next = this->head;
	this->head = node;
}

void OLList::insertAfterIndOL(const int index, const string& data){
	if (index < 0) return;
	Node* curr = this->head;
	for (int i = 0; i != index && curr != nullptr; i++){
		curr = curr->next;
	}
	if (curr == nullptr) return;
	Node* node = createNodeOL(data);
	node->next = curr->next;
	curr->next = node;
	if (curr == this->tail){
		this->tail = node;
	}
}


void OLList::insertBeforeIndOL(const int index, const string& data){
	if (index == 0) {addFrontOL(data); return;}
	if (index < 0) return;
	Node* curr = this->head;
	for (int i = 0; i != index - 1 && curr != nullptr; i++){
		curr = curr->next;
	}
	if (curr == nullptr) return;
	Node* node = createNodeOL(data);
	node->next = curr->next;
	curr->next = node;
	if (curr == this->tail){
		this->tail = node;
	}
}

void OLList::remValOL(const string& val){
	if (this->head == nullptr) return;
    if (this->head->data == val) {remFrontOL(); return;}
    Node* curr = this->head;
    Node* prev = nullptr;
    while (curr != nullptr && curr->data != val){
    	prev = curr;
    	curr = curr->next;
    }
    if (curr == nullptr) return;
    prev->next = curr->next;
    if (curr == this->tail){
		this->tail = prev;
	}
	delete curr;
}

int OLList::findValOL(const string& val) const
{
	if (this->head == nullptr) return -1;
	const Node* curr = this->head;
	int ind = 0;
    while (curr != nullptr && curr->data != val){
    	curr = curr->next;
    	ind++;
    }
    if (curr == nullptr) return -1;
    return ind;
}

void OLList::deleteBeforeIndOL(const int index) {
    if (this->head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    if (index == 1) {
	    remFrontOL();
	    return;
    }
    if (index <= 0) {
	    cout << "Неверный индекс.\n";
	    return;
    }
    Node* curr = this->head;
    for (int i = 0; i != index - 2 && curr != nullptr; i++) {
        curr = curr->next;
    }
    if (curr == nullptr || curr == this->tail || curr->next == nullptr) {
        cout << "Неверный индекс.\n";
        return;
    }
    const Node* node = curr->next;
    curr->next = node->next; 
    delete node;
}

void OLList::deleteAfterIndOL(const int index) {
    if (this->head == nullptr || this->head == this->tail) {
        cout << "Удаление невозможно.\n";
        return;
    }
    if (index < 0) {
	    cout << "Неверный индекс.\n";
	    return;
    }
    Node* curr = this->head;
    for (int i = 0; i != index && curr != nullptr; i++) {
        curr = curr->next;
    }
    if (curr == nullptr || curr == this->tail) {
        cout << "Неверный индекс.\n";
        return;
    }
    if (curr -> next == this->tail) {
        remBackOL();
        return;
    }
    const Node* node = curr -> next;
    curr -> next = node -> next;
    if (node == this->tail) {
        this->tail = curr;
    }
    delete node;
}
