#include <iostream>
#include <string>
#include "structures.h"
#include "massiv.h"

using namespace std;

Array* createArray(){
	Array* arr = new Array;
	arr->capacity = 10;
	arr->data = new string[arr->capacity];
	arr->size = 0;
	return arr;
}

void addBackArray(Array* arr, string val){
	if (arr->size >= arr->capacity){
		arr->capacity = arr->capacity*2;
		string* newData = new string[arr->capacity];
		for (int i = 0; i < arr->size; i++){
			newData[i] = arr->data[i];
		}
		delete[] arr->data;
		arr->data = newData;
	}
	arr->data[arr->size] = val;
	arr->size++;
}

void addElemIndArray(Array* arr, int ind, string val){
	if (0 > ind || ind > arr->size) return;
	if (arr->size >= arr->capacity){
		arr->capacity = arr->capacity*2;
		string* newData = new string[arr->capacity];
		for (int i = 0; i < arr->size; i++){
			newData[i] = arr->data[i];
		}
		delete[] arr->data;
		arr->data = newData;
	}
	for (int i = arr->size; i > ind; i--){
		arr->data[i] = arr->data[i - 1]; // Двигаем вправл
	}
	
	arr->data[ind] = val;
	arr->size++;
}

string getElemIndArray(Array* arr, int ind){
	if (0 > ind || ind >= arr->size) return "";
	return arr->data[ind];
}

void remElemIndArray(Array* arr, int ind){
	if (0 > ind || ind >= arr->size) return;
	for (int i = ind + 1; i < arr->size; i++){
		arr->data[i-1] = arr->data[i]; // Двигаем влево
	}
	arr->size--;
}

void changeElemIndArray(Array* arr, int ind, string val){
	if (0 > ind || ind >= arr->size) return;
	arr->data[ind] = val;
}

int getSizeArray(Array* arr){
	return arr->size;
}

void printArray(Array* arr){
	for (int i = 0; i < arr->size; i++){
		cout << arr->data[i]<< " ";
	}
	cout << endl;
}
