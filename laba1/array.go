package main

import "fmt"

func createArray() *Array {
	return &Array{
		data:     make([]string, 10),
		capacity: 10,
		size:     0,
	}
}

func addBackArray(arr *Array, value string) {
	if arr.size >= arr.capacity {
		newCapacity := arr.capacity * 2
		newData := make([]string, newCapacity)
		copy(newData, arr.data)
		arr.data = newData
		arr.capacity = newCapacity
	}
	arr.data[arr.size] = value
	arr.size++
}

func addElemIndArray(arr *Array, index int, value string) {
	if index < 0 || index > arr.size {
		return
	}
	if arr.size >= arr.capacity {
		newCapacity := arr.capacity * 2
		newData := make([]string, newCapacity)
		copy(newData, arr.data)
		arr.data = newData
		arr.capacity = newCapacity
	}
	for i := arr.size; i > index; i-- {
		arr.data[i] = arr.data[i-1]
	}
	arr.data[index] = value
	arr.size++
}

func remElemIndArray(arr *Array, index int) {
	if index < 0 || index >= arr.size {
		return
	}
	for i := index; i < arr.size-1; i++ {
		arr.data[i] = arr.data[i+1]
	}
	arr.size--
}

func getElemIndArray(arr *Array, index int) string {
	if index < 0 || index >= arr.size {
		return ""
	}
	return arr.data[index]
}

func changeElemIndArray(arr *Array, index int, value string) {
	if index < 0 || index >= arr.size {
		return
	}
	arr.data[index] = value
}

func printArray(arr *Array) {
	if arr.size == 0 {
		fmt.Println("Массив пуст")
		return
	}
	for i := 0; i < arr.size; i++ {
		fmt.Print(arr.data[i])
		if i < arr.size-1 {
			fmt.Print(" ")
		}
	}
	fmt.Println()
}
