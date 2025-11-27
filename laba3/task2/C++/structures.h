#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <string>
#include <utility>
using namespace std;

class Node {
	private:
		string data;
		Node* next;
		Node* prev;
	public:
		explicit Node(string data) : data(move(data)), next(nullptr), prev(nullptr) {} // explicit предотвращает неявные преобразования типов
		
		[[nodiscard]] string getData() const {return data;} // [[nodiscard]] генерирует предупреждение компилятора, если возвращаемое значение функции не используется
		[[nodiscard]] Node* getNext() const {return next;}
		[[nodiscard]] Node* getPrev() const {return prev;}
		
		friend class TLList;
		friend class Array;
		friend class OLList;
		friend class Queue;
		friend class Stack;
		~Node(){}
};

class Array {
	private:
		string* data;
		int capacity;
		int size;
	public:
		Array() : data(new string[10]), capacity(10), size(0) {}
		explicit Array(const int cap) : data(new string[cap]), capacity(cap), size(0) {}
		Array(string* dat, const int cap, const int sz) : data(dat), capacity(cap), size(sz) {}
		void addBackArray(const string& val);
		void addElemIndArray(int ind, const string& val);
		string getElemIndArray(int ind) const;
		void remElemIndArray(int ind);
		void changeElemIndArray(int ind, string val) const;
		int getSizeArray() const;
		void printArray() const;
		
		string* getData() const {return data;}
		int getCapacity() const {return capacity;}
		int getSize() const {return size;}
		
		~Array() {
		    delete[] data;
		}
};


class OLList {
	private:
		Node* head;
		Node* tail;

		static Node* createNodeOL(const string& data) {
		    return new Node(data);
		}
	public:
		void addBackOL(const string& data);
		void remFrontOL();
		void remBackOL();
		void printListOL() const;
		void addFrontOL(const string& data);
		void insertAfterIndOL(int index, const string& data);
		void insertBeforeIndOL(int index, const string& data);
		void remValOL(const string& val);
		int findValOL(const string& val) const;
		void deleteBeforeIndOL(int index);
		void deleteAfterIndOL(int index);
		
		Node* getHead() const {return head;}
    	Node* getTail() const {return tail;}

		OLList() : head(nullptr), tail(nullptr) {}
		~OLList() {
		    const Node* current = head;
			while (current != nullptr) {
				const Node* next = current->next;
				delete current;
				current = next;
			}
		}
};

class Queue {
    private:
		Node* head;
		Node* tail;

		static Node* createNodeQueue(const string& data) {
		    return new Node(data);
		}
	public:
		void printQueue() const;
		void addBackQueue(const string& data);
		void remFrontQueue();
		
		Node* getHead() const {return head;}
    	Node* getTail() const {return tail;}
    	
		Queue() : head(nullptr), tail(nullptr) {}
		~Queue() {
		    const Node* current = head;
			while (current != nullptr) {
				const Node* next = current->next;
				delete current;
				current = next;
			}
		}
};

class Stack {
    private:
		Node* top;

		static Node* createNodeSt(const string& data) {
		    return new Node(data);
		}
	public:
		void popStack();
		void printStack() const;
		void pushStack(const string& data);
		
		[[nodiscard]] Node* getTop() const {return top;}
		Stack() : top(nullptr) {}
		~Stack() {
		    const Node* current = top;
			while (current != nullptr) {
				const Node* next = current->next;
				delete current;
				current = next;
			}
		}
};

class TLList {
    private:
		Node* head;
		Node* tail;

		static Node* createNodeTL(string data) {
		    return new Node(move(data));
		}
	public:
		void addBackTL(const string& data);
		void remFrontTL();
		void remBackTL();
		void printListTL() const;
		void addFrontTL(const string& data);
		void insertAfterIndTL(int index, const string& data);
		void insertBeforeIndTL(int index, const string& data);
		void remValTL(const string& val);
		int findValTL(const string& val) const;
		void deleteBeforeIndTL(int index);
		void deleteAfterIndTL(int index);
		
		[[nodiscard]] Node* getHead() const {return head;}
    	[[nodiscard]] Node* getTail() const {return tail;}
		
		TLList() : head(nullptr), tail(nullptr) {}
		~TLList() {
		    const Node* current = head;
			while (current != nullptr) {
				const Node* next = current->next;
				delete current;
				current = next;
			}
		}
};

class NodeHash{
	private:
		string key;
		int data;
		NodeHash* next;
		NodeHash* prev;
	public:
		NodeHash(string k, const int d) : key(move(k)), data(d), next(nullptr), prev(nullptr) {}
		
		[[nodiscard]] string getKey() const {return key;}
		[[nodiscard]] int getData() const {return data;}
		[[nodiscard]] NodeHash* getNext() const {return next;}
		[[nodiscard]] NodeHash* getPrev() const {return prev;}
		
		friend class Hash;
		~NodeHash(){}
};

class Hash{
	private:
		NodeHash** cell;
		int capacity;
		int size;
	public:
		static int hashFunc(const string& key, int capacity);
		Hash() : capacity(10), size(0) {
		    cell = new NodeHash*[capacity];
		    for (int i = 0; i < capacity; i++) {
		        cell[i] = nullptr;
		    }
		}

		explicit Hash(const int cap) : capacity(cap), size(0) {
		    cell = new NodeHash*[capacity];
		    for (int i = 0; i < capacity; i++) {
		        cell[i] = nullptr;
		    }
		}
		
		void printHashTable() const;
		void deleteElement( const string& key);
		int findElement( const string& key) const;
		void addElement( const string& key, int data);
		
		int getCapacity() const {return capacity;}
    	int getSize() const {return size;}
    	NodeHash** getCells() const {return cell;}


		~Hash() {
		    for (int i = 0; i < capacity; i++) {
		        const NodeHash* current = cell[i];
		        while (current != nullptr) {
		            const NodeHash* next = current->next;
		            delete current;
		            current = next;
		        }
		    }
		    delete[] cell;
		}
};

struct NodeHashA{
	private:
		string key;
		int data;
		bool isDeleted;
		bool isEmpty;
	public:
		NodeHashA() : key(""), data(0), isDeleted(false), isEmpty(true) {}
   
    	NodeHashA(const string& k, const int d) : key(k), data(d), isDeleted(false), isEmpty(false) {}
    	
    	string getKey() const {return key;}
		int getData() const {return data;}
		bool getIsDeleted() const {return isDeleted;}
		bool getIsEmpty() const {return isEmpty;}
		
		friend class HashA;
		~NodeHashA(){}
};

class HashA{
	private:
		NodeHashA* cell;
		int capacity;
		int size;
	public:
		static int hashFunc(const string& key, int capacity);
		HashA() : capacity(10), size(0) {
		    cell = new NodeHashA[capacity];
		}

		explicit HashA(const int cap) : capacity(cap), size(0) {
		    cell = new NodeHashA[capacity];
		}
		
		void printHashTableA() const;
		void deleteElementA( const string& key);
		int findElementA( const string& key) const;
		void addElementA( const string& key, int data);
		void rehash();
		
		int getCapacity() const {return capacity;}
    	int getSize() const {return size;}
    	const NodeHashA* getCells() const {return cell;}

		~HashA() {
		    delete[] cell;
		}
};

enum Color {
    RED,
    BLACK
};

class NodeRB {
 private:
    int data;
    Color color;
    NodeRB* left;
    NodeRB* right;
    NodeRB* parent;
public:
    NodeRB(const int data, const Color color, NodeRB* parent, NodeRB* left, NodeRB* right)
    : data(data), color(color), left(left), right(right), parent(parent) {}
    NodeRB() : data(0), color(BLACK), left(nullptr), right(nullptr), parent(nullptr) {}
    friend class RBTree;
    
    int getData() const {return data;}
    Color getColor() const {return color;}
    NodeRB* getLeft() const {return left;}
    NodeRB* getRight() const {return right;}
    NodeRB* getParent() const {return parent;}
    
    void setLeft(NodeRB* node) {left = node;}
    void setRight(NodeRB* node) {right = node;}
    void setParent(NodeRB* node) {parent = node;}
    void setColor(const Color c) {color = c;}
    void setData(const int dat) {data = dat;}
};

class RBTree {
private:
    void printRBTreeHelper(const NodeRB* node, const string& prefix, bool isLeft);
    void rightRotate(NodeRB* x);
    void leftRotate(NodeRB* x);
    void fixInsert(NodeRB* node);
    void fixDelete(NodeRB* current);
    void deleteTree(const NodeRB* node);
    NodeRB* root;
    NodeRB* nilnode;
public:
    RBTree() {
        nilnode = new NodeRB();
        nilnode->color = BLACK;
        nilnode->parent = nilnode;
        nilnode->left = nilnode;
        nilnode->right = nilnode;
        root = nilnode;
    }
	NodeRB* findMin(NodeRB* node) const;
    ~RBTree() {
		if (root != nilnode) {
		    deleteTree(root);
		}
		delete nilnode;
	}

    void insertNode(int data);
    void deleteNode(int data);
    NodeRB* searchNode(int data) const;
    void printRBTree();
    
    NodeRB* getRoot() const {return root;}
    void setRoot(NodeRB* rot) {root = rot;}
    NodeRB* getNilnode() const {return nilnode;}
};
#endif
