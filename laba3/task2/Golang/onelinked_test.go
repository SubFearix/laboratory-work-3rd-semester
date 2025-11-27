package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/suite"
)

type OLListTestSuite struct {
	suite.Suite
	list *OLList
}

func (suite *OLListTestSuite) SetupTest() {
	suite.list = &OLList{}
}

func (suite *OLListTestSuite) fillList() {
	suite.list.addBackOL("A")
	suite.list.addBackOL("B")
	suite.list.addBackOL("C")
}

func (suite *OLListTestSuite) TestAddFront() {
	suite.list.addFrontOL("B")
	suite.list.addFrontOL("A")

	assert.Equal(suite.T(), "A", suite.list.getHead().data)
	assert.Equal(suite.T(), "B", suite.list.getTail().data)
	assert.Equal(suite.T(), suite.list.getHead().next, suite.list.getTail())
}

func (suite *OLListTestSuite) TestAddBack() {
	suite.list.addBackOL("A")
	suite.list.addBackOL("B")

	assert.Equal(suite.T(), "A", suite.list.getHead().data)
	assert.Equal(suite.T(), "B", suite.list.getTail().data)
	assert.Nil(suite.T(), suite.list.getTail().next)
}

func (suite *OLListTestSuite) TestRemFront() {
	suite.fillList()

	suite.list.remFrontOL()
	assert.Equal(suite.T(), "B", suite.list.getHead().data)

	suite.list.remFrontOL()
	suite.list.remFrontOL()
	assert.Nil(suite.T(), suite.list.getHead())
	assert.Nil(suite.T(), suite.list.getTail())
}

func (suite *OLListTestSuite) TestRemBack() {
	suite.fillList()

	suite.list.remBackOL()
	assert.Equal(suite.T(), "B", suite.list.getTail().data)

	suite.list.remBackOL()
	assert.Equal(suite.T(), suite.list.getHead(), suite.list.getTail())
	assert.Equal(suite.T(), "A", suite.list.getTail().data)

	suite.list.remBackOL()
	assert.Nil(suite.T(), suite.list.getHead())
	assert.Nil(suite.T(), suite.list.getTail())
}

func (suite *OLListTestSuite) TestInsertAfterInd() {
	suite.fillList()

	suite.list.insertAfterIndOL(1, "X")
	assert.Equal(suite.T(), 2, suite.list.findValOL("X"))
	assert.Equal(suite.T(), "X", suite.list.getHead().next.next.data)

	suite.list.insertAfterIndOL(3, "D")
	assert.Equal(suite.T(), "D", suite.list.getTail().data)
	assert.Equal(suite.T(), 4, suite.list.findValOL("D"))

	suite.list.insertAfterIndOL(-1, "Fail")
	suite.list.insertAfterIndOL(10, "Fail")
}

func (suite *OLListTestSuite) TestInsertBeforeInd() {
	suite.fillList()

	suite.list.insertBeforeIndOL(0, "Start")
	assert.Equal(suite.T(), "Start", suite.list.getHead().data)

	suite.list.insertBeforeIndOL(2, "X")
	assert.Equal(suite.T(), "X", suite.list.getHead().next.next.data)

	suite.list.insertBeforeIndOL(-1, "Fail")
	suite.list.insertBeforeIndOL(10, "Fail")
}

func (suite *OLListTestSuite) TestFindVal() {
	suite.fillList()

	assert.Equal(suite.T(), 1, suite.list.findValOL("B"))
	assert.Equal(suite.T(), 2, suite.list.findValOL("C"))
	assert.Equal(suite.T(), -1, suite.list.findValOL("Z"))
	assert.Equal(suite.T(), 0, suite.list.findValOL("A"))

	emptyList := &OLList{}
	assert.Equal(suite.T(), -1, emptyList.findValOL("A"))
}

func (suite *OLListTestSuite) TestRemVal() {
	suite.fillList()

	suite.list.remValOL("B")
	assert.Equal(suite.T(), -1, suite.list.findValOL("B"))
	assert.Equal(suite.T(), "C", suite.list.getHead().next.data)

	suite.list.remValOL("C")
	assert.Equal(suite.T(), "A", suite.list.getTail().data)

	suite.list.remValOL("A")
	assert.Nil(suite.T(), suite.list.getHead())

	suite.fillList()
	suite.list.remValOL("Z")
	assert.Equal(suite.T(), 2, suite.list.findValOL("C"))
}

func (suite *OLListTestSuite) TestDeleteBeforeInd() {
	suite.fillList()

	suite.list.deleteBeforeIndOL(1)
	assert.Equal(suite.T(), "B", suite.list.getHead().data)

	suite.list.deleteBeforeIndOL(1)
	assert.Equal(suite.T(), "C", suite.list.getHead().data)
	assert.Equal(suite.T(), suite.list.getHead(), suite.list.getTail())

	suite.list.deleteBeforeIndOL(0)
	suite.fillList()
	suite.list.deleteBeforeIndOL(3)
}

func (suite *OLListTestSuite) TestDeleteAfterInd() {
	suite.fillList()

	suite.list.deleteAfterIndOL(0)
	assert.Equal(suite.T(), "C", suite.list.getHead().next.data)

	suite.list.deleteAfterIndOL(0)
	assert.Equal(suite.T(), "A", suite.list.getTail().data)

	suite.list.deleteAfterIndOL(1)
	suite.list.deleteAfterIndOL(-2)
}

func (suite *OLListTestSuite) TestCreateNode() {
	node := suite.list.createNodeOL("test")
	assert.Equal(suite.T(), "test", node.data)
	assert.Nil(suite.T(), node.next)
}

func (suite *OLListTestSuite) TestEdgeCases() {
	emptyList := &OLList{}
	emptyList.remFrontOL()
	emptyList.remBackOL()
	emptyList.remValOL("anything")
	emptyList.deleteBeforeIndOL(0)
	emptyList.deleteAfterIndOL(0)

	assert.Nil(suite.T(), emptyList.getHead())
	assert.Nil(suite.T(), emptyList.getTail())

	suite.list.addBackOL("Single")
	assert.Equal(suite.T(), suite.list.getHead(), suite.list.getTail())
	assert.Equal(suite.T(), "Single", suite.list.getHead().data)

	suite.list.remFrontOL()
	assert.Nil(suite.T(), suite.list.getHead())
}

func (suite *OLListTestSuite) TestComplexOperations() {
	suite.list.addBackOL("A")
	suite.list.addBackOL("B")
	suite.list.addFrontOL("Start")
	suite.list.addBackOL("C")

	assert.Equal(suite.T(), "Start", suite.list.getHead().data)
	assert.Equal(suite.T(), "C", suite.list.getTail().data)
	assert.Equal(suite.T(), 3, suite.list.findValOL("C"))

	suite.list.insertAfterIndOL(1, "X")
	suite.list.insertBeforeIndOL(3, "Y")

	suite.list.remValOL("B")
	suite.list.deleteAfterIndOL(2)

	assert.Equal(suite.T(), "Start", suite.list.getHead().data)
	assert.Equal(suite.T(), "C", suite.list.getTail().data)
	assert.Equal(suite.T(), -1, suite.list.findValOL("B"))
	assert.Equal(suite.T(), 3, suite.list.findValOL("C"))
}

func (suite *OLListTestSuite) TestPrintList() {
	suite.list.printListOL()

	suite.fillList()
	suite.list.printListOL()
}

func (suite *OLListTestSuite) TestSpecialCases() {
	suite.list.deleteBeforeIndOL(3)
	suite.list.deleteAfterIndOL(6)
	suite.list.deleteAfterIndOL(-2)

	suite.fillList()
	suite.list.deleteAfterIndOL(2)
	suite.list.deleteBeforeIndOL(5)
}

func TestOLListTestSuite(t *testing.T) {
	suite.Run(t, new(OLListTestSuite))
}

func TestSingleElementList(t *testing.T) {
	list := &OLList{}
	list.addBackOL("Single")

	assert.Equal(t, "Single", list.getHead().data)
	assert.Equal(t, list.getHead(), list.getTail())
	assert.Nil(t, list.getHead().next)

	list.remFrontOL()
	assert.Nil(t, list.getHead())
	assert.Nil(t, list.getTail())

	list.addFrontOL("Single")
	list.remBackOL()
	assert.Nil(t, list.getHead())
	assert.Nil(t, list.getTail())
}

func TestMultipleAddAndRemove(t *testing.T) {
	list := &OLList{}

	for i := 0; i < 10; i++ {
		list.addBackOL(string(rune('A' + i)))
	}

	assert.Equal(t, 9, list.findValOL("J"))

	list.remValOL("E")
	list.deleteAfterIndOL(3)
	list.deleteBeforeIndOL(2)
	list.remFrontOL()
	list.remBackOL()

	assert.Equal(t, "C", list.getHead().data)
	assert.True(t, list.findValOL("J") == -1)
}

func TestFindOperations(t *testing.T) {
	list := &OLList{}

	assert.Equal(t, -1, list.findValOL("A"))

	list.addBackOL("A")
	list.addBackOL("B")
	list.addBackOL("A")

	assert.Equal(t, 0, list.findValOL("A"))

	assert.Equal(t, -1, list.findValOL("Z"))
}
