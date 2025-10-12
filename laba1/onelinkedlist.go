package main
import "fmt"
func createEmptyOLList() *OLList {
	return &OLList{head: nil, tail: nil}
}

func addBackOL(lst *OLList, value string) {
	newNode := &Node{data: value, next: nil}
	if lst.head == nil {
		lst.head = newNode
		lst.tail = newNode
	} else {
		lst.tail.next = newNode
		lst.tail = newNode
	}
}

func addFrontOL(lst *OLList, value string) {
	newNode := &Node{data: value, next: lst.head}
	lst.head = newNode
	if lst.tail == nil {
		lst.tail = newNode
	}
}

func insertAfterIndOL(lst *OLList, index int, value string) {
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
	newNode := &Node{data: value, next: curr.next}
	curr.next = newNode
	if curr == lst.tail {
		lst.tail = newNode
	}
}

func insertBeforeIndOL(lst *OLList, index int, value string) {
	if index < 0 {
		return
	}
	if index == 0 {
		addFrontOL(lst, value)
		return
	}
	prev := lst.head
	for i := 0; i < index-1 && prev != nil; i++ {
		prev = prev.next
	}
	if prev == nil {
		return
	}
	newNode := &Node{data: value, next: prev.next}
	prev.next = newNode
	if prev == lst.tail {
		lst.tail = newNode
	}
}

func remFrontOL(lst *OLList) {
	if lst.head == nil {
		return
	}
	lst.head = lst.head.next
	if lst.head == nil {
		lst.tail = nil
	}
}

func remBackOL(lst *OLList) {
	if lst.head == nil {
		return
	}
	if lst.head == lst.tail {
		lst.head = nil
		lst.tail = nil
		return
	}
	curr := lst.head
	for curr.next != lst.tail {
		curr = curr.next
	}
	curr.next = nil
	lst.tail = curr
}

func remValOL(lst *OLList, value string) {
	if lst.head == nil {
		return
	}
	if lst.head.data == value {
		remFrontOL(lst)
		return
	}
	prev := lst.head
	curr := lst.head.next
	for curr != nil {
		if curr.data == value {
			prev.next = curr.next
			if curr == lst.tail {
				lst.tail = prev
			}
			return
		}
		prev = curr
		curr = curr.next
	}
}

func deleteBeforeIndOL(lst *OLList, index int) {
	if index <= 0 || lst.head == nil {
		return
	}
	if index == 1 {
		remFrontOL(lst)
		return
	}
	prev := lst.head
	for i := 0; i < index-2 && prev != nil; i++ {
		prev = prev.next
	}
	if prev == nil || prev.next == nil {
		return
	}
	prev.next = prev.next.next
	if prev.next == nil {
		lst.tail = prev
	}
}

func deleteAfterIndOL(lst *OLList, index int) {
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
	curr.next = curr.next.next
	if curr.next == nil {
		lst.tail = curr
	}
}

func findValOL(lst *OLList, value string) int {
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

func printListOL(lst *OLList) {
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
