#include <iostream>
#include "structures.h"

using namespace std;

void TLList::addBackTL(const string& data)
{
    Node* node = createNodeTL(data);
    if (this->head == nullptr){
    	this->head = node;
    	this->tail = node;
    	return;
    }
    node->prev = this->tail; 
    this->tail->next = node;
    this->tail = node;
}

void TLList::remFrontTL()
{
	if (this->head == nullptr) return;
	const Node* temp = this->head;
	this->head = this->head->next;
	
	if (this->head == nullptr){
		this->tail = nullptr;
	} else {
		this->head->prev = nullptr;
	}
	delete temp;
}

void TLList::remBackTL()
{
    if (this->head == nullptr) return;
    if (this->head == this->tail) {remFrontTL(); return;}
    const Node* temp = this->tail;
    this->tail = this->tail->prev;
    this->tail->next = nullptr;
    delete temp;
}

void TLList::printListTL() const
{
	if (this->head == nullptr) return;
    const Node* curr = this->head;
    while (curr != nullptr) {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << endl;
}


void TLList::addFrontTL(const string& data){
	Node* node = createNodeTL(data);
	if (this->head == nullptr){
    	this->head = node;
    	this->tail = node;
    	return;
    }
	node->next = this->head;
	this->head->prev = node;
	this->head = node;
}

void TLList::insertAfterIndTL(const int index, const string& data){
	if (index < 0|| this->head == nullptr) return;
	Node* curr = this->head;
	int i = 0;
	for (; i != index && curr != nullptr; i++){
		curr = curr->next;
	}
	if (curr == nullptr) return;
	Node* node = createNodeTL(data);
	node->next = curr->next;
	node->prev = curr;
	if (curr->next != nullptr) { curr->next->prev = node;}
	curr->next = node;
	if (curr == this->tail){
		this->tail = node;
	}
}


void TLList::insertBeforeIndTL(const int index, const string& data){
	if (index == 0) {addFrontTL(data); return;}
	if (index < 0|| this->head == nullptr) return;
	Node* curr = this->head;
	int i = 0;
	for (; i != index && curr != nullptr; i++){
		curr = curr->next;
	}
	if (curr == nullptr) return;
	Node* node = createNodeTL(data);
	node->prev = curr->prev;
	node->next = curr;
	curr->prev->next = node;
	curr->prev = node;
}

void TLList::remValTL(const string& val){
	if (this->head == nullptr) return;
    if (this->head->data == val) {remFrontTL(); return;}
    const Node* curr = this->head;
    while (curr != nullptr && curr->data != val){
    	curr = curr->next;
    }
    if (curr == nullptr) return;
    curr->prev->next = curr->next;
    if (curr == this->tail){
		this->tail = curr->prev;
	} else {
		curr->next->prev = curr->prev;
	}
	delete curr;
}

int TLList::findValTL(const string& val) const
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

void TLList::deleteBeforeIndTL(const int index) {
    if (this->head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    if (index == 1) {
	    remFrontTL();
	    return;
    }
    if (index <= 0) {
	    cout << "Неверный индекс.\n";
	    return;
    }
    Node* curr = this->head;
    int i = 0;
    for (; i != index && curr != nullptr; i++) {
        curr = curr->next;
    }
    if (curr == nullptr || curr->prev == nullptr) {
        cout << "Неверный индекс.\n";
        return;
    }
    const Node* node = curr->prev;
    curr->prev = node->prev;
    if (node->prev != nullptr){
    	node->prev->next = curr;
    } else {
    	this->head = curr;
    }
    delete node;
}

void TLList::deleteAfterIndTL(const int index) {
    if (this->head == nullptr || this->head == this->tail) {
        cout << "Удаление невозможно.\n";
        return;
    }
    if (index < 0) {
        cout << "Неверный индекс.\n";
        return;
    }
    Node* curr = this->head;
    int i = 0;
    for (; i != index && curr != nullptr; i++) {
        curr = curr->next;
    }
    if (curr == nullptr || curr->next == nullptr) {
        cout << "Неверный индекс.\n";
        return;
    }
    const Node* node = curr -> next;
    curr -> next = node -> next;
    if (node->next != nullptr) {
        node->next->prev = curr;
    } else {
    	this->tail = curr;
    }
    delete node;
}
