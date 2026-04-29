package main

// sn

import "fmt"

var c = make(chan struct{})

func child() {
	fmt.Println("hijo")
	c <- struct{}{}
}

func main() {
	fmt.Println("padre - inicio")
	go child()
	<-c
	fmt.Println("padre - fin")
}
