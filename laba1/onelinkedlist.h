#ifndef ONELINKEDLIST_H
#define ONELINKEDLIST_H
#include <string>
#include "structures.h"

using namespace std;


Node* createNodeOL(string data);
OLList* createEmptyOLList();
void addBackOL(OLList* lst, string data);
void remFrontOL(OLList* lst);
void remBackOL(OLList* lst);
void printListOL(OLList* lst);
void addFrontOL(OLList* lst, string data);
void insertAfterIndOL(OLList* lst, int index, string data);
void insertBeforeIndOL(OLList* lst, int index, string data);
void remValOL(OLList* lst, string val);
int findValOL(OLList* lst, string val);
void deleteBeforeIndOL(OLList* lst, int index);
void deleteAfterIndOL(OLList* lst, int index);

#endif
