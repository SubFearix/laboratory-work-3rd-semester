#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include "structures.h"
using namespace std;


Node* createNodeQueue(string data);
Queue* createEmptyQueue();
void addBackQueue(Queue* que, string data);
void remFrontQueue(Queue* que);
void printQueue(Queue* que);


#endif
