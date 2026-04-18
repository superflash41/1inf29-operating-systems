package main

// 1inf29-lab4-guia.pdf, page 6
// the goal of this script is to show the use of unbuffered channels

import "fmt"

func main() {
	naturals := make(chan int)
	squares := make(chan int)

	// counter
	go func() {
		for x := range 20 {
			naturals <- x
		}
		close(naturals)
	}()

	// squarer
	go func() {
		for x := range naturals {
			squares <- x * x
		}
		close(squares)
	}()

	// printer (in main goroutine)
	for x := range squares {
		fmt.Println(x)
	}
}
