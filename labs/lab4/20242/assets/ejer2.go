package main

import (
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func A() {
	for x := 0; x < 100; x++ {
		fmt.Printf("a1")
		fmt.Printf("a2")
	}
	wg.Done()
}

func B() {
	for x := 0; x < 100; x++ {
		fmt.Printf("b1")
		fmt.Printf("b2")
	}
	wg.Done()
}

func main() {
	wg.Add(2)
	go A()
	go B()
	wg.Wait()
	fmt.Println()
}
