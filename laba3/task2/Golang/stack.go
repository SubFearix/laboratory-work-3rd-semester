package main
import "fmt"

type Stack struct {
	top *Node
}

func (this *Stack) createNodeSt(data string) *Node {
	return createNode(data)
}

func (this *Stack) popStack() {
	if this.top == nil {
		return
	}
	this.top = this.top.next
}

func (this *Stack) printStack() {
	curr := this.top
	for curr != nil {
		fmt.Print(curr.data, " ")
		curr = curr.next
	}
	fmt.Println()
}

func (this *Stack) pushStack(data string) {
	node := this.createNodeSt(data)
	if this.top == nil {
		this.top = node
		return
	}
	node.next = this.top
	this.top = node
}

func (this *Stack) getTop() *Node { return this.top }