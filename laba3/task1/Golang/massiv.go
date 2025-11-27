package main
import "fmt"

type Array struct {
	data     []string
	capacity int
	size     int
}

func NewArray() *Array {
	return &Array{
		data:     make([]string, 10),
		capacity: 10,
		size:     0,
	}
}

func (this *Array) addBackArray(val string) {
	if this.size >= this.capacity {
		this.capacity = this.capacity * 2
		newData := make([]string, this.capacity)
		copy(newData, this.data)
		this.data = newData
	}
	this.data[this.size] = val
	this.size++
}

func (this *Array) addElemIndArray(ind int, val string) {
	if 0 > ind || ind > this.size {
		return
	}
	if this.size >= this.capacity {
		this.capacity = this.capacity * 2
		newData := make([]string, this.capacity)
		copy(newData, this.data)
		this.data = newData
	}
	for i := this.size; i > ind; i-- {
		this.data[i] = this.data[i-1]
	}
	this.data[ind] = val
	this.size++
}

func (this *Array) getElemIndArray(ind int) string {
	if 0 > ind || ind >= this.size {
		return ""
	}
	return this.data[ind]
}

func (this *Array) remElemIndArray(ind int) {
	if 0 > ind || ind >= this.size {
		return
	}
	for i := ind + 1; i < this.size; i++ {
		this.data[i-1] = this.data[i]
	}
	this.size--
}

func (this *Array) changeElemIndArray(ind int, val string) {
	if 0 > ind || ind >= this.size {
		return
	}
	this.data[ind] = val
}

func (this *Array) getSizeArray() int {
	return this.size
}

func (this *Array) printArray() {
	for i := 0; i < this.size; i++ {
		fmt.Print(this.data[i], " ")
	}
	fmt.Println()
}

func (this *Array) getSize() int { return this.size }