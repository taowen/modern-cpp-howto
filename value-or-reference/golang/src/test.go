package main

import (
	"fmt"
	"unsafe"
)

type MyStruct struct {
	field int
}

func main() {
	{
		local_variable := 1
		modify_single_value(local_variable)
		fmt.Println(local_variable) // 1
	}
	{
		local_variable := 1
		modify_single_pointer(&local_variable)
		fmt.Println(local_variable) // 2
	}
	{
		local_variable := []int{1, 1}
		modify_list_value(local_variable)
		fmt.Println(local_variable) // []int{1, 2}
	}
	{
		local_variable := MyStruct{1}
		fmt.Println(unsafe.Pointer(&local_variable))
		local_variable.modify_struct_value()
		fmt.Println(local_variable) // {1}
		copied := local_variable.copy_my_self()
		fmt.Println(unsafe.Pointer(&copied))
	}
}

func modify_single_value(single int) {
	single = 2
}

func modify_single_pointer(single *int) {
	*single = 2
}

func modify_list_value(list []int) {
	list[1] = 2
}

// 何时使用指针
// https://golang.org/doc/faq#methods_on_values_or_pointers
func (self MyStruct) modify_struct_value() {
	fmt.Println(unsafe.Pointer(&self))
	self.field = 2
}
func (self MyStruct) copy_my_self() MyStruct {
	fmt.Println(unsafe.Pointer(&self))
	return self
}
