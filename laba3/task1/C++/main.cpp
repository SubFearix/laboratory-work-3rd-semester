
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "structures.h"

using namespace std;

map<string, Array*> arrays;
map<string, Stack*> stacks;
map<string, Queue*> queues;
map<string, OLList*> olists;
map<string, TLList*> tlists;
map<string, RBTree*> rbtrees;
map<string, Hash*> hashtables;
map<string, HashA*> hashtablesA;

vector<string> splitCommand(const string& str) {
    vector<string> tokens;
    string token;
    istringstream stream(str);
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

string toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

void deleteSubtree(RBTree* rbt, const NodeRB* node) {
    if (node == rbt->getNilnode()) return;
    deleteSubtree(rbt, node->getLeft());
    deleteSubtree(rbt, node->getRight());
    delete node;
}

void deleteRBTree(RBTree* rbt) {
    if (rbt) {
        deleteSubtree(rbt, rbt->getRoot());
        delete rbt->getNilnode();
        delete rbt;
    }
}

void cleanupAll() {
    for (auto& [fst, snd]: arrays) delete snd;
    arrays.clear();

    for (auto& [fst, snd]: stacks) delete snd;
    stacks.clear();

    for (auto& [fst, snd]: queues) delete snd;
    queues.clear();

    for (auto& [fst, snd]: olists) delete snd;
    olists.clear();

    for (auto& [fst, snd]: tlists) delete snd;
    tlists.clear();
    
    for (auto& [fst, snd]: rbtrees) delete snd;
    rbtrees.clear();
    
    for (auto& [fst, snd]: hashtables) delete snd;
    hashtables.clear();
    
    for (auto& [fst, snd]: hashtablesA) delete snd;
    hashtablesA.clear();
}

void saveRBTreeHelper(const NodeRB* node, RBTree* tree, ostream& os) {
    if (node == tree->getNilnode()) {
        os << "N ";
        return;
    }
    
    os << node->getData() << " " << (node->getColor() == RED ? "R " : "B ");
    saveRBTreeHelper(node->getLeft(), tree, os);
    saveRBTreeHelper(node->getRight(), tree, os);
}

void saveRBTreeToStream(RBTree* tree, ostream& os) {
    saveRBTreeHelper(tree->getRoot(), tree, os);
}

NodeRB* loadRBTreeHelper(RBTree* tree, NodeRB* parent, istream& is)
{
    string token;
    if (!(is >> token) || token == "N") {
        return tree->getNilnode();
    }

    const int data = stoi(token);
    string color;
    if (!(is >> color)) {
        return tree->getNilnode();
    }
    
    auto* node = new NodeRB();
    node->setData(data);
    node->setColor(color == "R" ? RED : BLACK);
    node->setParent(parent);
    node->setLeft(loadRBTreeHelper(tree, node, is));
    node->setRight(loadRBTreeHelper(tree, node, is));
    
    return node;
}

void loadRBTreeFromStream(RBTree* tree, istream& is) {
    tree->setRoot(loadRBTreeHelper(tree, tree->getNilnode(), is));
}

void saveHashToStream(const Hash* hash, ostream& stream)
{
    for (int i = 0; i < hash->getCapacity(); i++) {
        const NodeHash* current = hash->getCells()[i];
        while (current != nullptr) {
            stream << current->getKey() << " " << current->getData() << " ";
            current = current->getNext();
        }
    }
}

void saveHashAToStream(const HashA* hasha, ostream& stream) {
    for (int i = 0; i < hasha->getCapacity(); i++) {
        const NodeHashA& cell = hasha->getCells()[i];
        if (!cell.getIsEmpty() && !cell.getIsDeleted()) {
            stream << cell.getKey() << " " << cell.getData() << " ";
        }
    }
}

void loadHashFromStream(Hash* hash, istream& stream) {
    string key;
    int value;
    while (stream >> key >> value) {
        hash->addElement(key, value);
    }
}

void loadHashAFromStream(HashA* hasha, istream& stream) {
    string key;
    int value;
    while (stream >> key >> value) {
        hasha->addElementA(key, value);
    }
}

void loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> tokens = splitCommand(line);
        if (tokens.size() < 3) continue;

        const string& type = tokens[0];
        const string& name = tokens[1];

        if (type == "ARRAY") {
            auto* arr = new Array();
            int size = stoi(tokens[2]);
            for (int i = 0; i < size && i + 3 < tokens.size(); i++) {
                const string& value = tokens[i + 3];
                arr->addBackArray(value);
            }
            arrays[name] = arr;
        }
        else if (type == "STACK") {
            auto st = new Stack();
            for (size_t i = 2; i < tokens.size(); i++) {
                const string& value = tokens[i];
                st->pushStack(value);
            }
            stacks[name] = st;
        }
        else if (type == "QUEUE") {
            auto que = new Queue();
            for (size_t i = 2; i < tokens.size(); i++) {
                const string& value = tokens[i];
                que->addBackQueue(value);
            }
            queues[name] = que;
        }
        else if (type == "OLIST") {
            auto lst = new OLList();
            for (size_t i = 2; i < tokens.size(); i++) {
                const string& value = tokens[i];
                lst->addBackOL(value);
            }
            olists[name] = lst;
        }
        else if (type == "TLIST") {
            auto lst = new TLList();
            for (size_t i = 2; i < tokens.size(); i++) {
                const string& value = tokens[i];
                lst->addBackTL(value);
            }
            tlists[name] = lst;
        }
        else if (type == "RBTREE") {
			if (rbtrees.find(name) != rbtrees.end()) {
				deleteRBTree(rbtrees[name]);
			}
			rbtrees[name] = new RBTree();
			string treeData;
			for (size_t i = 2; i < tokens.size(); i++) {
				treeData += tokens[i] + " ";
			}
			string basic_string;
			while (getline(file, basic_string)) {
				if (basic_string.empty()) break;
				treeData += basic_string + " ";
			}
			
			if (!treeData.empty()) {
				istringstream treeStream(treeData);
				loadRBTreeFromStream(rbtrees[name], treeStream);
			}
		}
        else if (type == "HASH") {
			if (hashtables.find(name) != hashtables.end()) {
				delete hashtables[name];
			}
			hashtables[name] = new Hash();
			string hashData;
			for (size_t i = 2; i < tokens.size(); i++) {
				hashData += tokens[i] + " ";
			}
			string basic_string;
			while (getline(file, basic_string)) {
				if (basic_string.empty()) break;
				hashData += basic_string + " ";
			    vector<string> testTokens = splitCommand(hashData);
                if (testTokens.size() % 2 == 0) {
				    break;
				}
			}

			if (!hashData.empty()) {
				istringstream hashStream(hashData);
				loadHashFromStream(hashtables[name], hashStream);
			}
		}
		else if (type == "HASHA") {
			if (hashtablesA.find(name) != hashtablesA.end()) {
				delete hashtablesA[name];
			}
			hashtablesA[name] = new HashA();
			string hashData;
			for (size_t i = 2; i < tokens.size(); i++) {
				hashData += tokens[i] + " ";
			}
			string basic_string;
			while (getline(file, basic_string)) {
				if (basic_string.empty()) break;
				hashData += basic_string + " ";
			    vector<string> testTokens = splitCommand(hashData);
                if (testTokens.size() % 2 == 0) {
				    break;
				}
			}

			if (!hashData.empty()) {
				istringstream hashStream(hashData);
				loadHashAFromStream(hashtablesA[name], hashStream);
			}
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

    for (const auto& [fst, snd]: arrays) {
        file << "ARRAY " << fst << " " << snd->getSize();
        for (int i = 0; i < snd->getSize(); i++) {
            file << " " << snd->getElemIndArray(i);
        }
        file << "\n";
    }

    for (const auto& [fst, snd]: stacks) {
        file << "STACK " << fst;
        auto* temp = new Stack();
        const Node* curr = snd->getTop();
        while (curr != nullptr) {
            temp->pushStack(curr->getData());
            curr = curr->getNext();
        }
        curr = temp->getTop();
        while (curr != nullptr) {
            file << " " << curr->getData();
            curr = curr->getNext();
        }
        while (temp->getTop() != nullptr) {
            temp->popStack();
        }
        delete temp;
        file << "\n";
    }

    for (const auto& [fst, snd]: queues) {
        file << "QUEUE " << fst;
        const Node* curr = snd->getHead();
        while (curr != nullptr) {
            file << " " << curr->getData();
            curr = curr->getNext();
        }
        file << "\n";
    }

    for (const auto& [fst, snd]: olists) {
        file << "OLIST " << fst;
        const Node* curr = snd->getHead();
        while (curr != nullptr) {
            file << " " << curr->getData();
            curr = curr->getNext();
        }
        file << "\n";
    }

    for (const auto& [fst, snd]: tlists) {
        file << "TLIST " << fst;
        const Node* curr = snd->getHead();
        while (curr != nullptr) {
            file << " " << curr->getData();
            curr = curr->getNext();
        }
        file << "\n";
    }
    
    for (const auto& [fst, snd]: rbtrees) {
        file << "RBTREE " << fst;
        if (snd->getRoot() != snd->getNilnode()) {
            file << " ";
            saveRBTreeToStream(snd, file);
        }
        file << "\n";
    }
    
    for (const auto& [fst, snd]: hashtables) {
		file << "HASH " << fst;
		if (snd->getSize() > 0) {
		    file << " ";
		    saveHashToStream(snd, file);
		}
		file << "\n";
	}

	for (const auto& [fst, snd]: hashtablesA) {
		file << "HASHA " << fst;
		if (snd->getSize() > 0) {
		    file << " ";
		    saveHashAToStream(snd, file);
		}
		file << "\n";
	}

    file.close();
}

void printHelp() {
    cout << "==================================================\n";
    cout << "           СИСТЕМА УПРАВЛЕНИЯ СТРУКТУРАМИ ДАННЫХ\n";
    cout << "==================================================\n\n";
    
    cout << "КОМАНДЫ ДЛЯ МАССИВА:\n";
    cout << "  ACREATE <имя>                - Создать новый массив\n";
    cout << "  APUSH <массив> <значение>    - Добавить в конец массива\n";
    cout << "  APUSHFRONT <массив> <значение>- Добавить в начало массива\n";
    cout << "  APOP <массив>                - Удалить из конца массива\n";
    cout << "  AINSERT <массив> <инд> <знач>- Вставить по индексу в массив\n";
    cout << "  ADEL <массив> <инд>          - Удалить по индексу из массива\n";
    cout << "  AGET <массив> <инд>          - Получить элемент по индексу\n";
    cout << "  ASET <массив> <инд> <знач>   - Установить элемент по индексу\n";
    cout << "  ASIZE <массив>               - Получить размер массива\n\n";
    
    cout << "КОМАНДЫ ДЛЯ СТЕКА:\n";
    cout << "  SCREATE <имя>                - Создать новый стек\n";
    cout << "  SPUSH <стек> <значение>      - Поместить в стек\n";
    cout << "  SPOP <стек>                  - Извлечь из стека\n\n";
    
    cout << "КОМАНДЫ ДЛЯ ОЧЕРЕДИ:\n";
    cout << "  QCREATE <имя>                - Создать новую очередь\n";
    cout << "  QPUSH <очередь> <значение>   - Добавить в очередь\n";
    cout << "  QPOP <очередь>               - Извлечь из очереди\n\n";
    
    cout << "КОМАНДЫ ДЛЯ ОДНОСВЯЗНОГО СПИСКА:\n";
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
    cout << "  FSEARCH <список> <значение>  - Найти значение в списке\n\n";
    
    cout << "КОМАНДЫ ДЛЯ ДВУСВЯЗНОГО СПИСКА:\n";
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
    cout << "  LSEARCH <список> <значение>  - Найти значение в списке\n\n";
    
    cout << "КОМАНДЫ ДЛЯ КРАСНО-ЧЕРНОГО ДЕРЕВА:\n";
    cout << "  RBCREATE <имя>               - Создать красно-черное дерево\n";
    cout << "  RBINSERT <дерево> <значение> - Вставить значение в дерево\n";
    cout << "  RBDELETE <дерево> <значение> - Удалить значение из дерева\n";
    cout << "  RBSEARCH <дерево> <значение> - Найти значение в дереве\n\n";
    
    cout << "КОМАНДЫ ДЛЯ ХЭШ-ТАБЛИЦЫ С ЦЕПОЧКАМИ:\n";
    cout << "  HCREATE <имя>                - Создать хэш-таблицу\n";
    cout << "  HINSERT <таблица> <ключ> <значение> - Добавить элемент\n";
    cout << "  HDELETE <таблица> <ключ>     - Удалить элемент\n";
    cout << "  HSEARCH <таблица> <ключ>     - Найти элемент\n\n";
    
    cout << "КОМАНДЫ ДЛЯ ХЭШ-ТАБЛИЦЫ С ОТКРЫТОЙ АДРЕСАЦИЕЙ:\n";
    cout << "  ACREATEH <имя>               - Создать хэш-таблицу\n";
    cout << "  AINSERTH <таблица> <ключ> <значение> - Добавить элемент\n";
    cout << "  ADELETEH <таблица> <ключ>    - Удалить элемент\n";
    cout << "  ASEARCHH <таблица> <ключ>    - Найти элемент\n\n";
    
    cout << "ОБЩИЕ КОМАНДЫ:\n";
    cout << "  PRINT [структура]            - Вывести структуру(ы)\n";
    cout << "  CLEAR                        - Очистить все данные\n";
    cout << "  EXIT                         - Выход из программы\n";
    cout << "  HELP                         - Показать эту справку\n";
    cout << "==================================================\n";
}

bool processPrint(const vector<string>& tokens) {
    if (tokens.size() == 1) {
        for (const auto& [fst, snd]: arrays) {
            cout << "ARRAY " << fst << ": ";
            snd->printArray();
        }
        for (const auto& [fst, snd]: stacks) {
            cout << "STACK " << fst << ": ";
            snd->printStack();
        }
        for (const auto& [fst, snd]: queues) {
            cout << "QUEUE " << fst << ": ";
            snd->printQueue();
        }
        for (const auto& [fst, snd]: olists) {
            cout << "OLIST " << fst << ": ";
            snd->printListOL();
        }
        for (const auto& [fst, snd]: tlists) {
            cout << "TLIST " << fst << ": ";
            snd->printListTL();
        }
        for (const auto& [fst, snd]: rbtrees) {
            cout << "RBTREE " << fst << ": ";
            snd->printRBTree();
        }
        for (const auto& [fst, snd]: hashtables) {
            cout << "HASH " << fst << ":\n";
            snd->printHashTable();
        }
        for (const auto& [fst, snd]: hashtablesA) {
            cout << "HASHA " << fst << ":\n";
            snd->printHashTableA();
        }
    } else {
        const string& name = tokens[1];
        if (arrays.find(name) != arrays.end()) {
            cout << "ARRAY " << name << ": ";
            arrays[name]->printArray();
        } else if (stacks.find(name) != stacks.end()) {
            cout << "STACK " << name << ": ";
            stacks[name]->printStack();
        } else if (queues.find(name) != queues.end()) {
            cout << "QUEUE " << name << ": ";
            queues[name]->printQueue();
        } else if (olists.find(name) != olists.end()) {
            cout << "OLIST " << name << ": ";
            olists[name]->printListOL();
        } else if (tlists.find(name) != tlists.end()) {
            cout << "TLIST " << name << ": ";
            tlists[name]->printListTL();
        } else if (rbtrees.find(name) != rbtrees.end()) {
            cout << "RBTREE " << name << ": ";
            rbtrees[name]->printRBTree();
        } else if (hashtables.find(name) != hashtables.end()) {
            cout << "HASH " << name << ":\n";
            hashtables[name]->printHashTable();
        } else if (hashtablesA.find(name) != hashtablesA.end()) {
            cout << "HASHA " << name << ":\n";
            hashtablesA[name]->printHashTableA();
        } else {
            cout << "Структура не найдена: " << name << endl;
            return false;
        }
    }
    return true;
}

bool processArrayCommand(const vector<string>& tokens) {
    const string cmd = toUpper(tokens[0]);
    if (cmd == "ACREATE") {
        if (tokens.size() < 2) {
            cout << "Ошибка: ACREATE требует имя массива" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (arrays.find(name) != arrays.end()) {
            cout << "Массив '" << name << "' уже существует" << endl;
            return false;
        }
        arrays[name] = new Array();
        cout << "Массив '" << name << "' успешно создан" << endl;
        return true;
    }
    if (cmd == "APUSH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: APUSH требует имя массива и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (arrays.find(name) == arrays.end()) {
            arrays[name] = new Array();
        }
        const string& value = tokens[2];
        arrays[name]->addBackArray(value);
        cout << value << endl;
        return true;
    }
    if (cmd == "APUSHFRONT") {
        if (tokens.size() < 3) {
            cout << "Ошибка: APUSHFRONT требует имя массива и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (arrays.find(name) == arrays.end()) {
            arrays[name] = new Array();
        }
        const string& value = tokens[2];
        arrays[name]->addElemIndArray(0, value);
        cout << value << endl;
        return true;
    }
    if (cmd == "APOP") {
        if (tokens.size() < 2) {
            cout << "Ошибка: APOP требует имя массива" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (arrays.find(name) == arrays.end() || arrays[name]->getSize() == 0) {
            cout << "Ошибка: Массив пуст или не существует" << endl;
            return false;
        }
        const string value = arrays[name]->getElemIndArray(arrays[name]->getSize() - 1);
        arrays[name]->remElemIndArray(arrays[name]->getSize() - 1);
        cout << value << endl;
        return true;
    }
    if (cmd == "AINSERT") {
        if (tokens.size() < 4) {
            cout << "Ошибка: AINSERT требует имя массива, индекс и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (arrays.find(name) == arrays.end()) {
            arrays[name] = new Array();
        }
        const int index = stoi(tokens[2]);
        const string& value = tokens[3];
        arrays[name]->addElemIndArray(index, value);
        cout << value << endl;
        return true;
    }
    if (cmd == "ADEL") {
        if (tokens.size() < 3) {
            cout << "Ошибка: ADEL требует имя массива и индекс" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (arrays.find(name) == arrays.end()) {
            cout << "Ошибка: Массив не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        if (index < 0 || index >= arrays[name]->getSize()) {
            cout << "Ошибка: Индекс вне границ" << endl;
            return false;
        }
        const string value = arrays[name]->getElemIndArray(index);
        arrays[name]->remElemIndArray(index);
        cout << value << endl;
        return true;
    }
    if (cmd == "AGET") {
        if (tokens.size() < 3) {
            cout << "Ошибка: AGET требует имя массива и индекс" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (arrays.find(name) == arrays.end()) {
            cout << "Ошибка: Массив не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        const string value = arrays[name]->getElemIndArray(index);
        if (value.empty()) {
            cout << "Ошибка: Индекс вне границ" << endl;
            return false;
        }
        cout << value << endl;
        return true;
    }
    if (cmd == "ASET") {
        if (tokens.size() < 4) {
            cout << "Ошибка: ASET требует имя массива, индекс и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (arrays.find(name) == arrays.end()) {
            cout << "Ошибка: Массив не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        const string& value = tokens[3];
        arrays[name]->changeElemIndArray(index, value);
        cout << value << endl;
        return true;
    }
    if (cmd == "ASIZE") {
        if (tokens.size() < 2) {
            cout << "Ошибка: ASIZE требует имя массива" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (arrays.find(name) == arrays.end()) {
            cout << "Ошибка: Массив не существует" << endl;
            return false;
        }
        cout << arrays[name]->getSize() << endl;
        return true;
    }

    return false;
}

bool processStackCommand(const vector<string>& tokens) {
    const string cmd = toUpper(tokens[0]);
    if (cmd == "SCREATE") {
        if (tokens.size() < 2) {
            cout << "Ошибка: SCREATE требует имя стека" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (stacks.find(name) != stacks.end()) {
            cout << "Стек '" << name << "' уже существует" << endl;
            return false;
        }
        stacks[name] = new Stack();
        cout << "Стек '" << name << "' успешно создан" << endl;
        return true;
    }
    if (cmd == "SPUSH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: SPUSH требует имя стека и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (stacks.find(name) == stacks.end()) {
            stacks[name] = new Stack();
        }
        const string& value = tokens[2];
        stacks[name]->pushStack(value);
        cout << value << endl;
        return true;
    }
    if (cmd == "SPOP") {
        if (tokens.size() < 2) {
            cout << "Ошибка: SPOP требует имя стека" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (stacks.find(name) == stacks.end() || stacks[name]->getTop() == nullptr) {
            cout << "Ошибка: Стек пуст или не существует" << endl;
            return false;
        }
        const string value = stacks[name]->getTop()->getData();
        stacks[name]->popStack();
        cout << value << endl;
        return true;
    }

    return false;
}

bool processQueueCommand(const vector<string>& tokens) {
    const string cmd = toUpper(tokens[0]);
    if (cmd == "QCREATE") {
        if (tokens.size() < 2) {
            cout << "Ошибка: QCREATE требует имя очереди" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (queues.find(name) != queues.end()) {
            cout << "Очередь '" << name << "' уже существует" << endl;
            return false;
        }
        queues[name] = new Queue();
        cout << "Очередь '" << name << "' успешно создана" << endl;
        return true;
    }
    if (cmd == "QPUSH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: QPUSH требует имя очереди и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (queues.find(name) == queues.end()) {
            queues[name] = new Queue();
        }
        const string& value = tokens[2];
        queues[name]->addBackQueue(value);
        cout << value << endl;
        return true;
    }
    if (cmd == "QPOP") {
        if (tokens.size() < 2) {
            cout << "Ошибка: QPOP требует имя очереди" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (queues.find(name) == queues.end() || queues[name]->getHead() == nullptr) {
            cout << "Ошибка: Очередь пуста или не существует" << endl;
            return false;
        }
        const string value = queues[name]->getHead()->getData();
        queues[name]->remFrontQueue();
        cout << value << endl;
        return true;
    }

    return false;
}

bool processOLListCommand(const vector<string>& tokens) {
    const string cmd = toUpper(tokens[0]);
    
    if (cmd == "FCREATE") {
        if (tokens.size() < 2) {
            cout << "Ошибка: FCREATE требует имя списка" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) != olists.end()) {
            cout << "Односвязный список '" << name << "' уже существует" << endl;
            return false;
        }
        olists[name] = new OLList();
        cout << "Односвязный список '" << name << "' успешно создан" << endl;
        return true;
    }
    if (cmd == "FPUSH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: FPUSH требует имя списка и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end()) {
            olists[name] = new OLList();
        }
        const string& value = tokens[2];
        olists[name]->addBackOL(value);
        cout << value << endl;
        return true;
    }
    if (cmd == "FPUSHFRONT") {
        if (tokens.size() < 3) {
            cout << "Ошибка: FPUSHFRONT требует имя списка и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end()) {
            olists[name] = new OLList();
        }
        const string& value = tokens[2];
        olists[name]->addFrontOL(value);
        cout << value << endl;
        return true;
    }
    if (cmd == "FINSERTAFTER") {
        if (tokens.size() < 4) {
            cout << "Ошибка: FINSERTAFTER требует имя списка, индекс и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        const string& value = tokens[3];
        olists[name]->insertAfterIndOL(index, value);
        cout << value << endl;
        return true;
    }
    if (cmd == "FINSERTBEFORE") {
        if (tokens.size() < 4) {
            cout << "Ошибка: FINSERTBEFORE требует имя списка, индекс и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        const string& value = tokens[3];
        olists[name]->insertBeforeIndOL(index, value);
        cout << value << endl;
        return true;
    }
    if (cmd == "FDELHEAD") {
        if (tokens.size() < 2) {
            cout << "Ошибка: FDELHEAD требует имя списка" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end() || olists[name]->getHead() == nullptr) {
            cout << "Ошибка: Список пуст или не существует" << endl;
            return false;
        }
        olists[name]->remFrontOL();
        cout << "Голова успешно удалена" << endl;
        return true;
    }
    if (cmd == "FDELTAIL") {
        if (tokens.size() < 2) {
            cout << "Ошибка: FDELTAIL требует имя списка" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end() || olists[name]->getHead() == nullptr) {
            cout << "Ошибка: Список пуст или не существует" << endl;
            return false;
        }
        olists[name]->remBackOL();
        cout << "Хвост успешно удален" << endl;
        return true;
    }
    if (cmd == "FDELVALUE") {
        if (tokens.size() < 3) {
            cout << "Ошибка: FDELVALUE требует имя списка и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const string& value = tokens[2];
        olists[name]->remValOL(value);
        cout << "Значение " << value << " удалено" << endl;
        return true;
    }
    if (cmd == "FDELBEFORE") {
        if (tokens.size() < 3) {
            cout << "Ошибка: FDELBEFORE требует имя списка и индекс" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        olists[name]->deleteBeforeIndOL(index);
        cout << "Элемент перед индексом " << index << " удален" << endl;
        return true;
    }
    if (cmd == "FDELAFTER") {
        if (tokens.size() < 3) {
            cout << "Ошибка: FDELAFTER требует имя списка и индекс" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        olists[name]->deleteAfterIndOL(index);
        cout << "Элемент после индекса " << index << " удален" << endl;
        return true;
    }
    if (cmd == "FSEARCH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: FSEARCH требует имя списка и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (olists.find(name) == olists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const string& value = tokens[2];
        const int index = olists[name]->findValOL(value);
        if (index == -1) {
            cout << "Значение не найдено" << endl;
        } else {
            cout << "Значение найдено по индексу: " << index << endl;
        }
        return true;
    }

    return false;
}
bool processTLListCommand(const vector<string>& tokens) {
    const string cmd = toUpper(tokens[0]);
    
    if (cmd == "LCREATE") {
        if (tokens.size() < 2) {
            cout << "Ошибка: LCREATE требует имя списка" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) != tlists.end()) {
            cout << "Двусвязный список '" << name << "' уже существует" << endl;
            return false;
        }
        tlists[name] = new TLList();
        cout << "Двусвязный список '" << name << "' успешно создан" << endl;
        return true;
    }
    if (cmd == "LPUSH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: LPUSH требует имя списка и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end()) {
            tlists[name] = new TLList();
        }
        const string& value = tokens[2];
        tlists[name]->addBackTL(value);
        cout << value << endl;
        return true;
    }
    if (cmd == "LPUSHFRONT") {
        if (tokens.size() < 3) {
            cout << "Ошибка: LPUSHFRONT требует имя списка и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end()) {
            tlists[name] = new TLList();
        }
        const string& value = tokens[2];
        tlists[name]->addFrontTL(value);
        cout << value << endl;
        return true;
    }
    if (cmd == "LINSERTAFTER") {
        if (tokens.size() < 4) {
            cout << "Ошибка: LINSERTAFTER требует имя списка, индекс и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        const string& value = tokens[3];
        tlists[name]->insertAfterIndTL(index, value);
        cout << value << endl;
        return true;
    }
    if (cmd == "LINSERTBEFORE") {
        if (tokens.size() < 4) {
            cout << "Ошибка: LINSERTBEFORE требует имя списка, индекс и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        const string& value = tokens[3];
        tlists[name]->insertBeforeIndTL(index, value);
        cout << value << endl;
        return true;
    }
    if (cmd == "LDELHEAD") {
        if (tokens.size() < 2) {
            cout << "Ошибка: LDELHEAD требует имя списка" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end() || tlists[name]->getHead() == nullptr) {
            cout << "Ошибка: Список пуст или не существует" << endl;
            return false;
        }
        tlists[name]->remFrontTL();
        cout << "Голова успешно удалена" << endl;
        return true;
    }
    if (cmd == "LDELTAIL") {
        if (tokens.size() < 2) {
            cout << "Ошибка: LDELTAIL требует имя списка" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end() || tlists[name]->getHead() == nullptr) {
            cout << "Ошибка: Список пуст или не существует" << endl;
            return false;
        }
        tlists[name]->remBackTL();
        cout << "Хвост успешно удален" << endl;
        return true;
    }
    if (cmd == "LDELVALUE") {
        if (tokens.size() < 3) {
            cout << "Ошибка: LDELVALUE требует имя списка и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const string& value = tokens[2];
        tlists[name]->remValTL(value);
        cout << "Значение " << value << " удалено" << endl;
        return true;
    }
    if (cmd == "LDELBEFORE") {
        if (tokens.size() < 3) {
            cout << "Ошибка: LDELBEFORE требует имя списка и индекс" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        tlists[name]->deleteBeforeIndTL(index);
        cout << "Элемент перед индексом " << index << " удален" << endl;
        return true;
    }
    if (cmd == "LDELAFTER") {
        if (tokens.size() < 3) {
            cout << "Ошибка: LDELAFTER требует имя списка и индекс" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const int index = stoi(tokens[2]);
        tlists[name]->deleteAfterIndTL(index);
        cout << "Элемент после индекса " << index << " удален" << endl;
        return true;
    }
    if (cmd == "LSEARCH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: LSEARCH требует имя списка и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (tlists.find(name) == tlists.end()) {
            cout << "Ошибка: Список не существует" << endl;
            return false;
        }
        const string& value = tokens[2];
        const int index = tlists[name]->findValTL(value);
        if (index == -1) {
            cout << "Значение не найдено" << endl;
        } else {
            cout << "Значение найдено по индексу: " << index << endl;
        }
        return true;
    }

    return false;
}

bool processRBTreeCommand(const vector<string>& tokens) {
    const string cmd = toUpper(tokens[0]);
    
    if (cmd == "RBCREATE") {
        if (tokens.size() != 2) {
            cout << "Ошибка: неверное количество аргументов для RBCREATE" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (rbtrees.find(name) != rbtrees.end()) {
            cout << "Ошибка: дерево '" << name << "' уже существует" << endl;
            return false;
        }
        rbtrees[name] = new RBTree();
        cout << "Создание красно-черного дерева '" << name << "' успешно" << endl;
        return true;
    }
    if (cmd == "RBINSERT") {
        if (tokens.size() != 3) {
            cout << "Ошибка: неверное количество аргументов для RBINSERT" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (rbtrees.find(name) == rbtrees.end()) {
            rbtrees[name] = new RBTree();
            cout << "Дерево '" << name << "' автоматически создано" << endl;
        }
        const int value = stoi(tokens[2]);
        rbtrees[name]->insertNode(value);
        cout << "Вставка элемента " << value << " в дерево '" << name << "' успешна" << endl;
        return true;
    }
    if (cmd == "RBDELETE") {
        if (tokens.size() != 3) {
            cout << "Ошибка: неверное количество аргументов для RBDELETE" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (rbtrees.find(name) == rbtrees.end()) {
            cout << "Ошибка: дерево '" << name << "' не существует" << endl;
            return false;
        }
        const int value = stoi(tokens[2]);
        try {
            rbtrees[name]->deleteNode(value);
            cout << "Удаление элемента " << value << " из дерева '" << name << "' успешно" << endl;
        } catch (const runtime_error& e) {
            cerr << "Ошибка при удалении: " << e.what() << endl;
        }
        return true;
    }
    if (cmd == "RBSEARCH") {
        if (tokens.size() != 3) {
            cout << "Ошибка: неверное количество аргументов для RBSEARCH" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (rbtrees.find(name) == rbtrees.end()) {
            cout << "Ошибка: дерево '" << name << "' не существует" << endl;
            return false;
        }
        const int value = stoi(tokens[2]);
        const NodeRB* found = rbtrees[name]->searchNode(value);
        if (found != rbtrees[name]->getNilnode()) {
            cout << "Элемент " << value << " найден в дереве '" << name << "'" << endl;
        } else {
            cout << "Элемент " << value << " не найден в дереве '" << name << "'" << endl;
        }
        return true;
    }

    return false;
}

bool processHashCommand(const vector<string>& tokens) {
    const string cmd = toUpper(tokens[0]);
    
    if (cmd == "HCREATE") {
        if (tokens.size() < 2) {
            cout << "Ошибка: HCREATE требует имя таблицы" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (hashtables.find(name) != hashtables.end()) {
            cout << "Хэш-таблица '" << name << "' уже существует" << endl;
            return false;
        }
        hashtables[name] = new Hash();
        cout << "Хэш-таблица '" << name << "' успешно создана" << endl;
        return true;
    }
    if (cmd == "HINSERT") {
        if (tokens.size() < 4) {
            cout << "Ошибка: HINSERT требует имя таблицы, ключ и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (hashtables.find(name) == hashtables.end()) {
            hashtables[name] = new Hash();
        }
        const string& key = tokens[2];
        const int value = stoi(tokens[3]);
        hashtables[name]->addElement(key, value);
        cout << "Добавлен элемент [" << key << " -> " << value << "] в таблицу '" << name << "'" << endl;
        return true;
    }
    if (cmd == "HDELETE") {
        if (tokens.size() < 3) {
            cout << "Ошибка: HDELETE требует имя таблицы и ключ" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (hashtables.find(name) == hashtables.end()) {
            cout << "Ошибка: Хэш-таблица не существует" << endl;
            return false;
        }
        const string& key = tokens[2];
        hashtables[name]->deleteElement(key);
        cout << "Элемент с ключом '" << key << "' удален из таблицы '" << name << "'" << endl;
        return true;
    }
    if (cmd == "HSEARCH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: HSEARCH требует имя таблицы и ключ" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (hashtables.find(name) == hashtables.end()) {
            cout << "Ошибка: Хэш-таблица не существует" << endl;
            return false;
        }
        const string& key = tokens[2];
        const int result = hashtables[name]->findElement(key);
        if (result != -1) {
            cout << "Элемент найден: [" << key << " -> " << result << "]" << endl;
        } else {
            cout << "Элемент с ключом '" << key << "' не найден" << endl;
        }
        return true;
    }

    return false;
}

bool processHashACommand(const vector<string>& tokens) {
    const string cmd = toUpper(tokens[0]);
    
    if (cmd == "ACREATEH") {
        if (tokens.size() < 2) {
            cout << "Ошибка: ACREATEH требует имя таблицы" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (hashtablesA.find(name) != hashtablesA.end()) {
            cout << "Хэш-таблица '" << name << "' уже существует" << endl;
            return false;
        }
        hashtablesA[name] = new HashA();
        cout << "Хэш-таблица с открытой адресацией '" << name << "' успешно создана" << endl;
        return true;
    }
    if (cmd == "AINSERTH") {
        if (tokens.size() < 4) {
            cout << "Ошибка: AINSERTH требует имя таблицы, ключ и значение" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (hashtablesA.find(name) == hashtablesA.end()) {
            hashtablesA[name] = new HashA();
        }
        const string& key = tokens[2];
        const int value = stoi(tokens[3]);
        hashtablesA[name]->addElementA(key, value);
        cout << "Добавлен элемент [" << key << " -> " << value << "] в таблицу '" << name << "'" << endl;
        return true;
    }
    if (cmd == "ADELETEH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: ADELETEH требует имя таблицы и ключ" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (hashtablesA.find(name) == hashtablesA.end()) {
            cout << "Ошибка: Хэш-таблица не существует" << endl;
            return false;
        }
        const string& key = tokens[2];
        hashtablesA[name]->deleteElementA(key);
        cout << "Элемент с ключом '" << key << "' удален из таблицы '" << name << "'" << endl;
        return true;
    }
    if (cmd == "ASEARCHH") {
        if (tokens.size() < 3) {
            cout << "Ошибка: ASEARCHH требует имя таблицы и ключ" << endl;
            return false;
        }
        const string& name = tokens[1];
        if (hashtablesA.find(name) == hashtablesA.end()) {
            cout << "Ошибка: Хэш-таблица не существует" << endl;
            return false;
        }
        const string& key = tokens[2];
        const int result = hashtablesA[name]->findElementA(key);
        if (result != -1) {
            cout << "Элемент найден: [" << key << " -> " << result << "]" << endl;
        } else {
            cout << "Элемент с ключом '" << key << "' не найден" << endl;
        }
        return true;
    }

    return false;
}

bool processCommand(const vector<string>& tokens, const string& filename) {
    if (tokens.empty()) return true;

    const string cmd = toUpper(tokens[0]);
    
    if (cmd == "EXIT") {
        cout << "Выход из программы...\n";
        return false;
    }
    if (cmd == "HELP") {
        printHelp();
    }
    else if (cmd == "PRINT") {
        return processPrint(tokens);
    }
    else if (cmd == "CLEAR") {
        cleanupAll();
        ofstream file(filename, ofstream::trunc);
        file.close();
        cout << "База данных успешно очищена" << endl;
        return true;
    }
    else if (cmd == "ACREATE" || cmd == "APUSH" || cmd == "APUSHFRONT" ||
        cmd == "APOP" || cmd == "AINSERT" || cmd == "ADEL" ||
        cmd == "AGET" || cmd == "ASET" || cmd == "ASIZE") {
        return processArrayCommand(tokens);
    }
    else if (cmd == "SCREATE" || cmd == "SPUSH" || cmd == "SPOP") {
        return processStackCommand(tokens);
    }
    else if (cmd == "QCREATE" || cmd == "QPUSH" || cmd == "QPOP") {
        return processQueueCommand(tokens);
    }
    else if (cmd == "FCREATE" || cmd == "FPUSH" || cmd == "FPUSHFRONT" ||
        cmd == "FINSERTAFTER" || cmd == "FINSERTBEFORE" || cmd == "FDELHEAD" ||
        cmd == "FDELTAIL" || cmd == "FDELVALUE" || cmd == "FDELBEFORE" ||
        cmd == "FDELAFTER" || cmd == "FSEARCH") {
        return processOLListCommand(tokens);
    }
    else if (cmd == "LCREATE" || cmd == "LPUSH" || cmd == "LPUSHFRONT" ||
        cmd == "LINSERTAFTER" || cmd == "LINSERTBEFORE" || cmd == "LDELHEAD" ||
        cmd == "LDELTAIL" || cmd == "LDELVALUE" || cmd == "LDELBEFORE" ||
        cmd == "LDELAFTER" || cmd == "LSEARCH") {
        return processTLListCommand(tokens);
    }
    else if (cmd == "RBCREATE" || cmd == "RBINSERT" || cmd == "RBDELETE" || cmd == "RBSEARCH") {
        return processRBTreeCommand(tokens);
    }
    else if (cmd == "HCREATE" || cmd == "HINSERT" || cmd == "HSEARCH" || cmd == "HDELETE") {
        return processHashCommand(tokens);
    }
    else if (cmd == "ACREATEH" || cmd == "AINSERTH" || cmd == "ASEARCHH" || cmd == "ADELETEH") {
        return processHashACommand(tokens);
    }
    else {
        cout << "Неизвестная команда: " << tokens[0] << endl;
        cout << "Введите HELP для списка команд\n";
    }

    return true;
}

void parseArguments(const int argc, char* argv[], string& filename, string& query) {
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
        else if (arg == "--help") {
            printHelp();
            exit(0);
        }
    }
}

int main(const int argc, char* argv[]) {
    string filename, query;
    if (argc > 1) {
        parseArguments(argc, argv, filename, query);
        
        if (filename.empty() || query.empty()) {
            cerr << "Ошибка: Требуются оба параметра --file и --query\n";
            cout << "Использование: ./zap --file <имя_файла> --query \"команда\"\n";
            return 1;
        }
        
        loadFromFile(filename);
        const bool success = processCommand(splitCommand(query), filename);
        saveToFile(filename);
        
        return success ? 0 : 1;
    }
    cleanupAll();
    return 0;
}
