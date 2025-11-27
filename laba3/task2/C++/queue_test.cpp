#include <gtest/gtest.h>
#include "structures.h"
#include <string>

class QueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        q = new Queue();
    }
    void TearDown() override {
        delete q;
    }
    Queue* q;
};

TEST_F(QueueTest, AddBack) {
    q->addBackQueue("A");
    EXPECT_NE(q->getHead(), nullptr);
    EXPECT_EQ(q->getHead(), q->getTail());
    EXPECT_EQ(q->getHead()->getData(), "A");
}

TEST_F(QueueTest, RemFront) {
    q->addBackQueue("A");
    q->addBackQueue("B");
    q->addBackQueue("C");
    q->remFrontQueue();
    EXPECT_EQ(q->getHead()->getData(), "B");
    EXPECT_EQ(q->getTail()->getData(), "C");
}

TEST_F(QueueTest, Print) {
    q->addBackQueue("C");
    q->printQueue();
}