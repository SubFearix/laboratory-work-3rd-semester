package main

import (
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/suite"
)

type HashASuite struct {
	suite.Suite
	hashTable *HashA
}

func (suite *HashASuite) SetupTest() {
	suite.hashTable = NewHashA()
}

func TestHashASuite(t *testing.T) {
	suite.Run(t, new(HashASuite))
}

func (suite *HashASuite) TestAddElement() {
	suite.hashTable.addElementA("key1", 100)
	assert.Equal(suite.T(), 1, suite.hashTable.getSize())

	suite.hashTable.addElementA("key2", 200)
	assert.Equal(suite.T(), 2, suite.hashTable.getSize())
}

func (suite *HashASuite) TestAddElementUpdate() {
	suite.hashTable.addElementA("key1", 100)
	suite.hashTable.addElementA("key1", 200)

	assert.Equal(suite.T(), 1, suite.hashTable.getSize())
	assert.Equal(suite.T(), 200, suite.hashTable.findElementA("key1"))
}

func (suite *HashASuite) TestFindExistingElement() {
	suite.hashTable.addElementA("test1", 42)
	suite.hashTable.addElementA("test2", 84)

	assert.Equal(suite.T(), 42, suite.hashTable.findElementA("test1"))
	assert.Equal(suite.T(), 84, suite.hashTable.findElementA("test2"))
}

func (suite *HashASuite) TestFindNonExistingElement() {
	assert.Equal(suite.T(), -1, suite.hashTable.findElementA("nonexistent"))

	suite.hashTable.addElementA("existing", 100)
	assert.Equal(suite.T(), -1, suite.hashTable.findElementA("nonexistent"))
}

func (suite *HashASuite) TestDeleteExistingElement() {
	suite.hashTable.addElementA("delete_me", 123)
	assert.Equal(suite.T(), 1, suite.hashTable.getSize())

	var output string
	func() {
		suite.hashTable.deleteElementA("delete_me")
	}()
	_ = output

	assert.Equal(suite.T(), 0, suite.hashTable.getSize())
	assert.Equal(suite.T(), -1, suite.hashTable.findElementA("delete_me"))
}

func (suite *HashASuite) TestDeleteNonExistingElement() {
	initialSize := suite.hashTable.getSize()
	suite.hashTable.deleteElementA("nonexistent")
	assert.Equal(suite.T(), initialSize, suite.hashTable.getSize())
}

func (suite *HashASuite) TestHashFunction() {
	hash1 := suite.hashTable.hashFunc("test", 10)
	hash2 := suite.hashTable.hashFunc("test", 10)
	assert.Equal(suite.T(), hash1, hash2)

	assert.GreaterOrEqual(suite.T(), hash1, 0)
	assert.Less(suite.T(), hash1, 10)
}

func (suite *HashASuite) TestCollisionsAndLinearProbing() {
	smallTable := NewHashA()
	smallTable.addElementA("a", 1)
	smallTable.addElementA("b", 2)
	smallTable.addElementA("c", 3)

	assert.Equal(suite.T(), 1, smallTable.findElementA("a"))
	assert.Equal(suite.T(), 2, smallTable.findElementA("b"))
	assert.Equal(suite.T(), 3, smallTable.findElementA("c"))
}

func (suite *HashASuite) TestDeleteAndReadd() {
	suite.hashTable.addElementA("temp", 999)
	assert.Equal(suite.T(), 999, suite.hashTable.findElementA("temp"))

	suite.hashTable.deleteElementA("temp")
	assert.Equal(suite.T(), -1, suite.hashTable.findElementA("temp"))

	suite.hashTable.addElementA("temp", 111)
	assert.Equal(suite.T(), 111, suite.hashTable.findElementA("temp"))
}

func (suite *HashASuite) TestPrintTable() {
	suite.hashTable.addElementA("print1", 1)
	suite.hashTable.addElementA("print2", 2)
	suite.hashTable.addElementA("print3", 3)
	suite.hashTable.deleteElementA("print1")

	assert.NotPanics(suite.T(), func() {
		suite.hashTable.printHashTableA()
	})
}

func (suite *HashASuite) TestRehash() {
	initialCap := suite.hashTable.getCapacity()
	elementsToAdd := int(float64(initialCap)*0.75) + 1
	for i := 0; i <= elementsToAdd; i++ {
		key := strings.Repeat("a", i+1)
		suite.hashTable.addElementA(key, i)
	}

	assert.Equal(suite.T(), initialCap*2, suite.hashTable.getCapacity())
}

func (suite *HashASuite) TestEdgeCases() {
	assert.Equal(suite.T(), 0, suite.hashTable.getSize())
	assert.Equal(suite.T(), -1, suite.hashTable.findElementA("anything"))

	assert.NotPanics(suite.T(), func() {
		suite.hashTable.deleteElementA("nothing")
	})
}

func (suite *HashASuite) TestInternalState() {
	suite.hashTable.addElementA("test", 123)

	cells := suite.hashTable.getCells()
	found := false
	for i, cell := range cells {
		if !cell.getIsEmpty() && !cell.getIsDeleted() && cell.getKey() == "test" {
			found = true
			assert.Equal(suite.T(), 123, cell.getData())
			assert.Equal(suite.T(), "test", cell.getKey())
			assert.False(suite.T(), cell.getIsDeleted())
			assert.False(suite.T(), cell.getIsEmpty())
			break
		}
		_ = i
	}

	assert.True(suite.T(), found, "Element should be found in the table")
}

func (suite *HashASuite) TestDeletedElementsHandling() {
	suite.hashTable.addElementA("to_delete", 456)
	suite.hashTable.deleteElementA("to_delete")

	cells := suite.hashTable.getCells()
	for _, cell := range cells {
		if cell.getKey() == "to_delete" {
			assert.True(suite.T(), cell.getIsDeleted())
			break
		}
	}

	assert.Equal(suite.T(), -1, suite.hashTable.findElementA("to_delete"))
}
