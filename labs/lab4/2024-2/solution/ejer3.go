package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

// Saymon N. - 20211866

var (
	wg       sync.WaitGroup
	servings int      = 0 // in the pot
	empty    chan int = make(chan int)
	full     chan int = make(chan int)
	mutex    sync.Mutex
)

func cook() {
	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	for {
		<-empty
		putServingsInPot(5, r)
		full <- 1
	}
	wg.Done()
}

func putServingsInPot(n int, r *rand.Rand) {
	fmt.Printf("Put Servings %d In Pot\n", n)
	d := rand.Intn(10) // n will be between 0 and 10
	time.Sleep(time.Duration(d) * time.Millisecond)
}

func getServingFromPot(n int) {
	fmt.Printf("%d: Get serving from Pot\n", n)
}

func eat(n int) {
	fmt.Printf("%d: eating...\n", n)
}

func savage(n int) {
	r := rand.New(rand.NewSource(time.Now().UnixNano() + int64(n)))
	for {
		mutex.Lock()
		// verify if there are servings in the pot
		if servings == 0 {
			empty <- 1
			// wait til its full again
			<-full
			servings = 5 // refill
		}
		servings--
		getServingFromPot(n)
		mutex.Unlock()
		eat(n)
		d := r.Intn(10) // n will be between 0 and 10
		time.Sleep(time.Duration(d) * time.Millisecond)
	}
	wg.Done()
}

func main() {
	wg.Add(11)
	go cook()
	for x := 0; x < 10; x++ {
		go savage(x)
	}
	wg.Wait()
}

// note: sometimes the savages take some time to eat, while the cook
// is adding servings to the pot. this still makes the program correct
