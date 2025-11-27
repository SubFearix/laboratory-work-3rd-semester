#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include "structures.h"

using namespace std;

class RBTreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        tree = new RBTree();
    }

    void TearDown() override {
        delete tree;
    }
    RBTree* tree;

    bool isRedBlackTreeValid() {
        if (tree->getRoot() == tree->getNilnode()) return true;

        // свойство корень черный
        if (tree->getRoot()->getColor() != BLACK) {
            return false;
        }

        // свойство все пути содержат одинаковое количество черных узлов
        int blackCount = -1;
        return checkRedBlackProperties(tree->getRoot(), 0, blackCount);
    }

    bool checkRedBlackProperties(NodeRB* node, int currentBlackCount, int& expectedBlackCount) {
        if (node == tree->getNilnode()) {
            if (expectedBlackCount == -1) {
                expectedBlackCount = currentBlackCount;
            }
            return currentBlackCount == expectedBlackCount;
        }

        // свойство нет двух красных подряд
        if (node->getColor() == RED)
        {
            if (node->getLeft()->getColor() == RED || node->getRight()->getColor() == RED) {
                return false;
            }
        }
        int nextBlackCount = currentBlackCount + (node->getColor() == BLACK ? 1 : 0); // увеличиваем счетчик черных если текущий черный
        return checkRedBlackProperties(node->getLeft(), nextBlackCount, expectedBlackCount) &&
               checkRedBlackProperties(node->getRight(), nextBlackCount, expectedBlackCount);
    }

    int getTreeSize(NodeRB* node) {
        if (node == tree->getNilnode()) return 0;
        return 1 + getTreeSize(node->getLeft()) + getTreeSize(node->getRight());
    }

    bool containsValue(NodeRB* node, int value) {
        if (node == tree->getNilnode()) return false;
        if (node->getData() == value) return true;
        if (value < node->getData()) return containsValue(node->getLeft(), value);
        return containsValue(node->getRight(), value);
    }
};

TEST_F(RBTreeTest, InsertSingleElement) {
    tree->insertNode(42);

    EXPECT_NE(tree->getRoot(), tree->getNilnode());
    EXPECT_EQ(tree->getRoot()->getData(), 42);
    EXPECT_EQ(tree->getRoot()->getColor(), BLACK);
    EXPECT_TRUE(isRedBlackTreeValid());
}

TEST_F(RBTreeTest, InsertMultipleElements) {
    vector<int> values = {50, 30, 70, 20, 40, 60, 80};
    for (int value: values) {
        tree->insertNode(value);
    }

    EXPECT_TRUE(isRedBlackTreeValid());
    EXPECT_EQ(getTreeSize(tree->getRoot()), values.size());

    for (int value: values) {
        EXPECT_TRUE(containsValue(tree->getRoot(), value));
    }
}

TEST_F(RBTreeTest, SearchExistingElements) {
    tree->insertNode(50);
    tree->insertNode(30);
    tree->insertNode(70);
    tree->insertNode(20);

    NodeRB* found = tree->searchNode(30);
    EXPECT_NE(found, tree->getNilnode());
    EXPECT_EQ(found->getData(), 30);

    found = tree->searchNode(20);
    EXPECT_NE(found, tree->getNilnode());
    EXPECT_EQ(found->getData(), 20);
}

TEST_F(RBTreeTest, SearchNonExistingElements) {
    tree->insertNode(50);
    tree->insertNode(30);

    NodeRB* found = tree->searchNode(100);
    EXPECT_EQ(found, tree->getNilnode());

    found = tree->searchNode(-10);
    EXPECT_EQ(found, tree->getNilnode());

    RBTree emptyTree;
    found = emptyTree.searchNode(50);
    EXPECT_EQ(found, emptyTree.getNilnode());
}

TEST_F(RBTreeTest, InsertDuplicates) {
    tree->insertNode(42);
    tree->insertNode(42);

    EXPECT_TRUE(isRedBlackTreeValid());
    EXPECT_EQ(getTreeSize(tree->getRoot()), 1);
}

TEST_F(RBTreeTest, DeleteLeaf) {
    tree->insertNode(50);
    tree->insertNode(30);
    tree->insertNode(70);

    int initialSize = getTreeSize(tree->getRoot());
    tree->deleteNode(30);

    EXPECT_TRUE(isRedBlackTreeValid());
    EXPECT_EQ(getTreeSize(tree->getRoot()), initialSize - 1);
    EXPECT_EQ(tree->searchNode(30), tree->getNilnode());
}

// один потомок
TEST_F(RBTreeTest, DeleteNodeWithOneChild) {
    tree->insertNode(50);
    tree->insertNode(30);
    tree->insertNode(70);
    tree->insertNode(20);

    tree->deleteNode(30);

    EXPECT_TRUE(isRedBlackTreeValid());
    EXPECT_NE(tree->searchNode(20), tree->getNilnode());
    EXPECT_EQ(tree->searchNode(30), tree->getNilnode());
}

// два потомка
TEST_F(RBTreeTest, DeleteNodeWithTwoChildren) {
    tree->insertNode(50);
    tree->insertNode(30);
    tree->insertNode(70);
    tree->insertNode(20);
    tree->insertNode(40);

    tree->deleteNode(30);

    EXPECT_TRUE(isRedBlackTreeValid());
    EXPECT_EQ(tree->searchNode(30), tree->getNilnode());

    EXPECT_NE(tree->searchNode(20), tree->getNilnode());
    EXPECT_NE(tree->searchNode(40), tree->getNilnode());
}

TEST_F(RBTreeTest, DeleteRoot) {
    tree->insertNode(50);
    tree->insertNode(30);
    tree->insertNode(70);

    tree->deleteNode(50);

    EXPECT_TRUE(isRedBlackTreeValid());
    EXPECT_EQ(tree->searchNode(50), tree->getNilnode());
    EXPECT_NE(tree->getRoot(), tree->getNilnode());
}

TEST_F(RBTreeTest, DeleteFromEmptyTree) {
    EXPECT_NO_THROW(tree->deleteNode(42));
    EXPECT_TRUE(isRedBlackTreeValid());
}

TEST_F(RBTreeTest, DeleteNonExistingElement) {
    tree->insertNode(50);
    tree->insertNode(30);

    int initialSize = getTreeSize(tree->getRoot());
    tree->deleteNode(100);

    EXPECT_TRUE(isRedBlackTreeValid());
    EXPECT_EQ(getTreeSize(tree->getRoot()), initialSize);
}

TEST_F(RBTreeTest, SequentialInsertionsAndDeletions) {
    vector<int> values = {10, 20, 5, 15, 25, 3, 8, 12, 18, 22, 28};
    for (int value: values) {
        tree->insertNode(value);
        EXPECT_TRUE(isRedBlackTreeValid());
    }
    EXPECT_EQ(getTreeSize(tree->getRoot()), values.size());

    vector<int> toDelete = {5, 15, 25, 8};
    for (int value: toDelete) {
        tree->deleteNode(value);
        EXPECT_TRUE(isRedBlackTreeValid());
    }

    EXPECT_EQ(getTreeSize(tree->getRoot()), values.size() - toDelete.size());

    for (int value: values) {
        bool shouldExist = std::find(toDelete.begin(), toDelete.end(), value) == toDelete.end();
        EXPECT_EQ(containsValue(tree->getRoot(), value), shouldExist);
    }
}

// много операцтй
TEST_F(RBTreeTest, RedBlackPropertiesAfterOperations) {
    vector<int> operations;
    for (int i = 0; i < 50; i++) {
        operations.push_back(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(operations.begin(), operations.end(), g);

    for (int op: operations) {
        if (op % 3 == 0) {
            tree->deleteNode(op);
        } else {
            tree->insertNode(op);
        }
        EXPECT_TRUE(isRedBlackTreeValid());
    }
}

TEST_F(RBTreeTest, FindMinFunction) {
    EXPECT_EQ(tree->findMin(tree->getRoot()), tree->getNilnode());
    tree->insertNode(50);
    tree->insertNode(30);
    tree->insertNode(70);
    tree->insertNode(20);
    tree->insertNode(40);
    tree->insertNode(60);
    tree->insertNode(80);

    NodeRB* minNode = tree->findMin(tree->getRoot());
    EXPECT_NE(minNode, tree->getNilnode());
    EXPECT_EQ(minNode->getData(), 20);

    NodeRB* subtreeRoot = tree->searchNode(70);
    minNode = tree->findMin(subtreeRoot);
    EXPECT_NE(minNode, tree->getNilnode());
    EXPECT_EQ(minNode->getData(), 60);
}

TEST_F(RBTreeTest, PrintEmptyTree) {
    testing::internal::CaptureStdout();
    tree->printRBTree();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Дерево пустое") != string::npos);
}

TEST_F(RBTreeTest, PrintNonEmptyTree) {
    tree->insertNode(50);
    tree->insertNode(30);
    tree->insertNode(70);

    testing::internal::CaptureStdout();
    tree->printRBTree();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Красно-черное дерево") != string::npos);
}