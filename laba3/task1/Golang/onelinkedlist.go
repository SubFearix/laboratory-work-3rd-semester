package main
import "fmt"

type OLList struct {
	head *Node
	tail *Node
}

func (this *OLList) createNodeOL(data string) *Node {
	return createNode(data)
}

func (this *OLList) addBackOL(data string) {
	node := this.createNodeOL(data)
	if this.head == nil {
		this.head = node
		this.tail = node
		return
	}
	this.tail.next = node
	this.tail = node
}

func (this *OLList) remFrontOL() {
	if this.head == nil { return }
	this.head = this.head.next
	if this.head == nil {
		this.tail = nil
	}
}

func (this *OLList) remBackOL() {
	if this.head == nil {
		return
	}
	if this.head == this.tail {
		this.remFrontOL()
		return
	}
	curr := this.head
	for curr.next != this.tail {
		curr = curr.next
	}
	this.tail = curr
	this.tail.next = nil
}

func (this *OLList) printListOL() {
	curr := this.head
	for curr != nil {
		fmt.Print(curr.data, " ")
		curr = curr.next
	}
	fmt.Println()
}

func (this *OLList) addFrontOL(data string) {
	node := this.createNodeOL(data)
	if this.head == nil {
		this.head = node
		this.tail = node
		return
	}
	node.next = this.head
	this.head = node
}

func (this *OLList) insertAfterIndOL(index int, data string) {
	if index < 0 { return }
	curr := this.head
	i := 0
	for ; i != index && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil { return }
	node := this.createNodeOL(data)
	node.next = curr.next
	curr.next = node
	if curr == this.tail {
		this.tail = node
	}
}

func (this *OLList) insertBeforeIndOL(index int, data string) {
	if index == 0 {
		this.addFrontOL(data)
		return
	}
	if index < 0 { return }
	curr := this.head
	i := 0
	for ; i != index-1 && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil { return }
	node := this.createNodeOL(data)
	node.next = curr.next
	curr.next = node
	if curr == this.tail {
		this.tail = node
	}
}

func (this *OLList) remValOL(val string) {
	if this.head == nil { return }
	if this.head.data == val {
		this.remFrontOL()
		return
	}
	curr := this.head
	var prev *Node = nil
	for curr != nil && curr.data != val {
		prev = curr
		curr = curr.next
	}
	if curr == nil { return }
	prev.next = curr.next
	if curr == this.tail {
		this.tail = prev
	}
}

func (this *OLList) findValOL(val string) int {
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

func (this *OLList) deleteBeforeIndOL(index int) {
	if this.head == nil {
		fmt.Println("Список пуст.")
		return
	}
	if index == 1 {
		this.remFrontOL()
		return
	}
	if index <= 0 {
		fmt.Println("Неверный индекс.")
		return
	}
	curr := this.head
	i := 0
	for ; i != index-2 && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil || curr == this.tail || curr.next == nil {
		fmt.Println("Неверный индекс.")
		return
	}
	node := curr.next
	curr.next = node.next
}

func (this *OLList) deleteAfterIndOL(index int) {
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
	if curr == nil || curr == this.tail {
		fmt.Println("Неверный индекс.")
		return
	}
	if curr.next == this.tail {
		this.remBackOL()
		return
	}
	node := curr.next
	curr.next = node.next
	if node == this.tail {
		this.tail = curr
	}
}

func (this *OLList) getHead() *Node { return this.head }
func (this *OLList) getTail() *Node { return this.tail }