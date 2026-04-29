package main

import (
	"fmt"
	"sync"
)

// sn

const N = 10

var (
	mu      sync.Mutex
	condT1  *sync.Cond
	condT2  *sync.Cond
	counter int = 0
)

func t1(id int) {
	for {
		mu.Lock()
		for counter >= N {
			condT1.Wait()
		}
		fmt.Printf("[%d] working on task T1-%d\n", id, counter)
		counter++
		if counter == N {
			condT2.Signal()
		}
		mu.Unlock()
	}
}

func t2() {
	for {
		mu.Lock()
		for counter < N {
			condT2.Wait() // wait for t1's to finish
		}
		counter = 0
		fmt.Println("working on task T2")
		condT1.Broadcast() // t1s can run again
		mu.Unlock()
	}
}

func main() {
	condT1 = sync.NewCond(&mu)
	condT2 = sync.NewCond(&mu)
	var K int
	fmt.Println("input a value for K:")
	fmt.Scanf("%d", &K)
	// let's go
	for x := 0; x < K; x++ {
		go t1(x)
	}
	go t2()
	select {}
}
