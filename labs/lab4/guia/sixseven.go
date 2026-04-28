package main

import "fmt"

var (
	ch1 = make(chan struct{})
	ch2 = make(chan struct{})
)

func six() {
	for {
		<-ch1
		fmt.Print(6)
		ch2 <- struct{}{}
	}
}

func seven() {
	for {
		<-ch2
		fmt.Print(7)
		ch1 <- struct{}{}
	}
}

func main() {
	go six()
	go seven()
	ch1 <- struct{}{}
	select {} // blocked forever
}
