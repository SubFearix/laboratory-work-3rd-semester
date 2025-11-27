#include <iostream>
#include <string>
#include "structures.h"

using namespace std;

void Array::addBackArray(const string& val){
	if (this->size >= this->capacity){
		this->capacity = this->capacity*2;
		auto* newData = new string[this->capacity];
		for (int i = 0; i < this->size; i++){
			newData[i] = this->data[i];
		}
		delete[] this->data;
		this->data = newData;
	}
	this->data[this->size] = val;
	this->size++;
}

void Array::addElemIndArray(const int ind, const string& val){
	if (0 > ind || ind > this->size) return;
	if (this->size >= this->capacity){
		this->capacity = this->capacity*2;
		auto* newData = new string[this->capacity];
		for (int i = 0; i < this->size; i++){
			newData[i] = this->data[i];
		}
		delete[] this->data;
		this->data = newData;
	}
	for (int i = this->size; i > ind; i--){
		this->data[i] = this->data[i - 1]; // Двигаем вправл
	}
	
	this->data[ind] = val;
	this->size++;
}

string Array::getElemIndArray(const int ind) const {
	if (0 > ind || ind >= this->size) return "";
	return this->data[ind];
}

void Array::remElemIndArray(const int ind){
	if (0 > ind || ind >= this->size) return;
	for (int i = ind + 1; i < this->size; i++){
		this->data[i-1] = this->data[i]; // Двигаем влево
	}
	this->size--;
}

void Array::changeElemIndArray(const int ind, string val) const
{
	if (0 > ind || ind >= this->size) return;
	this->data[ind] = move(val);
}

int Array::getSizeArray() const {
	return this->size;
}

void Array::printArray() const
{
	for (int i = 0; i < this->size; i++){
		cout << this->data[i]<< " ";
	}
	cout << endl;
}
