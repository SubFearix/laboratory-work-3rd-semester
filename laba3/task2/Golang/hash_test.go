package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/suite"
)

type HashTableTestSuite struct {
	suite.Suite
	hashTable *Hash
}

func (suite *HashTableTestSuite) SetupTest() {
	suite.hashTable = NewHash()
}

func (suite *HashTableTestSuite) TestAddElement() {
	suite.hashTable.addElement("key1", 100)
	assert.Equal(suite.T(), 1, suite.hashTable.getSize())

	suite.hashTable.addElement("key2", 200)
	assert.Equal(suite.T(), 2, suite.hashTable.getSize())
}

func (suite *HashTableTestSuite) TestAddElementUpdate() {
	suite.hashTable.addElement("key1", 100)
	suite.hashTable.addElement("key1", 200)

	assert.Equal(suite.T(), 1, suite.hashTable.getSize())
	assert.Equal(suite.T(), 200, suite.hashTable.findElement("key1"))
}

func (suite *HashTableTestSuite) TestFindExistingElement() {
	suite.hashTable.addElement("test1", 42)
	suite.hashTable.addElement("test2", 84)

	assert.Equal(suite.T(), 42, suite.hashTable.findElement("test1"))
	assert.Equal(suite.T(), 84, suite.hashTable.findElement("test2"))
}

func (suite *HashTableTestSuite) TestFindNonExistingElement() {
	assert.Equal(suite.T(), -1, suite.hashTable.findElement("non_existing"))

	suite.hashTable.addElement("existing", 100)
	assert.Equal(suite.T(), -1, suite.hashTable.findElement("still_non_existing"))
}

func (suite *HashTableTestSuite) TestDeleteExistingElement() {
	suite.hashTable.addElement("delete_me", 123)
	assert.Equal(suite.T(), 1, suite.hashTable.getSize())

	suite.hashTable.deleteElement("delete_me")
	assert.Equal(suite.T(), 0, suite.hashTable.getSize())
	assert.Equal(suite.T(), -1, suite.hashTable.findElement("delete_me"))
}

func (suite *HashTableTestSuite) TestDeleteNonExistingElement() {
	initialSize := suite.hashTable.getSize()

	suite.hashTable.deleteElement("non_existing")

	assert.Equal(suite.T(), initialSize, suite.hashTable.getSize())
}

func (suite *HashTableTestSuite) TestHashFunction() {
	hash1 := suite.hashTable.hashFunc("test", 10)
	hash2 := suite.hashTable.hashFunc("test", 10)

	assert.Equal(suite.T(), hash1, hash2)

	assert.GreaterOrEqual(suite.T(), hash1, 0)
	assert.Less(suite.T(), hash1, 10)
}

func (suite *HashTableTestSuite) TestCollisionsAndChaining() {
	smallTable := &Hash{
		cell:     make([]*NodeHash, 3),
		capacity: 3,
		size:     0,
	}

	smallTable.addElement("a", 1)
	smallTable.addElement("b", 2)
	smallTable.addElement("c", 3)

	assert.Equal(suite.T(), 1, smallTable.findElement("a"))
	assert.Equal(suite.T(), 2, smallTable.findElement("b"))
	assert.Equal(suite.T(), 3, smallTable.findElement("c"))

	smallTable.addElement("d", 4)
	smallTable.addElement("e", 5)
	smallTable.addElement("f", 6)

	assert.Equal(suite.T(), 1, smallTable.findElement("a"))
	assert.Equal(suite.T(), 2, smallTable.findElement("b"))
	assert.Equal(suite.T(), 3, smallTable.findElement("c"))
	assert.Equal(suite.T(), 4, smallTable.findElement("d"))
	assert.Equal(suite.T(), 5, smallTable.findElement("e"))
	assert.Equal(suite.T(), 6, smallTable.findElement("f"))
}

func (suite *HashTableTestSuite) TestDeleteAndReadd() {
	suite.hashTable.addElement("temp", 999)
	assert.Equal(suite.T(), 999, suite.hashTable.findElement("temp"))

	suite.hashTable.deleteElement("temp")
	assert.Equal(suite.T(), -1, suite.hashTable.findElement("temp"))

	suite.hashTable.addElement("temp", 111)
	assert.Equal(suite.T(), 111, suite.hashTable.findElement("temp"))
}

func (suite *HashTableTestSuite) TestEdgeCases() {
	assert.Equal(suite.T(), 0, suite.hashTable.getSize())
	assert.Equal(suite.T(), -1, suite.hashTable.findElement("anything"))

	suite.hashTable.addElement("", 999)
	assert.Equal(suite.T(), 999, suite.hashTable.findElement(""))

	suite.hashTable.addElement("", 111)
	assert.Equal(suite.T(), 111, suite.hashTable.findElement(""))
}

func (suite *HashTableTestSuite) TestChainDeletion() {
	smallTable := &Hash{
		cell:     make([]*NodeHash, 2),
		capacity: 2,
		size:     0,
	}

	smallTable.addElement("key1", 1)
	smallTable.addElement("key2", 2)
	smallTable.addElement("key3", 3)

	initialSize := smallTable.getSize()

	smallTable.deleteElement("key2")

	assert.Equal(suite.T(), initialSize-1, smallTable.getSize())
	assert.Equal(suite.T(), -1, smallTable.findElement("key2"))
	assert.Equal(suite.T(), 1, smallTable.findElement("key1"))
	assert.Equal(suite.T(), 3, smallTable.findElement("key3"))

	smallTable.deleteElement("key1")
	assert.Equal(suite.T(), -1, smallTable.findElement("key1"))
	assert.Equal(suite.T(), 3, smallTable.findElement("key3"))

	smallTable.deleteElement("key3")
	assert.Equal(suite.T(), -1, smallTable.findElement("key3"))
	assert.Equal(suite.T(), 0, smallTable.getSize())
}

func (suite *HashTableTestSuite) TestNodeHashProperties() {
	node := NewNodeHash("test_key", 42)
	assert.Equal(suite.T(), "test_key", node.getKey())
	assert.Equal(suite.T(), 42, node.getData())
	assert.Nil(suite.T(), node.getNext())
	assert.Nil(suite.T(), node.prev)
}

func (suite *HashTableTestSuite) TestPrintTable() {
	suite.hashTable.addElement("print1", 1)
	suite.hashTable.addElement("print2", 2)
	suite.hashTable.addElement("print3", 3)
	suite.hashTable.deleteElement("print1")
	assert.NotPanics(suite.T(), func() {
		suite.hashTable.printHashTable()
	})
}

func TestHashTableTestSuite(t *testing.T) {
	suite.Run(t, new(HashTableTestSuite))
}
