package main

// the goal of this script is to show the use of channels
// with select to multiplex multiple workers that send updates over time

import "fmt"

// goroutine that streams approximations of pi over time
func pi(terms int, step int, out chan<- float64) {
	defer close(out)

	sum := 0.0
	for k := 0; k < terms; k++ { // leibniz formula for pi
		term := 1.0 / float64(2*k+1)
		if k%2 == 1 {
			sum -= term
		} else {
			sum += term
		}

		if (k+1)%step == 0 {
			out <- 4.0 * sum
		}
	}
}

// goroutine that streams approximations of Euler's number e over time
func e(terms int, step int, out chan<- float64) {
	defer close(out)

	sum := 0.0
	fact := 1.0
	for k := 0; k < terms; k++ { // taylor series for e
		if k > 0 {
			fact *= float64(k)
		}
		sum += 1.0 / fact

		if (k+1)%step == 0 {
			out <- sum
		}
	}
}

func main() {
	pi1 := make(chan float64)
	pi2 := make(chan float64)
	e1 := make(chan float64)
	e2 := make(chan float64)

	go pi(6_000_000, 3_000_000, pi1)
	go pi(8_000_000, 2_000_000, pi2)
	go e(4_000_000, 1_000_000, e1)
	go e(10_000_000, 2_000_000, e2)

	open := 4
	for open > 0 {
		select {
		case v, ok := <-pi1:
			if !ok {
				pi1 = nil
				open--
				fmt.Println("> pi1 finished")
				continue
			}
			fmt.Printf("pi1 update: %.10f\n", v)
		case v, ok := <-pi2:
			if !ok {
				pi2 = nil
				open--
				fmt.Println("> pi2 finished")
				continue
			}
			fmt.Printf("pi2 update: %.10f\n", v)
		case v, ok := <-e1:
			if !ok {
				e1 = nil
				open--
				fmt.Println("> e1 finished")
				continue
			}
			fmt.Printf("e1  update: %.10f\n", v)
		case v, ok := <-e2:
			if !ok {
				e2 = nil
				open--
				fmt.Println("> e2 finished")
				continue
			}
			fmt.Printf("e2  update: %.10f\n", v)
		}
	}

	fmt.Println("all operations finished.")
}
