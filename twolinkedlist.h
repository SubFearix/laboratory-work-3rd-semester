#ifndef TWOLINKEDLIST_H
#define TWOLINKEDLIST_H

#include <string>
#include "structures.h"
using namespace std;

Node* createNodeTL(string data);
TLList* createEmptyTLList();
void addBackTL(TLList* lst, string data);
void remFrontTL(TLList* lst);
void remBackTL(TLList* lst);
void printListTL(TLList* lst);
void addFrontTL(TLList* lst, string data);
void insertAfterIndTL(TLList* lst, int index, string data);
void insertBeforeIndTL(TLList* lst, int index, string data);
void remValTL(TLList* lst, string val);
int findValTL(TLList* lst, string val);
void deleteBeforeIndTL(TLList* lst, int index);
void deleteAfterIndTL(TLList* lst, int index);

#endif
