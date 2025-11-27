#include <iostream>
#include <string>
#include "structures.h"

using namespace std;

void HashA::rehash() {
    const int oldCapacity = this->capacity;
    const NodeHashA* oldTable = this->cell;
    
    this->capacity = oldCapacity * 2;
    this->cell = new NodeHashA[this->capacity];
    this->size = 0;
    
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].isEmpty == false && oldTable[i].isDeleted == false) {
            addElementA(oldTable[i].key, oldTable[i].data);
        }
    }
    delete[] oldTable;
}

int HashA::hashFunc(const string& key, const int capacity){
    int hash = 0;
    for (const char c: key) {
        hash = (hash * 37 + c) % capacity;
    }
    return hash % capacity;
}

void HashA::addElementA( const string& key, const int data){
	int i = 0;
    if (this->size >= this->capacity * 0.75) {
        cout << "Происходит рехэширование! Старый размер: " << this->capacity;
        rehash();
        cout << ", новый размер: " << this->capacity << endl;
    }
    
	int index = hashFunc(key, this->capacity);
	const int orig = index;
	while (this->cell[index].isEmpty == false && this->cell[index].isDeleted == false){
		if (this->cell[index].key == key){
			this->cell[index].data = data;
			return;
		}
		i++;
		index = (orig + i) % this->capacity;
		
		if (i == this->capacity) {
			cout << "Не найдено место для вставки!" << endl;
			return;
		}
	}
	
	this->cell[index].key = key;
	this->cell[index].data = data;
	this->cell[index].isDeleted = false;
	this->cell[index].isEmpty = false;
	this->size++;
}

int HashA::findElementA( const string& key) const
{
	int index = hashFunc(key, this->capacity);
	int i = 0;
	const int orig = index;
	
	while (i < this->capacity){
		if (this->cell[index].isEmpty == true && this->cell[index].isDeleted == false){
			return -1;
		}
		if (this->cell[index].isEmpty == false && this->cell[index].isDeleted == false){
			if (this->cell[index].key == key){
				return this->cell[index].data;
			}
		}	
		i++;
		index = (orig + i) % this->capacity;
	}
	return -1;
}

void HashA::deleteElementA( const string& key){
	int index = hashFunc(key, this->capacity);
	int i = 0;
	const int orig = index;
	
	while (i < this->capacity){
		if (this->cell[index].isEmpty == true && this->cell[index].isDeleted == false){
			cout << "Элемент '" << key << "' не найден!" << endl;
            return;
		}
		if (this->cell[index].isEmpty == false && this->cell[index].isDeleted == false){
			if (this->cell[index].key == key){
				this->cell[index].isDeleted = true;
				this->size--;
				cout << "Элемент '" << key << "' успешно удален!" << endl;
            	return;
			}
		}	
		i++;
		index = (orig + i) % this->capacity;
	}
}

void HashA::printHashTableA() const
{
    cout << "Хэш-таблица с открытой адресацией (size: " << this->size << ", capacity: " << this->capacity << "):" << endl;
    
    for (int i = 0; i < this->capacity; i++) {
        cout << "Ячейка " << i << ": ";
        
        if (this->cell[i].isEmpty) {
            if (this->cell[i].isDeleted) {
                cout << "[DELETED]";
            } else {
                cout << "empty";
            }
        } else {
            cout << "[" << this->cell[i].key << " -> " << this->cell[i].data << "]";
            if (this->cell[i].isDeleted) {
                cout << " (deleted)";
            }
        }
        cout << endl;
    }
}
