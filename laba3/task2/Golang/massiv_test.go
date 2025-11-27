package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/suite"
)

type ArrayTestSuite struct {
	suite.Suite
	arr *Array
}

func (suite *ArrayTestSuite) SetupTest() {
	suite.arr = NewArray()
}

func (suite *ArrayTestSuite) TestAddBack() {
	suite.arr.addBackArray("First")
	suite.arr.addBackArray("Second")

	assert.Equal(suite.T(), 2, suite.arr.getSize())
	assert.Equal(suite.T(), "First", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), "Second", suite.arr.getElemIndArray(1))
	assert.Equal(suite.T(), 10, suite.arr.capacity)
}

func (suite *ArrayTestSuite) TestGetElementOutOfRange() {
	assert.Equal(suite.T(), "", suite.arr.getElemIndArray(-1))
	assert.Equal(suite.T(), "", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), "", suite.arr.getElemIndArray(10))

	suite.arr.addBackArray("A")
	assert.Equal(suite.T(), "", suite.arr.getElemIndArray(1))
	assert.Equal(suite.T(), "", suite.arr.getElemIndArray(-1))
}

func (suite *ArrayTestSuite) TestChangeElement() {
	suite.arr.addBackArray("Old")
	suite.arr.addBackArray("Value")

	suite.arr.changeElemIndArray(0, "New")

	assert.Equal(suite.T(), "New", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), "Value", suite.arr.getElemIndArray(1))

	suite.arr.changeElemIndArray(2, "Fail")
	assert.Equal(suite.T(), "", suite.arr.getElemIndArray(2))
	suite.arr.changeElemIndArray(-1, "Fail")
}

func (suite *ArrayTestSuite) TestAddElementAtIndex() {
	suite.arr.addBackArray("A")
	suite.arr.addBackArray("B")

	suite.arr.addElemIndArray(1, "X")
	assert.Equal(suite.T(), 3, suite.arr.getSize())
	assert.Equal(suite.T(), "A", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), "X", suite.arr.getElemIndArray(1))
	assert.Equal(suite.T(), "B", suite.arr.getElemIndArray(2))
	suite.arr.addElemIndArray(0, "Start")
	assert.Equal(suite.T(), "Start", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), 4, suite.arr.getSize())

	suite.arr.addElemIndArray(4, "End")
	assert.Equal(suite.T(), "End", suite.arr.getElemIndArray(4))
	assert.Equal(suite.T(), 5, suite.arr.getSize())

	suite.arr.addElemIndArray(6, "Fail")
	assert.Equal(suite.T(), 5, suite.arr.getSize())
	suite.arr.addElemIndArray(-1, "Fail")
	assert.Equal(suite.T(), 5, suite.arr.getSize())
}

func (suite *ArrayTestSuite) TestRehashAddBack() {
	for i := 0; i < 10; i++ {
		suite.arr.addBackArray(string(rune('0' + i)))
	}

	assert.Equal(suite.T(), 10, suite.arr.getSize())
	assert.Equal(suite.T(), 10, suite.arr.capacity)
	suite.arr.addBackArray("10")
	assert.Equal(suite.T(), 11, suite.arr.getSize())
	assert.Equal(suite.T(), 20, suite.arr.capacity)

	assert.Equal(suite.T(), "0", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), "9", suite.arr.getElemIndArray(9))
	assert.Equal(suite.T(), "10", suite.arr.getElemIndArray(10))
}

func (suite *ArrayTestSuite) TestRehashAddAtIndex() {
	for i := 0; i < 10; i++ {
		suite.arr.addBackArray(string(rune('A' + i)))
	}

	initialCapacity := suite.arr.capacity
	assert.Equal(suite.T(), 10, suite.arr.getSize())

	suite.arr.addElemIndArray(5, "X")
	assert.Equal(suite.T(), 11, suite.arr.getSize())
	assert.Equal(suite.T(), initialCapacity*2, suite.arr.capacity)

	assert.Equal(suite.T(), "A", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), "E", suite.arr.getElemIndArray(4))
	assert.Equal(suite.T(), "X", suite.arr.getElemIndArray(5))
	assert.Equal(suite.T(), "F", suite.arr.getElemIndArray(6))
	assert.Equal(suite.T(), "J", suite.arr.getElemIndArray(10))
}

func (suite *ArrayTestSuite) TestRemoveElementAtIndex() {
	suite.arr.addBackArray("A")
	suite.arr.addBackArray("B")
	suite.arr.addBackArray("C")
	suite.arr.addBackArray("D")

	suite.arr.remElemIndArray(1)
	assert.Equal(suite.T(), 3, suite.arr.getSize())
	assert.Equal(suite.T(), "A", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), "C", suite.arr.getElemIndArray(1))
	assert.Equal(suite.T(), "D", suite.arr.getElemIndArray(2))

	suite.arr.remElemIndArray(2)
	assert.Equal(suite.T(), 2, suite.arr.getSize())
	assert.Equal(suite.T(), "A", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), "C", suite.arr.getElemIndArray(1))

	suite.arr.remElemIndArray(0)
	assert.Equal(suite.T(), 1, suite.arr.getSize())
	assert.Equal(suite.T(), "C", suite.arr.getElemIndArray(0))

	suite.arr.remElemIndArray(0)
	assert.Equal(suite.T(), 0, suite.arr.getSize())

	suite.arr.remElemIndArray(0)
	assert.Equal(suite.T(), 0, suite.arr.getSize())
	suite.arr.remElemIndArray(-1)
	assert.Equal(suite.T(), 0, suite.arr.getSize())
}

func (suite *ArrayTestSuite) TestGetSize() {
	assert.Equal(suite.T(), 0, suite.arr.getSize())
	assert.Equal(suite.T(), 0, suite.arr.getSizeArray())

	suite.arr.addBackArray("A")
	assert.Equal(suite.T(), 1, suite.arr.getSize())
	assert.Equal(suite.T(), 1, suite.arr.getSizeArray())

	suite.arr.addBackArray("B")
	assert.Equal(suite.T(), 2, suite.arr.getSize())
}

func (suite *ArrayTestSuite) TestComplexOperations() {
	for i := 0; i < 5; i++ {
		suite.arr.addBackArray(string(rune('A' + i)))
	}
	assert.Equal(suite.T(), 5, suite.arr.getSize())

	suite.arr.addElemIndArray(2, "X")
	assert.Equal(suite.T(), 6, suite.arr.getSize())
	assert.Equal(suite.T(), "X", suite.arr.getElemIndArray(2))

	suite.arr.changeElemIndArray(0, "Start")
	assert.Equal(suite.T(), "Start", suite.arr.getElemIndArray(0))

	suite.arr.remElemIndArray(3)
	assert.Equal(suite.T(), 5, suite.arr.getSize())

	expected := []string{"Start", "B", "X", "D", "E"}
	for i, exp := range expected {
		assert.Equal(suite.T(), exp, suite.arr.getElemIndArray(i))
	}
}

func (suite *ArrayTestSuite) TestEdgeCases() {
	assert.Equal(suite.T(), 0, suite.arr.getSize())
	assert.Equal(suite.T(), "", suite.arr.getElemIndArray(0))

	suite.arr.addBackArray("")
	assert.Equal(suite.T(), 1, suite.arr.getSize())
	assert.Equal(suite.T(), "", suite.arr.getElemIndArray(0))

	suite.arr.addElemIndArray(0, "First")
	assert.Equal(suite.T(), "First", suite.arr.getElemIndArray(0))
	assert.Equal(suite.T(), "", suite.arr.getElemIndArray(1))
}

func (suite *ArrayTestSuite) TestPrintArray() {
	suite.arr.addBackArray("A")
	suite.arr.addBackArray("B")
	suite.arr.addBackArray("C")

	suite.arr.printArray()

	assert.Equal(suite.T(), 3, suite.arr.getSize())
}

func TestArrayTestSuite(t *testing.T) {
	suite.Run(t, new(ArrayTestSuite))
}

func TestNewArray(t *testing.T) {
	arr := NewArray()
	assert.Equal(t, 0, arr.getSize())
	assert.Equal(t, 10, arr.capacity)
	assert.Equal(t, 10, len(arr.data))
}

func TestMultipleReallocations(t *testing.T) {
	arr := NewArray()
	for i := 0; i < 100; i++ {
		arr.addBackArray(string(rune('A' + i%26)))
	}

	assert.Equal(t, 100, arr.getSize())
	assert.True(t, arr.capacity >= 100)

	for i := 0; i < 100; i++ {
		expected := string(rune('A' + i%26))
		assert.Equal(t, expected, arr.getElemIndArray(i))
	}
}

func TestAddAndRemoveSequence(t *testing.T) {
	arr := NewArray()

	arr.addBackArray("A")
	arr.addBackArray("B")
	arr.addElemIndArray(1, "X")
	arr.remElemIndArray(0)
	arr.addBackArray("C")
	arr.addElemIndArray(0, "Start")

	assert.Equal(t, 4, arr.getSize())
	assert.Equal(t, "Start", arr.getElemIndArray(0))
	assert.Equal(t, "X", arr.getElemIndArray(1))
	assert.Equal(t, "B", arr.getElemIndArray(2))
	assert.Equal(t, "C", arr.getElemIndArray(3))
}
