package main

import (
	"fmt"
	"sync"
)

// Saymon N. - 20211866

var (
	wg    sync.WaitGroup
	c     []chan int
	count int
	mu    sync.Mutex // for the count
)

func task(n int) {
	defer wg.Done()
	for x := 0; x < 50; x++ {
		<-c[n] // wait to keep printing
		fmt.Printf("%d", n)
		mu.Lock()
		count++
		newLine := (count % 5) == 0
		mu.Unlock()
		if newLine {
			fmt.Println() // endl
			for j := 0; j < 5; j++ {
				c[j] <- 1 // keep printing
			}
		}
	}
}

func main() {
	count = 0
	c = make([]chan int, 5)
	for i := range c {
		c[i] = make(chan int, 1)
	}
	for x := 0; x < 5; x++ {
		wg.Add(1)
		go task(x)
	}
	for j := 0; j < 5; j++ {
		c[j] <- 1
	}
	wg.Wait()
	for i := 0; i < 5; i++ {
		close(c[i])
	}
}
