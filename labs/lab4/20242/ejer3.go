package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

var (
	wg sync.WaitGroup
	t  int
)

func cook() {
	for {
		putServingsInPot(5)
	}
	wg.Done()
}

func putServingsInPot(n int) {
	fmt.Printf("Put Servings %d In Pot\n", n)
	t = rand.Intn(10) // n will be between 0 and 10
	time.Sleep(time.Duration(t) * time.Millisecond)
}

func getServingFromPot(n int) {
	fmt.Printf("%d: Get serving from Pot\n", n)
}

func eat(n int) {
	fmt.Printf("%d: eating...\n", n)
}

func savage(n int) {
	for {
		getServingFromPot(n)
		eat(n)
		t = rand.Intn(10) // n will be between 0 and 10
		time.Sleep(time.Duration(t) * time.Millisecond)
	}
	wg.Done()
}

func main() {
	rand.Seed(time.Now().UnixNano())
	wg.Add(11)
	go cook()
	for x := 0; x < 10; x++ {
		go savage(x)
	}
	wg.Wait()
}
