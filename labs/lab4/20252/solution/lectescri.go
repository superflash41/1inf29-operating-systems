package main

import (
	"fmt"
	"sync"
)

// sn

var (
	mu       sync.Mutex
	wg       sync.WaitGroup
	resource sync.Mutex
	writers  int = 0
	readers  int = 0
	shared   int = 0
)

func reader(id int) {
	defer wg.Done()
	for x := 0; x < 3; x++ {
		mu.Lock()
		if writers > 0 || readers == 0 {
			mu.Unlock()
			resource.Lock()
			mu.Lock()
		}
		readers++
		mu.Unlock()
		// read the resource
		fmt.Printf("[r%d] just reading from the shared resource: %d\n", id, shared)
		mu.Lock()
		readers--
		if readers == 0 {
			resource.Unlock()
		}
		mu.Unlock()
	}
}

func writer(id int) {
	defer wg.Done()
	for x := 0; x < 3; x++ {
		mu.Lock()
		writers++
		mu.Unlock()
		resource.Lock()
		// write on the resource
		shared += 10
		fmt.Printf("[w%d] writing on the shared resource: %d\n", id, shared)
		mu.Lock()
		writers--
		mu.Unlock()
		resource.Unlock()
	}
}

func main() {
	fmt.Println("3 readers and 2 writers:")
	wg.Add(5)
	go reader(1)
	go writer(1)
	go reader(2)
	go writer(2)
	go reader(3)
	wg.Wait()
	fmt.Println("process completed.")
}
