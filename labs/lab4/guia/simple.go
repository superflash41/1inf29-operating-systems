package main

// 1inf29-lab4-guia.pdf, page 3
// the goal of this script is to prove we cannot assume
// the order in which goroutines will execute

import (
	"fmt"
	"time"
)

func routine(n int) {
	fmt.Printf("i am goroutine %d\n", n)
}

func main() {
	for x := range 5 {
		go routine(x)
	}

	// this sleep is only to keep main alive long enough to see output.
	// we will later prefer synchronization tools like sync.WaitGroup
	time.Sleep(1 * time.Second) // REMEMBER THIS IS NOT A SYNCHRONIZATION TOOL
}
