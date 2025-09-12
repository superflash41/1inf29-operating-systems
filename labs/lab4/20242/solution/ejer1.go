package main

import (
	"fmt"
	"sync"
)

// Saymon N. - 20211866

var wg, wr, ws sync.WaitGroup

func worker1() {
	for x := 0; x < 10; x++ {
		fmt.Printf("I like to ")
        wr.Done()
        ws.Wait()
		fmt.Println("systems")
        ws.Add(1)
	}
	wg.Done()
}

func worker2() {
	for x := 0; x < 10; x++ {
        wr.Wait()
		fmt.Printf("study ")
		fmt.Printf("operating ")
        ws.Done()
        wr.Add(1)
	}
	wg.Done()
}

func main() {
	wg.Add(2)
    ws.Add(1)
    wr.Add(1)
	go worker1()
	go worker2()
	wg.Wait()
}
