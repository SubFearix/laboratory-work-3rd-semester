#include <iostream>
#include <string>
#include "structures.h"

using namespace std;

int Hash::hashFunc(const string& key, const int capacity){
    int hash = 0;
    for (const char c: key) {
        hash = (hash * 37 + c) % capacity;
    }
    return hash % capacity;
}

void Hash::addElement(const string& key, const int data){
    const int index = hashFunc(key, this->capacity);
    NodeHash* begin = this->cell[index];
    NodeHash* current = begin;
    
    // Проверяем, существует ли уже ключ
    while (current != nullptr){
        if (current->getKey() == key){
            current->data = data; // Прямое обращение, так как мы friend
            return;
        }
        current = current->getNext();
    }
    
    // Создаем новый узел
    auto* node = new NodeHash(key, data);
    node->next = begin;
    node->prev = nullptr;
    
    if (begin != nullptr) {
        begin->prev = node;
    }
    
    this->cell[index] = node;
    this->size++;
}

int Hash::findElement(const string& key) const
{
    const int index = hashFunc(key, this->capacity);
    NodeHash* current = this->cell[index];
    
    while (current != nullptr){
        if (current->getKey() == key){
            return current->getData();
        }
        current = current->getNext();
    }
    return -1;
}

void Hash::deleteElement(const string& key){
    const int index = hashFunc(key, this->capacity);
    NodeHash* current = this->cell[index];
    
    while (current != nullptr){
        if (current->getKey() == key){
            // Обновляем указатели соседних узлов
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else {
                this->cell[index] = current->next;
            }
            
            if (current->next != nullptr) {
                current->next->prev = current->prev;
            }
            
            delete current;
            this->size--;
            return;
        }
        current = current->getNext();
    }
}

void Hash::printHashTable() const
{
    cout << "Хэш-таблица (size: " << this->size << ", capacity: " << this->capacity << "):" << endl;
    
    for (int i = 0; i < this->capacity; i++) {
        cout << "Ячейка " << i << ": ";

        NodeHash* current = this->cell[i];
        if (current == nullptr) {
            cout << "empty";
        } else {
            bool first = true;
            while (current != nullptr) {
                if (!first) {
                    cout << " -> ";
                }
                cout << "[" << current->getKey() << " -> " << current->getData() << "]";
                current = current->getNext();
                first = false;
            }
        }
        cout << endl;
    }
}

Hash::Hash(const Hash& other) {
    this->capacity = other.capacity;
    this->size = 0;
    this->cell = new NodeHash*[this->capacity];
    
    for (int i = 0; i < this->capacity; i++) {
        this->cell[i] = nullptr;
    }
    
    for (int i = 0; i < other.capacity; i++) {
        NodeHash* current = other.cell[i];
        while (current != nullptr) {
            this->addElement(current->getKey(), current->getData());
            current = current->getNext();
        }
    }
}

Hash& Hash::operator=(const Hash& other) {
    if (this == &other) {
        return *this;
    }
    
    for (int i = 0; i < capacity; i++) {
        NodeHash* current = cell[i];
        while (current != nullptr) {
            NodeHash* next = current->getNext();
            delete current;
            current = next;
        }
    }
    delete[] cell;
    this->capacity = other.capacity;
    this->size = 0;
    this->cell = new NodeHash*[this->capacity];
    
    for (int i = 0; i < this->capacity; i++) {
        this->cell[i] = nullptr;
    }
    
    for (int i = 0; i < other.capacity; i++) {
        NodeHash* current = other.cell[i];
        while (current != nullptr) {
            this->addElement(current->getKey(), current->getData());
            current = current->getNext();
        }
    }
    
    return *this;
}
