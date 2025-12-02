package main
import "fmt"

type Queue struct {
	head *Node
	tail *Node
}

func (this *Queue) createNodeQueue(data string) *Node {
	return createNode(data)
}

func (this *Queue) remFrontQueue() {
	if this.head == nil {
		return
	}
	// temp := this.head
	this.head = this.head.next
	
	if this.head == nil {
		this.tail = nil
	}
}

func (this *Queue) addBackQueue(data string) {
	node := this.createNodeQueue(data)
	if this.head == nil {
		this.head = node
		this.tail = node
		return
	}
	this.tail.next = node
	this.tail = node
}

func (this *Queue) printQueue() {
	curr := this.head
	for curr != nil {
		fmt.Print(curr.data, " ")
		curr = curr.next
	}
	fmt.Println()
}

func (this *Queue) getHead() *Node { return this.head }
func (this *Queue) getTail() *Node { return this.tail }