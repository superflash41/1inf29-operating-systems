// main.go
package main

import (
	"fmt"
	"sync"
)

func task(n int) {
	for x := 0; x < 50; x++ {
		fmt.Printf("%d", n)
	}
	wg.Done()
}

var wg sync.WaitGroup

func main() {

	for x := 0; x < 5; x++ {
		wg.Add(1)
		go task(x)
	}
	wg.Wait()
}
