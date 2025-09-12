package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

// Saymon N. - 20211866

var wg sync.WaitGroup
var aDone chan int

func A() {
	r := rand.New(rand.NewSource(time.Now().UnixNano() + 1))
	for x := 0; x < 100; x++ {
		limit1 := r.Intn(3000)
		for i := 0; i < limit1; i++ {
			// simulate some work
		}
		fmt.Printf("a1")
		<-aDone
		limit2 := r.Intn(3000)
		for i := 0; i < limit2; i++ {
			// simulate some work
		}
		fmt.Printf("a2")
		aDone <- 1
		fmt.Printf("\n")
	}
	wg.Done()
}

func B() {
	r := rand.New(rand.NewSource(time.Now().UnixNano() + 2))
	for x := 0; x < 100; x++ {
		limit1 := r.Intn(3000)
		for i := 0; i < limit1; i++ {
			// simulate some work
		}
		fmt.Printf("b1")
		aDone <- 1
		limit2 := r.Intn(3000)
		for i := 0; i < limit2; i++ {
			// simulate some work
		}
		fmt.Printf("b2")
		<-aDone
		fmt.Printf("\n")
	}
	wg.Done()
}

func main() {
	wg.Add(2)
	aDone = make(chan int)
	go A()
	go B()
	wg.Wait()
	fmt.Println()
}
