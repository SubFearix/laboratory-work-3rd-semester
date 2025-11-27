#include <gtest/gtest.h>
#include "structures.h"
#include "hashchain.cpp"

using namespace std;

class HashTest : public ::testing::Test {
protected:
    void SetUp() override {
        hashTable = new Hash(5);
    }

    void TearDown() override {
        delete hashTable;
    }

    Hash* hashTable;
};

TEST_F(HashTest, addElement) {
    hashTable->addElement("key1", 100);
    EXPECT_EQ(hashTable->getSize(), 1);

    hashTable->addElement("key2", 200);
    EXPECT_EQ(hashTable->getSize(), 2);
}

// Обновление значения
TEST_F(HashTest, addElementUpdate) {
    hashTable->addElement("key1", 100);
    hashTable->addElement("key1", 200);

    EXPECT_EQ(hashTable->getSize(), 1);
    EXPECT_EQ(hashTable->findElement("key1"), 200);
}

// Поиск существующих
TEST_F(HashTest, FindExistingElement) {
    hashTable->addElement("test1", 42);
    hashTable->addElement("test2", 84);

    EXPECT_EQ(hashTable->findElement("test1"), 42);
    EXPECT_EQ(hashTable->findElement("test2"), 84);
}

// Удаление существующих
TEST_F(HashTest, DeleteExistingElement) {
    hashTable->addElement("delete_me", 123);
    EXPECT_EQ(hashTable->getSize(), 1);

    hashTable->deleteElement("delete_me");
    EXPECT_EQ(hashTable->getSize(), 0);
    EXPECT_EQ(hashTable->findElement("delete_me"), -1);
}

TEST_F(HashTest, HashFunction) {
    const int hash1 = hashTable->hashFunc("test", 10);
    const int hash2 = hashTable->hashFunc("test", 10);
    EXPECT_EQ(hash1, hash2);

    EXPECT_GE(hash1, 0);
    EXPECT_LT(hash1, 10);
}

// Коллизии
TEST_F(HashTest, CollisionsAndLinearProbing) {
    Hash smallTable(3);

    smallTable.addElement("a", 1);
    smallTable.addElement("b", 2);
    smallTable.addElement("c", 3);
    EXPECT_EQ(smallTable.findElement("a"), 1);

    smallTable.addElement("d", 4);
    smallTable.addElement("f", 7);
    smallTable.addElement("g", 8);
    smallTable.addElement("o", 88);
    smallTable.addElement("i", 888);
    smallTable.addElement("l", 8888);
    smallTable.addElement("t", 9);
    smallTable.addElement("m", 5);

    EXPECT_EQ(smallTable.findElement("a"), 1);
    EXPECT_EQ(smallTable.findElement("b"), 2);
    EXPECT_EQ(smallTable.findElement("m"), 5);
    const int sz = smallTable.getSize();
    smallTable.deleteElement("i");
    EXPECT_EQ(smallTable.getSize(), sz-1);
}

TEST_F(HashTest, DeleteAndReadd) {
    hashTable->addElement("temp", 999);
    EXPECT_EQ(hashTable->findElement("temp"), 999);

    hashTable->deleteElement("temp");
    EXPECT_EQ(hashTable->findElement("temp"), -1);

    hashTable->addElement("temp", 111);
    EXPECT_EQ(hashTable->findElement("temp"), 111);
}

TEST_F(HashTest, PrintTable) {
    hashTable->addElement("print1", 1);
    hashTable->addElement("print2", 2);
    hashTable->addElement("print3", 3);
    hashTable->deleteElement("print1");

    testing::internal::CaptureStdout();
    hashTable->printHashTable();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("print1") != string::npos ||
                output.find("print2") != string::npos ||
                output.find("Хэш-таблица") != string::npos);
}

// Особые
TEST_F(HashTest, EdgeCases) {
    EXPECT_EQ(hashTable->getSize(), 0);
    EXPECT_EQ(hashTable->findElement("anything"), -1);
}
