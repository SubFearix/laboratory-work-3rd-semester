#include <gtest/gtest.h>
#include "structures.h"
#include "massiv.cpp"

using namespace std;

class ArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        arr = new Array(5);
    }

    void TearDown() override {
        delete arr;
    }

    Array* arr;
};

TEST_F(ArrayTest, GetElementOutOfRange) {
    EXPECT_EQ(arr->getElemIndArray(-1), "");
    EXPECT_EQ(arr->getElemIndArray(0), "");

    arr->addBackArray("A");
    EXPECT_EQ(arr->getElemIndArray(1), "");
}

TEST_F(ArrayTest, ChangeElement) {
    arr->addBackArray("Old");
    arr->addBackArray("Value");

    arr->changeElemIndArray(0, "New");

    EXPECT_EQ(arr->getElemIndArray(0), "New");
    EXPECT_EQ(arr->getElemIndArray(1), "Value");

    arr->changeElemIndArray(2, "Fail");
    EXPECT_EQ(arr->getElemIndArray(2), "");
}

TEST_F(ArrayTest, AddBack) {
    arr->addBackArray("First");
    arr->addBackArray("Second");

    EXPECT_EQ(arr->getSize(), 2);
    EXPECT_EQ(arr->getElemIndArray(0), "First");
    EXPECT_EQ(arr->getElemIndArray(1), "Second");
    EXPECT_EQ(arr->getCapacity(), 5);
}

TEST_F(ArrayTest, AddElementAtIndex) {
    arr->addBackArray("A");
    arr->addBackArray("B");

    arr->addElemIndArray(1, "X");
    EXPECT_EQ(arr->getSize(), 3);
    EXPECT_EQ(arr->getElemIndArray(1), "X");
    EXPECT_EQ(arr->getElemIndArray(2), "B");

    arr->addElemIndArray(0, "Start");
    EXPECT_EQ(arr->getElemIndArray(0), "Start");
    EXPECT_EQ(arr->getSize(), 4);

    arr->addElemIndArray(6, "Fail");
    EXPECT_EQ(arr->getSize(), 4);

    arr->addElemIndArray(-1, "Fail");
    EXPECT_EQ(arr->getSize(), 4);
}

TEST_F(ArrayTest, RehashAddBack) {
    arr->addBackArray("1");
    arr->addBackArray("2");
    arr->addBackArray("3");
    arr->addBackArray("4");
    arr->addBackArray("5");

    EXPECT_EQ(arr->getCapacity(), 5);
    arr->addBackArray("6");

    EXPECT_EQ(arr->getCapacity(), 10);
    EXPECT_EQ(arr->getSize(), 6);
    EXPECT_EQ(arr->getElemIndArray(4), "5");
}

TEST_F(ArrayTest, RehashAddAtIndex) {
    arr->addBackArray("1");
    arr->addBackArray("2");
    arr->addBackArray("3");
    arr->addBackArray("4");

    EXPECT_EQ(arr->getCapacity(), 5);

    arr->addElemIndArray(1, "X");
    EXPECT_EQ(arr->getSize(), 5);
    EXPECT_EQ(arr->getCapacity(), 5);

    arr->addElemIndArray(2, "Y");

    EXPECT_EQ(arr->getCapacity(), 10);
    EXPECT_EQ(arr->getSize(), 6);
    EXPECT_EQ(arr->getElemIndArray(2), "Y");
}

TEST_F(ArrayTest, RemoveElementAtIndex) {
    arr->addBackArray("A");
    arr->addBackArray("B");
    arr->addBackArray("C");

    arr->remElemIndArray(1);
    EXPECT_EQ(arr->getSize(), 2);
    EXPECT_EQ(arr->getElemIndArray(0), "A");
    EXPECT_EQ(arr->getElemIndArray(1), "C");

    arr->remElemIndArray(1);
    EXPECT_EQ(arr->getSize(), 1);
    EXPECT_EQ(arr->getElemIndArray(0), "A");

    arr->remElemIndArray(0);
    EXPECT_EQ(arr->getSize(), 0);

    arr->remElemIndArray(0);
    EXPECT_EQ(arr->getSize(), 0);
    arr->remElemIndArray(-1);
    EXPECT_EQ(arr->getSizeArray(), 0);
}

TEST_F(ArrayTest, PrintArray)
{
    arr->addBackArray("A");
    arr->addBackArray("B");
    arr->addBackArray("C");
    arr->printArray();
}