#include <gtest/gtest.h>
#include "structures.h"
#include <string>

class StackTest : public ::testing::Test {
protected:
    void SetUp() override {
        s = new Stack();
    }
    void TearDown() override {
        delete s;
    }
    Stack* s;
};

TEST_F(StackTest, push) {
    s->pushStack("A");
    EXPECT_EQ(s->getTop()->getData(), "A");
}

TEST_F(StackTest, pop) {
    s->pushStack("A");
    s->pushStack("B");
    s->pushStack("C");
    s->popStack();
    EXPECT_EQ(s->getTop()->getData(), "B");
}

TEST_F(StackTest, Print) {
    s->pushStack("C");
    s->printStack();
}