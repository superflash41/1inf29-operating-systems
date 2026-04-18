# Concurrency
## Concurrency vs Parallelism
> Concurrency is about *dealing* with lots of things at once.
>
> Parallelism is about *doing* lots of things at once.
> 
> \- Rob Pike (creator of Go)

## Go
also referred to as Golang, it is a expressive, concise, clean, and efficient programming language.

### hello, world!
in Go, every program is part of a package, and the `main` package serves as the entry point for Go applications.

we define a simple program that outputs `hello, world!`.

```go
package main
import "fmt"

func main() {
    fmt.Println("hello, world!")
}
```

## concurrency in Go
we will use Go in this lab due to its **concurrency mechanisms**, which are among its most notable features.

### goroutines
goroutines are **lightweight threads** managed by the Go runtime. they are used to **execute functions concurrently**.

```go
go func() {
	fmt.Println("this is running in a goroutine")
}()
```

### channels
channels are tools for **communicating between goroutines**. they can be **buffered** or **unbuffered**, which means they can allow the passing of one or more than one value at a time through a channel.

```go
ch := make(chan int)

go func() {
	ch <- 41 // sends the value to the channel
}()
value := <- ch // receives the value from the channel
fmt.Println(value)
```

### mutexes
mutexes are **synchronization primitives** used in concurrent programming to ensure that multiple threads or **goroutines do not simultaneously access a shared resource**.

```go
package main
import (
	"fmt"
	"sync"
)

func routine(n int) {
	defer wg.Done()
	fmt.Printf("i am goroutine %d\n", n)
}

var wg sync.WaitGroup

func main() {
	for x := 0; x < 5; x++ {
		wg.Add(1)
		go routine(x)
	}
	wg.Wait()
}
```

---
# References
- [Go Official Documentation](https://go.dev/doc/)
- [Laboratory Guide](./guia/1inf29-lab4-guia.pdf)