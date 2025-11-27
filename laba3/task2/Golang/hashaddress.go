package main
import "fmt"

type NodeHashA struct {
	key       string
	data      int
	isDeleted bool
	isEmpty   bool
}

func (this *NodeHashA) getKey() string     { return this.key }
func (this *NodeHashA) getData() int       { return this.data }
func (this *NodeHashA) getIsDeleted() bool { return this.isDeleted }
func (this *NodeHashA) getIsEmpty() bool   { return this.isEmpty }

type HashA struct {
	cell     []NodeHashA
	capacity int
	size     int
}

func NewHashA() *HashA {
	h := &HashA{capacity: 10, size: 0}
	h.cell = make([]NodeHashA, h.capacity)
	for i := range h.cell {
		h.cell[i].isEmpty = true
	}
	return h
}

func (this *HashA) rehash() {
	oldCapacity := this.capacity
	oldTable := this.cell
	this.capacity = oldCapacity * 2
	this.cell = make([]NodeHashA, this.capacity)
	for i := range this.cell {
		this.cell[i].isEmpty = true
	}
	this.size = 0
	for i := 0; i < oldCapacity; i++ {
		if !oldTable[i].isEmpty && !oldTable[i].isDeleted {
			this.addElementA(oldTable[i].key, oldTable[i].data)
		}
	}
}

func (this *HashA) hashFunc(key string, capacity int) int {
	hash := 0
	for _, c := range key {
		hash = (hash*37 + int(c)) % capacity
	}
	return hash % capacity
}

func (this *HashA) addElementA(key string, data int) {
	if float64(this.size) >= float64(this.capacity)*0.75 {
		fmt.Print("Происходит рехэширование! Старый размер: ", this.capacity)
		this.rehash()
		fmt.Println(", новый размер: ", this.capacity)
	}
	index := this.hashFunc(key, this.capacity)
	orig := index
	i := 0
	for !this.cell[index].isEmpty && !this.cell[index].isDeleted {
		if this.cell[index].key == key {
			this.cell[index].data = data
			return
		}
		i++
		index = (orig + i) % this.capacity
		if i == this.capacity {
			fmt.Println("Не найдено место для вставки!")
			return
		}
	}
	this.cell[index].key = key
	this.cell[index].data = data
	this.cell[index].isDeleted = false
	this.cell[index].isEmpty = false
	this.size++
}

func (this *HashA) findElementA(key string) int {
	index := this.hashFunc(key, this.capacity)
	i := 0
	orig := index
	for i < this.capacity {
		if this.cell[index].isEmpty && !this.cell[index].isDeleted {
			return -1
		}
		if !this.cell[index].isEmpty && !this.cell[index].isDeleted {
			if this.cell[index].key == key {
				return this.cell[index].data
			}
		}
		i++
		index = (orig + i) % this.capacity
	}
	return -1
}

func (this *HashA) deleteElementA(key string) {
	index := this.hashFunc(key, this.capacity)
	i := 0
	orig := index
	for i < this.capacity {
		if this.cell[index].isEmpty && !this.cell[index].isDeleted {
			fmt.Println("Элемент '", key, "' не найден!")
			return
		}
		if !this.cell[index].isEmpty && !this.cell[index].isDeleted {
			if this.cell[index].key == key {
				this.cell[index].isDeleted = true
				this.size--
				fmt.Println("Элемент '", key, "' успешно удален!")
				return
			}
		}
		i++
		index = (orig + i) % this.capacity
	}
	fmt.Println("Элемент '", key, "' не найден!")
}

func (this *HashA) printHashTableA() {
	fmt.Printf("Хэш-таблица с открытой адресацией (size: %d, capacity: %d):\n", this.size, this.capacity)
	for i := 0; i < this.capacity; i++ {
		fmt.Print("Ячейка ", i, ": ")
		if this.cell[i].isEmpty {
			if this.cell[i].isDeleted {
				fmt.Print("[DELETED]")
			} else {
				fmt.Print("empty")
			}
		} else {
			fmt.Printf("[%s -> %d]", this.cell[i].key, this.cell[i].data)
			if this.cell[i].isDeleted {
				fmt.Print(" (deleted)")
			}
		}
		fmt.Println()
	}
}

func (this *HashA) getCapacity() int         { return this.capacity }
func (this *HashA) getSize() int             { return this.size }
func (this *HashA) getCells() []NodeHashA    { return this.cell }