package main

import (
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func worker1() {
	for {
		fmt.Printf("A")
	}
	wg.Done()
}

func worker2() {
	for {
		fmt.Printf("B")
	}
	wg.Done()
}

func worker3() {
	for {
		fmt.Printf("C")
	}
	wg.Done()
}

func worker4() {
	for {
		fmt.Printf("D")
	}
	wg.Done()
}

func worker5() {
	for {
		fmt.Printf("E")
	}
	wg.Done()
}

func main() {
	wg.Add(5)
	go worker1()
	go worker2()
	go worker3()
	go worker4()
	go worker5()
	wg.Wait()
	fmt.Printf("\n")
}
