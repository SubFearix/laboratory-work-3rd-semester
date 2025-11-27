package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/suite"
)

type QueueTestSuite struct {
	suite.Suite
	queue *Queue
}

func (suite *QueueTestSuite) SetupTest() {
	suite.queue = &Queue{}
}

func (suite *QueueTestSuite) TestAddBack() {
	suite.queue.addBackQueue("A")

	assert.NotNil(suite.T(), suite.queue.getHead())
	assert.Equal(suite.T(), suite.queue.getHead(), suite.queue.getTail())
	assert.Equal(suite.T(), "A", suite.queue.getHead().data)
	assert.Nil(suite.T(), suite.queue.getHead().next)

	suite.queue.addBackQueue("B")
	assert.Equal(suite.T(), "A", suite.queue.getHead().data)
	assert.Equal(suite.T(), "B", suite.queue.getTail().data)
	assert.Equal(suite.T(), suite.queue.getHead().next, suite.queue.getTail())
}

func (suite *QueueTestSuite) TestRemFront() {
	suite.queue.addBackQueue("A")
	suite.queue.addBackQueue("B")
	suite.queue.addBackQueue("C")

	suite.queue.remFrontQueue()
	assert.Equal(suite.T(), "B", suite.queue.getHead().data)
	assert.Equal(suite.T(), "C", suite.queue.getTail().data)

	suite.queue.remFrontQueue()
	assert.Equal(suite.T(), "C", suite.queue.getHead().data)
	assert.Equal(suite.T(), suite.queue.getHead(), suite.queue.getTail())

	suite.queue.remFrontQueue()
	assert.Nil(suite.T(), suite.queue.getHead())
	assert.Nil(suite.T(), suite.queue.getTail())
}

func (suite *QueueTestSuite) TestRemFrontEmpty() {
	suite.queue.remFrontQueue()
	assert.Nil(suite.T(), suite.queue.getHead())
	assert.Nil(suite.T(), suite.queue.getTail())
}

func (suite *QueueTestSuite) TestCreateNode() {
	node := suite.queue.createNodeQueue("test")
	assert.Equal(suite.T(), "test", node.data)
	assert.Nil(suite.T(), node.next)
	assert.Nil(suite.T(), node.prev)
}

func (suite *QueueTestSuite) TestPrintQueue() {
	suite.queue.printQueue()
	suite.queue.addBackQueue("X")
	suite.queue.addBackQueue("Y")
	suite.queue.addBackQueue("Z")
	suite.queue.printQueue()

	assert.Equal(suite.T(), "X", suite.queue.getHead().data)
	assert.Equal(suite.T(), "Z", suite.queue.getTail().data)
}

func (suite *QueueTestSuite) TestEdgeCases() {
	assert.Nil(suite.T(), suite.queue.getHead())
	assert.Nil(suite.T(), suite.queue.getTail())

	suite.queue.remFrontQueue()
	suite.queue.remFrontQueue()
	suite.queue.remFrontQueue()
	assert.Nil(suite.T(), suite.queue.getHead())

	suite.queue.addBackQueue("Recovery")
	assert.Equal(suite.T(), "Recovery", suite.queue.getHead().data)
	assert.Equal(suite.T(), suite.queue.getHead(), suite.queue.getTail())
}

func TestQueueTestSuite(t *testing.T) {
	suite.Run(t, new(QueueTestSuite))
}

func TestQueueEmptyState(t *testing.T) {
	queue := &Queue{}

	assert.Nil(t, queue.getHead())
	assert.Nil(t, queue.getTail())

	queue.remFrontQueue()
	queue.printQueue()

	node := queue.createNodeQueue("test")
	assert.Equal(t, "test", node.data)
}
