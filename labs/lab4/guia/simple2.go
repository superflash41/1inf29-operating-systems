package main

// 1inf29-lab4-guia.pdf, page 4
// the goal of this script is to show how to correctly
// wait for goroutines to finish using sync.WaitGroup

import (
	"fmt"
	"sync"
)

func routine(n int) {
	defer wg.Done()
	fmt.Printf("i am goroutine %d\n", n)
}

var wg sync.WaitGroup

func main() {
	for x := range 5 {
		wg.Add(1)
		go routine(x)
	}

	// we correctly wait for all goroutines to finish before exiting main
	wg.Wait()
}
