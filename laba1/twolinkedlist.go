package main
import "fmt"
func createEmptyTLList() *TLList {
	return &TLList{head: nil, tail: nil}
}
func addBackTL(lst *TLList, value string) {
	newNode := &Node{data: value, next: nil, prev: lst.tail}
	if lst.tail == nil {
		lst.head = newNode
		lst.tail = newNode
	} else {
		lst.tail.next = newNode
		lst.tail = newNode
	}
}
func addFrontTL(lst *TLList, value string) {
	newNode := &Node{data: value, next: lst.head, prev: nil}
	if lst.head == nil {
		lst.head = newNode
		lst.tail = newNode
	} else {
		lst.head.prev = newNode
		lst.head = newNode
	}
}

func insertAfterIndTL(lst *TLList, index int, value string) {
	if index < 0 {
		return
	}
	curr := lst.head
	for i := 0; i < index && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil {
		return
	}
	newNode := &Node{data: value, next: curr.next, prev: curr}
	if curr.next != nil {
		curr.next.prev = newNode
	}
	curr.next = newNode
	if curr == lst.tail {
		lst.tail = newNode
	}
}

func insertBeforeIndTL(lst *TLList, index int, value string) {
	if index < 0 {
		return
	}
	if index == 0 {
		addFrontTL(lst, value)
		return
	}
	curr := lst.head
	for i := 0; i < index && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil {
		return
	}
	newNode := &Node{data: value, next: curr, prev: curr.prev}
	curr.prev.next = newNode
	curr.prev = newNode
}

func remFrontTL(lst *TLList) {
	if lst.head == nil {
		return
	}
	lst.head = lst.head.next
	if lst.head != nil {
		lst.head.prev = nil
	} else {
		lst.tail = nil
	}
}

func remBackTL(lst *TLList) {
	if lst.tail == nil {
		return
	}
	lst.tail = lst.tail.prev
	if lst.tail != nil {
		lst.tail.next = nil
	} else {
		lst.head = nil
	}
}

func remValTL(lst *TLList, value string) {
	curr := lst.head
	for curr != nil {
		if curr.data == value {
			if curr.prev != nil {
				curr.prev.next = curr.next
			} else {
				lst.head = curr.next
			}
			if curr.next != nil {
				curr.next.prev = curr.prev
			} else {
				lst.tail = curr.prev
			}
			return
		}
		curr = curr.next
	}
}

func deleteBeforeIndTL(lst *TLList, index int) {
	if index <= 0 || lst.head == nil {
		return
	}
	if index == 1 {
		remFrontTL(lst)
		return
	}
	curr := lst.head
	for i := 0; i < index && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil || curr.prev == nil {
		return
	}
	nodeToDelete := curr.prev
	if nodeToDelete.prev != nil {
		nodeToDelete.prev.next = curr
	} else {
		lst.head = curr
	}
	curr.prev = nodeToDelete.prev
}

func deleteAfterIndTL(lst *TLList, index int) {
	if index < 0 || lst.head == nil {
		return
	}
	curr := lst.head
	for i := 0; i < index && curr != nil; i++ {
		curr = curr.next
	}
	if curr == nil || curr.next == nil {
		return
	}
	nodeToDelete := curr.next
	curr.next = nodeToDelete.next
	if nodeToDelete.next != nil {
		nodeToDelete.next.prev = curr
	} else {
		lst.tail = curr
	}
}

func findValTL(lst *TLList, value string) int {
	curr := lst.head
	index := 0
	for curr != nil {
		if curr.data == value {
			return index
		}
		curr = curr.next
		index++
	}
	return -1
}

func printListTL(lst *TLList) {
	if lst.head == nil {
		fmt.Println("Список пуст")
		return
	}
	curr := lst.head
	for curr != nil {
		fmt.Print(curr.data)
		if curr.next != nil {
			fmt.Print(" ")
		}
		curr = curr.next
	}
	fmt.Println()
}
