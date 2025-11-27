#include <gtest/gtest.h>
#include "structures.h"
#include <string>

class OLListTest : public ::testing::Test {
protected:
    void SetUp() override {
        list = new OLList();
    }
    void TearDown() override {
        delete list;
    }
    OLList* list;
    void FillList() const
    {
        list->addBackOL("A");
        list->addBackOL("B");
        list->addBackOL("C");
    }
};

TEST_F(OLListTest, AddFront) {
    list->addFrontOL("B");
    list->addFrontOL("A");
    EXPECT_EQ(list->getHead()->getData(), "A");
    EXPECT_EQ(list->getTail()->getData(), "B");
}

TEST_F(OLListTest, AddBack) {
    list->addBackOL("A");
    list->addBackOL("B");
    EXPECT_EQ(list->getHead()->getData(), "A");
    EXPECT_EQ(list->getTail()->getData(), "B");
}

TEST_F(OLListTest, RemFront) {
    FillList();
    list->remFrontOL();
    EXPECT_EQ(list->getHead()->getData(), "B");
    list->remFrontOL();
    list->remFrontOL();
    EXPECT_EQ(list->getHead(), nullptr);
}

TEST_F(OLListTest, RemBack) {
    FillList();
    list->remBackOL();
    EXPECT_EQ(list->getTail()->getData(), "B");
    list->remBackOL();
    EXPECT_EQ(list->getHead(), list->getTail());
    EXPECT_EQ(list->getTail()->getData(), "A");
    list->remBackOL();
    EXPECT_EQ(list->getHead(), nullptr);
}

TEST_F(OLListTest, InsertAfterInd) {
    FillList();
    list->insertAfterIndOL(1, "X");
    EXPECT_EQ(list->findValOL("X"), 2);
    EXPECT_EQ(list->getHead()->getNext()->getNext()->getData(), "X");

    list->insertAfterIndOL(3, "D");
    EXPECT_EQ(list->getTail()->getData(), "D");
    EXPECT_EQ(list->findValOL("D"), 4);
}

TEST_F(OLListTest, InsertBeforeInd) {
    FillList();
    list->insertBeforeIndOL(0, "Start");
    EXPECT_EQ(list->getHead()->getData(), "Start");

    list->insertBeforeIndOL(2, "X");
    EXPECT_EQ(list->getHead()->getNext()->getNext()->getData(), "X");
}

TEST_F(OLListTest, FindVal) {
    FillList();
    EXPECT_EQ(list->findValOL("B"), 1);
    EXPECT_EQ(list->findValOL("C"), 2);
    EXPECT_EQ(list->findValOL("Z"), -1);
    EXPECT_EQ(list->findValOL("A"), 0);
}

TEST_F(OLListTest, RemVal) {
    FillList();
    list->remValOL("B");
    EXPECT_EQ(list->findValOL("B"), -1);
    EXPECT_EQ(list->getHead()->getNext()->getData(), "C");

    list->remValOL("C");
    EXPECT_EQ(list->getTail()->getData(), "A");

    list->remValOL("A");
    EXPECT_EQ(list->getHead(), nullptr);

    ASSERT_NO_THROW(list->remValOL("Z"));
}

TEST_F(OLListTest, DeleteBeforeInd) {
    FillList();
    list->deleteBeforeIndOL(1);
    EXPECT_EQ(list->getHead()->getData(), "B");

    list->deleteBeforeIndOL(1);
    EXPECT_EQ(list->getHead()->getData(), "C");
    EXPECT_EQ(list->getHead(), list->getTail());

    ASSERT_NO_THROW(list->deleteBeforeIndOL(0));
    FillList();
    list->deleteBeforeIndOL(3);
}

TEST_F(OLListTest, DeleteAfterInd) {
    FillList();
    list->deleteAfterIndOL(0);
    EXPECT_EQ(list->getHead()->getNext()->getData(), "C");

    list->deleteAfterIndOL(0);
    EXPECT_EQ(list->getTail()->getData(), "A");

    ASSERT_NO_THROW(list->deleteAfterIndOL(1));
}

TEST_F(OLListTest, Print) {
    FillList();
    list->printListOL();
}

TEST_F(OLListTest, Special) {
    list->deleteBeforeIndOL(3);
    FillList();
    list->deleteBeforeIndOL(5);
    list->deleteAfterIndOL(6);
    list->deleteAfterIndOL(-2);
    list->deleteAfterIndOL(2);
}