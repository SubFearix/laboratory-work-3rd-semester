package main
import "fmt"

type TLList struct {
	head *Node
	tail *Node
}

func (this *TLList) createNodeTL(data string) *Node {
	return createNode(data)
}

func (this *TLList) addBackTL(data string) {
	node := this.createNodeTL(data)
	if this.head == nil {
		this.head = node
		this.tail = node
		return
	}
	node.prev = this.tail
	this.tail.next = node
	this.tail = node
}

func (this *TLList) remFrontTL() {
	if this.head == nil { return }
	this.head = this.head.next
	if this.head == nil {
		this.tail = nil
	} else {
		this.head.prev = nil
	}
}

func (this *TLList) remBackTL() {
	if this.head == nil { return }
	if this.head == this.tail {
		this.remFrontTL()
		return
	}
	this.tail = this.tail.prev
	this.tail.next = nil
}

func (this *TLList) printListTL() {
	if this.head == nil { return }
	curr := this.head
	for curr != nil {
		fmt.Print(curr.data, " ")
		curr = curr.next
	}
	fmt.Println()
}

func (this *TLList) addFrontTL(data string) {
	node := this.createNodeTL(data)
	if this.head == nil {
		this.head = node
		this.tail = node
		return
	}
	node.next = this.head
	this.head.prev = node
	this.head = node
}

func (this *TLList) insertAfterIndTL(index int, data string) {
	if index < 0 || this.head == nil { return }
	curr := this.head
	i := 0
	for ; i != index && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil { return }
	node := this.createNodeTL(data)
	node.next = curr.next
	node.prev = curr
	if curr.next != nil {
		curr.next.prev = node
	}
	curr.next = node
	if curr == this.tail {
		this.tail = node
	}
}

func (this *TLList) insertBeforeIndTL(index int, data string) {
	if index == 0 {
		this.addFrontTL(data)
		return
	}
	if index < 0 || this.head == nil { return }
	curr := this.head
	i := 0
	for ; i != index && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil { return }
	node := this.createNodeTL(data)
	node.prev = curr.prev
	node.next = curr
	curr.prev.next = node
	curr.prev = node
}

func (this *TLList) remValTL(val string) {
	if this.head == nil { return }
	if this.head.data == val {
		this.remFrontTL()
		return
	}
	curr := this.head
	for curr != nil && curr.data != val {
		curr = curr.next
	}
	if curr == nil { return }
	curr.prev.next = curr.next
	if curr == this.tail {
		this.tail = curr.prev
	} else {
		curr.next.prev = curr.prev
	}
}

func (this *TLList) findValTL(val string) int {
	if this.head == nil { return -1 }
	curr := this.head
	ind := 0
	for curr != nil && curr.data != val {
		curr = curr.next
		ind++
	}
	if curr == nil { return -1 }
	return ind
}

func (this *TLList) deleteBeforeIndTL(index int) {
	if this.head == nil {
		fmt.Println("Список пуст.")
		return
	}
	if index == 1 {
		this.remFrontTL()
		return
	}
	if index <= 0 {
		fmt.Println("Неверный индекс.")
		return
	}
	curr := this.head
	i := 0
	for ; i != index && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil || curr.prev == nil {
		fmt.Println("Неверный индекс.")
		return
	}
	node := curr.prev
	curr.prev = node.prev
	if node.prev != nil {
		node.prev.next = curr
	} else {
		this.head = curr
	}
}

func (this *TLList) deleteAfterIndTL(index int) {
	if this.head == nil || this.head == this.tail {
		fmt.Println("Удаление невозможно.")
		return
	}
	if index < 0 {
		fmt.Println("Неверный индекс.")
		return
	}
	curr := this.head
	i := 0
	for ; i != index && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil || curr.next == nil {
		fmt.Println("Неверный индекс.")
		return
	}
	node := curr.next
	curr.next = node.next
	if node.next != nil {
		node.next.prev = curr
	} else {
		this.tail = curr
	}
}

func (this *TLList) getHead() *Node { return this.head }
func (this *TLList) getTail() *Node { return this.tail }