package main
import "fmt"
func createEmptyStack() *Stack {
	return &Stack{top: nil}
}
func pushStack(st *Stack, value string) {
	newNode := &Node{data: value, next: st.top}
	st.top = newNode
}
func popStack(st *Stack) {
	if st.top == nil {
		return
	}
	st.top = st.top.next
}
func printStack(st *Stack) {
	if st.top == nil {
		fmt.Println("Стек пуст")
		return
	}
	curr := st.top
	for curr != nil {
		fmt.Print(curr.data)
		if curr.next != nil {
			fmt.Print(" ")
		}
		curr = curr.next
	}
	fmt.Println()
}
