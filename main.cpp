#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "massiv.h"
#include "queue.h"
#include "stack.h"
#include "onelinkedlist.h"
#include "twolinkedlist.h"
#include "rbtree.h"
#include "structures.h"

using namespace std;


map<string, Array*> arrays;
map<string, Stack*> stacks;
map<string, Queue*> queues;
map<string, OLList*> olists;
map<string, TLList*> tlists;
map<string, RBTree*> rbtrees;

vector<string> splitString(const string& str) {
    vector<string> tokens;
    istringstream iss(str);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void deleteSubtree(RBTree* rbt, NodeRB* node) { // удаление потомков
    if (node == rbt->nilnode) return;
    deleteSubtree(rbt, node->left);
    deleteSubtree(rbt, node->right);
    delete node;
}

void deleteRBTree(RBTree* rbt) {
    if (rbt) {
        deleteSubtree(rbt, rbt->root);
        delete rbt->nilnode;
        delete rbt;
    }
}


string toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

void loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> tokens = splitString(line);
        if (tokens.size() < 3) continue;

        string type = tokens[0];
        string name = tokens[1];

        if (type == "ARRAY") {
            Array* arr = createArray();
            int size = stoi(tokens[2]);
            for (int i = 0; i < size && i + 3 < tokens.size(); i++) {
                string value = tokens[i + 3];
                addBackArray(arr, value);
            }
            arrays[name] = arr;
        }
        else if (type == "STACK") {
            Stack* st = createEmptyStack();
            for (size_t i = 2; i < tokens.size(); i++) {
                string value = tokens[i];
                pushStack(st, value);
            }
            stacks[name] = st;
        }
        else if (type == "QUEUE") {
            Queue* que = createEmptyQueue();
            for (size_t i = 2; i < tokens.size(); i++) {
                string value = tokens[i];
                addBackQueue(que, value);
            }
            queues[name] = que;
        }
        else if (type == "OLIST") {
            OLList* lst = createEmptyOLList();
            for (size_t i = 2; i < tokens.size(); i++) {
                string value = tokens[i];
                addBackOL(lst, value);
            }
            olists[name] = lst;
        }
        else if (type == "TLIST") {
            TLList* lst = createEmptyTLList();
            for (size_t i = 2; i < tokens.size(); i++) {
                string value = tokens[i];
                addBackTL(lst, value);
            }
            tlists[name] = lst;
        }
        else if (type == "RBTREE") {
			if (rbtrees.find(name) != rbtrees.end()) {
				deleteRBTree(rbtrees[name]);
			}
			rbtrees[name] = createEmptyRBTree();
			
			// Собираем все данные дерева из оставшихся токенов
			string treeData;
			for (size_t i = 2; i < tokens.size(); i++) {
				treeData += tokens[i] + " ";
			}
			
			// Если данных недостаточно, читаем следующую строку
			if (treeData.empty() || treeData.find("N") == string::npos) {
				string nextLine;
				if (getline(file, nextLine)) {
				    treeData += nextLine;
				}
			}
			
			istringstream treeStream(treeData);
			loadRBTreeFromStream(rbtrees[name], treeStream);
		}
    }
    file.close();
}

void saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Невозможно открыть файл " << filename << " для записи" << endl;
        return;
    }

    for (const auto& pair: arrays) {
        file << "ARRAY " << pair.first << " " << pair.second->size;
        for (int i = 0; i < pair.second->size; i++) {
            file << " " << getElemIndArray(pair.second, i);
        }
        file << "\n";
    }

    for (const auto& pair: stacks) {
        file << "STACK " << pair.first;
        Stack* temp = createEmptyStack();
        Node* curr = pair.second->top;
        while (curr != nullptr) {
            pushStack(temp, curr->data);
            curr = curr->next;
        }
        curr = temp->top;
        while (curr != nullptr) {
            file << " " << curr->data;
            curr = curr->next;
        }
        while (temp->top != nullptr) {
            popStack(temp);
        }
        delete temp;
        file << "\n";
    }

    for (const auto& pair: queues) {
        file << "QUEUE " << pair.first;
        Node* curr = pair.second->head;
        while (curr != nullptr) {
            file << " " << curr->data;
            curr = curr->next;
        }
        file << "\n";
    }

    for (const auto& pair: olists) {
        file << "OLIST " << pair.first;
        Node* curr = pair.second->head;
        while (curr != nullptr) {
            file << " " << curr->data;
            curr = curr->next;
        }
        file << "\n";
    }

    for (const auto& pair: tlists) {
        file << "TLIST " << pair.first;
        Node* curr = pair.second->head;
        while (curr != nullptr) {
            file << " " << curr->data;
            curr = curr->next;
        }
        file << "\n";
    }
    for (const auto& pair: rbtrees) {
    file << "RBTREE " << pair.first;
    if (pair.second->root != pair.second->nilnode) {
        file << " ";
        saveRBTreeToStream(pair.second, file);
    }
    file << "\n";
}

    file.close();
}

enum CommandType {
    CMD_PRINT,
    CMD_APUSH,
    CMD_ACREATE,
    CMD_AINSERT,
    CMD_ADEL,
    CMD_AGET,
    CMD_ASET,
    CMD_ASIZE,
    CMD_APUSHFRONT,
    CMD_APOP,
    
    CMD_SCREATE,
    CMD_SPUSH,
    CMD_SPOP,
    
    CMD_QCREATE,
    CMD_QPUSH,
    CMD_QPOP,
    
    CMD_FCREATE,
    CMD_FINSERTBEFORE,
    CMD_FINSERTAFTER,
    CMD_FPUSH,
    CMD_FPUSHFRONT,
    CMD_FDELHEAD,
    CMD_FDELTAIL,
    CMD_FDELBEFORE,
    CMD_FDELAFTER,
    CMD_FDELVALUE,
    CMD_FSEARCH,
    
    CMD_LCREATE,
    CMD_LINSERTBEFORE,
    CMD_LINSERTAFTER,
    CMD_LPUSH,
    CMD_LPUSHFRONT,
    CMD_LDELHEAD,
    CMD_LDELTAIL,
    CMD_LDELBEFORE,
    CMD_LDELAFTER,
    CMD_LDELVALUE,
    CMD_LSEARCH,
    
    CMD_RBCREATE,
    CMD_RBINSERT, 
    CMD_RBDELETE,
    CMD_RBSEARCH,
    
    CMD_CLEAR,
    CMD_UNKNOWN
};
void cleanupAll() {
    for (auto& pair: arrays) {
        delete[] pair.second->data;
        delete pair.second;
    }
    arrays.clear();

    for (auto& pair: stacks) {
        while (pair.second->top != nullptr) {
            popStack(pair.second);
        }
        delete pair.second;
    }
    stacks.clear();

    for (auto& pair: queues) {
        while (pair.second->head != nullptr) {
            remFrontQueue(pair.second);
        }
        delete pair.second;
    }
    queues.clear();

    for (auto& pair: olists) {
        while (pair.second->head != nullptr) {
            remFrontOL(pair.second);
        }
        delete pair.second;
    }
    olists.clear();

    for (auto& pair: tlists) {
        while (pair.second->head != nullptr) {
            remFrontTL(pair.second);
        }
        delete pair.second;
    }
    tlists.clear();
    
    for (auto& pair: rbtrees) {
        deleteRBTree(pair.second);
    }
    rbtrees.clear();
}

CommandType getCommandType(const string& command) {
    if (command == "PRINT") return CMD_PRINT;
    if (command == "APUSH") return CMD_APUSH;
    if (command == "ACREATE") return CMD_ACREATE;
    if (command == "AINSERT") return CMD_AINSERT;
    if (command == "ADEL") return CMD_ADEL;
    if (command == "AGET") return CMD_AGET;
    if (command == "ASET") return CMD_ASET;
    if (command == "ASIZE") return CMD_ASIZE;
    if (command == "APUSHFRONT") return CMD_APUSHFRONT;
    if (command == "APOP") return CMD_APOP;
    
    if (command == "SCREATE") return CMD_SCREATE;
    if (command == "SPUSH") return CMD_SPUSH;
    if (command == "SPOP") return CMD_SPOP;
    
    if (command == "QCREATE") return CMD_QCREATE;
    if (command == "QPUSH") return CMD_QPUSH;
    if (command == "QPOP") return CMD_QPOP;
    
    
    if (command == "FCREATE") return CMD_FCREATE;
    if (command == "FINSERTBEFORE") return CMD_FINSERTBEFORE;
    if (command == "FINSERTAFTER") return CMD_FINSERTAFTER;
    if (command == "FCREATE") return CMD_FCREATE;
    if (command == "FPUSH") return CMD_FPUSH;
    if (command == "FPUSHFRONT") return CMD_FPUSHFRONT;
    if (command == "FDELHEAD") return CMD_FDELHEAD;
    if (command == "FDELTAIL") return CMD_FDELTAIL;
    if (command == "FDELBEFORE") return CMD_FDELBEFORE;
    if (command == "FDELAFTER") return CMD_FDELAFTER;
    if (command == "FDELVALUE") return CMD_FDELVALUE;
    if (command == "FSEARCH") return CMD_FSEARCH;
    
    if (command == "LCREATE") return CMD_LCREATE;
    if (command == "LINSERTBEFORE") return CMD_LINSERTBEFORE;
    if (command == "LINSERTAFTER") return CMD_LINSERTAFTER;
    if (command == "LCREATE") return CMD_LCREATE;
    if (command == "LPUSH") return CMD_LPUSH;
    if (command == "LPUSHFRONT") return CMD_LPUSHFRONT;
    if (command == "LDELHEAD") return CMD_LDELHEAD;
    if (command == "LDELTAIL") return CMD_LDELTAIL;
    if (command == "LDELBEFORE") return CMD_LDELBEFORE;
    if (command == "LDELAFTER") return CMD_LDELAFTER;
    if (command == "LDELVALUE") return CMD_LDELVALUE;
    if (command == "LSEARCH") return CMD_LSEARCH;
    
    if (command == "RBCREATE") return CMD_RBCREATE;
	if (command == "RBINSERT") return CMD_RBINSERT;
	if (command == "RBDELETE") return CMD_RBDELETE;
	if (command == "RBSEARCH") return CMD_RBSEARCH;
	if (command == "CLEAR") return CMD_CLEAR;
    return CMD_UNKNOWN;
}

bool executeQuery(const string& query, const string& filename) {
    vector<string> tokens = splitString(query);
    if (tokens.empty()) return false;

    string commandStr = toUpper(tokens[0]);
    CommandType command = getCommandType(commandStr);

    switch (command) {
        case CMD_PRINT: {
            if (tokens.size() == 1) {
                for (const auto& pair: arrays) {
                    cout << "ARRAY " << pair.first << ": ";
                    printArray(pair.second);
                }
                for (const auto& pair: stacks) {
                    cout << "STACK " << pair.first << ": ";
                    printStack(pair.second);
                }
                for (const auto& pair: queues) {
                    cout << "QUEUE " << pair.first << ": ";
                    printQueue(pair.second);
                }
                for (const auto& pair: olists) {
                    cout << "OLIST " << pair.first << ": ";
                    printListOL(pair.second);
                }
                for (const auto& pair: tlists) {
                    cout << "TLIST " << pair.first << ": ";
                    printListTL(pair.second);
                }
                for (const auto& pair: rbtrees) {
				    cout << "RBTREE " << pair.first << ": ";
				    printRBTree(pair.second);
				}
            } else {
                string name = tokens[1];
                if (arrays.find(name) != arrays.end()) {
                    printArray(arrays[name]);
                } else if (stacks.find(name) != stacks.end()) {
                    printStack(stacks[name]);
                } else if (queues.find(name) != queues.end()) {
                    printQueue(queues[name]);
                } else if (olists.find(name) != olists.end()) {
                    printListOL(olists[name]);
                } else if (tlists.find(name) != tlists.end()) {
                    printListTL(tlists[name]); 
                } else if (rbtrees.find(name) != rbtrees.end()) {
                    printRBTree(rbtrees[name]);
                } else {
                    cout << "Структура не найдена: " << name << endl;
                    return false;
                }
            }
            return true;
        }

        case CMD_ACREATE: {
            if (tokens.size() < 2) {
                cout << "Ошибка: ACREATE требует имя массива" << endl;
                return false;
            }
            string name = tokens[1];
            if (arrays.find(name) != arrays.end()) {
                cout << "Массив '" << name << "' уже существует" << endl;
                return false;
            }
            arrays[name] = createArray();
            cout << "Массив '" << name << "' успешно создан" << endl;
            return true;
        }

        case CMD_APUSH: {
            if (tokens.size() < 3) {
                cout << "Ошибка: APUSH требует имя массива и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (arrays.find(name) == arrays.end()) {
                arrays[name] = createArray();
            }
            string value = tokens[2];
            addBackArray(arrays[name], value);
            cout << value << endl;
            return true;
        }

        case CMD_APUSHFRONT: {
            if (tokens.size() < 3) {
                cout << "Ошибка: APUSHFRONT требует имя массива и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (arrays.find(name) == arrays.end()) {
                arrays[name] = createArray();
            }
            string value = tokens[2];
            addElemIndArray(arrays[name], 0, value);
            cout << value << endl;
            return true;
        }

        case CMD_APOP: {
            if (tokens.size() < 2) {
                cout << "Ошибка: APOP требует имя массива" << endl;
                return false;
            }
            string name = tokens[1];
            if (arrays.find(name) == arrays.end() || arrays[name]->size == 0) {
                cout << "Ошибка: Массив пуст или не существует" << endl;
                return false;
            }
            string value = getElemIndArray(arrays[name], arrays[name]->size - 1);
            remElemIndArray(arrays[name], arrays[name]->size - 1);
            cout << value << endl;
            return true;
        }

        case CMD_AINSERT: {
            if (tokens.size() < 4) {
                cout << "Ошибка: AINSERT требует имя массива, индекс и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (arrays.find(name) == arrays.end()) {
                arrays[name] = createArray();
            }
            int index = stoi(tokens[2]);
            string value = tokens[3];
            addElemIndArray(arrays[name], index, value);
            cout << value << endl;
            return true;
        }

        case CMD_ADEL: {
            if (tokens.size() < 3) {
                cout << "Ошибка: ADEL требует имя массива и индекс" << endl;
                return false;
            }
            string name = tokens[1];
            if (arrays.find(name) == arrays.end()) {
                cout << "Ошибка: Массив не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            if (index < 0 || index >= arrays[name]->size) {
                cout << "Ошибка: Индекс вне границ" << endl;
                return false;
            }
            string value = getElemIndArray(arrays[name], index);
            remElemIndArray(arrays[name], index);
            cout << value << endl;
            return true;
        }

        case CMD_AGET: {
            if (tokens.size() < 3) {
                cout << "Ошибка: AGET требует имя массива и индекс" << endl;
                return false;
            }
            string name = tokens[1];
            if (arrays.find(name) == arrays.end()) {
                cout << "Ошибка: Массив не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            string value = getElemIndArray(arrays[name], index);
            if (value == "") {
                cout << "Ошибка: Индекс вне границ" << endl;
                return false;
            }
            cout << value << endl;
            return true;
        }

        case CMD_ASET: {
            if (tokens.size() < 4) {
                cout << "Ошибка: ASET требует имя массива, индекс и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (arrays.find(name) == arrays.end()) {
                cout << "Ошибка: Массив не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            string value = tokens[3];
            changeElemIndArray(arrays[name], index, value);
            cout << value << endl;
            return true;
        }

        case CMD_ASIZE: {
            if (tokens.size() < 2) {
                cout << "Ошибка: ASIZE требует имя массива" << endl;
                return false;
            }
            string name = tokens[1];
            if (arrays.find(name) == arrays.end()) {
                cout << "Ошибка: Массив не существует" << endl;
                return false;
            }
            cout << arrays[name]->size << endl;
            return true;
        }

        case CMD_SCREATE: {
            if (tokens.size() < 2) {
                cout << "Ошибка: SCREATE требует имя стека" << endl;
                return false;
            }
            string name = tokens[1];
            if (stacks.find(name) != stacks.end()) {
                cout << "Стек '" << name << "' уже существует" << endl;
                return false;
            }
            stacks[name] = createEmptyStack();
            cout << "Стек '" << name << "' успешно создан" << endl;
            return true;
        }

        case CMD_SPUSH: {
            if (tokens.size() < 3) {
                cout << "Ошибка: SPUSH требует имя стека и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (stacks.find(name) == stacks.end()) {
                stacks[name] = createEmptyStack();
            }
            string value = tokens[2];
            pushStack(stacks[name], value);
            cout << value << endl;
            return true;
        }

        case CMD_SPOP: {
            if (tokens.size() < 2) {
                cout << "Ошибка: SPOP требует имя стека" << endl;
                return false;
            }
            string name = tokens[1];
            if (stacks.find(name) == stacks.end() || stacks[name]->top == nullptr) {
                cout << "Ошибка: Стек пуст или не существует" << endl;
                return false;
            }
            string value = stacks[name]->top->data;
            popStack(stacks[name]);
            cout << value << endl;
            return true;
        }

        case CMD_QCREATE: {
            if (tokens.size() < 2) {
                cout << "Ошибка: QCREATE требует имя очереди" << endl;
                return false;
            }
            string name = tokens[1];
            if (queues.find(name) != queues.end()) {
                cout << "Очередь '" << name << "' уже существует" << endl;
                return false;
            }
            queues[name] = createEmptyQueue();
            cout << "Очередь '" << name << "' успешно создана" << endl;
            return true;
        }

        case CMD_QPUSH: {
            if (tokens.size() < 3) {
                cout << "Ошибка: QPUSH требует имя очереди и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (queues.find(name) == queues.end()) {
                queues[name] = createEmptyQueue();
            }
            string value = tokens[2];
            addBackQueue(queues[name], value);
            cout << value << endl;
            return true;
        }

        case CMD_QPOP: {
            if (tokens.size() < 2) {
                cout << "Ошибка: QPOP требует имя очереди" << endl;
                return false;
            }
            string name = tokens[1];
            if (queues.find(name) == queues.end() || queues[name]->head == nullptr) {
                cout << "Ошибка: Очередь пуста или не существует" << endl;
                return false;
            }
            string value = queues[name]->head->data;
            remFrontQueue(queues[name]);
            cout << value << endl;
            return true;
        }

        case CMD_FCREATE: {
            if (tokens.size() < 2) {
                cout << "Ошибка: FCREATE требует имя списка" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) != olists.end()) {
                cout << "Односвязный список '" << name << "' уже существует" << endl;
                return false;
            }
            olists[name] = createEmptyOLList();
            cout << "Односвязный список '" << name << "' успешно создан" << endl;
            return true;
        }

        case CMD_FPUSH: {
            if (tokens.size() < 3) {
                cout << "Ошибка: FPUSH требует имя списка и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end()) {
                olists[name] = createEmptyOLList();
            }
            string value = tokens[2];
            addBackOL(olists[name], value);
            cout << value << endl;
            return true;
        }

        case CMD_FPUSHFRONT: {
            if (tokens.size() < 3) {
                cout << "Ошибка: FPUSHFRONT требует имя списка и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end()) {
                olists[name] = createEmptyOLList();
            }
            string value = tokens[2];
            addFrontOL(olists[name], value);
            cout << value << endl;
            return true;
        }

        case CMD_FINSERTAFTER: {
            if (tokens.size() < 4) {
                cout << "Ошибка: FINSERTAFTER требует имя списка, индекс и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            string value = tokens[3];
            insertAfterIndOL(olists[name], index, value);
            cout << value << endl;
            return true;
        }

        case CMD_FINSERTBEFORE: {
            if (tokens.size() < 4) {
                cout << "Ошибка: FINSERTBEFORE требует имя списка, индекс и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            string value = tokens[3];
            insertBeforeIndOL(olists[name], index, value);
            cout << value << endl;
            return true;
        }

        case CMD_FDELHEAD: {
            if (tokens.size() < 2) {
                cout << "Ошибка: FDELHEAD требует имя списка" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end() || olists[name]->head == nullptr) {
                cout << "Ошибка: Список пуст или не существует" << endl;
                return false;
            }
            remFrontOL(olists[name]);
            cout << "Голова успешно удалена" << endl;
            return true;
        }

        case CMD_FDELTAIL: {
            if (tokens.size() < 2) {
                cout << "Ошибка: FDELTAIL требует имя списка" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end() || olists[name]->head == nullptr) {
                cout << "Ошибка: Список пуст или не существует" << endl;
                return false;
            }
            remBackOL(olists[name]);
            cout << "Хвост успешно удален" << endl;
            return true;
        }

        case CMD_FDELVALUE: {
            if (tokens.size() < 3) {
                cout << "Ошибка: FDELVALUE требует имя списка и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            string value = tokens[2];
            remValOL(olists[name], value);
            cout << "Значение " << value << " удалено" << endl;
            return true;
        }

        case CMD_FDELBEFORE: {
            if (tokens.size() < 3) {
                cout << "Ошибка: FDELBEFORE требует имя списка и индекс" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            deleteBeforeIndOL(olists[name], index);
            cout << "Элемент перед индексом " << index << " удален" << endl;
            return true;
        }

        case CMD_FDELAFTER: {
            if (tokens.size() < 3) {
                cout << "Ошибка: FDELAFTER требует имя списка и индекс" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            deleteAfterIndOL(olists[name], index);
            cout << "Элемент после индекса " << index << " удален" << endl;
            return true;
        }

        case CMD_FSEARCH: {
            if (tokens.size() < 3) {
                cout << "Ошибка: FSEARCH требует имя списка и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (olists.find(name) == olists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            string value = tokens[2];
            int index = findValOL(olists[name], value);
            if (index == -1) {
                cout << "Значение не найдено" << endl;
            } else {
                cout << "Значение найдено по индексу: " << index << endl;
            }
            return true;
        }

        case CMD_LCREATE: {
            if (tokens.size() < 2) {
                cout << "Ошибка: LCREATE требует имя списка" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) != tlists.end()) {
                cout << "Двусвязный список '" << name << "' уже существует" << endl;
                return false;
            }
            tlists[name] = createEmptyTLList();
            cout << "Двусвязный список '" << name << "' успешно создан" << endl;
            return true;
        }

        case CMD_LPUSH: {
            if (tokens.size() < 3) {
                cout << "Ошибка: LPUSH требует имя списка и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end()) {
                tlists[name] = createEmptyTLList();
            }
            string value = tokens[2];
            addBackTL(tlists[name], value);
            cout << value << endl;
            return true;
        }

        case CMD_LPUSHFRONT: {
            if (tokens.size() < 3) {
                cout << "Ошибка: LPUSHFRONT требует имя списка и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end()) {
                tlists[name] = createEmptyTLList();
            }
            string value = tokens[2];
            addFrontTL(tlists[name], value);
            cout << value << endl;
            return true;
        }

        case CMD_LINSERTAFTER: {
            if (tokens.size() < 4) {
                cout << "Ошибка: LINSERTAFTER требует имя списка, индекс и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            string value = tokens[3];
            insertAfterIndTL(tlists[name], index, value);
            cout << value << endl;
            return true;
        }

        case CMD_LINSERTBEFORE: {
            if (tokens.size() < 4) {
                cout << "Ошибка: LINSERTBEFORE требует имя списка, индекс и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            string value = tokens[3];
            insertBeforeIndTL(tlists[name], index, value);
            cout << value << endl;
            return true;
        }

        case CMD_LDELHEAD: {
            if (tokens.size() < 2) {
                cout << "Ошибка: LDELHEAD требует имя списка" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end() || tlists[name]->head == nullptr) {
                cout << "Ошибка: Список пуст или не существует" << endl;
                return false;
            }
            remFrontTL(tlists[name]);
            cout << "Голова успешно удалена" << endl;
            return true;
        }

        case CMD_LDELTAIL: {
            if (tokens.size() < 2) {
                cout << "Ошибка: LDELTAIL требует имя списка" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end() || tlists[name]->head == nullptr) {
                cout << "Ошибка: Список пуст или не существует" << endl;
                return false;
            }
            remBackTL(tlists[name]);
            cout << "Хвост успешно удален" << endl;
            return true;
        }

        case CMD_LDELVALUE: {
            if (tokens.size() < 3) {
                cout << "Ошибка: LDELVALUE требует имя списка и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            string value = tokens[2];
            remValTL(tlists[name], value);
            cout << "Значение " << value << " удалено" << endl;
            return true;
        }

        case CMD_LDELBEFORE: {
            if (tokens.size() < 3) {
                cout << "Ошибка: LDELBEFORE требует имя списка и индекс" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            deleteBeforeIndTL(tlists[name], index);
            cout << "Элемент перед индексом " << index << " удален" << endl;
            return true;
        }

        case CMD_LDELAFTER: {
            if (tokens.size() < 3) {
                cout << "Ошибка: LDELAFTER требует имя списка и индекс" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            int index = stoi(tokens[2]);
            deleteAfterIndTL(tlists[name], index);
            cout << "Элемент после индекса " << index << " удален" << endl;
            return true;
        }

        case CMD_LSEARCH: {
            if (tokens.size() < 3) {
                cout << "Ошибка: LSEARCH требует имя списка и значение" << endl;
                return false;
            }
            string name = tokens[1];
            if (tlists.find(name) == tlists.end()) {
                cout << "Ошибка: Список не существует" << endl;
                return false;
            }
            string value = tokens[2];
            int index = findValTL(tlists[name], value);
            if (index == -1) {
                cout << "Значение не найдено" << endl;
            } else {
                cout << "Значение найдено по индексу: " << index << endl;
            }
            return true;
        }
        
        case CMD_RBCREATE: {
			if (tokens.size() != 2) {
				cout << "Ошибка: неверное количество аргументов для RBCREATE" << endl;
				return false;
			}
			string name = tokens[1];
			if (rbtrees.find(name) != rbtrees.end()) {
				cout << "Ошибка: дерево '" << name << "' уже существует" << endl;
				return false;
			}
			rbtrees[name] = createEmptyRBTree();
			cout << "Создание красно-черного дерева '" << name << "' успешно" << endl;
			return true;
		}
        
        case CMD_RBINSERT: {
			if (tokens.size() != 3) {
				cout << "Ошибка: неверное количество аргументов для RBINSERT" << endl;
				return false;
			}
			string name = tokens[1];
			if (rbtrees.find(name) == rbtrees.end()) {
				// Автоматически создаем дерево, если оно не существует
				rbtrees[name] = createEmptyRBTree();
				cout << "Дерево '" << name << "' автоматически создано" << endl;
			}
			int value = stoi(tokens[2]);
			insertNode(rbtrees[name], value);
			cout << "Вставка элемента " << value << " в дерево '" << name << "' успешна" << endl;
			return true;
		}
        case CMD_RBDELETE: {
            if (tokens.size() != 3) {
                cout << "Ошибка: неверное количество аргументов для RBDELETE" << endl;
                return false;
            }
            string name = tokens[1];
            if (rbtrees.find(name) == rbtrees.end()) {
                cout << "Ошибка: дерево '" << name << "' не существует" << endl;
                return false;
            }
            int value = stoi(tokens[2]);
            try {
                deleteNode(rbtrees[name], value);
                cout << "Удаление элемента " << value << " из дерева '" << name << "' успешно" << endl;
            } catch (const runtime_error& e) {
                cerr << "Ошибка при удалении: " << e.what() << endl;
            }
            return true;
        }
        case CMD_RBSEARCH: {
            if (tokens.size() != 3) {
                cout << "Ошибка: неверное количество аргументов для RBSEARCH" << endl;
                return false;
            }
            string name = tokens[1];
            if (rbtrees.find(name) == rbtrees.end()) {
                cout << "Ошибка: дерево '" << name << "' не существует" << endl;
                return false;
            }
            int value = stoi(tokens[2]);
            NodeRB* found = searchNode(rbtrees[name], value);
            if (found != rbtrees[name]->nilnode) {
                cout << "Элемент " << value << " найден в дереве '" << name << "'" << endl;
            } else {
                cout << "Элемент " << value << " не найден в дереве '" << name << "'" << endl;
            }
            return true;
        }

        case CMD_CLEAR: {
            cleanupAll();
            ofstream file(filename, ofstream::trunc);
            file.close();
            cout << "База данных успешно очищена" << endl;
            return true;
        }

        case CMD_UNKNOWN:
        default: {
            cout << "Ошибка: Неизвестная команда '" << commandStr << "'" << endl;
            return false;
        }
    }
}



void printUsage() {
    cout << "Использование: ./dbms --file <имя_файла> --query <команда>\n";
    cout << "Команды:\n";
    cout << "  ACREATE <имя>                - Создать новый массив\n";
    cout << "  APUSH <массив> <значение>    - Добавить в конец массива\n";
    cout << "  APUSHFRONT <массив> <значение>- Добавить в начало массива\n";
    cout << "  APOP <массив>                - Удалить из конца массива\n";
    cout << "  AINSERT <массив> <инд> <знач>- Вставить по индексу в массив\n";
    cout << "  ADEL <массив> <инд>          - Удалить по индексу из массива\n";
    cout << "  AGET <массив> <инд>          - Получить элемент по индексу\n";
    cout << "  ASET <массив> <инд> <знач>   - Установить элемент по индексу\n";
    cout << "  ASIZE <массив>               - Получить размер массива\n";
    cout << "  SCREATE <имя>                - Создать новый стек\n";
    cout << "  SPUSH <стек> <значение>      - Поместить в стек\n";
    cout << "  SPOP <стек>                  - Извлечь из стека\n";
    cout << "  QCREATE <имя>                - Создать новую очередь\n";
    cout << "  QPUSH <очередь> <значение>   - Добавить в очередь\n";
    cout << "  QPOP <очередь>               - Извлечь из очереди\n";
    cout << "  FCREATE <имя>                - Создать односвязный список\n";
    cout << "  FPUSH <список> <значение>    - Добавить в конец списка\n";
    cout << "  FPUSHFRONT <список> <значение>- Добавить в начало списка\n";
    cout << "  FINSERTAFTER <список> <инд> <знач>- Вставить после индекса\n";
    cout << "  FINSERTBEFORE <список> <инд> <знач>- Вставить перед индексом\n";
    cout << "  FDELHEAD <список>            - Удалить голову списка\n";
    cout << "  FDELTAIL <список>            - Удалить хвост списка\n";
    cout << "  FDELVALUE <список> <значение>- Удалить значение из списка\n";
    cout << "  FDELBEFORE <список> <инд>    - Удалить перед индексом\n";
    cout << "  FDELAFTER <список> <инд>     - Удалить после индекса\n";
    cout << "  FSEARCH <список> <значение>  - Найти значение в списке\n";
    cout << "  LCREATE <имя>                - Создать двусвязный список\n";
    cout << "  LPUSH <список> <значение>    - Добавить в конец списка\n";
    cout << "  LPUSHFRONT <список> <значение>- Добавить в начало списка\n";
    cout << "  LINSERTAFTER <список> <инд> <знач>- Вставить после индекса\n";
    cout << "  LINSERTBEFORE <список> <инд> <знач>- Вставить перед индексом\n";
    cout << "  LDELHEAD <список>            - Удалить голову списка\n";
    cout << "  LDELTAIL <список>            - Удалить хвост списка\n";
    cout << "  LDELVALUE <список> <значение>- Удалить значение из списка\n";
    cout << "  LDELBEFORE <список> <инд>    - Удалить перед индексом\n";
    cout << "  LDELAFTER <список> <инд>     - Удалить после индекса\n";
    cout << "  LSEARCH <список> <значение>  - Найти значение в списке\n";
    cout << "  RBCREATE <имя>               - Создать красно-черное дерево\n";
	cout << "  RBINSERT <дерево> <значение> - Вставить значение в дерево\n";
	cout << "  RBDELETE <дерево> <значение> - Удалить значение из дерева\n";
	cout << "  RBSEARCH <дерево> <значение> - Найти значение в дереве\n";
    cout << "  PRINT [структура]            - Вывести структуру(ы)\n";
    cout << "  CLEAR                        - Очистить все данные\n";
}

int main(int argc, char* argv[]) {
    string filename;
    string query;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        } else if (arg == "--help") {
            printUsage();
            return 0;
        }
    }
    
    if (filename.empty() || query.empty()) {
        cerr << "Ошибка: Требуются оба параметра --file и --query\n";
        printUsage();
        return 1;
    }
    
    loadFromFile(filename);
    bool success = executeQuery(query, filename);
    saveToFile(filename);
    
    return 0;
}
