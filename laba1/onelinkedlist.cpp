#include <iostream>
#include "structures.h"
#include "onelinkedlist.h"

using namespace std;


Node* createNodeOL(string data)
{
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    return node;
}

OLList* createEmptyOLList() {
    OLList* lst = new OLList;
    lst->head = nullptr;
    lst->tail = nullptr;
    return lst;
}

void addBackOL(OLList* lst, string data)
{
    Node* node = createNodeOL(data);
    if (lst->head == nullptr){
    	lst->head = node;
    	lst->tail = node;
    	return;
    }
    lst->tail->next = node;
    lst->tail = node;
}

void remFrontOL(OLList* lst)
{
	Node* temp = lst->head;
	lst->head = lst->head -> next;
	delete temp;
	
	if (lst->head == nullptr) lst->tail = nullptr;
}

void remBackOL(OLList* lst)
{
    if (lst->head == nullptr) return;
    if (lst->head == lst->tail) {remFrontOL(lst); return;}
    Node* curr = lst->head;
    while (curr -> next != lst->tail){
    	curr = curr -> next;
    }
    delete lst->tail;
    lst->tail = curr;
    lst->tail -> next = nullptr;
}

void printListOL(OLList* lst) {
    Node* curr = lst->head;
    while (curr != nullptr) {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << endl;
}


void addFrontOL(OLList* lst, string data){
	Node* node = createNodeOL(data);
	if (lst->head == nullptr){
    	lst->head = node;
    	lst->tail = node;
    	return;
    }
	node->next = lst->head;
	lst->head = node;
}

void insertAfterIndOL(OLList* lst, int index, string data){
	if (index < 0) return;
	Node* curr = lst->head;
	int i = 0;
	for (; i != index && curr != nullptr; i++){
		curr = curr->next;
	}
	if (curr == nullptr) return;
	Node* node = createNodeOL(data);
	node->next = curr->next;
	curr->next = node;
	if (curr == lst->tail){
		lst->tail = node;
	}
}


void insertBeforeIndOL(OLList* lst, int index, string data){
	if (index == 0) {addFrontOL(lst, data); return;}
	if (index < 0) return;
	Node* curr = lst->head;
	int i = 0;
	for (; i != index - 1 && curr != nullptr; i++){
		curr = curr->next;
	}
	if (curr == nullptr) return;
	Node* node = createNodeOL(data);
	node->next = curr->next;
	curr->next = node;
	if (curr == lst->tail){
		lst->tail = node;
	}
}

void remValOL(OLList* lst, string val){
	if (lst->head == nullptr) return;
    if (lst->head->data == val) {remFrontOL(lst); return;}
    Node* curr = lst->head;
    Node* prev = nullptr;
    while (curr != nullptr && curr->data != val){
    	prev = curr;
    	curr = curr->next;
    }
    if (curr == nullptr) return;
    prev->next = curr->next;
    if (curr == lst->tail){
		lst->tail = prev;
	}
	delete curr;
}

int findValOL(OLList* lst, string val){
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

void deleteBeforeIndOL(OLList* lst, int index) {
    if (lst->head == nullptr) {
        cout << "Список пуст.\n";
        return;
    }
    else if (index == 1) {
        remFrontOL(lst);
        return;
    }
    else if (index <= 0) {
        cout << "Неверный индекс.\n";
        return;
    }
    Node* curr = lst->head;
    int i = 0;
    for (; i != index - 2 && curr != nullptr; i++) {
        curr = curr->next;
    }
    if (curr == nullptr || curr == lst->tail || curr->next == nullptr) {
        cout << "Неверный индекс.\n";
        return;
    }
    Node* node = curr->next;
    curr->next = node->next; 
    delete node;
}

void deleteAfterIndOL(OLList* lst, int index) {
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
    if (curr == nullptr || curr == lst->tail) {
        cout << "Неверный индекс.\n";
        return;
    }
    if (curr -> next == lst->tail) {
        remBackOL(lst);
        return;
    }
    Node* node = curr -> next;
    curr -> next = node -> next;
    if (node == lst->tail) {
        lst->tail = curr;
    }
    delete node;
}
