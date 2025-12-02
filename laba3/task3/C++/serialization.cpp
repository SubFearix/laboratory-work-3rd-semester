#include <fstream>
#include <string>
#include "structures.h"

using namespace std;


void serializeArray(const Array& arr, const string& filename, bool binary) {
    if (binary) {
        ofstream file(filename, ios::binary);
        int size = arr.getSizeArray();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        
        for (int i = 0; i < size; i++) {
            string elem = arr.getElemIndArray(i);
            size_t len = elem.length();
            file.write(reinterpret_cast<const char*>(&len), sizeof(len));
            file.write(elem.c_str(), len);
        }
        file.close();
    } else {
        ofstream file(filename);
        int size = arr.getSizeArray();
        file << size << "\n";
        
        for (int i = 0; i < size; i++) {
            file << arr.getElemIndArray(i) << "\n";
        }
        file.close();
    }
}

Array deserializeArray(const string& filename, bool binary) {
    Array arr;
    
    if (binary) {
        ifstream file(filename, ios::binary);
        int size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        
        for (int i = 0; i < size; i++) {
            size_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(len));
            string elem(len, '\0');
            file.read(&elem[0], len);
            arr.addBackArray(elem);
        }
        file.close();
    } else {
        ifstream file(filename);
        int size;
        file >> size;
        file.ignore();
        
        for (int i = 0; i < size; i++) {
            string elem;
            getline(file, elem);
            arr.addBackArray(elem);
        }
        file.close();
    }
    
    return arr;
}

void serializeOLList(const OLList& list, const string& filename, bool binary) {
    if (binary) {
        ofstream file(filename, ios::binary);
        Node* curr = list.getHead();
        int count = 0;

        Node* temp = curr;
        while (temp != nullptr) {
            count++;
            temp = temp->getNext();
        }
        
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        while (curr != nullptr) {
            string data = curr->getData();
            size_t len = data.length();
            file.write(reinterpret_cast<const char*>(&len), sizeof(len));
            file.write(data.c_str(), len);
            curr = curr->getNext();
        }
        file.close();
    } else {
        ofstream file(filename);
        Node* curr = list.getHead();
        int count = 0;
        
        Node* temp = curr;
        while (temp != nullptr) {
            count++;
            temp = temp->getNext();
        }
        
        file << count << "\n";
        
        while (curr != nullptr) {
            file << curr->getData() << "\n";
            curr = curr->getNext();
        }
        file.close();
    }
}

OLList deserializeOLList(const string& filename, bool binary) {
    OLList list;
    
    if (binary) {
        ifstream file(filename, ios::binary);
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for (int i = 0; i < count; i++) {
            size_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(len));
            string data(len, '\0');
            file.read(&data[0], len);
            list.addBackOL(data);
        }
        file.close();
    } else {
        ifstream file(filename);
        int count;
        file >> count;
        file.ignore();
        
        for (int i = 0; i < count; i++) {
            string data;
            getline(file, data);
            list.addBackOL(data);
        }
        file.close();
    }
    
    return list;
}

void serializeTLList(const TLList& list, const string& filename, bool binary) {
    if (binary) {
        ofstream file(filename, ios::binary);
        Node* curr = list.getHead();
        int count = 0;
        
        Node* temp = curr;
        while (temp != nullptr) {
            count++;
            temp = temp->getNext();
        }
        
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        while (curr != nullptr) {
            string data = curr->getData();
            size_t len = data.length();
            file.write(reinterpret_cast<const char*>(&len), sizeof(len));
            file.write(data.c_str(), len);
            curr = curr->getNext();
        }
        file.close();
    } else {
        ofstream file(filename);
        Node* curr = list.getHead();
        int count = 0;
        
        Node* temp = curr;
        while (temp != nullptr) {
            count++;
            temp = temp->getNext();
        }
        
        file << count << "\n";
        
        while (curr != nullptr) {
            file << curr->getData() << "\n";
            curr = curr->getNext();
        }
        file.close();
    }
}

TLList deserializeTLList(const string& filename, bool binary) {
    TLList list;
    
    if (binary) {
        ifstream file(filename, ios::binary);
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for (int i = 0; i < count; i++) {
            size_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(len));
            string data(len, '\0');
            file.read(&data[0], len);
            list.addBackTL(data);
        }
        file.close();
    } else {
        ifstream file(filename);
        int count;
        file >> count;
        file.ignore();
        
        for (int i = 0; i < count; i++) {
            string data;
            getline(file, data);
            list.addBackTL(data);
        }
        file.close();
    }
    
    return list;
}


void serializeStack(const Stack& stack, const string& filename, bool binary) {
    if (binary) {
        ofstream file(filename, ios::binary);
        Node* curr = stack.getTop();
        int count = 0;
        
        Node* temp = curr;
        while (temp != nullptr) {
            count++;
            temp = temp->getNext();
        }
        
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        while (curr != nullptr) {
            string data = curr->getData();
            size_t len = data.length();
            file.write(reinterpret_cast<const char*>(&len), sizeof(len));
            file.write(data.c_str(), len);
            curr = curr->getNext();
        }
        file.close();
    } else {
        ofstream file(filename);
        Node* curr = stack.getTop();
        int count = 0;
        
        Node* temp = curr;
        while (temp != nullptr) {
            count++;
            temp = temp->getNext();
        }
        
        file << count << "\n";
        
        while (curr != nullptr) {
            file << curr->getData() << "\n";
            curr = curr->getNext();
        }
        file.close();
    }
}

Stack deserializeStack(const string& filename, bool binary) {
    Stack stack;
    
    if (binary) {
        ifstream file(filename, ios::binary);
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        string* temp = new string[count];
        for (int i = 0; i < count; i++) {
            size_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(len));
            temp[i].resize(len);
            file.read(&temp[i][0], len);
        }
        
        for (int i = count - 1; i >= 0; i--) {
            stack.pushStack(temp[i]);
        }
        
        delete[] temp;
        file.close();
    } else {
        ifstream file(filename);
        int count;
        file >> count;
        file.ignore();
        
        string* temp = new string[count];
        for (int i = 0; i < count; i++) {
            getline(file, temp[i]);
        }
        
        for (int i = count - 1; i >= 0; i--) {
            stack.pushStack(temp[i]);
        }
        
        delete[] temp;
        file.close();
    }
    
    return stack;
}

void serializeQueue(const Queue& queue, const string& filename, bool binary) {
    if (binary) {
        ofstream file(filename, ios::binary);
        Node* curr = queue.getHead();
        int count = 0;
        
        Node* temp = curr;
        while (temp != nullptr) {
            count++;
            temp = temp->getNext();
        }
        
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        while (curr != nullptr) {
            string data = curr->getData();
            size_t len = data.length();
            file.write(reinterpret_cast<const char*>(&len), sizeof(len));
            file.write(data.c_str(), len);
            curr = curr->getNext();
        }
        file.close();
    } else {
        ofstream file(filename);
        Node* curr = queue.getHead();
        int count = 0;
        
        Node* temp = curr;
        while (temp != nullptr) {
            count++;
            temp = temp->getNext();
        }
        
        file << count << "\n";
        
        while (curr != nullptr) {
            file << curr->getData() << "\n";
            curr = curr->getNext();
        }
        file.close();
    }
}

Queue deserializeQueue(const string& filename, bool binary) {
    Queue queue;
    
    if (binary) {
        ifstream file(filename, ios::binary);
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for (int i = 0; i < count; i++) {
            size_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(len));
            string data(len, '\0');
            file.read(&data[0], len);
            queue.addBackQueue(data);
        }
        file.close();
    } else {
        ifstream file(filename);
        int count;
        file >> count;
        file.ignore();
        
        for (int i = 0; i < count; i++) {
            string data;
            getline(file, data);
            queue.addBackQueue(data);
        }
        file.close();
    }
    
    return queue;
}


void serializeHash(const Hash& hash, const string& filename, bool binary) {
    if (binary) {
        ofstream file(filename, ios::binary);
        int capacity = hash.getCapacity();
        
        file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
        
        NodeHash** cells = hash.getCells();
        int totalPairs = 0;

        for (int i = 0; i < capacity; i++) {
            NodeHash* curr = cells[i];
            while (curr != nullptr) {
                totalPairs++;
                curr = curr->getNext();
            }
        }
        
        file.write(reinterpret_cast<const char*>(&totalPairs), sizeof(totalPairs));
        
        for (int i = 0; i < capacity; i++) {
            NodeHash* curr = cells[i];
            while (curr != nullptr) {
                string key = curr->getKey();
                int data = curr->getData();
                size_t keyLen = key.length();
                
                file.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
                file.write(key.c_str(), keyLen);
                file.write(reinterpret_cast<const char*>(&data), sizeof(data));
                
                curr = curr->getNext();
            }
        }
        file.close();
    } else {
        ofstream file(filename);
        int capacity = hash.getCapacity();
        
        file << capacity << "\n";
        
        NodeHash** cells = hash.getCells();
        int totalPairs = 0;
        
        for (int i = 0; i < capacity; i++) {
            NodeHash* curr = cells[i];
            while (curr != nullptr) {
                totalPairs++;
                curr = curr->getNext();
            }
        }
        
        file << totalPairs << "\n";
        for (int i = 0; i < capacity; i++) {
            NodeHash* curr = cells[i];
            while (curr != nullptr) {
                file << curr->getKey() << "\n";
                file << curr->getData() << "\n";
                curr = curr->getNext();
            }
        }
        file.close();
    }
}

Hash deserializeHash(const string& filename, bool binary) {
    if (binary) {
        ifstream file(filename, ios::binary);
        int capacity;
        
        file.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
        
        Hash hash(capacity);
        
        int totalPairs;
        file.read(reinterpret_cast<char*>(&totalPairs), sizeof(totalPairs));
        
        for (int i = 0; i < totalPairs; i++) {
            size_t keyLen;
            file.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));
            string key(keyLen, '\0');
            file.read(&key[0], keyLen);
            
            int data;
            file.read(reinterpret_cast<char*>(&data), sizeof(data));
            
            hash.addElement(key, data);
        }
        
        file.close();
        return hash;
    } else {
        ifstream file(filename);
        int capacity;
        
        file >> capacity;
        
        Hash hash(capacity);
        
        int totalPairs;
        file >> totalPairs;
        file.ignore();
        
        for (int i = 0; i < totalPairs; i++) {
            string key;
            int data;
            getline(file, key);
            file >> data;
            file.ignore();
            
            hash.addElement(key, data);
        }
        
        file.close();
        return hash;
    }
}

void serializeHashA(const HashA& hash, const string& filename, bool binary) {
    if (binary) {
        ofstream file(filename, ios::binary);
        int capacity = hash.getCapacity();
        
        file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
        
        const NodeHashA* cells = hash.getCells();
        int totalPairs = 0;

        for (int i = 0; i < capacity; i++) {
            if (!cells[i].getIsEmpty() && !cells[i].getIsDeleted()) {
                totalPairs++;
            }
        }
        
        file.write(reinterpret_cast<const char*>(&totalPairs), sizeof(totalPairs));

        for (int i = 0; i < capacity; i++) {
            if (!cells[i].getIsEmpty() && !cells[i].getIsDeleted()) {
                string key = cells[i].getKey();
                int data = cells[i].getData();
                size_t keyLen = key.length();
                
                file.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
                file.write(key.c_str(), keyLen);
                file.write(reinterpret_cast<const char*>(&data), sizeof(data));
            }
        }
        file.close();
    } else {
        ofstream file(filename);
        int capacity = hash.getCapacity();
        
        file << capacity << "\n";
        const NodeHashA* cells = hash.getCells();
        int totalPairs = 0;
        for (int i = 0; i < capacity; i++) {
            if (!cells[i].getIsEmpty() && !cells[i].getIsDeleted()) {
                totalPairs++;
            }
        }
        
        file << totalPairs << "\n";
        
        for (int i = 0; i < capacity; i++) {
            if (!cells[i].getIsEmpty() && !cells[i].getIsDeleted()) {
                file << cells[i].getKey() << "\n";
                file << cells[i].getData() << "\n";
            }
        }
        file.close();
    }
}

HashA deserializeHashA(const string& filename, bool binary) {
    if (binary) {
        ifstream file(filename, ios::binary);
        int capacity;
        
        file.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
        
        HashA hash(capacity);
        
        int totalPairs;
        file.read(reinterpret_cast<char*>(&totalPairs), sizeof(totalPairs));
        
        for (int i = 0; i < totalPairs; i++) {
            size_t keyLen;
            file.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));
            string key(keyLen, '\0');
            file.read(&key[0], keyLen);
            
            int data;
            file.read(reinterpret_cast<char*>(&data), sizeof(data));
            
            hash.addElementA(key, data);
        }
        
        file.close();
        return hash;
    } else {
        ifstream file(filename);
        int capacity;
        
        file >> capacity;
        
        HashA hash(capacity);
        
        int totalPairs;
        file >> totalPairs;
        file.ignore();
        
        for (int i = 0; i < totalPairs; i++) {
            string key;
            int data;
            getline(file, key);
            file >> data;
            file.ignore();
            
            hash.addElementA(key, data);
        }
        
        file.close();
        return hash;
    }
}

void serializeRBTreeHelper(const NodeRB* node, const NodeRB* nilnode, ofstream& file, bool binary) {
    if (node == nilnode) {
        if (binary) {
            bool isNil = true;
            file.write(reinterpret_cast<const char*>(&isNil), sizeof(isNil));
        } else {
            file << "nil\n";
        }
        return;
    }
    
    if (binary) {
        bool isNil = false;
        file.write(reinterpret_cast<const char*>(&isNil), sizeof(isNil));
        
        int data = node->getData();
        int color = static_cast<int>(node->getColor());
        
        file.write(reinterpret_cast<const char*>(&data), sizeof(data));
        file.write(reinterpret_cast<const char*>(&color), sizeof(color));
    } else {
        file << "node\n";
        file << node->getData() << "\n";
        file << static_cast<int>(node->getColor()) << "\n";
    }
    
    serializeRBTreeHelper(node->getLeft(), nilnode, file, binary);
    serializeRBTreeHelper(node->getRight(), nilnode, file, binary);
}

void serializeRBTree(const RBTree& tree, const string& filename, bool binary) {
    if (binary) {
        ofstream file(filename, ios::binary);
        serializeRBTreeHelper(tree.getRoot(), tree.getNilnode(), file, binary);
        file.close();
    } else {
        ofstream file(filename);
        serializeRBTreeHelper(tree.getRoot(), tree.getNilnode(), file, binary);
        file.close();
    }
}

NodeRB* deserializeRBTreeHelper(RBTree& tree, ifstream& file, bool binary) {
    if (binary) {
        bool isNil;
        file.read(reinterpret_cast<char*>(&isNil), sizeof(isNil));
        
        if (isNil) {
            return tree.getNilnode();
        }
        
        int data, colorInt;
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
        file.read(reinterpret_cast<char*>(&colorInt), sizeof(colorInt));
        
        Color color = static_cast<Color>(colorInt);
        auto* node = new NodeRB(data, color, tree.getNilnode(), tree.getNilnode(), tree.getNilnode());
        
        node->setLeft(deserializeRBTreeHelper(tree, file, binary));
        if (node->getLeft() != tree.getNilnode()) {
            node->getLeft()->setParent(node);
        }
        
        node->setRight(deserializeRBTreeHelper(tree, file, binary));
        if (node->getRight() != tree.getNilnode()) {
            node->getRight()->setParent(node);
        }
        
        return node;
    } else {
        string marker;
        getline(file, marker);
        
        if (marker == "nil") {
            return tree.getNilnode();
        }
        
        int data, colorInt;
        file >> data >> colorInt;
        file.ignore();
        
        Color color = static_cast<Color>(colorInt);
        auto* node = new NodeRB(data, color, tree.getNilnode(), tree.getNilnode(), tree.getNilnode());
        
        node->setLeft(deserializeRBTreeHelper(tree, file, binary));
        if (node->getLeft() != tree.getNilnode()) {
            node->getLeft()->setParent(node);
        }
        
        node->setRight(deserializeRBTreeHelper(tree, file, binary));
        if (node->getRight() != tree.getNilnode()) {
            node->getRight()->setParent(node);
        }
        
        return node;
    }
}

RBTree deserializeRBTree(const string& filename, bool binary) {
    RBTree tree;
    
    if (binary) {
        ifstream file(filename, ios::binary);
        tree.setRoot(deserializeRBTreeHelper(tree, file, binary));
        file.close();
    } else {
        ifstream file(filename);
        tree.setRoot(deserializeRBTreeHelper(tree, file, binary));
        file.close();
    }
    
    return tree;
}
