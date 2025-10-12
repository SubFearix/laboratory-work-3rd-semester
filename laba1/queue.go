package main
import "fmt"
func createEmptyQueue() *Queue {
	return &Queue{head: nil, tail: nil}
}
func addBackQueue(que *Queue, value string) {
	newNode := &Node{data: value, next: nil}
	if que.tail == nil {
		que.head = newNode
		que.tail = newNode
	} else {
		que.tail.next = newNode
		que.tail = newNode
	}
}
func remFrontQueue(que *Queue) {
	if que.head == nil {
		return
	}
	que.head = que.head.next
	if que.head == nil {
		que.tail = nil
	}
}
func printQueue(que *Queue) {
	if que.head == nil {
		fmt.Println("Очередь пуста")
		return
	}
	curr := que.head
	for curr != nil {
		fmt.Print(curr.data)
		if curr.next != nil {
			fmt.Print(" ")
		}
		curr = curr.next
	}
	fmt.Println()
}
