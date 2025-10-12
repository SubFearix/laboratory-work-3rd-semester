#include <iostream>
#include <sstream>
#include "structures.h"
#include "rbtree.h"

using namespace std;

RBTree* createEmptyRBTree() {
    RBTree* rbt = new RBTree;
    rbt->nilnode = new NodeRB;
    rbt->nilnode->color = BLACK;
    rbt->nilnode->parent = rbt->nilnode;
    rbt->nilnode->left = rbt->nilnode;
    rbt->nilnode->right = rbt->nilnode;
    rbt->root = rbt->nilnode;
    return rbt;
}

void leftRotate(RBTree* rbt, NodeRB* x) { // левый поворот вокруг узла х
	NodeRB* y = x->right;
	x->right = y->left; // теперь правый ребенок х это левый узел у
	if (y->left != rbt->nilnode){ // если он реальный, то мы сообщаем, что родитель изменился
		y->left->parent = x;
	}
	y->parent = x->parent; // подвязываем к родителю х ещё и у (поднимаем на один уровень)
	if (x == rbt->root){ // если х был корнем, то после поворота - корень у
		rbt->root = y;
	} else { // указываем родителю х на нового ребенка
		if (x->parent->left == x){
			x->parent->left = y;
		}
		else if (x->parent->right == x){
			x->parent->right = y;
		}
	}
	y->left = x; // х теперь левый ребенок у
	x->parent = y; // родитель х теперь у
}

void rightRotate(RBTree* rbt, NodeRB* x) {
	NodeRB* y = x->left;
	x->left = y->right;
	if (y->right != rbt->nilnode){
		y->right->parent = x;
	}
	y->parent = x->parent;	
	if (x == rbt->root){	
		rbt->root = y;
	} else {
		if (x->parent->left == x){
			x->parent->left = y;
		}
		else if (x->parent->right == x){
			x->parent->right = y;
		}
	}
	y->right = x;
	x->parent = y;
}

void insertNode(RBTree* rbt, int data) {
    NodeRB* node = new NodeRB;
    node->data = data;
    node->color = RED;
    node->left = rbt->nilnode;
    node->right = rbt->nilnode;
    node->parent = rbt->nilnode;

    NodeRB* parent = rbt->nilnode;
    NodeRB* current = rbt->root;
    while (current != rbt->nilnode) {
        parent = current;
        if (node->data < current->data) {
            current = current->left;
        }
        else if (node->data > current->data) {
            current = current->right;
        }
        else {
            delete node;
            return;
        }
    }

    node->parent = parent; // вставка в дерево
    if (parent == rbt->nilnode) {
        rbt->root = node;
    }
    else if (node->data < parent->data) {
        parent->left = node;
    }
    else {
        parent->right = node;
    }

    fixInsert(rbt, node);
}

void fixInsert(RBTree* rbt, NodeRB* node) { // восстановление свойств КЧД
    while (node->parent->color == RED) { // восстановление свойства 4
        if (node->parent == node->parent->parent->left) { // родитель - левый потомок дедушки
            NodeRB* uncle = node->parent->parent->right;
            if (uncle->color == RED) { // случай 1 - дядя красный
                uncle->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else { // дядя черный
                if (node == node->parent->right) { // случай 2 - правый потомок
                    node = node->parent;
                    leftRotate(rbt, node);
                }

                node->parent->color = BLACK; // случай 3 - левый потомок
                node->parent->parent->color = RED;
                rightRotate(rbt,node->parent->parent);
            }
        }
        else { // родитель - правый потомок дедушки
            NodeRB* uncle = node->parent->parent->left;
            if (uncle->color == RED) { // случай 1 - дядя красный
                uncle->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else { // дядя черный
                if (node == node->parent->left) { // случай 2 - левый потомок
                    node = node->parent;
                    rightRotate(rbt,node);
                }

                node->parent->color = BLACK; // случай 3 - правый потомок
                node->parent->parent->color = RED;
                leftRotate(rbt, node->parent->parent);
            }
        }
    }
    rbt->root->color = BLACK; // восстановление свойства 2 
}

NodeRB* searchNode(RBTree* rbt, int data){
	NodeRB* current = rbt->root;
	while (current != rbt->nilnode && current->data != data){
		if (data < current->data){
			current = current->left;
		} else{
			current = current->right;
		}
	}
	return current;
}

NodeRB* findMin(RBTree* rbt, NodeRB* node) {
    while (node->left != rbt->nilnode) {
        node = node->left;
    }
    return node;
}

void fixDelete(RBTree* rbt, NodeRB* current){
	while (current != rbt->root && current->color == BLACK){
		if (current == current->parent->left){
			NodeRB* bro = current->parent->right;
			if (bro->color == RED){
				bro->color = BLACK;
				current->parent->color = RED;
				leftRotate(rbt, current->parent);
				bro = current->parent->right;
			} else {
				if (bro->left->color == BLACK && bro->right->color == BLACK) {
					bro->color = RED;
					current = current->parent;
				} else if (bro->left->color == RED && bro->right->color == BLACK) {
					bro->left->color = BLACK;
					bro->color = RED;
					rightRotate(rbt, bro);
					bro = current->parent->right;
				} else {
					bro->color = current->parent->color;
					current->parent->color = BLACK;
					bro->right->color = BLACK;
					leftRotate(rbt, current->parent);
					current = rbt->root; // завершмем цикл
				}
			}
		} else {
			NodeRB* bro = current->parent->left;
			if (bro->color == RED){
				bro->color = BLACK;
				current->parent->color = RED;
				rightRotate(rbt, current->parent);
				bro = current->parent->left;
			} else {
				if (bro->left->color == BLACK && bro->right->color == BLACK) {
					bro->color = RED;
					current = current->parent;
				} else if (bro->left->color == RED && bro->right->color == BLACK) {
					bro->right->color = BLACK;
					bro->color = RED;
					leftRotate(rbt, bro);
					bro = current->parent->left;
				} else {
					bro->color = current->parent->color;
					current->parent->color = BLACK;
					bro->left->color = BLACK;
					rightRotate(rbt, current->parent);
					current = rbt->root;
				}
			}
		}
	}
	current->color = BLACK;
}

void deleteNode(RBTree* rbt, int data) {
    NodeRB* node = searchNode(rbt, data);
    if (node == rbt->nilnode) { return; }
    
    Color origColor = node->color;
    NodeRB* child;
    
    // нет детей или один
    if (node->left == rbt->nilnode) {
        child = node->right;
        if (node == rbt->root) {
            rbt->root = child;
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        if (child != rbt->nilnode) {
            child->parent = node->parent;
        }
        delete node;
        
        if (origColor == BLACK) {
            fixDelete(rbt, child);
        }
    } 
    else if (node->right == rbt->nilnode) {
        child = node->left;
        if (node == rbt->root) {
            rbt->root = child;
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        if (child != rbt->nilnode) {
            child->parent = node->parent;
        }
        delete node;
        
        if (origColor == BLACK) {
            fixDelete(rbt, child);
        }
    }
    // два ребенка
    else {
        NodeRB* successor = findMin(rbt, node->right);
        origColor = successor->color;
        child = successor->right;
        if (successor->parent == node) {
            if (child != rbt->nilnode) {
                child->parent = successor;
            }
        } 
        else {
            if (successor == successor->parent->left) {
                successor->parent->left = child;
            } else {
                successor->parent->right = child;
            }
            if (child != rbt->nilnode) {
                child->parent = successor->parent;
            }
            successor->right = node->right;
            if (node->right != rbt->nilnode) {
                node->right->parent = successor;
            }
        }
        if (node == rbt->root) {
            rbt->root = successor;
        } else if (node == node->parent->left) {
            node->parent->left = successor;
        } else {
            node->parent->right = successor;
        }
        successor->parent = node->parent;
        successor->left = node->left;
        if (node->left != rbt->nilnode) {
            node->left->parent = successor;
        }
        successor->color = node->color;
        delete node;
        if (origColor == BLACK) {
            fixDelete(rbt, child);
        }
    }
}

void printRBTreeHelper(NodeRB* node, RBTree* rbt, string prefix, bool isLeft) {
    if (node == rbt->nilnode) {
        return;
    }
    const string RED_COLOR = "\033[1;31m";
    const string BLACK_COLOR = "\033[1;30m";
    const string NIL_COLOR = "\033[0;90m";
    const string RESET_COLOR = "\033[0m";
    cout << prefix;
    cout << (isLeft ? "├── " : "└── ");
    string color_str = (node->color == RED) ? "R" : "B";
    string color_code = (node->color == RED) ? RED_COLOR : BLACK_COLOR;
    cout << color_code << node->data << "(" << color_str << ")" << RESET_COLOR << endl;
    string newPrefix = prefix + (isLeft ? "│   " : "    ");
    if (node->right != rbt->nilnode || node->left != rbt->nilnode) {
        if (node->right != rbt->nilnode) {
            printRBTreeHelper(node->right, rbt, newPrefix, true);
        } else {
            cout << newPrefix << "├── " << NIL_COLOR << "nil(B)" << RESET_COLOR << endl;
        }
        
        if (node->left != rbt->nilnode) {
            printRBTreeHelper(node->left, rbt, newPrefix, false);
        } else {
            cout << newPrefix << "└── " << NIL_COLOR << "nil(B)" << RESET_COLOR << endl;
        }
    }
}

void printRBTree(RBTree* rbt) {
    if (rbt->root == rbt->nilnode) {
        cout << "Дерево пустое" << endl;
        return;
    }
    cout << "Красно-черное дерево:" << endl;
    printRBTreeHelper(rbt->root, rbt, "", false);
}

void saveRBTreeHelper(NodeRB* node, RBTree* rbt, ostream& os) {
    if (node == rbt->nilnode) {
        os << "N ";
        return;
    }
    
    os << node->data << " " << (node->color == RED ? "R " : "B ");
    saveRBTreeHelper(node->left, rbt, os);
    saveRBTreeHelper(node->right, rbt, os);
}

void saveRBTreeToStream(RBTree* rbt, ostream& os) {
    saveRBTreeHelper(rbt->root, rbt, os);
}

NodeRB* loadRBTreeHelper(RBTree* rbt, NodeRB* parent, istream& is) {
    string token;
    if (!(is >> token) || token == "N") {
        return rbt->nilnode;
    }
    
    int data = stoi(token);
    string color;
    if (!(is >> color)) {
        return rbt->nilnode;
    }
    
    NodeRB* node = new NodeRB;
    node->data = data;
    node->color = (color == "R" ? RED : BLACK);
    node->parent = parent;
    node->left = loadRBTreeHelper(rbt, node, is);
    node->right = loadRBTreeHelper(rbt, node, is);
    
    return node;
}

void loadRBTreeFromStream(RBTree* rbt, istream& is) {
    rbt->root = loadRBTreeHelper(rbt, rbt->nilnode, is);
}
