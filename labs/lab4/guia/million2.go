package main

// 1inf29-lab4-guia.pdf, page 10
// the goal of this script is to show correct concurrent access
// to a shared variable using sync.Mutex

import (
	"fmt"
	"sync"
)

func worker() {
	defer wg.Done()
	for x := 0; x < 1000000; x++ {
		mu.Lock()
		count++
		mu.Unlock()
	}
}

var (
	count int
	wg    sync.WaitGroup
	mu    sync.Mutex
)

func main() {
	for x := 0; x < 5; x++ {
		wg.Add(1)
		go worker()
	}
	wg.Wait()
	fmt.Println("the expected count is 5 million, but the actual count is", count)
}
