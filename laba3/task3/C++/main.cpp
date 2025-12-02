#include <iostream>
#include <cassert>
#include "structures.h"

using namespace std;

void testArraySerialization() {
    Array arr;
    arr.addBackArray("элемент1");
    arr.addBackArray("элемент2");
    arr.addBackArray("элемент3");
    arr.addBackArray("test");
    
    cout << "Исходный массив: ";
    arr.printArray();
    serializeArray(arr, "array.txt", false);
    Array arrText = deserializeArray("array.txt", false);
    cout << "Десериализованный массив (текст): ";
    arrText.printArray();
    
    assert(arr.getSizeArray() == arrText.getSizeArray());
    for (int i = 0; i < arr.getSizeArray(); i++) {
        assert(arr.getElemIndArray(i) == arrText.getElemIndArray(i));
    }
    cout << "Текстовая сериализация прошла успешно" << endl;
    
    serializeArray(arr, "array.bin", true);
    Array arrBin = deserializeArray("array.bin", true);
    cout << "Десериализованный массив (бинарный): ";
    arrBin.printArray();
    
    assert(arr.getSizeArray() == arrBin.getSizeArray());
    for (int i = 0; i < arr.getSizeArray(); i++) {
        assert(arr.getElemIndArray(i) == arrBin.getElemIndArray(i));
    }
    cout << "Бинарная сериализация прошла успешно" << endl;
}

void testOLListSerialization() {
    OLList list;
    list.addBackOL("первый");
    list.addBackOL("второй");
    list.addBackOL("третий");
    
    cout << "Исходный список: ";
    list.printListOL();
    
    serializeOLList(list, "ollist.txt", false);
    OLList listText = deserializeOLList("ollist.txt", false);
    cout << "Десериализованный список (текст): ";
    listText.printListOL();
    cout << "Текстовая сериализация прошла успешно" << endl;
    
    serializeOLList(list, "ollist.bin", true);
    OLList listBin = deserializeOLList("ollist.bin", true);
    cout << "Десериализованный список (бинарный): ";
    listBin.printListOL();
    cout << "Бинарная сериализация прошла успешно" << endl;
}

void testTLListSerialization() {
    TLList list;
    list.addBackTL("A");
    list.addBackTL("B");
    list.addBackTL("C");
    list.addFrontTL("Start");
    
    cout << "Исходный двусвязный список: ";
    list.printListTL();
    
    serializeTLList(list, "tllist.txt", false);
    TLList listText = deserializeTLList("tllist.txt", false);
    cout << "Десериализованный список (текст): ";
    listText.printListTL();
    cout << "Текстовая сериализация прошла успешно" << endl;
    
    serializeTLList(list, "tllist.bin", true);
    TLList listBin = deserializeTLList("tllist.bin", true);
    cout << "Десериализованный список (бинарный): ";
    listBin.printListTL();
    cout << "Бинарная сериализация прошла успешно" << endl;
}

void testStackSerialization() {
	Stack stack;
    stack.pushStack("bottom");
    stack.pushStack("middle");
    stack.pushStack("top");
    
    cout << "Исходный стек: ";
    stack.printStack();

    serializeStack(stack, "stack.txt", false);
    Stack stackText = deserializeStack("stack.txt", false);
    cout << "Десериализованный стек (текст): ";
    stackText.printStack();
    cout << "Текстовая сериализация прошла успешно" << endl;
    
    serializeStack(stack, "stack.bin", true);
    Stack stackBin = deserializeStack("stack.bin", true);
    cout << "Десериализованный стек (бинарный): ";
    stackBin.printStack();
    cout << "Бинарная сериализация прошла успешно" << endl;
}

void testQueueSerialization() {
	Queue queue;
    queue.addBackQueue("first");
    queue.addBackQueue("second");
    queue.addBackQueue("third");
    
    cout << "Исходная очередь: ";
    queue.printQueue();
    
    serializeQueue(queue, "queue.txt", false);
    Queue queueText = deserializeQueue("queue.txt", false);
    cout << "Десериализованная очередь (текст): ";
    queueText.printQueue();
    cout << "Текстовая сериализация прошла успешно" << endl;
    
    serializeQueue(queue, "queue.bin", true);
    Queue queueBin = deserializeQueue("queue.bin", true);
    cout << "Десериализованная очередь (бинарный): ";
    queueBin.printQueue();
    cout << "Бинарная сериализация прошла успешно" << endl;
}

void testHashSerialization() {
    Hash hash(5);
    hash.addElement("ключ1", 100);
    hash.addElement("ключ2", 200);
    hash.addElement("key3", 300);
    hash.addElement("test", 42);
    
    cout << "Исходная хэш-таблица c цепочками:" << endl;
    hash.printHashTable();

    serializeHash(hash, "hash.txt", false);
    Hash hashText = deserializeHash("hash.txt", false);
    cout << "Десериализованная хэш-таблица (текст):" << endl;
    hashText.printHashTable();
    
    assert(hash.findElement("ключ1") == hashText.findElement("ключ1"));
    assert(hash.findElement("test") == hashText.findElement("test"));
    cout << "Текстовая сериализация прошла успешно" << endl;

    serializeHash(hash, "hash.bin", true);
    Hash hashBin = deserializeHash("hash.bin", true);
    cout << "Десериализованная хэш-таблица (бинарный):" << endl;
    hashBin.printHashTable();
    
    assert(hash.findElement("ключ2") == hashBin.findElement("ключ2"));
    assert(hash.findElement("key3") == hashBin.findElement("key3"));
    cout << "Бинарная сериализация прошла успешно" << endl;
}

void testHashASerialization() {
    HashA hashA(8);
    hashA.addElementA("alpha", 10);
    hashA.addElementA("beta", 20);
    hashA.addElementA("gamma", 30);
    hashA.addElementA("delta", 40);
    
    cout << "Исходная хэш-таблица с открытой адресацией:" << endl;
    hashA.printHashTableA();
    serializeHashA(hashA, "hasha.txt", false);
    HashA hashAText = deserializeHashA("hasha.txt", false);
    cout << "Десериализованная хэш-таблица (текст):" << endl;
    hashAText.printHashTableA();
    
    assert(hashA.findElementA("alpha") == hashAText.findElementA("alpha"));
    assert(hashA.findElementA("delta") == hashAText.findElementA("delta"));
    cout << "Текстовая сериализация прошла успешно" << endl;

    serializeHashA(hashA, "hasha.bin", true);
    HashA hashABin = deserializeHashA("hasha.bin", true);
    cout << "Десериализованная хэш-таблица (бинарный):" << endl;
    hashABin.printHashTableA();
    
    assert(hashA.findElementA("beta") == hashABin.findElementA("beta"));
    assert(hashA.findElementA("gamma") == hashABin.findElementA("gamma"));
    cout << "Бинарная сериализация прошла успешно" << endl;
}

void testRBTreeSerialization() {   
    RBTree tree;
    tree.insertNode(10);
    tree.insertNode(5);
    tree.insertNode(15);
    tree.insertNode(3);
    tree.insertNode(7);
    tree.insertNode(12);
    tree.insertNode(17);
    
    cout << "Исходное красно-черное дерево:" << endl;
    tree.printRBTree();
    serializeRBTree(tree, "rbtree.txt", false);
    RBTree treeText = deserializeRBTree("rbtree.txt", false);
    cout << "Десериализованное дерево (текст):" << endl;
    treeText.printRBTree();
    
    assert(tree.searchNode(5) != tree.getNilnode());
    assert(treeText.searchNode(5) != treeText.getNilnode());
    cout << "Текстовая сериализация прошла успешно" << endl;

    serializeRBTree(tree, "rbtree.bin", true);
    RBTree treeBin = deserializeRBTree("rbtree.bin", true);
    cout << "Десериализованное дерево (бинарный):" << endl;
    treeBin.printRBTree();
    
    assert(tree.searchNode(15) != tree.getNilnode());
    assert(treeBin.searchNode(15) != treeBin.getNilnode());
    cout << "Бинарная сериализация прошла успешно" << endl;
}

int main() {
    try {
        testArraySerialization();
        testOLListSerialization();
        testTLListSerialization();
        testStackSerialization();
        testQueueSerialization();
        testHashSerialization();
        testHashASerialization();
        testRBTreeSerialization();
        
        cout << "ВСЕ ТЕСТЫ ПРОШЛИ УСПЕШНО!" << endl;
        
    } catch (const exception& e) {
        cerr << "\nОШИБКА: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
