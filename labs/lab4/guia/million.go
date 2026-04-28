package main

// 1inf29-lab4-guia.pdf, page 9
// the goal of this script is to show concurrent access
// to a shared variable without synchronization tools

import (
	"fmt"
	"sync"
)

func worker() {
	defer wg.Done()
	for x := 0; x < 1000000; x++ {
		count++
	}
}

var count int
var wg sync.WaitGroup

func main() {
	for x := 0; x < 5; x++ {
		wg.Add(1)
		go worker()
	}
	wg.Wait()
	fmt.Println("the expected count is 5 million, but the actual count is", count)
}
