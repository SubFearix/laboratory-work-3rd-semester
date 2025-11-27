package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

var (
	arrays      = make(map[string]*Array)
	stacks      = make(map[string]*Stack)
	queues      = make(map[string]*Queue)
	olists      = make(map[string]*OLList)
	tlists      = make(map[string]*TLList)
	hashtables  = make(map[string]*Hash)
	hashtablesA = make(map[string]*HashA)
)

func splitCommand(str string) []string {
	return strings.Fields(str)
}

func toUpper(str string) string {
	return strings.ToUpper(str)
}

func cleanupAll() {
	arrays = make(map[string]*Array)
	stacks = make(map[string]*Stack)
	queues = make(map[string]*Queue)
	olists = make(map[string]*OLList)
	tlists = make(map[string]*TLList)
	hashtables = make(map[string]*Hash)
	hashtablesA = make(map[string]*HashA)
}

func saveHashToStream(hash *Hash, stream *bufio.Writer) {
	cells := hash.getCells()
	for i := 0; i < hash.getCapacity(); i++ {
		current := cells[i]
		for current != nil {
			stream.WriteString(current.getKey() + " " + strconv.Itoa(current.getData()) + " ")
			current = current.getNext()
		}
	}
}

func saveHashAToStream(hasha *HashA, stream *bufio.Writer) {
	cells := hasha.getCells()
	for i := 0; i < hasha.getCapacity(); i++ {
		cell := &cells[i]
		if !cell.getIsEmpty() && !cell.getIsDeleted() {
			stream.WriteString(cell.getKey() + " " + strconv.Itoa(cell.getData()) + " ")
		}
	}
}

func loadHashFromStream(hash *Hash, scanner *bufio.Scanner) {
	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			break
		}
		tokens := splitCommand(line)
		if len(tokens)%2 != 0 {
			continue
		}
		for i := 0; i < len(tokens); i += 2 {
			key := tokens[i]
			value, _ := strconv.Atoi(tokens[i+1])
			hash.addElement(key, value)
		}
	}
}

func loadHashAFromStream(hasha *HashA, scanner *bufio.Scanner) {
	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			break
		}
		tokens := splitCommand(line)
		if len(tokens)%2 != 0 {
			continue
		}
		for i := 0; i < len(tokens); i += 2 {
			key := tokens[i]
			value, _ := strconv.Atoi(tokens[i+1])
			hasha.addElementA(key, value)
		}
	}
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

		tokens := splitCommand(line)
		if len(tokens) < 3 {
			continue
		}

		typeStr := tokens[0]
		name := tokens[1]

		switch typeStr {
		case "ARRAY":
			arr := NewArray()
			size, _ := strconv.Atoi(tokens[2])
			for i := 0; i < size && i+3 < len(tokens); i++ {
				value := tokens[i+3]
				arr.addBackArray(value)
			}
			arrays[name] = arr

		case "STACK":
			st := &Stack{}
			for i := 2; i < len(tokens); i++ {
				value := tokens[i]
				st.pushStack(value)
			}
			stacks[name] = st

		case "QUEUE":
			que := &Queue{}
			for i := 2; i < len(tokens); i++ {
				value := tokens[i]
				que.addBackQueue(value)
			}
			queues[name] = que

		case "OLIST":
			lst := &OLList{}
			for i := 2; i < len(tokens); i++ {
				value := tokens[i]
				lst.addBackOL(value)
			}
			olists[name] = lst

		case "TLIST":
			lst := &TLList{}
			for i := 2; i < len(tokens); i++ {
				value := tokens[i]
				lst.addBackTL(value)
			}
			tlists[name] = lst

		case "HASH":
			if _, exists := hashtables[name]; exists {
			}
			hashtables[name] = NewHash()
			hashData := ""
			for i := 2; i < len(tokens); i++ {
				hashData += tokens[i] + " "
			}

			for scanner.Scan() {
				nextLine := scanner.Text()
				if nextLine == "" {
					break
				}
				hashData += nextLine + " "
				testTokens := splitCommand(hashData)
				if len(testTokens)%2 == 0 {
					break
				}
			}

			if hashData != "" {
				reader := strings.NewReader(hashData)
				hashScanner := bufio.NewScanner(reader)
				loadHashFromStream(hashtables[name], hashScanner)
			}

		case "HASHA":
			if _, exists := hashtablesA[name]; exists {
			}
			hashtablesA[name] = NewHashA()
			hashData := ""
			for i := 2; i < len(tokens); i++ {
				hashData += tokens[i] + " "
			}

			for scanner.Scan() {
				nextLine := scanner.Text()
				if nextLine == "" {
					break
				}
				hashData += nextLine + " "
				testTokens := splitCommand(hashData)
				if len(testTokens)%2 == 0 {
					break
				}
			}

			if hashData != "" {
				reader := strings.NewReader(hashData)
				hashScanner := bufio.NewScanner(reader)
				loadHashAFromStream(hashtablesA[name], hashScanner)
			}
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

	writer := bufio.NewWriter(file)
	defer writer.Flush()

	keys := make([]string, 0, len(arrays))
	for k := range arrays {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	for _, name := range keys {
		arr := arrays[name]
		writer.WriteString("ARRAY " + name + " " + strconv.Itoa(arr.getSize()))
		for i := 0; i < arr.getSize(); i++ {
			writer.WriteString(" " + arr.getElemIndArray(i))
		}
		writer.WriteString("\n")
	}

	keys = make([]string, 0, len(stacks))
	for k := range stacks {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	for _, name := range keys {
		st := stacks[name]
		writer.WriteString("STACK " + name)
		temp := &Stack{}
		curr := st.getTop()
		for curr != nil {
			temp.pushStack(curr.getData())
			curr = curr.getNext()
		}
		curr = temp.getTop()
		for curr != nil {
			writer.WriteString(" " + curr.getData())
			curr = curr.getNext()
		}
		writer.WriteString("\n")
	}

	keys = make([]string, 0, len(queues))
	for k := range queues {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	for _, name := range keys {
		que := queues[name]
		writer.WriteString("QUEUE " + name)
		curr := que.getHead()
		for curr != nil {
			writer.WriteString(" " + curr.getData())
			curr = curr.getNext()
		}
		writer.WriteString("\n")
	}

	keys = make([]string, 0, len(olists))
	for k := range olists {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	for _, name := range keys {
		lst := olists[name]
		writer.WriteString("OLIST " + name)
		curr := lst.getHead()
		for curr != nil {
			writer.WriteString(" " + curr.getData())
			curr = curr.getNext()
		}
		writer.WriteString("\n")
	}

	keys = make([]string, 0, len(tlists))
	for k := range tlists {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	for _, name := range keys {
		lst := tlists[name]
		writer.WriteString("TLIST " + name)
		curr := lst.getHead()
		for curr != nil {
			writer.WriteString(" " + curr.getData())
			curr = curr.getNext()
		}
		writer.WriteString("\n")
	}

	// Сохраняем хэш-таблицы с цепочками
	keys = make([]string, 0, len(hashtables))
	for k := range hashtables {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	for _, name := range keys {
		hash := hashtables[name]
		writer.WriteString("HASH " + name)
		if hash.getSize() > 0 {
			writer.WriteString(" ")
			saveHashToStream(hash, writer)
		}
		writer.WriteString("\n")
	}

	keys = make([]string, 0, len(hashtablesA))
	for k := range hashtablesA {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	for _, name := range keys {
		hasha := hashtablesA[name]
		writer.WriteString("HASHA " + name)
		if hasha.getSize() > 0 {
			writer.WriteString(" ")
			saveHashAToStream(hasha, writer)
		}
		writer.WriteString("\n")
	}
}

func printHelp() {
	fmt.Println("==================================================")
	fmt.Println("           СИСТЕМА УПРАВЛЕНИЯ СТРУКТУРАМИ ДАННЫХ")
	fmt.Println("==================================================")
	fmt.Println()

	fmt.Println("КОМАНДЫ ДЛЯ МАССИВА:")
	fmt.Println("  ACREATE <имя>                - Создать новый массив")
	fmt.Println("  APUSH <массив> <значение>    - Добавить в конец массива")
	fmt.Println("  APUSHFRONT <массив> <значение>- Добавить в начало массива")
	fmt.Println("  APOP <массив>                - Удалить из конца массива")
	fmt.Println("  AINSERT <массив> <инд> <знач>- Вставить по индексу в массив")
	fmt.Println("  ADEL <массив> <инд>          - Удалить по индексу из массива")
	fmt.Println("  AGET <массив> <инд>          - Получить элемент по индексу")
	fmt.Println("  ASET <массив> <инд> <знач>   - Установить элемент по индексу")
	fmt.Println("  ASIZE <массив>               - Получить размер массива")
	fmt.Println()

	fmt.Println("КОМАНДЫ ДЛЯ СТЕКА:")
	fmt.Println("  SCREATE <имя>                - Создать новый стек")
	fmt.Println("  SPUSH <стек> <значение>      - Поместить в стек")
	fmt.Println("  SPOP <стек>                  - Извлечь из стека")
	fmt.Println()

	fmt.Println("КОМАНДЫ ДЛЯ ОЧЕРЕДИ:")
	fmt.Println("  QCREATE <имя>                - Создать новую очередь")
	fmt.Println("  QPUSH <очередь> <значение>   - Добавить в очередь")
	fmt.Println("  QPOP <очередь>               - Извлечь из очереди")
	fmt.Println()

	fmt.Println("КОМАНДЫ ДЛЯ ОДНОСВЯЗНОГО СПИСКА:")
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
	fmt.Println()

	fmt.Println("КОМАНДЫ ДЛЯ ДВУСВЯЗНОГО СПИСКА:")
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
	fmt.Println()

	fmt.Println("КОМАНДЫ ДЛЯ ХЭШ-ТАБЛИЦЫ С ЦЕПОЧКАМИ:")
	fmt.Println("  HCREATE <имя>                - Создать хэш-таблицу")
	fmt.Println("  HINSERT <таблица> <ключ> <значение> - Добавить элемент")
	fmt.Println("  HDELETE <таблица> <ключ>     - Удалить элемент")
	fmt.Println("  HSEARCH <таблица> <ключ>     - Найти элемент")
	fmt.Println()

	fmt.Println("КОМАНДЫ ДЛЯ ХЭШ-ТАБЛИЦЫ С ОТКРЫТОЙ АДРЕСАЦИЕЙ:")
	fmt.Println("  ACREATEH <имя>               - Создать хэш-таблицу")
	fmt.Println("  AINSERTH <таблица> <ключ> <значение> - Добавить элемент")
	fmt.Println("  ADELETEH <таблица> <ключ>    - Удалить элемент")
	fmt.Println("  ASEARCHH <таблица> <ключ>    - Найти элемент")
	fmt.Println()

	fmt.Println("ОБЩИЕ КОМАНДЫ:")
	fmt.Println("  PRINT [структура]            - Вывести структуру(ы)")
	fmt.Println("  CLEAR                        - Очистить все данные")
	fmt.Println("  EXIT                         - Выход из программы")
	fmt.Println("  HELP                         - Показать эту справку")
	fmt.Println("==================================================")
}

func processPrint(tokens []string) bool {
	if len(tokens) == 1 {
		keys := make([]string, 0, len(arrays))
		for k := range arrays {
			keys = append(keys, k)
		}
		sort.Strings(keys)
		for _, name := range keys {
			fmt.Printf("ARRAY %s: ", name)
			arrays[name].printArray()
		}

		keys = make([]string, 0, len(stacks))
		for k := range stacks {
			keys = append(keys, k)
		}
		sort.Strings(keys)
		for _, name := range keys {
			fmt.Printf("STACK %s: ", name)
			stacks[name].printStack()
		}

		keys = make([]string, 0, len(queues))
		for k := range queues {
			keys = append(keys, k)
		}
		sort.Strings(keys)
		for _, name := range keys {
			fmt.Printf("QUEUE %s: ", name)
			queues[name].printQueue()
		}

		keys = make([]string, 0, len(olists))
		for k := range olists {
			keys = append(keys, k)
		}
		sort.Strings(keys)
		for _, name := range keys {
			fmt.Printf("OLIST %s: ", name)
			olists[name].printListOL()
		}

		keys = make([]string, 0, len(tlists))
		for k := range tlists {
			keys = append(keys, k)
		}
		sort.Strings(keys)
		for _, name := range keys {
			fmt.Printf("TLIST %s: ", name)
			tlists[name].printListTL()
		}

		keys = make([]string, 0, len(hashtables))
		for k := range hashtables {
			keys = append(keys, k)
		}
		sort.Strings(keys)
		for _, name := range keys {
			fmt.Printf("HASH %s:\n", name)
			hashtables[name].printHashTable()
		}

		keys = make([]string, 0, len(hashtablesA))
		for k := range hashtablesA {
			keys = append(keys, k)
		}
		sort.Strings(keys)
		for _, name := range keys {
			fmt.Printf("HASHA %s:\n", name)
			hashtablesA[name].printHashTableA()
		}
	} else {
		name := tokens[1]
		if arr, exists := arrays[name]; exists {
			fmt.Printf("ARRAY %s: ", name)
			arr.printArray()
		} else if st, exists := stacks[name]; exists {
			fmt.Printf("STACK %s: ", name)
			st.printStack()
		} else if que, exists := queues[name]; exists {
			fmt.Printf("QUEUE %s: ", name)
			que.printQueue()
		} else if lst, exists := olists[name]; exists {
			fmt.Printf("OLIST %s: ", name)
			lst.printListOL()
		} else if lst, exists := tlists[name]; exists {
			fmt.Printf("TLIST %s: ", name)
			lst.printListTL()
		} else if hash, exists := hashtables[name]; exists {
			fmt.Printf("HASH %s:\n", name)
			hash.printHashTable()
		} else if hasha, exists := hashtablesA[name]; exists {
			fmt.Printf("HASHA %s:\n", name)
			hasha.printHashTableA()
		} else {
			fmt.Printf("Структура не найдена: %s\n", name)
			return false
		}
	}
	return true
}

func processArrayCommand(tokens []string) bool {
	cmd := toUpper(tokens[0])
	switch cmd {
	case "ACREATE":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: ACREATE требует имя массива")
			return false
		}
		name := tokens[1]
		if _, exists := arrays[name]; exists {
			fmt.Printf("Массив '%s' уже существует\n", name)
			return false
		}
		arrays[name] = NewArray()
		fmt.Printf("Массив '%s' успешно создан\n", name)
		return true

	case "APUSH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: APUSH требует имя массива и значение")
			return false
		}
		name := tokens[1]
		if _, exists := arrays[name]; !exists {
			arrays[name] = NewArray()
		}
		value := tokens[2]
		arrays[name].addBackArray(value)
		fmt.Println(value)
		return true

	case "APUSHFRONT":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: APUSHFRONT требует имя массива и значение")
			return false
		}
		name := tokens[1]
		if _, exists := arrays[name]; !exists {
			arrays[name] = NewArray()
		}
		value := tokens[2]
		arrays[name].addElemIndArray(0, value)
		fmt.Println(value)
		return true

	case "APOP":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: APOP требует имя массива")
			return false
		}
		name := tokens[1]
		arr, exists := arrays[name]
		if !exists || arr.getSize() == 0 {
			fmt.Println("Ошибка: Массив пуст или не существует")
			return false
		}
		value := arr.getElemIndArray(arr.getSize() - 1)
		arr.remElemIndArray(arr.getSize() - 1)
		fmt.Println(value)
		return true

	case "AINSERT":
		if len(tokens) < 4 {
			fmt.Println("Ошибка: AINSERT требует имя массива, индекс и значение")
			return false
		}
		name := tokens[1]
		if _, exists := arrays[name]; !exists {
			arrays[name] = NewArray()
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		arrays[name].addElemIndArray(index, value)
		fmt.Println(value)
		return true

	case "ADEL":
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
		if index < 0 || index >= arr.getSize() {
			fmt.Println("Ошибка: Индекс вне границ")
			return false
		}
		value := arr.getElemIndArray(index)
		arr.remElemIndArray(index)
		fmt.Println(value)
		return true

	case "AGET":
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
		value := arr.getElemIndArray(index)
		if value == "" {
			fmt.Println("Ошибка: Индекс вне границ")
			return false
		}
		fmt.Println(value)
		return true

	case "ASET":
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
		arr.changeElemIndArray(index, value)
		fmt.Println(value)
		return true

	case "ASIZE":
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
		fmt.Println(arr.getSize())
		return true
	}

	return false
}

func processStackCommand(tokens []string) bool {
	cmd := toUpper(tokens[0])
	switch cmd {
	case "SCREATE":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: SCREATE требует имя стека")
			return false
		}
		name := tokens[1]
		if _, exists := stacks[name]; exists {
			fmt.Printf("Стек '%s' уже существует\n", name)
			return false
		}
		stacks[name] = &Stack{}
		fmt.Printf("Стек '%s' успешно создан\n", name)
		return true

	case "SPUSH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: SPUSH требует имя стека и значение")
			return false
		}
		name := tokens[1]
		if _, exists := stacks[name]; !exists {
			stacks[name] = &Stack{}
		}
		value := tokens[2]
		stacks[name].pushStack(value)
		fmt.Println(value)
		return true

	case "SPOP":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: SPOP требует имя стека")
			return false
		}
		name := tokens[1]
		st, exists := stacks[name]
		if !exists || st.getTop() == nil {
			fmt.Println("Ошибка: Стек пуст или не существует")
			return false
		}
		value := st.getTop().getData()
		st.popStack()
		fmt.Println(value)
		return true
	}

	return false
}

func processQueueCommand(tokens []string) bool {
	cmd := toUpper(tokens[0])
	switch cmd {
	case "QCREATE":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: QCREATE требует имя очереди")
			return false
		}
		name := tokens[1]
		if _, exists := queues[name]; exists {
			fmt.Printf("Очередь '%s' уже существует\n", name)
			return false
		}
		queues[name] = &Queue{}
		fmt.Printf("Очередь '%s' успешно создана\n", name)
		return true

	case "QPUSH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: QPUSH требует имя очереди и значение")
			return false
		}
		name := tokens[1]
		if _, exists := queues[name]; !exists {
			queues[name] = &Queue{}
		}
		value := tokens[2]
		queues[name].addBackQueue(value)
		fmt.Println(value)
		return true

	case "QPOP":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: QPOP требует имя очереди")
			return false
		}
		name := tokens[1]
		que, exists := queues[name]
		if !exists || que.getHead() == nil {
			fmt.Println("Ошибка: Очередь пуста или не существует")
			return false
		}
		value := que.getHead().getData()
		que.remFrontQueue()
		fmt.Println(value)
		return true
	}

	return false
}

func processOLListCommand(tokens []string) bool {
	cmd := toUpper(tokens[0])
	switch cmd {
	case "FCREATE":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: FCREATE требует имя списка")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; exists {
			fmt.Printf("Односвязный список '%s' уже существует\n", name)
			return false
		}
		olists[name] = &OLList{}
		fmt.Printf("Односвязный список '%s' успешно создан\n", name)
		return true

	case "FPUSH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FPUSH требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			olists[name] = &OLList{}
		}
		value := tokens[2]
		olists[name].addBackOL(value)
		fmt.Println(value)
		return true

	case "FPUSHFRONT":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FPUSHFRONT требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			olists[name] = &OLList{}
		}
		value := tokens[2]
		olists[name].addFrontOL(value)
		fmt.Println(value)
		return true

	case "FINSERTAFTER":
		if len(tokens) < 4 {
			fmt.Println("Ошибка: FINSERTAFTER требует имя списка, индекс и значение")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		olists[name].insertAfterIndOL(index, value)
		fmt.Println(value)
		return true

	case "FINSERTBEFORE":
		if len(tokens) < 4 {
			fmt.Println("Ошибка: FINSERTBEFORE требует имя списка, индекс и значение")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		olists[name].insertBeforeIndOL(index, value)
		fmt.Println(value)
		return true

	case "FDELHEAD":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: FDELHEAD требует имя списка")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists || lst.getHead() == nil {
			fmt.Println("Ошибка: Список пуст или не существует")
			return false
		}
		lst.remFrontOL()
		fmt.Println("Голова успешно удалена")
		return true

	case "FDELTAIL":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: FDELTAIL требует имя списка")
			return false
		}
		name := tokens[1]
		lst, exists := olists[name]
		if !exists || lst.getHead() == nil {
			fmt.Println("Ошибка: Список пуст или не существует")
			return false
		}
		lst.remBackOL()
		fmt.Println("Хвост успешно удален")
		return true

	case "FDELVALUE":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FDELVALUE требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		value := tokens[2]
		olists[name].remValOL(value)
		fmt.Printf("Значение %s удалено\n", value)
		return true

	case "FDELBEFORE":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FDELBEFORE требует имя списка и индекс")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		olists[name].deleteBeforeIndOL(index)
		fmt.Printf("Элемент перед индексом %d удален\n", index)
		return true

	case "FDELAFTER":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FDELAFTER требует имя списка и индекс")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		olists[name].deleteAfterIndOL(index)
		fmt.Printf("Элемент после индекса %d удален\n", index)
		return true

	case "FSEARCH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: FSEARCH требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := olists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		value := tokens[2]
		index := olists[name].findValOL(value)
		if index == -1 {
			fmt.Println("Значение не найдено")
		} else {
			fmt.Printf("Значение найдено по индексу: %d\n", index)
		}
		return true
	}

	return false
}

func processTLListCommand(tokens []string) bool {
	cmd := toUpper(tokens[0])
	switch cmd {
	case "LCREATE":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: LCREATE требует имя списка")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; exists {
			fmt.Printf("Двусвязный список '%s' уже существует\n", name)
			return false
		}
		tlists[name] = &TLList{}
		fmt.Printf("Двусвязный список '%s' успешно создан\n", name)
		return true

	case "LPUSH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LPUSH требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			tlists[name] = &TLList{}
		}
		value := tokens[2]
		tlists[name].addBackTL(value)
		fmt.Println(value)
		return true

	case "LPUSHFRONT":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LPUSHFRONT требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			tlists[name] = &TLList{}
		}
		value := tokens[2]
		tlists[name].addFrontTL(value)
		fmt.Println(value)
		return true

	case "LINSERTAFTER":
		if len(tokens) < 4 {
			fmt.Println("Ошибка: LINSERTAFTER требует имя списка, индекс и значение")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		tlists[name].insertAfterIndTL(index, value)
		fmt.Println(value)
		return true

	case "LINSERTBEFORE":
		if len(tokens) < 4 {
			fmt.Println("Ошибка: LINSERTBEFORE требует имя списка, индекс и значение")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		tlists[name].insertBeforeIndTL(index, value)
		fmt.Println(value)
		return true

	case "LDELHEAD":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: LDELHEAD требует имя списка")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists || lst.getHead() == nil {
			fmt.Println("Ошибка: Список пуст или не существует")
			return false
		}
		lst.remFrontTL()
		fmt.Println("Голова успешно удалена")
		return true

	case "LDELTAIL":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: LDELTAIL требует имя списка")
			return false
		}
		name := tokens[1]
		lst, exists := tlists[name]
		if !exists || lst.getHead() == nil {
			fmt.Println("Ошибка: Список пуст или не существует")
			return false
		}
		lst.remBackTL()
		fmt.Println("Хвост успешно удален")
		return true

	case "LDELVALUE":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LDELVALUE требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		value := tokens[2]
		tlists[name].remValTL(value)
		fmt.Printf("Значение %s удалено\n", value)
		return true

	case "LDELBEFORE":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LDELBEFORE требует имя списка и индекс")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		tlists[name].deleteBeforeIndTL(index)
		fmt.Printf("Элемент перед индексом %d удален\n", index)
		return true

	case "LDELAFTER":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LDELAFTER требует имя списка и индекс")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		index, _ := strconv.Atoi(tokens[2])
		tlists[name].deleteAfterIndTL(index)
		fmt.Printf("Элемент после индекса %d удален\n", index)
		return true

	case "LSEARCH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: LSEARCH требует имя списка и значение")
			return false
		}
		name := tokens[1]
		if _, exists := tlists[name]; !exists {
			fmt.Println("Ошибка: Список не существует")
			return false
		}
		value := tokens[2]
		index := tlists[name].findValTL(value)
		if index == -1 {
			fmt.Println("Значение не найдено")
		} else {
			fmt.Printf("Значение найдено по индексу: %d\n", index)
		}
		return true
	}

	return false
}

func processHashCommand(tokens []string) bool {
	cmd := toUpper(tokens[0])
	switch cmd {
	case "HCREATE":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: HCREATE требует имя таблицы")
			return false
		}
		name := tokens[1]
		if _, exists := hashtables[name]; exists {
			fmt.Printf("Хэш-таблица '%s' уже существует\n", name)
			return false
		}
		hashtables[name] = NewHash()
		fmt.Printf("Хэш-таблица '%s' успешно создана\n", name)
		return true

	case "HINSERT":
		if len(tokens) < 4 {
			fmt.Println("Ошибка: HINSERT требует имя таблицы, ключ и значение")
			return false
		}
		name := tokens[1]
		if _, exists := hashtables[name]; !exists {
			hashtables[name] = NewHash()
		}
		key := tokens[2]
		value, _ := strconv.Atoi(tokens[3])
		hashtables[name].addElement(key, value)
		fmt.Printf("Добавлен элемент [%s -> %d] в таблицу '%s'\n", key, value, name)
		return true

	case "HDELETE":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: HDELETE требует имя таблицы и ключ")
			return false
		}
		name := tokens[1]
		if _, exists := hashtables[name]; !exists {
			fmt.Println("Ошибка: Хэш-таблица не существует")
			return false
		}
		key := tokens[2]
		hashtables[name].deleteElement(key)
		fmt.Printf("Элемент с ключом '%s' удален из таблицы '%s'\n", key, name)
		return true

	case "HSEARCH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: HSEARCH требует имя таблицы и ключ")
			return false
		}
		name := tokens[1]
		if _, exists := hashtables[name]; !exists {
			fmt.Println("Ошибка: Хэш-таблица не существует")
			return false
		}
		key := tokens[2]
		result := hashtables[name].findElement(key)
		if result != -1 {
			fmt.Printf("Элемент найден: [%s -> %d]\n", key, result)
		} else {
			fmt.Printf("Элемент с ключом '%s' не найден\n", key)
		}
		return true
	}

	return false
}

func processHashACommand(tokens []string) bool {
	cmd := toUpper(tokens[0])
	switch cmd {
	case "ACREATEH":
		if len(tokens) < 2 {
			fmt.Println("Ошибка: ACREATEH требует имя таблицы")
			return false
		}
		name := tokens[1]
		if _, exists := hashtablesA[name]; exists {
			fmt.Printf("Хэш-таблица '%s' уже существует\n", name)
			return false
		}
		hashtablesA[name] = NewHashA()
		fmt.Printf("Хэш-таблица с открытой адресацией '%s' успешно создана\n", name)
		return true

	case "AINSERTH":
		if len(tokens) < 4 {
			fmt.Println("Ошибка: AINSERTH требует имя таблицы, ключ и значение")
			return false
		}
		name := tokens[1]
		if _, exists := hashtablesA[name]; !exists {
			hashtablesA[name] = NewHashA()
		}
		key := tokens[2]
		value, _ := strconv.Atoi(tokens[3])
		hashtablesA[name].addElementA(key, value)
		fmt.Printf("Добавлен элемент [%s -> %d] в таблицу '%s'\n", key, value, name)
		return true

	case "ADELETEH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: ADELETEH требует имя таблицы и ключ")
			return false
		}
		name := tokens[1]
		if _, exists := hashtablesA[name]; !exists {
			fmt.Println("Ошибка: Хэш-таблица не существует")
			return false
		}
		key := tokens[2]
		hashtablesA[name].deleteElementA(key)
		fmt.Printf("Элемент с ключом '%s' удален из таблицы '%s'\n", key, name)
		return true

	case "ASEARCHH":
		if len(tokens) < 3 {
			fmt.Println("Ошибка: ASEARCHH требует имя таблицы и ключ")
			return false
		}
		name := tokens[1]
		if _, exists := hashtablesA[name]; !exists {
			fmt.Println("Ошибка: Хэш-таблица не существует")
			return false
		}
		key := tokens[2]
		result := hashtablesA[name].findElementA(key)
		if result != -1 {
			fmt.Printf("Элемент найден: [%s -> %d]\n", key, result)
		} else {
			fmt.Printf("Элемент с ключом '%s' не найден\n", key)
		}
		return true
	}

	return false
}

func processCommand(tokens []string, filename string) bool {
	if len(tokens) == 0 {
		return true
	}

	cmd := toUpper(tokens[0])

	switch cmd {
	case "EXIT":
		fmt.Println("Выход из программы...")
		return false

	case "HELP":
		printHelp()

	case "PRINT":
		return processPrint(tokens)

	case "CLEAR":
		cleanupAll()
		file, _ := os.Create(filename)
		file.Close()
		fmt.Println("База данных успешно очищена")
		return true

	default:
		firstChar := rune(cmd[0])
		switch {
		case firstChar == 'A' && !strings.HasPrefix(cmd, "ACREATEH") && !strings.HasPrefix(cmd, "AINSERTH") && !strings.HasPrefix(cmd, "ASEARCHH") && !strings.HasPrefix(cmd, "ADELETEH"):
			return processArrayCommand(tokens)
		case firstChar == 'S':
			return processStackCommand(tokens)
		case firstChar == 'Q':
			return processQueueCommand(tokens)
		case firstChar == 'F':
			return processOLListCommand(tokens)
		case firstChar == 'L':
			return processTLListCommand(tokens)
		case firstChar == 'H':
			return processHashCommand(tokens)
		case strings.HasPrefix(cmd, "ACREATEH") || strings.HasPrefix(cmd, "AINSERTH") || strings.HasPrefix(cmd, "ASEARCHH") || strings.HasPrefix(cmd, "ADELETEH"):
			return processHashACommand(tokens)
		default:
			fmt.Printf("Неизвестная команда: %s\n", tokens[0])
			fmt.Println("Введите HELP для списка команд")
		}
	}

	return true
}

func parseArguments(args []string) (filename, query string) {
	for i := 1; i < len(args); i++ {
		arg := args[i]
		switch arg {
		case "--file":
			if i+1 < len(args) {
				filename = args[i+1]
				i++
			}
		case "--query":
			if i+1 < len(args) {
				query = args[i+1]
				i++
			}
		case "--help":
			printHelp()
			os.Exit(0)
		}
	}
	return
}

func main() {
	if len(os.Args) > 1 {
		filename, query := parseArguments(os.Args)
		
		if filename == "" || query == "" {
			fmt.Fprintln(os.Stderr, "Ошибка: Требуются оба параметра --file и --query")
			fmt.Println("Использование: ./zap --file <имя_файла> --query \"команда\"")
			os.Exit(1)
		}
		
		loadFromFile(filename)
		success := processCommand(splitCommand(query), filename)
		saveToFile(filename)
		
		if !success {
			os.Exit(1)
		}
		os.Exit(0)
	}
}
