package main

// sn

import "fmt"

var x int

func child() {
	fmt.Println("hijo")
	x--
}

func main() {
	x = 1
	fmt.Println("padre - inicio")
	go child()
	for x > 0 {
	}
	fmt.Println("padre - fin")
}
