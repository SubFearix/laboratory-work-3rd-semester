package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"unicode"
)

var arrays = make(map[string]*Array)
var stacks = make(map[string]*Stack)
var queues = make(map[string]*Queue)
var olists = make(map[string]*OLList)
var tlists = make(map[string]*TLList)

type Node struct {
	data string
	next *Node
	prev *Node
}

type Array struct {
	data     []string
	capacity int
	size     int
}

type OLList struct {
	head *Node
	tail *Node
}

type Queue struct {
	head *Node
	tail *Node
}

type Stack struct {
	top *Node
}

type TLList struct {
	head *Node
	tail *Node
}

func splitString(str string) []string {
	return strings.Fields(str)
}

func toUpper(str string) string {
	return strings.Map(unicode.ToUpper, str)
}

func loadFromFile(filename string) {
	file, err := os.Open(filename)
	if err != nil {
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			continue
		}

		tokens := splitString(line)
		if len(tokens) < 3 {
			continue
		}

		typeStr := tokens[0]
		name := tokens[1]

		switch typeStr {
		case "ARRAY":
			arr := createArray()
			size, _ := strconv.Atoi(tokens[2])
			for i := 0; i < size && i+3 < len(tokens); i++ {
				value := tokens[i+3]
				addBackArray(arr, value)
			}
			arrays[name] = arr

		case "STACK":
			st := createEmptyStack()
			for i := 2; i < len(tokens); i++ {
				value := tokens[i]
				pushStack(st, value)
			}
			stacks[name] = st

		case "QUEUE":
			que := createEmptyQueue()
			for i := 2; i < len(tokens); i++ {
				value := tokens[i]
				addBackQueue(que, value)
			}
			queues[name] = que

		case "OLIST":
			lst := createEmptyOLList()
			for i := 2; i < len(tokens); i++ {
				value := tokens[i]
				addBackOL(lst, value)
			}
			olists[name] = lst

		case "TLIST":
			lst := createEmptyTLList()
			for i := 2; i < len(tokens); i++ {
				value := tokens[i]
				addBackTL(lst, value)
			}
			tlists[name] = lst
		}
	}
}

func saveToFile(filename string) {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка: Невозможно открыть файл %s для записи\n", filename)
		return
	}
	defer file.Close()

	for name, arr := range arrays {
		file.WriteString(fmt.Sprintf("ARRAY %s %d", name, arr.size))
		for i := 0; i < arr.size; i++ {
			file.WriteString(" " + getElemIndArray(arr, i))
		}
		file.WriteString("\n")
	}

	for name, st := range stacks {
		file.WriteString("STACK " + name)
		temp := createEmptyStack()
		curr := st.top
		for curr != nil {
			pushStack(temp, curr.data)
			curr = curr.next
		}
		curr = temp.top
		for curr != nil {
			file.WriteString(" " + curr.data)
			curr = curr.next
		}
		for temp.top != nil {
			popStack(temp)
		}
		file.WriteString("\n")
	}

	for name, que := range queues {
		file.WriteString("QUEUE " + name)
		curr := que.head
		for curr != nil {
			file.WriteString(" " + curr.data)
			curr = curr.next
		}
		file.WriteString("\n")
	}

	for name, lst := range olists {
		file.WriteString("OLIST " + name)
		curr := lst.head
		for curr != nil {
			file.WriteString(" " + curr.data)
			curr = curr.next
		}
		file.WriteString("\n")
	}

	for name, lst := range tlists {
		file.WriteString("TLIST " + name)
		curr := lst.head
		for curr != nil {
			file.WriteString(" " + curr.data)
			curr = curr.next
		}
		file.WriteString("\n")
	}
}
type CommandType int

const (
	CMD_PRINT CommandType = iota
	CMD_APUSH
	CMD_ACREATE
	CMD_AINSERT
	CMD_ADEL
	CMD_AGET
	CMD_ASET
	CMD_ASIZE
	CMD_APUSHFRONT
	CMD_APOP

	CMD_SCREATE
	CMD_SPUSH
	CMD_SPOP

	CMD_QCREATE
	CMD_QPUSH
	CMD_QPOP

	CMD_FCREATE
	CMD_FINSERTBEFORE
	CMD_FINSERTAFTER
	CMD_FPUSH
	CMD_FPUSHFRONT
	CMD_FDELHEAD
	CMD_FDELTAIL
	CMD_FDELBEFORE
	CMD_FDELAFTER
	CMD_FDELVALUE
	CMD_FSEARCH

	CMD_LCREATE
	CMD_LINSERTBEFORE
	CMD_LINSERTAFTER
	CMD_LPUSH
	CMD_LPUSHFRONT
	CMD_LDELHEAD
	CMD_LDELTAIL
	CMD_LDELBEFORE
	CMD_LDELAFTER
	CMD_LDELVALUE
	CMD_LSEARCH

	CMD_CLEAR
	CMD_UNKNOWN
)

func cleanupAll() {
	for name := range arrays {
		delete(arrays, name)
	}

	for name, st := range stacks {
		for st.top != nil {
			popStack(st)
		}
		delete(stacks, name)
	}

	for name, que := range queues {
		for que.head != nil {
			remFrontQueue(que)
		}
		delete(queues, name)
	}

	for name, lst := range olists {
		for lst.head != nil {
			remFrontOL(lst)
		}
		delete(olists, name)
	}

	for name, lst := range tlists {
		for lst.head != nil {
			remFrontTL(lst)
		}
		delete(tlists, name)
	}
}

func getCommandType(command string) CommandType {
	switch command {
	case "PRINT":
		return CMD_PRINT
	case "APUSH":
		return CMD_APUSH
	case "ACREATE":
		return CMD_ACREATE
	case "AINSERT":
		return CMD_AINSERT
	case "ADEL":
		return CMD_ADEL
	case "AGET":
		return CMD_AGET
	case "ASET":
		return CMD_ASET
	case "ASIZE":
		return CMD_ASIZE
	case "APUSHFRONT":
		return CMD_APUSHFRONT
	case "APOP":
		return CMD_APOP

	case "SCREATE":
		return CMD_SCREATE
	case "SPUSH":
		return CMD_SPUSH
	case "SPOP":
		return CMD_SPOP

	case "QCREATE":
		return CMD_QCREATE
	case "QPUSH":
		return CMD_QPUSH
	case "QPOP":
		return CMD_QPOP

	case "FCREATE":
		return CMD_FCREATE
	case "FINSERTBEFORE":
		return CMD_FINSERTBEFORE
	case "FINSERTAFTER":
		return CMD_FINSERTAFTER
	case "FPUSH":
		return CMD_FPUSH
	case "FPUSHFRONT":
		return CMD_FPUSHFRONT
	case "FDELHEAD":
		return CMD_FDELHEAD
	case "FDELTAIL":
		return CMD_FDELTAIL
	case "FDELBEFORE":
		return CMD_FDELBEFORE
	case "FDELAFTER":
		return CMD_FDELAFTER
	case "FDELVALUE":
		return CMD_FDELVALUE
	case "FSEARCH":
		return CMD_FSEARCH

	case "LCREATE":
		return CMD_LCREATE
	case "LINSERTBEFORE":
		return CMD_LINSERTBEFORE
	case "LINSERTAFTER":
		return CMD_LINSERTAFTER
	case "LPUSH":
		return CMD_LPUSH
	case "LPUSHFRONT":
		return CMD_LPUSHFRONT
	case "LDELHEAD":
		return CMD_LDELHEAD
	case "LDELTAIL":
		return CMD_LDELTAIL
	case "LDELBEFORE":
		return CMD_LDELBEFORE
	case "LDELAFTER":
		return CMD_LDELAFTER
	case "LDELVALUE":
		return CMD_LDELVALUE
	case "LSEARCH":
		return CMD_LSEARCH
	case "CLEAR":
		return CMD_CLEAR
	default:
		return CMD_UNKNOWN
	}
}

func executeQuery(query string, filename string) bool {
	tokens := splitString(query)
	if len(tokens) == 0 {
		return false
	}

	commandStr := toUpper(tokens[0])
	command := getCommandType(commandStr)

	switch command {
	case CMD_PRINT:
		if len(tokens) == 1 {
			for name, arr := range arrays {
				fmt.Printf("ARRAY %s: ", name)
				printArray(arr)
			}
			for name, st := range stacks {
				fmt.Printf("STACK %s: ", name)
				printStack(st)
			}
			for name, que := range queues {
				fmt.Printf("QUEUE %s: ", name)
				printQueue(que)
			}
			for name, lst := range olists {
				fmt.Printf("OLIST %s: ", name)
				printListOL(lst)
			}
			for name, lst := range tlists {
				fmt.Printf("TLIST %s: ", name)
				printListTL(lst)
			}
		} else {
			name := tokens[1]
			if arr, exists := arrays[name]; exists {
				printArray(arr)
			} else if st, exists := stacks[name]; exists {
				printStack(st)
			} else if que, exists := queues[name]; exists {
				printQueue(que)
			} else if lst, exists := olists[name]; exists {
				printListOL(lst)
			} else if lst, exists := tlists[name]; exists {
				printListTL(lst)
			} else {
				fmt.Printf("Структура не найдена: %s\n", name)
				return false
			}
		}
		return true

	case CMD_ACREATE:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: ACREATE требует имя массива")
			return false
		}
		name := tokens[1]
		if _, exists := arrays[name]; exists {
			fmt.Printf("Массив '%s' уже существует\n", name)
			return false
		}
		arrays[name] = createArray()
		fmt.Printf("Массив '%s' успешно создан\n", name)
		return true

	case CMD_APUSH:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: APUSH требует имя массива и значение")
			return false
		}
		name := tokens[1]
		if _, exists := arrays[name]; !exists {
			arrays[name] = createArray()
		}
		value := tokens[2]
		addBackArray(arrays[name], value)
		fmt.Println(value)
		return true

	case CMD_APUSHFRONT:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: APUSHFRONT требует имя массива и значение")
			return false
		}
		name := tokens[1]
		if _, exists := arrays[name]; !exists {
			arrays[name] = createArray()
		}
		value := tokens[2]
		addElemIndArray(arrays[name], 0, value)
		fmt.Println(value)
		return true

	case CMD_APOP:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: APOP требует имя массива")
			return false
		}
		name := tokens[1]
		arr, exists := arrays[name]
		if !exists || arr.size == 0 {
			fmt.Println("Ошибка: Массив пуст или не существует")
			return false
		}
		value := getElemIndArray(arr, arr.size-1)
		remElemIndArray(arr, arr.size-1)
		fmt.Println(value)
		return true

	case CMD_AINSERT:
		if len(tokens) < 4 {
			fmt.Println("Ошибка: AINSERT требует имя массива, индекс и значение")
			return false
		}
		name := tokens[1]
		if _, exists := arrays[name]; !exists {
			arrays[name] = createArray()
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		addElemIndArray(arrays[name], index, value)
		fmt.Println(value)
		return true

	case CMD_ADEL:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: ADEL требует имя массива и индекс")
			return false
		}
		name := tokens[1]
		arr, exists := arrays[name]
		if !exists {
			fmt.Println("Ошибка: Массив не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		if index < 0 || index >= arr.size {
			fmt.Println("Ошибка: Индекс вне границ")
			return false
		}
		value := getElemIndArray(arr, index)
		remElemIndArray(arr, index)
		fmt.Println(value)
		return true

	case CMD_AGET:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: AGET требует имя массива и индекс")
			return false
		}
		name := tokens[1]
		arr, exists := arrays[name]
		if !exists {
			fmt.Println("Ошибка: Массив не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := getElemIndArray(arr, index)
		if value == "" {
			fmt.Println("Ошибка: Индекс вне границ")
			return false
		}
		fmt.Println(value)
		return true

	case CMD_ASET:
		if len(tokens) < 4 {
			fmt.Println("Ошибка: ASET требует имя массива, индекс и значение")
			return false
		}
		name := tokens[1]
		arr, exists := arrays[name]
		if !exists {
			fmt.Println("Ошибка: Массив не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		changeElemIndArray(arr, index, value)
		fmt.Println(value)
		return true

	case CMD_ASIZE:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: ASIZE требует имя массива")
			return false
		}
		name := tokens[1]
		arr, exists := arrays[name]
		if !exists {
			fmt.Println("Ошибка: Массив не существует")
			return false
		}
		fmt.Println(arr.size)
		return true

	case CMD_SCREATE:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: SCREATE требует имя стека")
			return false
		}
		name := tokens[1]
		if _, exists := stacks[name]; exists {
			fmt.Printf("Стек '%s' уже существует\n", name)
			return false
		}
		stacks[name] = createEmptyStack()
		fmt.Printf("Стек '%s' успешно создан\n", name)
		return true

	case CMD_SPUSH:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: SPUSH требует имя стека и значение")
			return false
		}
		name := tokens[1]
		if _, exists := stacks[name]; !exists {
			stacks[name] = createEmptyStack()
		}
		value := tokens[2]
		pushStack(stacks[name], value)
		fmt.Println(value)
		return true

	case CMD_SPOP:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: SPOP требует имя стека")
			return false
		}
		name := tokens[1]
		st, exists := stacks[name]
		if !exists || st.top == nil {
			fmt.Println("Ошибка: Стек пуст или не существует")
			return false
		}
		value := st.top.data
		popStack(st)
		fmt.Println(value)
		return true

	case CMD_QCREATE:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: QCREATE требует имя очереди")
			return false
		}
		name := tokens[1]
		if _, exists := queues[name]; exists {
			fmt.Printf("Очередь '%s' уже существует\n", name)
			return false
		}
		queues[name] = createEmptyQueue()
		fmt.Printf("Очередь '%s' успешно создана\n", name)
		return true

	case CMD_QPUSH:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: QPUSH требует имя очереди и значение")
			return false
		}
		name := tokens[1]
		if _, exists := queues[name]; !exists {
			queues[name] = createEmptyQueue()
		}
		value := tokens[2]
		addBackQueue(queues[name], value)
		fmt.Println(value)
		return true

	case CMD_QPOP:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: QPOP требует имя очереди")
			return false
		}
		name := tokens[1]
		que, exists := queues[name]
		if !exists || que.head == nil {
			fmt.Println("Ошибка: Очередь пуста или не существует")
			return false
		}
		value := que.head.data
		remFrontQueue(que)
		fmt.Println(value)
		return true

	case CMD_FCREATE:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: FCREATE требует имя списка")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; exists {
			fmt.Printf("Односвязный список '%s' уже существует\n", name)
			return false
		}
		olists[name] = createEmptyOLList()
		fmt.Printf("Односвязный список '%s' успешно создан\n", name)
		return true

	case CMD_FPUSH:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FPUSH требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			olists[name] = createEmptyOLList()
		}
		value := tokens[2]
		addBackOL(olists[name], value)
		fmt.Println(value)
		return true

	case CMD_FPUSHFRONT:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FPUSHFRONT требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			olists[name] = createEmptyOLList()
		}
		value := tokens[2]
		addFrontOL(olists[name], value)
		fmt.Println(value)
		return true

	case CMD_FINSERTAFTER:
		if len(tokens) < 4 {
			fmt.Println("Ошибка: FINSERTAFTER требует имя списка, индекс и значение")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		insertAfterIndOL(lst, index, value)
		fmt.Println(value)
		return true

	case CMD_FINSERTBEFORE:
		if len(tokens) < 4 {
			fmt.Println("Ошибка: FINSERTBEFORE требует имя списка, индекс и значение")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		insertBeforeIndOL(lst, index, value)
		fmt.Println(value)
		return true

	case CMD_FDELHEAD:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: FDELHEAD требует имя списка")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists || lst.head == nil {
			fmt.Println("Ошибка: Список пуст или не существует")
			return false
		}
		remFrontOL(lst)
		fmt.Println("Голова успешно удалена")
		return true

	case CMD_FDELTAIL:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: FDELTAIL требует имя списка")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists || lst.head == nil {
			fmt.Println("Ошибка: Список пуст или не существует")
			return false
		}
		remBackOL(lst)
		fmt.Println("Хвост успешно удален")
		return true

	case CMD_FDELVALUE:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FDELVALUE требует имя списка и значение")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		value := tokens[2]
		remValOL(lst, value)
		fmt.Printf("Значение %s удалено\n", value)
		return true

	case CMD_FDELBEFORE:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FDELBEFORE требует имя списка и индекс")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		deleteBeforeIndOL(lst, index)
		fmt.Printf("Элемент перед индексом %d удален\n", index)
		return true

	case CMD_FDELAFTER:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FDELAFTER требует имя списка и индекс")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		deleteAfterIndOL(lst, index)
		fmt.Printf("Элемент после индекса %d удален\n", index)
		return true

	case CMD_FSEARCH:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FSEARCH требует имя списка и значение")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		value := tokens[2]
		index := findValOL(lst, value)
		if index == -1 {
			fmt.Println("Значение не найдено")
		} else {
			fmt.Printf("Значение найдено по индексу: %d\n", index)
		}
		return true

	case CMD_LCREATE:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: LCREATE требует имя списка")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; exists {
			fmt.Printf("Двусвязный список '%s' уже существует\n", name)
			return false
		}
		tlists[name] = createEmptyTLList()
		fmt.Printf("Двусвязный список '%s' успешно создан\n", name)
		return true

	case CMD_LPUSH:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LPUSH требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			tlists[name] = createEmptyTLList()
		}
		value := tokens[2]
		addBackTL(tlists[name], value)
		fmt.Println(value)
		return true

	case CMD_LPUSHFRONT:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LPUSHFRONT требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			tlists[name] = createEmptyTLList()
		}
		value := tokens[2]
		addFrontTL(tlists[name], value)
		fmt.Println(value)
		return true

	case CMD_LINSERTAFTER:
		if len(tokens) < 4 {
			fmt.Println("Ошибка: LINSERTAFTER требует имя списка, индекс и значение")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		insertAfterIndTL(lst, index, value)
		fmt.Println(value)
		return true

	case CMD_LINSERTBEFORE:
		if len(tokens) < 4 {
			fmt.Println("Ошибка: LINSERTBEFORE требует имя списка, индекс и значение")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		insertBeforeIndTL(lst, index, value)
		fmt.Println(value)
		return true

	case CMD_LDELHEAD:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: LDELHEAD требует имя списка")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists || lst.head == nil {
			fmt.Println("Ошибка: Список пуст или не существует")
			return false
		}
		remFrontTL(lst)
		fmt.Println("Голова успешно удалена")
		return true

	case CMD_LDELTAIL:
		if len(tokens) < 2 {
			fmt.Println("Ошибка: LDELTAIL требует имя списка")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists || lst.head == nil {
			fmt.Println("Ошибка: Список пуст или не существует")
			return false
		}
		remBackTL(lst)
		fmt.Println("Хвост успешно удален")
		return true

	case CMD_LDELVALUE:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LDELVALUE требует имя списка и значение")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		value := tokens[2]
		remValTL(lst, value)
		fmt.Printf("Значение %s удалено\n", value)
		return true

	case CMD_LDELBEFORE:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LDELBEFORE требует имя списка и индекс")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		deleteBeforeIndTL(lst, index)
		fmt.Printf("Элемент перед индексом %d удален\n", index)
		return true

	case CMD_LDELAFTER:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LDELAFTER требует имя списка и индекс")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		deleteAfterIndTL(lst, index)
		fmt.Printf("Элемент после индекса %d удален\n", index)
		return true

	case CMD_LSEARCH:
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LSEARCH требует имя списка и значение")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		value := tokens[2]
		index := findValTL(lst, value)
		if index == -1 {
			fmt.Println("Значение не найдено")
		} else {
			fmt.Printf("Значение найдено по индексу: %d\n", index)
		}
		return true

	case CMD_CLEAR:
		cleanupAll()
		file, _ := os.Create(filename)
		file.Close()
		fmt.Println("База данных успешно очищена")
		return true

	case CMD_UNKNOWN:
		fallthrough
	default:
		fmt.Printf("Ошибка: Неизвестная команда '%s'\n", commandStr)
		return false
	}
}

func printUsage() {
	fmt.Println("Использование: ./dbms --file <имя_файла> --query <команда>")
	fmt.Println("Команды:")
	fmt.Println("  ACREATE <имя>                - Создать новый массив")
	fmt.Println("  APUSH <массив> <значение>    - Добавить в конец массива")
	fmt.Println("  APUSHFRONT <массив> <значение>- Добавить в начало массива")
	fmt.Println("  APOP <массив>                - Удалить из конца массива")
	fmt.Println("  AINSERT <массив> <инд> <знач>- Вставить по индексу в массив")
	fmt.Println("  ADEL <массив> <инд>          - Удалить по индексу из массива")
	fmt.Println("  AGET <массив> <инд>          - Получить элемент по индексу")
	fmt.Println("  ASET <массив> <инд> <знач>   - Установить элемент по индексу")
	fmt.Println("  ASIZE <массив>               - Получить размер массива")
	fmt.Println("  SCREATE <имя>                - Создать новый стек")
	fmt.Println("  SPUSH <стек> <значение>      - Поместить в стек")
	fmt.Println("  SPOP <стек>                  - Извлечь из стека")
	fmt.Println("  QCREATE <имя>                - Создать новую очередь")
	fmt.Println("  QPUSH <очередь> <значение>   - Добавить в очередь")
	fmt.Println("  QPOP <очередь>               - Извлечь из очереди")
	fmt.Println("  FCREATE <имя>                - Создать односвязный список")
	fmt.Println("  FPUSH <список> <значение>    - Добавить в конец списка")
	fmt.Println("  FPUSHFRONT <список> <значение>- Добавить в начало списка")
	fmt.Println("  FINSERTAFTER <список> <инд> <знач>- Вставить после индекса")
	fmt.Println("  FINSERTBEFORE <список> <инд> <знач>- Вставить перед индексом")
	fmt.Println("  FDELHEAD <список>            - Удалить голову списка")
	fmt.Println("  FDELTAIL <список>            - Удалить хвост списка")
	fmt.Println("  FDELVALUE <список> <значение>- Удалить значение из списка")
	fmt.Println("  FDELBEFORE <список> <инд>    - Удалить перед индексом")
	fmt.Println("  FDELAFTER <список> <инд>     - Удалить после индекса")
	fmt.Println("  FSEARCH <список> <значение>  - Найти значение в списке")
	fmt.Println("  LCREATE <имя>                - Создать двусвязный список")
	fmt.Println("  LPUSH <список> <значение>    - Добавить в конец списка")
	fmt.Println("  LPUSHFRONT <список> <значение>- Добавить в начало списка")
	fmt.Println("  LINSERTAFTER <список> <инд> <знач>- Вставить после индекса")
	fmt.Println("  LINSERTBEFORE <список> <инд> <знач>- Вставить перед индексом")
	fmt.Println("  LDELHEAD <список>            - Удалить голову списка")
	fmt.Println("  LDELTAIL <список>            - Удалить хвост списка")
	fmt.Println("  LDELVALUE <список> <значение>- Удалить значение из списка")
	fmt.Println("  LDELBEFORE <список> <инд>    - Удалить перед индексом")
	fmt.Println("  LDELAFTER <список> <инд>     - Удалить после индекса")
	fmt.Println("  LSEARCH <список> <значение>  - Найти значение в списке")
	fmt.Println("  PRINT [структура]            - Вывести структуру(ы)")
	fmt.Println("  CLEAR                        - Очистить все данные")
}

func main() {
	var filename, query string

	for i := 1; i < len(os.Args); i++ {
		arg := os.Args[i]
		if arg == "--file" && i+1 < len(os.Args) {
			filename = os.Args[i+1]
			i++
		} else if arg == "--query" && i+1 < len(os.Args) {
			query = os.Args[i+1]
			i++
		} else if arg == "--help" {
			printUsage()
			return
		}
	}

	if filename == "" || query == "" {
		fmt.Println("Ошибка: Требуются оба параметра --file и --query")
		printUsage()
		os.Exit(1)
	}

	loadFromFile(filename)
	executeQuery(query, filename)
	saveToFile(filename)
}
