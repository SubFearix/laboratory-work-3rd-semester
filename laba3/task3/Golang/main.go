package main

import (
	"fmt"
)

func main() {
	fmt.Println("1. МАССИВ:")
	array := NewArray()
	array.addBackArray("Элемент1")
	array.addBackArray("Элемент2")
	array.addBackArray("Элемент3")
	array.addElemIndArray(1, "Вставка")
	
	fmt.Print("Исходный массив: ")
	array.printArray()
	
	array.SerializeBinary("array.bin")
	array.SerializeText("array.txt")
	
	loadedArrayBin := DeserializeArrayBinary("array.bin")
	loadedArrayTxt := DeserializeArrayText("array.txt")
	
	fmt.Print("Загруженный из bin массив: ")
	loadedArrayBin.printArray()
	
	fmt.Print("Загруженный из txt массив: ")
	loadedArrayTxt.printArray()
	
	fmt.Println("\n2. СТЭК:")
	stack := &Stack{}
	stack.pushStack("Третий")
	stack.pushStack("Второй")
	stack.pushStack("Первый")
	
	fmt.Print("Стек: ")
	stack.printStack()
	
	stack.SerializeBinary("stack.bin")
	stack.SerializeText("stack.txt")
	
	loadedStackBin := DeserializeStackBinary("stack.bin")
	loadedStackTxt := DeserializeStackText("stack.txt")
	
	fmt.Print("Загруженный из bin стек: ")
	loadedStackBin.printStack()
	
	fmt.Print("Загруженный из txt стек: ")
	loadedStackTxt.printStack()
	
	fmt.Println("\n3. ОЧЕРЕДЬ:")
	queue := &Queue{}
	queue.addBackQueue("Первый")
	queue.addBackQueue("Второй")
	queue.addBackQueue("Третий")
	
	fmt.Print("Очередь: ")
	queue.printQueue()
	
	queue.SerializeBinary("queue.bin")
	queue.SerializeText("queue.txt")
	
	loadedQueueBin := DeserializeQueueBinary("queue.bin")
	loadedQueueTxt := DeserializeQueueText("queue.txt")
	
	fmt.Print("Загруженная из bin очередь: ")
	loadedQueueBin.printQueue()
	
	fmt.Print("Загруженная из txt очередь: ")
	loadedQueueTxt.printQueue()
	
	fmt.Println("\n4. ОДНОСВЯЗНЫЙ СПИСОК:")
	olList := &OLList{}
	olList.addBackOL("Нода1")
	olList.addBackOL("Нода2")
	olList.addFrontOL("Нода0")
	olList.insertAfterIndOL(1, "Вставка")
	
	fmt.Print("Односвязный список: ")
	olList.printListOL()
	
	olList.SerializeBinary("ollist.bin")
	olList.SerializeText("ollist.txt")
	
	loadedOLListBin := DeserializeOLListBinary("ollist.bin")
	loadedOLListTxt := DeserializeOLListText("ollist.txt")
	
	fmt.Print("Загруженный из bin список: ")
	loadedOLListBin.printListOL()
	
	fmt.Print("Загруженный из txt список: ")
	loadedOLListTxt.printListOL()
	
	fmt.Println("\n5. ДВУСВЯЗНЫЙ СПИСОК:")
	tlList := &TLList{}
	tlList.addBackTL("Узел1")
	tlList.addBackTL("Узел2")
	tlList.addFrontTL("Узел0")
	tlList.insertAfterIndTL(1, "Вставка")
	
	fmt.Print("Двусвязный список: ")
	tlList.printListTL()

	tlList.SerializeBinary("tllist.bin")
	tlList.SerializeText("tllist.txt")
	
	loadedTLListBin := DeserializeTLListBinary("tllist.bin")
	loadedTLListTxt := DeserializeTLListText("tllist.txt")
	
	fmt.Print("Загруженный из bin двусвязный список: ")
	loadedTLListBin.printListTL()
	
	fmt.Print("Загруженный из txt двусвязный список: ")
	loadedTLListTxt.printListTL()

	fmt.Println("\n6. ХЭШ-ТАБЛИЦА (Метод цепочек):")
	hash := NewHash()
	hash.addElement("яблоко", 100)
	hash.addElement("банан", 50)
	hash.addElement("апельсин", 75)
	hash.addElement("груша", 60)
	
	hash.printHashTable()

	hash.SerializeBinary("hash.bin")
	hash.SerializeText("hash.txt")
	
	loadedHashBin := DeserializeHashBinary("hash.bin")
	loadedHashTxt := DeserializeHashText("hash.txt")
	
	fmt.Println("\nЗагруженная из bin хэш-таблица:")
	loadedHashBin.printHashTable()
	
	fmt.Println("\nЗагруженная из txt хэш-таблица:")
	loadedHashTxt.printHashTable()
	
	fmt.Println("\n7. ХЭШ-ТАБЛИЦА (Открытая адресация):")
	hashA := NewHashA()
	hashA.addElementA("key1", 111)
	hashA.addElementA("key2", 222)
	hashA.addElementA("key3", 333)
	hashA.addElementA("key4", 444)
	
	hashA.printHashTableA()
	
	hashA.SerializeBinary("hashA.bin")
	hashA.SerializeText("hashA.txt")
	
	loadedHashABin := DeserializeHashABinary("hashA.bin")
	loadedHashATxt := DeserializeHashAText("hashA.txt")
	
	fmt.Println("\nЗагруженная из bin хэш-таблица с открытой адресацией:")
	loadedHashABin.printHashTableA()
	
	fmt.Println("\nЗагруженная из txt хэш-таблица с открытой адресацией:")
	loadedHashATxt.printHashTableA()
}
