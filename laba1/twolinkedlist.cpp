#include <iostream>
#include "structures.h"
#include "twolinkedlist.h"

using namespace std;

Node* createNodeTL(string data)
{
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    node->prev = nullptr;
    return node;
}

TLList* createEmptyTLList() {
    TLList* lst = new TLList;
    lst->head = nullptr;
    lst->tail = nullptr;
    return lst;
}

void addBackTL(TLList* lst, string data)
{
    Node* node = createNodeTL(data);
    if (lst->head == nullptr){
    	lst->head = node;
    	lst->tail = node;
    	return;
    }
    node->prev = lst->tail; 
    lst->tail->next = node;
    lst->tail = node;
}

void remFrontTL(TLList* lst)
{
	if (lst->head == nullptr) return;
	Node* temp = lst->head;
	lst->head = lst->head->next;
	
	if (lst->head == nullptr){
		lst->tail = nullptr;
	} else {
		lst->head->prev = nullptr;
	}
	delete temp;
}

void remBackTL(TLList* lst)
{
    if (lst->head == nullptr) return;
    if (lst->head == lst->tail) {remFrontTL(lst); return;}
    Node* temp = lst->tail;
    lst->tail = lst->tail->prev;
    lst->tail->next = nullptr;
    delete temp;
}

void printListTL(TLList* lst) {
	if (lst->head == nullptr) return;
    Node* curr = lst->head;
    while (curr != nullptr) {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << endl;
}


void addFrontTL(TLList* lst, string data){
	Node* node = createNodeTL(data);
	if (lst->head == nullptr){
    	lst->head = node;
    	lst->tail = node;
    	return;
    }
	node->next = lst->head;
	lst->head->prev = node;
	lst->head = node;
}

void insertAfterIndTL(TLList* lst, int index, string data){
	if (index < 0|| lst->head == nullptr) return;
	Node* curr = lst->head;
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
	if (curr == lst->tail){
		lst->tail = node;
	}
}


void insertBeforeIndTL(TLList* lst, int index, string data){
	if (index == 0) {addFrontTL(lst, data); return;}
	if (index < 0|| lst->head == nullptr) return;
	Node* curr = lst->head;
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

void remValTL(TLList* lst, string val){
	if (lst->head == nullptr) return;
    if (lst->head->data == val) {remFrontTL(lst); return;}
    Node* curr = lst->head;
    while (curr != nullptr && curr->data != val){
    	curr = curr->next;
    }
    if (curr == nullptr) return;
    curr->prev->next = curr->next;
    if (curr == lst->tail){
		lst->tail = curr->prev;
	} else {
		curr->next->prev = curr->prev;
	}
	delete curr;
}

int findValTL(TLList* lst, string val){
	if (lst->head == nullptr) return -1;
	Node* curr = lst->head;
	int ind = 0;
    while (curr != nullptr && curr->data != val){
    	curr = curr->next;
    	ind++;
    }
    if (curr == nullptr) return -1;
    return ind;
}

void deleteBeforeIndTL(TLList* lst, int index) {
    if (lst->head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    else if (index == 1) {
        remFrontTL(lst);
        return;
    }
    else if (index <= 0) {
        cout << "Неверный индекс.\n";
        return;
    }
    Node* curr = lst->head;
    int i = 0;
    for (; i != index && curr != nullptr; i++) {
        curr = curr->next;
    }
    if (curr == nullptr || curr->prev == nullptr) {
        cout << "Неверный индекс.\n";
        return;
    }
    Node* node = curr->prev;
    curr->prev = node->prev;
    if (node->prev != nullptr){
    	node->prev->next = curr;
    } else {
    	lst->head = curr;
    }
    delete node;
}

void deleteAfterIndTL(TLList* lst, int index) {
    if (lst->head == nullptr || lst->head == lst->tail) {
        cout << "Удаление невозможно.\n";
        return;
    }
    else if (index < 0) {
        cout << "Неверный индекс.\n";
        return;
    }
    Node* curr = lst->head;
    int i = 0;
    for (; i != index && curr != nullptr; i++) {
        curr = curr->next;
    }
    if (curr == nullptr || curr->next == nullptr) {
        cout << "Неверный индекс.\n";
        return;
    }
    Node* node = curr -> next;
    curr -> next = node -> next;
    if (node->next != nullptr) {
        node->next->prev = curr;
    } else {
    	lst->tail = curr;
    }
    delete node;
}
