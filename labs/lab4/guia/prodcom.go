package main

// 1inf29-lab4-guia.pdf, page 14
// the goal of this script is to show the producer-consumer problem using sync.Cond

import (
	"fmt"
	"sync"
)

var (
	buff      [5]int = [5]int{-1, -1, -1, -1, -1}
	index     int
	empty     int = 5
	wg        sync.WaitGroup
	mu        sync.Mutex
	condFull  *sync.Cond
	condEmpty *sync.Cond
)

func producer() {
	defer wg.Done()
	for n := range 20 {
		mu.Lock()
		for empty == 0 {
			condFull.Wait()
		}
		item := n * n
		index = n % 5
		buff[index] = item
		empty--
		fmt.Printf("produced %d at index %d - %v\n", item, index, buff)
		condEmpty.Signal()
		mu.Unlock()
	}
}

func consumer() {
	defer wg.Done()
	var item int
	for n := range 20 {
		mu.Lock()
		for empty == 5 {
			condEmpty.Wait()
		}
		index = n % 5
		item = buff[index]
		buff[index] = -1
		empty++
		fmt.Printf("consumed %d at index %d - %v\n", item, index, buff)
		condFull.Signal()
		mu.Unlock()
	}
}

func main() {
	condFull = sync.NewCond(&mu)
	condEmpty = sync.NewCond(&mu)
	wg.Add(2)
	go consumer()
	go producer()
	wg.Wait()
}
