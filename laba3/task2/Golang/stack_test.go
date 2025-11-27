package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/suite"
)

type StackTestSuite struct {
	suite.Suite
	stack *Stack
}

func (suite *StackTestSuite) SetupTest() {
	suite.stack = &Stack{}
}

func (suite *StackTestSuite) TestPush() {
	suite.stack.pushStack("A")

	assert.NotNil(suite.T(), suite.stack.getTop())
	assert.Equal(suite.T(), "A", suite.stack.getTop().data)
	assert.Nil(suite.T(), suite.stack.getTop().next)

	suite.stack.pushStack("B")
	assert.Equal(suite.T(), "B", suite.stack.getTop().data)
	assert.Equal(suite.T(), "A", suite.stack.getTop().next.data)
	assert.Nil(suite.T(), suite.stack.getTop().next.next)
}

func (suite *StackTestSuite) TestPop() {
	suite.stack.pushStack("A")
	suite.stack.pushStack("B")
	suite.stack.pushStack("C")

	suite.stack.popStack()
	assert.Equal(suite.T(), "B", suite.stack.getTop().data)
	assert.Equal(suite.T(), "A", suite.stack.getTop().next.data)

	suite.stack.popStack()
	assert.Equal(suite.T(), "A", suite.stack.getTop().data)
	assert.Nil(suite.T(), suite.stack.getTop().next)

	suite.stack.popStack()
	assert.Nil(suite.T(), suite.stack.getTop())
}

func (suite *StackTestSuite) TestPopEmpty() {
	suite.stack.popStack()
	assert.Nil(suite.T(), suite.stack.getTop())
}

func (suite *StackTestSuite) TestCreateNode() {
	node := suite.stack.createNodeSt("test")
	assert.Equal(suite.T(), "test", node.data)
	
	assert.Nil(suite.T(), node.next)
	assert.Nil(suite.T(), node.prev)
}

func (suite *StackTestSuite) TestPrintStack() {
	suite.stack.printStack()

	suite.stack.pushStack("X")
	suite.stack.pushStack("Y")
	suite.stack.pushStack("Z")
	suite.stack.printStack()

	assert.Equal(suite.T(), "Z", suite.stack.getTop().data)
	assert.Equal(suite.T(), "Y", suite.stack.getTop().next.data)
	assert.Equal(suite.T(), "X", suite.stack.getTop().next.next.data)
}

func (suite *StackTestSuite) TestEdgeCases() {

	assert.Nil(suite.T(), suite.stack.getTop())

	suite.stack.popStack()
	suite.stack.popStack()
	suite.stack.popStack()
	assert.Nil(suite.T(), suite.stack.getTop())

	suite.stack.pushStack("Recovery")
	assert.Equal(suite.T(), "Recovery", suite.stack.getTop().data)
	assert.Nil(suite.T(), suite.stack.getTop().next)
}

func TestStackTestSuite(t *testing.T) {
	suite.Run(t, new(StackTestSuite))
}

func TestStackMixedOperations(t *testing.T) {
	stack := &Stack{}

	stack.pushStack("A")
	stack.pushStack("B")
	stack.popStack()
	stack.pushStack("C")
	stack.pushStack("D")
	stack.popStack()
	stack.pushStack("E")

	assert.Equal(t, "E", stack.getTop().data)
	assert.Equal(t, "C", stack.getTop().next.data)
	assert.Equal(t, "A", stack.getTop().next.next.data)
	assert.Nil(t, stack.getTop().next.next.next)
}

func TestStackGetTop(t *testing.T) {
	stack := &Stack{}

	assert.Nil(t, stack.getTop())
	stack.pushStack("A")
	assert.Equal(t, "A", stack.getTop().data)

	stack.pushStack("B")
	assert.Equal(t, "B", stack.getTop().data)

	stack.popStack()
	assert.Equal(t, "A", stack.getTop().data)
}

func TestStackCreateNode(t *testing.T) {
	stack := &Stack{}

	node1 := stack.createNodeSt("test1")
	assert.Equal(t, "test1", node1.data)
	assert.Nil(t, node1.next)
	assert.Nil(t, node1.prev)

	node2 := stack.createNodeSt("test2")
	assert.Equal(t, "test2", node2.data)
	assert.Nil(t, node2.next)
}
