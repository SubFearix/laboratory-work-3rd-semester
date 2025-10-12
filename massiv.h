#ifndef MASSIV_H
#define MASSIV_H
#include <string>
#include "structures.h"

using namespace std;


Array* createArray();
void addBackArray(Array* arr, string val);
void addElemIndArray(Array* arr, int ind, string val);
string getElemIndArray(Array* arr, int ind);
void remElemIndArray(Array* arr, int ind);
void changeElemIndArray(Array* arr, int ind, string val);
int getSizeArray(Array* arr);
void printArray(Array* arr);


#endif
