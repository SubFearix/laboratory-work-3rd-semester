package main
import "fmt"

type NodeHash struct {
	key  string
	data int
	next *NodeHash
	prev *NodeHash
}

func NewNodeHash(k string, d int) *NodeHash {
	return &NodeHash{key: k, data: d, next: nil, prev: nil}
}

func (this *NodeHash) getKey() string     { return this.key }
func (this *NodeHash) getData() int       { return this.data }
func (this *NodeHash) getNext() *NodeHash { return this.next }

type Hash struct {
	cell     []*NodeHash
	capacity int
	size     int
}

func NewHash() *Hash {
	return &Hash{
		cell:     make([]*NodeHash, 10),
		capacity: 10,
		size:     0,
	}
}

func (this *Hash) hashFunc(key string, capacity int) int {
	hash := 0
	for _, c := range key {
		hash = (hash*37 + int(c)) % capacity
	}
	return hash % capacity
}

func (this *Hash) addElement(key string, data int) {
	index := this.hashFunc(key, this.capacity)
	begin := this.cell[index]
	current := begin
	for current != nil {
		if current.key == key {
			current.data = data
			return
		}
		current = current.next
	}
	node := NewNodeHash(key, data)
	node.next = begin
	node.prev = nil
	if this.cell[index] != nil {
		this.cell[index].prev = node
	}
	this.cell[index] = node
	this.size++
}

func (this *Hash) findElement(key string) int {
	index := this.hashFunc(key, this.capacity)
	current := this.cell[index]
	for current != nil {
		if current.key == key {
			return current.data
		}
		current = current.next
	}
	return -1
}

func (this *Hash) deleteElement(key string) {
	index := this.hashFunc(key, this.capacity)
	delVal := this.cell[index]
	for delVal != nil {
		if delVal.key == key {
			if delVal.prev != nil {
				delVal.prev.next = delVal.next
			} else {
				this.cell[index] = delVal.next
			}
			if delVal.next != nil {
				delVal.next.prev = delVal.prev
			}
			this.size--
			return
		}
		delVal = delVal.next
	}
}

func (this *Hash) printHashTable() {
	fmt.Printf("Хэш-таблица (size: %d, capacity: %d):\n", this.size, this.capacity)
	for i := 0; i < this.capacity; i++ {
		fmt.Printf("Ячейка %d: ", i)
		current := this.cell[i]
		if current == nil {
			fmt.Print("empty")
		} else {
			for current != nil {
				fmt.Printf("[%s -> %d]", current.key, current.data)
				if current.next != nil {
					fmt.Print(" -> ")
				}
				current = current.next
			}
		}
		fmt.Println()
	}
}

func (this *Hash) getCapacity() int       { return this.capacity }
func (this *Hash) getSize() int           { return this.size }
func (this *Hash) getCells() []*NodeHash { return this.cell }