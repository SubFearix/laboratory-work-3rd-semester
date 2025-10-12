#ifndef STACK_H
#define STACK_H
#include <string>
#include "structures.h"
using namespace std;


Node* createNodeStack(string data);
Stack* createEmptyStack();
void pushStack(Stack* st, string data);
void popStack(Stack* st);
void printStack(Stack* st);




#endif
