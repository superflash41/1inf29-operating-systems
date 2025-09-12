package main

import (
	"fmt"
	"sync"
)

// Saymon N. - 20211866

var (
	wg   sync.WaitGroup
	c    []chan int
	aorb int
	mu   sync.Mutex
)

func getAorb() int {
	mu.Lock()
	defer mu.Unlock()
	return aorb
}

func toggleAorb() {
	mu.Lock()
	if aorb == 0 {
		aorb = 1
	} else {
		aorb = 0
	}
	mu.Unlock()
}

func worker1() { // c[0]
	defer wg.Done()
	for {
		<-c[0]
		fmt.Printf("A")
		if getAorb() == 1 {
			c[2] <- 1
		} else {
			c[1] <- 1
		}
	}
}

func worker2() { // c[1]
	defer wg.Done()
	for {
		<-c[1]
		fmt.Printf("B")
		if getAorb() == 1 {
			c[0] <- 1
		} else {
			c[2] <- 1
		}
	}
}

func worker3() { // c[2]
	defer wg.Done()
	for {
		<-c[2]
		fmt.Printf("C")
		toggleAorb()
		c[3] <- 1
	}
}

func worker4() { // c[3]
	defer wg.Done()
	for {
		<-c[3]
		fmt.Printf("D")
		c[4] <- 1
	}
}

func worker5() { // c[4]
	defer wg.Done()
	for {
		<-c[4]
		fmt.Printf("E")
		if getAorb() == 1 {
			c[1] <- 1
		} else {
			c[0] <- 1
		}
	}
}

func main() {
	aorb = 0 // AB->0   BA->1
	c = make([]chan int, 5)
	for i := range c {
		c[i] = make(chan int, 1)
	}
	wg.Add(5)
	go worker1()
	go worker2()
	go worker3()
	go worker4()
	go worker5()
	c[0] <- 1 // start w A
	wg.Wait()
	fmt.Printf("\n")
}
