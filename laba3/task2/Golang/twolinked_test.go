package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/suite"
)

type TLListTestSuite struct {
	suite.Suite
	list *TLList
}

func (suite *TLListTestSuite) SetupTest() {
	suite.list = &TLList{}
}

func (suite *TLListTestSuite) fillList() {
	suite.list.addBackTL("A")
	suite.list.addBackTL("B")
	suite.list.addBackTL("C")
}

func (suite *TLListTestSuite) TestAddFront() {
	suite.list.addFrontTL("B")
	suite.list.addFrontTL("A")

	assert.Equal(suite.T(), "A", suite.list.getHead().data)
	assert.Equal(suite.T(), "B", suite.list.getTail().data)
	assert.Equal(suite.T(), suite.list.getHead().next, suite.list.getTail())
	assert.Equal(suite.T(), suite.list.getTail().prev, suite.list.getHead())
	assert.Nil(suite.T(), suite.list.getHead().prev)
}

func (suite *TLListTestSuite) TestAddBack() {
	suite.list.addBackTL("A")
	suite.list.addBackTL("B")

	assert.Equal(suite.T(), "A", suite.list.getHead().data)
	assert.Equal(suite.T(), "B", suite.list.getTail().data)
	assert.Equal(suite.T(), suite.list.getHead().next, suite.list.getTail())
	assert.Equal(suite.T(), suite.list.getTail().prev, suite.list.getHead())
	assert.Nil(suite.T(), suite.list.getTail().next)
}

func (suite *TLListTestSuite) TestInsertAfter() {
	suite.list.addBackTL("nul")
	suite.list.insertAfterIndTL(0, "A")
	assert.Equal(suite.T(), 1, suite.list.findValTL("A"))

	suite.list.addBackTL("B")
	suite.list.addBackTL("C")
	suite.list.addBackTL("D")
	suite.list.insertAfterIndTL(4, "AA")
	assert.Equal(suite.T(), "AA", suite.list.getTail().data)
}

func (suite *TLListTestSuite) TestInsertBefore() {
	suite.list.addBackTL("B")
	suite.list.insertBeforeIndTL(0, "A")
	assert.Equal(suite.T(), "A", suite.list.getHead().data)

	suite.list.addBackTL("C")
	suite.list.addBackTL("D")
	suite.list.insertBeforeIndTL(3, "AA")
	assert.Equal(suite.T(), 3, suite.list.findValTL("AA"))
}

func (suite *TLListTestSuite) TestRemFront() {
	suite.fillList()

	suite.list.remFrontTL()
	assert.Equal(suite.T(), "B", suite.list.getHead().data)
	assert.Nil(suite.T(), suite.list.getHead().prev)

	suite.list.remFrontTL()
	suite.list.remFrontTL()
	assert.Nil(suite.T(), suite.list.getHead())
	assert.Nil(suite.T(), suite.list.getTail())

	suite.list.remFrontTL()
}

func (suite *TLListTestSuite) TestRemBack() {
	suite.fillList()

	suite.list.remBackTL()
	assert.Equal(suite.T(), "B", suite.list.getTail().data)
	assert.Nil(suite.T(), suite.list.getTail().next)

	suite.list.remBackTL()
	suite.list.remBackTL()
	assert.Nil(suite.T(), suite.list.getHead())
	assert.Nil(suite.T(), suite.list.getTail())

	suite.list.remBackTL()
}

func (suite *TLListTestSuite) TestRemSingleElement() {
	suite.list.addBackTL("Single")
	suite.list.remBackTL()
	assert.Nil(suite.T(), suite.list.getHead())
	assert.Nil(suite.T(), suite.list.getTail())
}

func (suite *TLListTestSuite) TestFindVal() {
	suite.fillList()

	assert.Equal(suite.T(), 1, suite.list.findValTL("B"))
	assert.Equal(suite.T(), -1, suite.list.findValTL("Z"))
}

func (suite *TLListTestSuite) TestDeleteBeforeInd() {
	suite.list.deleteBeforeIndTL(3)

	suite.fillList()
	suite.list.deleteBeforeIndTL(2)
	assert.Equal(suite.T(), "A", suite.list.getHead().data)
	assert.Nil(suite.T(), suite.list.getHead().prev)

	suite.list.deleteBeforeIndTL(1)
	assert.Equal(suite.T(), "C", suite.list.getHead().data)

	suite.list.deleteBeforeIndTL(0)
}

func (suite *TLListTestSuite) TestDeleteAfterInd() {
	suite.fillList()

	suite.list.deleteAfterIndTL(-6)

	suite.list.deleteAfterIndTL(0)
	assert.Equal(suite.T(), "C", suite.list.getHead().next.data)
	assert.Equal(suite.T(), "A", suite.list.getTail().prev.data)

	suite.list.deleteAfterIndTL(0)
	assert.Equal(suite.T(), "A", suite.list.getTail().data)

	suite.list.deleteAfterIndTL(0)
}

func (suite *TLListTestSuite) TestRemVal() {
	suite.list.remValTL("B")

	suite.fillList()
	suite.list.remValTL("A")
	assert.Equal(suite.T(), "B", suite.list.getHead().data)

	suite.list.addFrontTL("A")
	suite.list.remValTL("B")
	suite.list.remValTL("C")
	assert.Equal(suite.T(), "A", suite.list.getTail().data)
}

func (suite *TLListTestSuite) TestPrintList() {
	suite.list.printListTL()
	suite.fillList()
	suite.list.printListTL()
}

func TestTLListTestSuite(t *testing.T) {
	suite.Run(t, new(TLListTestSuite))
}

func TestTLListCreateNode(t *testing.T) {
	list := &TLList{}
	node := list.createNodeTL("test")
	assert.Equal(t, "test", node.data)
	assert.Nil(t, node.next)
	assert.Nil(t, node.prev)
}

func TestTLListGetHeadTail(t *testing.T) {
	list := &TLList{}

	assert.Nil(t, list.getHead())
	assert.Nil(t, list.getTail())

	list.addBackTL("A")
	assert.Equal(t, "A", list.getHead().data)
	assert.Equal(t, "A", list.getTail().data)

	list.addBackTL("B")
	assert.Equal(t, "A", list.getHead().data)
	assert.Equal(t, "B", list.getTail().data)
}
