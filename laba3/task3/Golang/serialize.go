package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type BinaryWriter struct {
	file *os.File
	err  error
}

func NewBinaryWriter(filename string) *BinaryWriter {
	file, err := os.Create(filename)
	return &BinaryWriter{file: file, err: err}
}

func (w *BinaryWriter) WriteUint32(value uint32) {
	if w.err != nil {
		return
	}
	w.err = binary.Write(w.file, binary.LittleEndian, value)
}

func (w *BinaryWriter) WriteInt32(value int32) {
	if w.err != nil {
		return
	}
	w.err = binary.Write(w.file, binary.LittleEndian, value)
}

func (w *BinaryWriter) WriteString(str string) {
	if w.err != nil {
		return
	}
	length := uint32(len(str))
	w.WriteUint32(length)
	if length > 0 {
		_, w.err = w.file.Write([]byte(str))
	}
}

func (w *BinaryWriter) WriteBool(value bool) {
	if w.err != nil {
		return
	}
	var byteVal byte
	if value {
		byteVal = 1
	}
	w.err = binary.Write(w.file, binary.LittleEndian, byteVal)
}

func (w *BinaryWriter) Close() error {
	if w.file != nil {
		return w.file.Close()
	}
	return nil
}

func (w *BinaryWriter) Error() error {
	return w.err
}

type BinaryReader struct {
	file *os.File
	err  error
}

func NewBinaryReader(filename string) *BinaryReader {
	file, err := os.Open(filename)
	return &BinaryReader{file: file, err: err}
}

func (r *BinaryReader) ReadUint32() uint32 {
	if r.err != nil {
		return 0
	}
	var value uint32
	r.err = binary.Read(r.file, binary.LittleEndian, &value)
	return value
}

func (r *BinaryReader) ReadInt32() int32 {
	if r.err != nil {
		return 0
	}
	var value int32
	r.err = binary.Read(r.file, binary.LittleEndian, &value)
	return value
}

func (r *BinaryReader) ReadString() string {
	if r.err != nil {
		return ""
	}
	length := r.ReadUint32()
	if length == 0 {
		return ""
	}
	buffer := make([]byte, length)
	_, r.err = r.file.Read(buffer)
	return string(buffer)
}

func (r *BinaryReader) ReadBool() bool {
	if r.err != nil {
		return false
	}
	var byteVal byte
	r.err = binary.Read(r.file, binary.LittleEndian, &byteVal)
	return byteVal == 1
}

func (r *BinaryReader) Close() error {
	if r.file != nil {
		return r.file.Close()
	}
	return nil
}

func (r *BinaryReader) Error() error {
	return r.err
}

func writeTextLine(writer *bufio.Writer, line string) error {
	_, err := writer.WriteString(line + "\n")
	return err
}

func readTextLine(reader *bufio.Reader) (string, error) {
	return reader.ReadString('\n')
}

func (n *Node) SerializeBinary(filename string) error {
	writer := NewBinaryWriter(filename)
	defer writer.Close()

	if writer.err != nil {
		return writer.err
	}

	current := n
	for current != nil {
		writer.WriteString(current.data)
		current = current.next
	}
	writer.WriteString("")

	return writer.Error()
}

func DeserializeNodeBinary(filename string) *Node {
	reader := NewBinaryReader(filename)
	defer reader.Close()

	if reader.err != nil {
		return nil
	}

	var head, tail *Node
	for {
		data := reader.ReadString()
		if data == "" || reader.err != nil {
			break
		}
		node := createNode(data)
		if head == nil {
			head = node
			tail = node
		} else {
			tail.next = node
			tail = node
		}
	}

	return head
}

func (n *Node) SerializeText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	defer writer.Flush()

	current := n
	for current != nil {
		if err := writeTextLine(writer, current.data); err != nil {
			return err
		}
		current = current.next
	}
	writeTextLine(writer, "END")

	return nil
}

func DeserializeNodeText(filename string) *Node {
	file, err := os.Open(filename)
	if err != nil {
		return nil
	}
	defer file.Close()

	reader := bufio.NewReader(file)
	var head, tail *Node

	for {
		line, err := readTextLine(reader)
		if err != nil {
			break
		}
		line = strings.TrimSpace(line)
		if line == "END" {
			break
		}
		node := createNode(line)
		if head == nil {
			head = node
			tail = node
		} else {
			tail.next = node
			tail = node
		}
	}

	return head
}

func (a *Array) SerializeBinary(filename string) error {
	writer := NewBinaryWriter(filename)
	defer writer.Close()

	if writer.err != nil {
		return writer.err
	}

	writer.WriteUint32(uint32(a.capacity))
	writer.WriteUint32(uint32(a.size))

	for i := 0; i < a.size; i++ {
		writer.WriteString(a.data[i])
	}

	return writer.Error()
}

func DeserializeArrayBinary(filename string) *Array {
	reader := NewBinaryReader(filename)
	defer reader.Close()

	if reader.err != nil {
		return nil
	}

	array := &Array{}
	array.capacity = int(reader.ReadUint32())
	array.size = int(reader.ReadUint32())
	array.data = make([]string, array.capacity)

	for i := 0; i < array.size; i++ {
		array.data[i] = reader.ReadString()
	}

	return array
}

func (a *Array) SerializeText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	defer writer.Flush()

	writeTextLine(writer, fmt.Sprintf("CAPACITY:%d", a.capacity))
	writeTextLine(writer, fmt.Sprintf("SIZE:%d", a.size))
	writeTextLine(writer, "DATA_START")

	for i := 0; i < a.size; i++ {
		writeTextLine(writer, a.data[i])
	}

	writeTextLine(writer, "DATA_END")

	return nil
}

func DeserializeArrayText(filename string) *Array {
	file, err := os.Open(filename)
	if err != nil {
		return nil
	}
	defer file.Close()

	reader := bufio.NewReader(file)
	array := &Array{}

	for {
		line, err := readTextLine(reader)
		if err != nil {
			break
		}
		line = strings.TrimSpace(line)

		if strings.HasPrefix(line, "CAPACITY:") {
			val, _ := strconv.Atoi(strings.TrimPrefix(line, "CAPACITY:"))
			array.capacity = val
		} else if strings.HasPrefix(line, "SIZE:") {
			val, _ := strconv.Atoi(strings.TrimPrefix(line, "SIZE:"))
			array.size = val
			array.data = make([]string, array.capacity)
		} else if line == "DATA_START" {
			for i := 0; i < array.size; i++ {
				dataLine, _ := readTextLine(reader)
				array.data[i] = strings.TrimSpace(dataLine)
			}
		}
	}

	return array
}
func (s *Stack) SerializeBinary(filename string) error {
	return s.top.SerializeBinary(filename)
}

func DeserializeStackBinary(filename string) *Stack {
	head := DeserializeNodeBinary(filename)
	return &Stack{top: head}
}

func (s *Stack) SerializeText(filename string) error {
	return s.top.SerializeText(filename)
}

func DeserializeStackText(filename string) *Stack {
	head := DeserializeNodeText(filename)
	return &Stack{top: head}
}

func (q *Queue) SerializeBinary(filename string) error {
	return q.head.SerializeBinary(filename)
}

func DeserializeQueueBinary(filename string) *Queue {
	head := DeserializeNodeBinary(filename)
	var tail *Node
	current := head
	for current != nil && current.next != nil {
		current = current.next
	}
	tail = current
	
	return &Queue{head: head, tail: tail}
}

func (q *Queue) SerializeText(filename string) error {
	return q.head.SerializeText(filename)
}

func DeserializeQueueText(filename string) *Queue {
	head := DeserializeNodeText(filename)
	var tail *Node
	current := head
	for current != nil && current.next != nil {
		current = current.next
	}
	tail = current
	
	return &Queue{head: head, tail: tail}
}
func (ol *OLList) SerializeBinary(filename string) error {
	return ol.head.SerializeBinary(filename)
}

func DeserializeOLListBinary(filename string) *OLList {
	head := DeserializeNodeBinary(filename)
	var tail *Node
	current := head
	for current != nil && current.next != nil {
		current = current.next
	}
	tail = current
	
	return &OLList{head: head, tail: tail}
}

func (ol *OLList) SerializeText(filename string) error {
	return ol.head.SerializeText(filename)
}

func DeserializeOLListText(filename string) *OLList {
	head := DeserializeNodeText(filename)
	var tail *Node
	current := head
	for current != nil && current.next != nil {
		current = current.next
	}
	tail = current
	
	return &OLList{head: head, tail: tail}
}

func (tl *TLList) SerializeBinary(filename string) error {
	writer := NewBinaryWriter(filename)
	defer writer.Close()

	if writer.err != nil {
		return writer.err
	}
	current := tl.head
	for current != nil {
		writer.WriteString(current.data)
		current = current.next
	}
	writer.WriteString("")

	return writer.Error()
}

func DeserializeTLListBinary(filename string) *TLList {
	reader := NewBinaryReader(filename)
	defer reader.Close()

	if reader.err != nil {
		return nil
	}

	var head, tail *Node
	var prev *Node = nil

	for {
		data := reader.ReadString()
		if data == "" || reader.err != nil {
			break
		}
		node := createNode(data)
		node.prev = prev
		
		if head == nil {
			head = node
		} else {
			tail.next = node
			node.prev = tail
		}
		
		tail = node
		prev = node
	}

	return &TLList{head: head, tail: tail}
}

func (tl *TLList) SerializeText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	defer writer.Flush()

	current := tl.head
	for current != nil {
		if err := writeTextLine(writer, current.data); err != nil {
			return err
		}
		current = current.next
	}
	writeTextLine(writer, "END")

	return nil
}

func DeserializeTLListText(filename string) *TLList {
	file, err := os.Open(filename)
	if err != nil {
		return nil
	}
	defer file.Close()

	reader := bufio.NewReader(file)
	var head, tail *Node
	var prev *Node = nil

	for {
		line, err := readTextLine(reader)
		if err != nil {
			break
		}
		line = strings.TrimSpace(line)
		if line == "END" {
			break
		}
		node := createNode(line)
		node.prev = prev
		
		if head == nil {
			head = node
		} else {
			tail.next = node
			node.prev = tail
		}
		
		tail = node
		prev = node
	}

	return &TLList{head: head, tail: tail}
}

func (h *Hash) SerializeBinary(filename string) error {
	writer := NewBinaryWriter(filename)
	defer writer.Close()

	if writer.err != nil {
		return writer.err
	}
	writer.WriteUint32(uint32(h.capacity))
	writer.WriteUint32(uint32(h.size))
	for i := 0; i < h.capacity; i++ {
		count := 0
		current := h.cell[i]
		for current != nil {
			count++
			current = current.next
		}
		writer.WriteUint32(uint32(count))

		current = h.cell[i]
		for current != nil {
			writer.WriteString(current.key)
			writer.WriteInt32(int32(current.data))
			current = current.next
		}
	}

	return writer.Error()
}

func DeserializeHashBinary(filename string) *Hash {
	reader := NewBinaryReader(filename)
	defer reader.Close()

	if reader.err != nil {
		return nil
	}

	hash := &Hash{}
	hash.capacity = int(reader.ReadUint32())
	hash.size = int(reader.ReadUint32())
	hash.cell = make([]*NodeHash, hash.capacity)

	for i := 0; i < hash.capacity; i++ {
		chainLength := int(reader.ReadUint32())
		var prev *NodeHash = nil
		
		for j := 0; j < chainLength; j++ {
			key := reader.ReadString()
			data := int(reader.ReadInt32())
			
			node := NewNodeHash(key, data)
			if prev == nil {
				hash.cell[i] = node
			} else {
				prev.next = node
			}
			prev = node
		}
	}

	return hash
}

func (h *Hash) SerializeText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	defer writer.Flush()
	writeTextLine(writer, fmt.Sprintf("CAPACITY:%d", h.capacity))
	writeTextLine(writer, fmt.Sprintf("SIZE:%d", h.size))
	writeTextLine(writer, "BUCKETS_START")

	for i := 0; i < h.capacity; i++ {
		writeTextLine(writer, fmt.Sprintf("BUCKET:%d", i))
		
		count := 0
		current := h.cell[i]
		for current != nil {
			count++
			current = current.next
		}
		writeTextLine(writer, fmt.Sprintf("COUNT:%d", count))
		
		current = h.cell[i]
		for current != nil {
			writeTextLine(writer, fmt.Sprintf("%s:%d", current.key, current.data))
			current = current.next
		}
		writeTextLine(writer, "BUCKET_END")
	}

	writeTextLine(writer, "BUCKETS_END")

	return nil
}

func DeserializeHashText(filename string) *Hash {
	file, err := os.Open(filename)
	if err != nil {
		return nil
	}
	defer file.Close()

	reader := bufio.NewReader(file)
	hash := &Hash{}
	var currentBucket *NodeHash = nil
	var bucketIndex = -1

	for {
		line, err := readTextLine(reader)
		if err != nil {
			break
		}
		line = strings.TrimSpace(line)

		if strings.HasPrefix(line, "CAPACITY:") {
			val, _ := strconv.Atoi(strings.TrimPrefix(line, "CAPACITY:"))
			hash.capacity = val
			hash.cell = make([]*NodeHash, hash.capacity)
		} else if strings.HasPrefix(line, "SIZE:") {
			val, _ := strconv.Atoi(strings.TrimPrefix(line, "SIZE:"))
			hash.size = val
		} else if strings.HasPrefix(line, "BUCKET:") {
			val, _ := strconv.Atoi(strings.TrimPrefix(line, "BUCKET:"))
			bucketIndex = val
			currentBucket = nil
		} else if strings.Contains(line, ":") && !strings.HasPrefix(line, "COUNT:") && 
			!strings.HasPrefix(line, "BUCKET") && line != "BUCKETS_START" && line != "BUCKETS_END" {
			parts := strings.Split(line, ":")
			if len(parts) == 2 {
				key := parts[0]
				data, _ := strconv.Atoi(parts[1])
				
				node := NewNodeHash(key, data)
				if currentBucket == nil {
					hash.cell[bucketIndex] = node
				} else {
					currentBucket.next = node
				}
				currentBucket = node
			}
		}
	}

	return hash
}

func (h *HashA) SerializeBinary(filename string) error {
	writer := NewBinaryWriter(filename)
	defer writer.Close()

	if writer.err != nil {
		return writer.err
	}

	writer.WriteUint32(uint32(h.capacity))
	writer.WriteUint32(uint32(h.size))

	for i := 0; i < h.capacity; i++ {
		cell := h.cell[i]
		writer.WriteString(cell.key)
		writer.WriteInt32(int32(cell.data))
		writer.WriteBool(cell.isDeleted)
		writer.WriteBool(cell.isEmpty)
	}

	return writer.Error()
}

func DeserializeHashABinary(filename string) *HashA {
	reader := NewBinaryReader(filename)
	defer reader.Close()

	if reader.err != nil {
		return nil
	}

	hash := &HashA{}
	hash.capacity = int(reader.ReadUint32())
	hash.size = int(reader.ReadUint32())
	hash.cell = make([]NodeHashA, hash.capacity)

	for i := 0; i < hash.capacity; i++ {
		hash.cell[i].key = reader.ReadString()
		hash.cell[i].data = int(reader.ReadInt32())
		hash.cell[i].isDeleted = reader.ReadBool()
		hash.cell[i].isEmpty = reader.ReadBool()
	}

	return hash
}

func (h *HashA) SerializeText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	defer writer.Flush()
	writeTextLine(writer, fmt.Sprintf("CAPACITY:%d", h.capacity))
	writeTextLine(writer, fmt.Sprintf("SIZE:%d", h.size))
	writeTextLine(writer, "CELLS_START")

	for i := 0; i < h.capacity; i++ {
		cell := h.cell[i]
		if !cell.isEmpty {
			status := "ACTIVE"
			if cell.isDeleted {
				status = "DELETED"
			}
			writeTextLine(writer, fmt.Sprintf("%d:%s:%d:%s", i, cell.key, cell.data, status))
		} else {
			writeTextLine(writer, fmt.Sprintf("%d:EMPTY", i))
		}
	}

	writeTextLine(writer, "CELLS_END")

	return nil
}

func DeserializeHashAText(filename string) *HashA {
	file, err := os.Open(filename)
	if err != nil {
		return nil
	}
	defer file.Close()

	reader := bufio.NewReader(file)
	hash := &HashA{}

	for {
		line, err := readTextLine(reader)
		if err != nil {
			break
		}
		line = strings.TrimSpace(line)

		if strings.HasPrefix(line, "CAPACITY:") {
			val, _ := strconv.Atoi(strings.TrimPrefix(line, "CAPACITY:"))
			hash.capacity = val
			hash.cell = make([]NodeHashA, hash.capacity)
			for i := range hash.cell {
				hash.cell[i].isEmpty = true
			}
		} else if strings.HasPrefix(line, "SIZE:") {
			val, _ := strconv.Atoi(strings.TrimPrefix(line, "SIZE:"))
			hash.size = val
		} else if strings.Contains(line, ":") && !strings.HasPrefix(line, "CELLS") {
			parts := strings.Split(line, ":")
			if len(parts) >= 2 {
				index, _ := strconv.Atoi(parts[0])
				
				if parts[1] == "EMPTY" {
					hash.cell[index].isEmpty = true
				} else if len(parts) == 4 {
					hash.cell[index].key = parts[1]
					data, _ := strconv.Atoi(parts[2])
					hash.cell[index].data = data
					hash.cell[index].isEmpty = false
					hash.cell[index].isDeleted = (parts[3] == "DELETED")
				}
			}
		}
	}

	return hash
}
