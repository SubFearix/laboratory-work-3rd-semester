#include <gtest/gtest.h>
#include "structures.h"
#include "hashaddress.cpp"

using namespace std;

class HashATest : public ::testing::Test {
protected:
    void SetUp() override {
        hashTable = new HashA(5);
    }

    void TearDown() override {
        delete hashTable;
    }

    HashA* hashTable;
};

TEST_F(HashATest, AddElement) {
    hashTable->addElementA("key1", 100);
    EXPECT_EQ(hashTable->getSize(), 1);

    hashTable->addElementA("key2", 200);
    EXPECT_EQ(hashTable->getSize(), 2);
}

// Обновление значения
TEST_F(HashATest, AddElementUpdate) {
    hashTable->addElementA("key1", 100);
    hashTable->addElementA("key1", 200);

    EXPECT_EQ(hashTable->getSize(), 1);
    EXPECT_EQ(hashTable->findElementA("key1"), 200);
}

// Поиск существующих
TEST_F(HashATest, FindExistingElement) {
    hashTable->addElementA("test1", 42);
    hashTable->addElementA("test2", 84);

    EXPECT_EQ(hashTable->findElementA("test1"), 42);
    EXPECT_EQ(hashTable->findElementA("test2"), 84);
}

// Поиск несуществующих
TEST_F(HashATest, FindNonExistingElement) {
    EXPECT_EQ(hashTable->findElementA("nonexistent"), -1);

    hashTable->addElementA("existing", 100);
    EXPECT_EQ(hashTable->findElementA("nonexistent"), -1);
}

// Удаление существующих
TEST_F(HashATest, DeleteExistingElement) {
    hashTable->addElementA("delete_me", 123);
    EXPECT_EQ(hashTable->getSize(), 1);

    hashTable->deleteElementA("delete_me");
    EXPECT_EQ(hashTable->getSize(), 0);
    EXPECT_EQ(hashTable->findElementA("delete_me"), -1);
}

// Удаление несуществующих
TEST_F(HashATest, DeleteNonExistingElement) {
    testing::internal::CaptureStdout();
    hashTable->deleteElementA("nonexistent");
    const string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("не найден") != string::npos);
}

TEST_F(HashATest, HashFunction) {
    const int hash1 = hashTable->hashFunc("test", 10);
    const int hash2 = hashTable->hashFunc("test", 10);
    EXPECT_EQ(hash1, hash2);

    EXPECT_GE(hash1, 0);
    EXPECT_LT(hash1, 10);
}

// Коллизии
TEST_F(HashATest, CollisionsAndLinearProbing) {
    HashA smallTable(3);

    smallTable.addElementA("a", 1);
    smallTable.addElementA("b", 2);
    smallTable.addElementA("c", 3);

    EXPECT_EQ(smallTable.findElementA("a"), 1);
    EXPECT_EQ(smallTable.findElementA("b"), 2);
    EXPECT_EQ(smallTable.findElementA("c"), 3);
}

TEST_F(HashATest, DeleteAndReadd) {
    hashTable->addElementA("temp", 999);
    EXPECT_EQ(hashTable->findElementA("temp"), 999);

    hashTable->deleteElementA("temp");
    EXPECT_EQ(hashTable->findElementA("temp"), -1);

    hashTable->addElementA("temp", 111);
    EXPECT_EQ(hashTable->findElementA("temp"), 111);
}

TEST_F(HashATest, PrintTable) {
    hashTable->addElementA("print1", 1);
    hashTable->addElementA("print2", 2);
    hashTable->addElementA("print3", 3);
    hashTable->deleteElementA("print1");

    testing::internal::CaptureStdout();
    hashTable->printHashTableA();
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("print1") != string::npos ||
                output.find("print2") != string::npos ||
                output.find("Хэш-таблица") != string::npos);
}
// Рехэширование
TEST_F(HashATest, Rehash)
{
    const int cap = hashTable->getCapacity();
    hashTable->addElementA("hash", 1);
    hashTable->addElementA("hash2", 2);
    hashTable->addElementA("hash3", 3);
    hashTable->addElementA("hash4", 4);
    hashTable->addElementA("hash5", 5);
    EXPECT_EQ(hashTable->getCapacity(), cap * 2);
}

// Особые
TEST_F(HashATest, EdgeCases) {
    EXPECT_EQ(hashTable->getSize(), 0);

    EXPECT_EQ(hashTable->findElementA("anything"), -1);

    testing::internal::CaptureStdout();
    hashTable->deleteElementA("nothing");
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("не найден") != string::npos);
}