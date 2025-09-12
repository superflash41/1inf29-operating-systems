package main

import (
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func worker1() {
	for x := 0; x < 10; x++ {
		fmt.Printf("I like to ")
		fmt.Println("systems")
	}
	wg.Done()
}

func worker2() {
	for x := 0; x < 10; x++ {
		fmt.Printf("study ")
		fmt.Printf("operating ")
	}
	wg.Done()
}

func main() {
	wg.Add(2)
	go worker1()
	go worker2()
	wg.Wait()
}
