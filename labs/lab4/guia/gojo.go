package main

// the goal of this script is to show the use of unbuffered channels
// for synchronization, without sync.WaitGroup

import "fmt"

var (
	a = make(chan bool)
	b = make(chan bool)
	c = make(chan bool)
)

func satoru() {
	fmt.Printf("satoru ")
	a <- true
}

func gojo() {
	<-a
	fmt.Printf("gojo ")
	b <- true
}

func is() {
	<-b
	fmt.Printf("is the strongest!\n")
	c <- true
}

func main() {
	go satoru()
	go gojo()
	go is()

	<-c
}
