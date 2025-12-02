#include <iostream>
#include <sstream>
#include "structures.h"

using namespace std;

void RBTree::leftRotate(NodeRB* x) { // левый поворот вокруг узла х
	NodeRB* y = x->right;
	x->right = y->left; // теперь правый ребенок х это левый узел у
	if (y->left != this->nilnode){ // если он реальный, то мы сообщаем, что родитель изменился
		y->left->parent = x;
	}
	y->parent = x->parent; // подвязываем к родителю х ещё и у (поднимаем на один уровень)
	if (x == this->root){ // если х был корнем, то после поворота - корень у
		this->root = y;
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

void RBTree::rightRotate(NodeRB* x) {
	NodeRB* y = x->left;
	x->left = y->right;
	if (y->right != this->nilnode){
		y->right->parent = x;
	}
	y->parent = x->parent;	
	if (x == this->root){	
		this->root = y;
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

void RBTree::insertNode(const int data) {
    auto* node = new NodeRB;
    node->data = data;
    node->color = RED;
    node->left = this->nilnode;
    node->right = this->nilnode;
    node->parent = this->nilnode;

    NodeRB* parent = this->nilnode;
    NodeRB* current = this->root;
    while (current != this->nilnode) {
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
    if (parent == this->nilnode) {
        this->root = node;
    }
    else if (node->data < parent->data) {
        parent->left = node;
    }
    else {
        parent->right = node;
    }

    fixInsert(node);
}

void RBTree::fixInsert(NodeRB* node) { // восстановление свойств КЧД
    while (node->parent->color == RED) { // восстановление свойства 4
        if (node->parent == node->parent->parent->left) {
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
                    leftRotate(node);
                }

                node->parent->color = BLACK; // случай 3 - левый потомок
                node->parent->parent->color = RED;
                rightRotate(node->parent->parent);
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
                    rightRotate(node);
                }

                node->parent->color = BLACK; // случай 3 - правый потомок
                node->parent->parent->color = RED;
                leftRotate(node->parent->parent);
            }
        }
    }
    this->root->color = BLACK; // восстановление свойства 2 
}

NodeRB* RBTree::searchNode(const int data) const
{
	NodeRB* current = this->root;
	while (current != this->nilnode && current->data != data){
		if (data < current->data){
			current = current->left;
		} else{
			current = current->right;
		}
	}
	return current;
}

NodeRB* RBTree::findMin(NodeRB* node) const
{
    while (node->left != this->nilnode) {
        node = node->left;
    }
    return node;
}

void RBTree::fixDelete(NodeRB* current){
	while (current != this->root && current->color == BLACK){
		if (current == current->parent->left){
			NodeRB* bro = current->parent->right;
			if (bro->color == RED){
				bro->color = BLACK;
				current->parent->color = RED;
				leftRotate(current->parent);
				bro = current->parent->right;
			} else {
				if (bro->left->color == BLACK && bro->right->color == BLACK) {
					bro->color = RED;
					current = current->parent;
				} else if (bro->left->color == RED && bro->right->color == BLACK) {
					bro->left->color = BLACK;
					bro->color = RED;
					rightRotate(bro);
					bro = current->parent->right;
				} else {
					bro->color = current->parent->color;
					current->parent->color = BLACK;
					bro->right->color = BLACK;
					leftRotate(current->parent);
					current = this->root; // завершмем цикл
				}
			}
		} else {
			NodeRB* bro = current->parent->left;
			if (bro->color == RED){
				bro->color = BLACK;
				current->parent->color = RED;
				rightRotate(current->parent);
				bro = current->parent->left;
			} else {
				if (bro->left->color == BLACK && bro->right->color == BLACK) {
					bro->color = RED;
					current = current->parent;
				} else if (bro->left->color == RED && bro->right->color == BLACK) {
					bro->right->color = BLACK;
					bro->color = RED;
					leftRotate(bro);
					bro = current->parent->left;
				} else {
					bro->color = current->parent->color;
					current->parent->color = BLACK;
					bro->left->color = BLACK;
					rightRotate(current->parent);
					current = this->root;
				}
			}
		}
	}
	current->color = BLACK;
}

void RBTree::deleteNode(const int data) {
    const NodeRB* node = searchNode(data);
    if (node == this->nilnode) { return; }
    
    Color origColor = node->color;
    NodeRB* child;
    
    // нет детей или один
    if (node->left == this->nilnode) {
        child = node->right;
        if (node == this->root) {
            this->root = child;
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        if (child != this->nilnode) {
            child->parent = node->parent;
        }
        delete node;
        
        if (origColor == BLACK) {
            fixDelete(child);
        }
    } 
    else if (node->right == this->nilnode) {
        child = node->left;
        if (node == this->root) {
            this->root = child;
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        if (child != this->nilnode) {
            child->parent = node->parent;
        }
        delete node;
        
        if (origColor == BLACK) {
            fixDelete(child);
        }
    }
    // два ребенка
    else {
        NodeRB* successor = findMin(node->right);
        origColor = successor->color;
        child = successor->right;
        if (successor->parent == node) {
            child->parent = successor;
        } 
        else {
            if (successor == successor->parent->left) {
                successor->parent->left = child;
            } else {
                successor->parent->right = child;
            }
            if (child != this->nilnode) {
                child->parent = successor->parent;
            }
            successor->right = node->right;
            if (node->right != this->nilnode) {
                node->right->parent = successor;
            }
        }
        if (node == this->root) {
            this->root = successor;
        } else if (node == node->parent->left) {
            node->parent->left = successor;
        } else {
            node->parent->right = successor;
        }
        successor->parent = node->parent;
        successor->left = node->left;
        if (node->left != this->nilnode) {
            node->left->parent = successor;
        }
        successor->color = node->color;
        delete node;
        if (origColor == BLACK) {
            fixDelete(child);
        }
    }
}

void RBTree::printRBTreeHelper(const NodeRB* node, const string& prefix, const bool isLeft) {
    if (node == this->nilnode) {
        return;
    }
    const string RED_COLOR = "\033[1;31m";
    const string BLACK_COLOR = "\033[1;30m";
    const string NIL_COLOR = "\033[0;90m";
    const string RESET_COLOR = "\033[0m";
    cout << prefix;
    cout << (isLeft ? "├── " : "└── ");
    const string color_str = node->color == RED ? "R" : "B";
    const string color_code = node->color == RED ? RED_COLOR : BLACK_COLOR;
    cout << color_code << node->data << "(" << color_str << ")" << RESET_COLOR << endl;
    const string newPrefix = prefix + (isLeft ? "│   " : "    ");
    if (node->right != this->nilnode || node->left != this->nilnode) {
        if (node->right != this->nilnode) {
            printRBTreeHelper(node->right, newPrefix, true);
        } else {
            cout << newPrefix << "├── " << NIL_COLOR << "nil(B)" << RESET_COLOR << endl;
        }
        
        if (node->left != this->nilnode) {
            printRBTreeHelper(node->left, newPrefix, false);
        } else {
            cout << newPrefix << "└── " << NIL_COLOR << "nil(B)" << RESET_COLOR << endl;
        }
    }
}

void RBTree::deleteTree(const NodeRB* node) {
	if (node->left != nilnode) {
	    deleteTree(node->left);
	}
	if (node->right != nilnode) {
	    deleteTree(node->right);
	}
	delete node;
}

void RBTree::printRBTree() {
    if (this->root == this->nilnode) {
        cout << "Дерево пустое" << endl;
        return;
    }
    cout << "Красно-черное дерево:" << endl;
    printRBTreeHelper(this->root, "", false);
}
