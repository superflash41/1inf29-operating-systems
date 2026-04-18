package main

// the goal of this script is to show how select works
// and how to use it with multiple channels for synchronization

import "fmt"

var (
	cmdCh  = make(chan string) // channel of all commands
	outCh  = make(chan int)    // channel for values to be printed
	doneCh = make(chan struct{})
)

func counter() {
	defer close(doneCh)
	defer close(outCh)

	value := 0
	for cmd := range cmdCh {
		switch cmd {
		case "inc":
			value++
		case "dec":
			value--
		case "print":
			outCh <- value // the value is sent to main to be printed
		default:
			// ignore unknown commands
		}
	}
}

func main() {
	go counter()

	commands := []string{
		"inc", "inc", "inc", "print", "dec", "print", "dec", "inc", "print",
	}

	i := 0
	closedCmd := false

	for {
		var (
			sendCh chan<- string
			cmd    string
		)
		if i < len(commands) && !closedCmd {
			sendCh = cmdCh
			cmd = commands[i]
		}
		if i >= len(commands) && !closedCmd {
			close(cmdCh)
			closedCmd = true
			sendCh = nil
		}

		select {
		case sendCh <- cmd:
			fmt.Println("cmd:", cmd)
			i++

		case v, ok := <-outCh:
			if !ok {
				<-doneCh
				fmt.Println("done: counter finished; outCh closed")
				return
			}
			fmt.Println("counter says:", v)
		}
	}
}
