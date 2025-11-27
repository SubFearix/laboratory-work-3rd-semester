package main

type Node struct {
	data string
	next *Node
	prev *Node
}

func createNode(data string) *Node {
	return &Node{data: data, next: nil, prev: nil}
}

func (this *Node) getData() string { return this.data }
func (this *Node) getNext() *Node  { return this.next }
func (this *Node) getPrev() *Node  { return this.prev }