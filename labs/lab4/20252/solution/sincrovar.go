package main

// sn

import (
	"fmt"
	"sync"
)

var (
	x    int
	mu   sync.Mutex
	cond *sync.Cond
)

func child() {
	mu.Lock()
	defer mu.Unlock()
	fmt.Println("hijo")
	x++
	cond.Signal()
}

func main() {
	x = 0
	cond = sync.NewCond(&mu)
	mu.Lock()
	defer mu.Unlock()
	fmt.Println("padre - inicio")
	go child()
	for x == 0 {
		cond.Wait()
	}
	fmt.Println("padre - fin")
}
