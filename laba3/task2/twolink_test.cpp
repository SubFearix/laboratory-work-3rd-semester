#include <gtest/gtest.h>
#include "structures.h"
#include <string>

class TLListTest : public ::testing::Test {
protected:
    TLList* list;
    void SetUp() override {
        list = new TLList();
    }
    void TearDown() override {
        delete list;
    }
    void FillList() {
        list->addBackTL("A");
        list->addBackTL("B");
        list->addBackTL("C");
    }
};

TEST_F(TLListTest, AddFront) {
    list->addFrontTL("B");
    list->addFrontTL("A");
    EXPECT_EQ(list->getHead()->getData(), "A");
    EXPECT_EQ(list->getTail()->getData(), "B");
    EXPECT_EQ(list->getHead()->getNext(), list->getTail());
    EXPECT_EQ(list->getTail()->getPrev(), list->getHead());
    EXPECT_EQ(list->getHead()->getPrev(), nullptr);
}

TEST_F(TLListTest, AddBack) {
    list->addBackTL("A");
    list->addBackTL("B");
    EXPECT_EQ(list->getHead()->getData(), "A");
    EXPECT_EQ(list->getTail()->getData(), "B");
    EXPECT_EQ(list->getHead()->getNext(), list->getTail());
    EXPECT_EQ(list->getTail()->getPrev(), list->getHead());
    EXPECT_EQ(list->getTail()->getNext(), nullptr);
}

TEST_F(TLListTest, insertAfter) {
    list->addBackTL("nul");
    list->insertAfterIndTL(0, "A");
    EXPECT_EQ(list->findValTL("A"), 1);
    list->addBackTL("B");
    list->addBackTL("C");
    list->addBackTL("D");
    list->insertAfterIndTL(4, "AA");
    EXPECT_EQ(list->getTail()->getData(), "AA");
    list->printListTL();
}

TEST_F(TLListTest, insertBefore) {
    list->addBackTL("B");
    list->insertBeforeIndTL(0, "A");
    EXPECT_EQ(list->getHead()->getData(), "A");
    list->addBackTL("C");
    list->addBackTL("D");
    list->insertBeforeIndTL(3, "AA");
    EXPECT_EQ(list->findValTL("AA"), 3);
}

TEST_F(TLListTest, RemFront) {
    FillList();
    list->remFrontTL();
    EXPECT_EQ(list->getHead()->getData(), "B");
    EXPECT_EQ(list->getHead()->getPrev(), nullptr);
    list->remFrontTL();
    list->remFrontTL();
    EXPECT_EQ(list->getHead(), nullptr);
    EXPECT_EQ(list->getTail(), nullptr);
    ASSERT_NO_THROW(list->remFrontTL());
}

TEST_F(TLListTest, RemBack) {
    FillList();
    list->remBackTL();
    EXPECT_EQ(list->getTail()->getData(), "B");
    EXPECT_EQ(list->getTail()->getNext(), nullptr);
    list->remBackTL();
    list->remBackTL();
    EXPECT_EQ(list->getHead(), nullptr);
    ASSERT_NO_THROW(list->remBackTL());
}

TEST_F(TLListTest, RemSingleElement) {
    list->addBackTL("Single");
    list->remBackTL();
    EXPECT_EQ(list->getHead(), nullptr);
    EXPECT_EQ(list->getTail(), nullptr);
}

TEST_F(TLListTest, FindVal) {
    FillList();
    EXPECT_EQ(list->findValTL("B"), 1);
    EXPECT_EQ(list->findValTL("Z"), -1);
}

TEST_F(TLListTest, DeleteBeforeInd) {
    list->deleteBeforeIndTL(3);
    FillList();
    list->deleteBeforeIndTL(2);
    EXPECT_EQ(list->getHead()->getData(), "A");
    EXPECT_EQ(list->getHead()->getPrev(), nullptr);

    list->deleteBeforeIndTL(1);
    EXPECT_EQ(list->getHead()->getData(), "C");

    // Проверка особого условия
    ASSERT_NO_THROW(list->deleteBeforeIndTL(0));
}

TEST_F(TLListTest, DeleteAfterInd) {
    FillList();
    list->deleteAfterIndTL(-6);
    list->deleteAfterIndTL(0);
    EXPECT_EQ(list->getHead()->getNext()->getData(), "C");
    EXPECT_EQ(list->getTail()->getPrev()->getData(), "A");

    list->deleteAfterIndTL(0);
    EXPECT_EQ(list->getTail()->getData(), "A");

    // Проверка граничного условия
    ASSERT_NO_THROW(list->deleteAfterIndTL(0));
}

TEST_F(TLListTest, remVal)
{
    list->remValTL("B");
    FillList();
    list->remValTL("A");
    EXPECT_EQ(list->getHead()->getData(), "B");
    list->addFrontTL("A");
    list->remValTL("B");
    list->remValTL("C");
    EXPECT_EQ(list->getTail()->getData(), "A");
}