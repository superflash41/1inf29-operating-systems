# notas

este lab trata de concurrencia en Go, por lo que lo más importante será evitar generar *race conditions*.

## concurrencia

la concurrencia es una **abstracción** para describir la ejecución de múltiples tareas al mismo tiempo. al igual que en el lab pasado, trabajaremos con hilos (pero esta vez en Go) llamados **gorutinas** (*goroutines* en inglés).

la clave será evitar que todas las gorutinas accedan a un mismo recurso al mismo tiempo y que logren sincronizarse correctamente en caso se requiera seguir cierto orden de ejecución.


## gorutinas y canales

### 1. ejemplo simple

empezamos con el ejemplo de la página 3 ([simple.go](./simple.go)).

```go
package main

// 1inf29-lab4-guia.pdf, page 3
// the goal of this script is to prove we cannot assume
// the order in which goroutines will execute

import (
	"fmt"
	"time"
)

func routine(n int) {
	fmt.Printf("i am goroutine %d\n", n)
}

func main() {
	for x := range 5 {
		go routine(x)
	}

	// this sleep is only to keep main alive long enough to see output.
	// we will later prefer synchronization tools like sync.WaitGroup
	time.Sleep(1 * time.Second) // REMEMBER THIS IS NOT A SYNCHRONIZATION TOOL
}
```

primero, la gorutina principal (main) lanza 5 gorutinas con un bucle (usar `range` es lo mismo que usar `for i := 0; i < 5; i++`), y luego cada una se vuelve independiente e imprime su número. el resultado es que **no se puede asumir el orden en que se ejecutarán**. cada vez que se corre el programa con `go run simple.go` se obtiene un orden diferente.

> es importante notar que `time.Sleep` no es una herramienta de sincronización. **evita usarla en el lab.**

### 2. ejemplo simple con sincronización

adaptamos el ejemplo anterior para usar `sync.WaitGroup` en lugar del `time.Sleep` ([simple2.go](./simple2.go)).

```go
package main

// 1inf29-lab4-guia.pdf, page 4
// the goal of this script is to show how to correctly
// wait for goroutines to finish using sync.WaitGroup

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
	for x := range 5 {
		wg.Add(1)
		go routine(x)
	}

	// we correctly wait for all goroutines to finish before exiting main
	wg.Wait()
}
```

el flujo en este ejemplo es similar, pero ahora se usa la variable global `wg` para sincronizar la ejecución. un `WaitGroup` es un **contador de gorutinas**. por ello, antes de lanzar cada una aumentamos en 1 el contador con `wg.Add(1)`, y al finalizar cada gorutina se llama a `wg.Done()` para disminuir la cuenta. `wg.Wait()` **bloquea la ejecución** de `main` hasta que el contador llegue a 0.

entonces, puedes usar el `WaitGroup` como un punto de parada cuando debes esperar que muchas tareas independientes terminen antes de continuar con el programa.


### 3. ejemplo con canales

los canales son el **medio de comunicación entre gorutinas**. estos pueden ser **con búfer** (cuando pueden tener más de un elemento a la vez) o **sin búfer** (cuando solo aguantan un elemento).

lo chévere de los canales sin búfer es que se bloquean si intentas meterles más de un elemento a la vez, o si intentas sacar un elemento cuando no tienen nada. por esto, es que se pueden usar como herramientas de sincronización.

en el ejemplo de la página 6 ([squares.go](./squares.go)) se muestra el uso de estos canales:

```go
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
```

en este ejemplo se usan 2 canales sin búfer y las gorutinas  `counter` y ` squarer` son creadas con funciones anónimas (es otra forma de definir gorutinas en Go).

veamos el orden de ejecución:
1. `counter` genera los números naturales del 0 al 19 iterando con `range` (recuerda que es lo mismo que `for i := 0; i < 20; i++`) y los envía al canal `naturals`. ahora, como el canal es sin búfer, `counter` se bloquea apenas se envía el primer número, por lo que el bucle se queda en *pausa*.

2. `squarer` es la otra gorutina, y en este caso `range` actúa diferente porque la iteración la hace sobre un canal. `range` aquí actúa como un bucle infinito que va a iterar hasta que el canal `naturals` se cierre. así, al inicio no encuentra ningún número en el canal y se bloquea esperando a que `counter` le envíe un número. cuando `counter` envía el primer número, `squarer` lo saca del canal, lo eleva al cuadrado y lo envía al canal `squares`. 

3. con el canal `naturals` liberado, `counter` puede enviar el siguiente número. además, `main` recibe el número al cuadrado del canal `squares` y lo desbloquea. esto permite que `squarer` envíe el siguiente número al canal `squares`, y así sucesivamente.

4. el primero en terminar es `counter`, quien con `close(naturals)` permite que el bucle de `squarer` termine, lo que a su vez hace que `squarer` cierre el canal `squares` y permita al bucle de `main` terminar.

la moraleja es que si usas canales sin búfer, puedes sincronizar gorutinas sin necesidad de `sync.WaitGroup`, y puedes iterar sobre los canales con `for range`  **sin olvidar cerrarlos**.


### 4. satoru gojo is the strongest

sabiendo cómo usar canales sin búfer se pueden sincronizar gorutinas de forma interesante. chequea el siguien ejemplo:

```go
package main

import (
    "fmt"
    "time"
)

func satoru() {
	fmt.Printf("satoru ")
}

func gojo() {
	fmt.Printf("gojo ")
}

func is() {
	fmt.Printf("is the strongest!\n")
}

func main() {
	go satoru()
	go gojo()
	go is()

    time.Sleep(1 * time.Second) // recuerda que esto no sincroniza, solo es para el ejemplo
}
```

estos son sus resultados de ejecución:

```bash
❯ go run -race gojo.go 
gojo is the strongest!
satoru

❯ go run -race gojo.go
is the strongest!
satoru gojo

❯ go run -race gojo.go
is the strongest!
gojo satoru
```

como se ve (y como se esperaba) el orden de ejecución es diferente cada vez. el objetivo será sincronizar las gorutinas para imprimir el mensaje en orden. en [gojo.go](./gojo.go) se muestra una alternativa de solu usando canales sin búfer.

en la guía hay un ejemplo en la página 7 sobre cómo usar un canal con `struct{}` para demostrar que el tipo de dato del canal no importa. generalmente se trabaja con `struct{}`, `int` o `bool`.

en las páginas 7 y 8 de la guía se mencionan los canales con búfer. puedes considerarlos iguales a los canales que ya usamos, salvo que estos tienen mayor capacidad. igual se bloquean si intentas meterles más elementos de los que pueden aguantar, o si intentas sacar un elemento cuando no tienen nada.


### 5. bucle infinito con select

en la página 8 también se menciona el uso del `select`. en [math.go](./math.go) se muestra un ejemplo:

```go
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
```

todos sabemos de funpro (y calint) las fórmulas para aproximar $\pi$ y $e$.

$$ \pi = 4 \sum_{k=0}^{\infty} \frac{(-1)^k}{2k+1} = 4 \left(1 - \frac{1}{3} + \frac{1}{5} - \frac{1}{7} + \cdots\right) $$

$$ e = \sum_{k=0}^{\infty} \frac{1}{k!} = 1 + \frac{1}{1} + \frac{1}{2} + \frac{1}{6} + \frac{1}{24} + \cdots $$


en este ejemplo, usamos gorutinas para calcular estos valores de modo que cada cierto tiempo envíen actualizaciones a `main` a través de canales. va así:

1. `main` crea 4 canales independientes y llama a cada gorutina con sus respectivos número de términos (`terms`), número de pasos entre cada actualización (`step`) y el canal correspondiente.

2. cada gorutina calcula aproximaciones (de $\pi$ o $e$) con un bucle y cuando llega a un múltiplo de `step` envía una actualización al canal. al finalizar, cada gorutina cierra su canal.

3. lo que más nos interesa es saber cómo `main` usa el `select` para manejar las respuestas.

    > el `select` es una estructura que permite manejar muchas condiciones (como un `switch`). si ninguna condición se cumple, el `select` se bloquea hasta que alguna se cumpla, y si muchas se cumplen al mismo tiempo se selecciona una al azar.

    el `for open > 4` es un bucle infinito que itera mientras el valor de `open` sea mayor a 0, y el `select` tiene 4 casos. cada caso intenta leer un valor del canal correspondiente.

    > para leer de los canales se usa la sintaxis `v, ok := <-ch`, donde `v` es el elemento y `ok` es un booleano que indica si el canal sigue abierto o si ya se cerró. antes solo sacábamos elementos con `v := <-ch`, pero ahora aprovechamos esta ventaja del lenguaje Go para detectar el cierre de los canales.

    entonces, como podemos saber si un canal está cerrado solo nos queda checkear con `if !ok { ... }` y en ese caso poner el canal a `nil` para que no se vuelva a seleccionar. solo cuando un canal ha sido cerrado, su `case` imprime un mensaje de finalización y disminuye el contador `open`. cuando todos terminan `open` llega a 0 y el bucle termina.

aquí un resultado de ejecución:

```bash
❯ go run -race math.go
e1  update: 2.7182818285
pi2 update: 3.1415921536
e2  update: 2.7182818285
e1  update: 2.7182818285
pi1 update: 3.1415923203
e1  update: 2.7182818285
pi2 update: 3.1415924036
e2  update: 2.7182818285
e1  update: 2.7182818285
> e1 finished
pi2 update: 3.1415924869
pi1 update: 3.1415924869
> pi1 finished
e2  update: 2.7182818285
pi2 update: 3.1415925286
> pi2 finished
e2  update: 2.7182818285
e2  update: 2.7182818285
> e2 finished
all operations finished.
```

### 67

veamos otro problema sobre sincronización de muchas gorutinas:

```go
package main

import (
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func six() {
	defer wg.Done()
	for {
		fmt.Print(6)
	}
}

func seven() {
	defer wg.Done()
	for {
		fmt.Print(7)
	}
}

func main() {
	wg.Add(2)
	go six()
	go seven()
	wg.Wait()
	fmt.Printf("will i ever get here?\n")
}
```

este script genera la siguiente salida:

```bash
❯ go run -race sixseven.go
666666767766666677777766666677...
```

el objetivo es hacer que la salida sea algo como `6767676767...` (alternando entre 6 y 7). un ejemplo de solu se encuentra en [sixseven.go](./sixseven.go) usando canales sin búfer.


## mutexes y variables de condición

### 8. ejemplo con mutex

en la página 9 de la guía se muestra un ejemplo de un problema que puede occurir si no se coordina el acceso a un recurso compartido. el código se encuentra en [million.go](./million.go):

```go
package main

// 1inf29-lab4-guia.pdf, page 9
// the goal of this script is to show concurrent access
// to a shared variable without synchronization tools

import (
	"fmt"
	"sync"
)

func worker() {
	defer wg.Done()
	for x := 0; x < 1000000; x++ {
		count++
	}
}

var count int
var wg sync.WaitGroup

func main() {
	for x := 0; x < 5; x++ {
		wg.Add(1)
		go worker()
	}
	wg.Wait()
	fmt.Println("the expected count is 5 million, but the actual count is", count)
}
```

este simple ejemplo crea 5 gorutinas para incrementar un contador compartido de modo que el resultado esperado sea 5 millones. sin embargo, al ejecutar el programa se observa que el resultado no es el esperado.

```bash
❯ go run million.go 
the expected count is 5 million, but the actual count is 1027472

❯ go run million.go
the expected count is 5 million, but the actual count is 1035164

❯ go run million.go
the expected count is 5 million, but the actual count is 1032443
```

es más, al ejecutar el programa con la bandera `-race` se detecta una condición de carrera:

```bash
❯ go run -race million.go
==================
WARNING: DATA RACE
Read at 0x000100813670 by goroutine 8:
  main.worker()
      /Users/sn/Documents/repo/p/1inf29-operating-systems/labs/lab4/guia/million.go:11 +0x84

Previous write at 0x000100813670 by goroutine 7:
  main.worker()
      /Users/sn/Documents/repo/p/1inf29-operating-systems/labs/lab4/guia/million.go:11 +0x9c

Goroutine 8 (running) created at:
  main.main()
      /Users/sn/Documents/repo/p/1inf29-operating-systems/labs/lab4/guia/million.go:21 +0x44

Goroutine 7 (running) created at:
  main.main()
      /Users/sn/Documents/repo/p/1inf29-operating-systems/labs/lab4/guia/million.go:21 +0x44
==================
==================
WARNING: DATA RACE
Write at 0x000100813670 by goroutine 8:
  main.worker()
      /Users/sn/Documents/repo/p/1inf29-operating-systems/labs/lab4/guia/million.go:11 +0x9c

Previous write at 0x000100813670 by goroutine 7:
  main.worker()
      /Users/sn/Documents/repo/p/1inf29-operating-systems/labs/lab4/guia/million.go:11 +0x9c

Goroutine 8 (running) created at:
  main.main()
      /Users/sn/Documents/repo/p/1inf29-operating-systems/labs/lab4/guia/million.go:21 +0x44

Goroutine 7 (running) created at:
  main.main()
      /Users/sn/Documents/repo/p/1inf29-operating-systems/labs/lab4/guia/million.go:21 +0x44
==================
the expected count is 5 million, but the actual count is 4498783
Found 2 data race(s)
exit status 66
```

el problema es que el acceso a la variable `count` no se encuentra restringido a solo una gorutina a la vez. imagina el siguiente escenario que se puede dar dado que la ejecución es concurrente:
1. la gorutina A lee el valor de `count` cuando es 100.
2. la gorutina B lee el mismo valor de `count` (100).
3. la gorutina A incrementa el valor a 101
4. la gorutina B (que no sabe de ese cambio) incrementa el valor a 101 también.
5. ambas gorutinas escriben el mismo valor (101) en `count`, perdiendo una de las actualizaciones.

este proceso se repite muchas veces, resultando en un valor final de `count` menor a 5 millones. para solucionar este problema, se usa un `sync.Mutex` como en [million2.go](./million2.go):

```go
package main

// 1inf29-lab4-guia.pdf, page 10
// the goal of this script is to show correct concurrent access
// to a shared variable using sync.Mutex

import (
	"fmt"
	"sync"
)

func worker() {
	defer wg.Done()
	for x := 0; x < 1000000; x++ {
		mu.Lock()
		count++
		mu.Unlock()
	}
}

var (
	count int
	wg    sync.WaitGroup
	mu    sync.Mutex
)

func main() {
	for x := 0; x < 5; x++ {
		wg.Add(1)
		go worker()
	}
	wg.Wait()
	fmt.Println("the expected count is 5 million, but the actual count is", count)
}
```

`sync.Mutex` actúa como un candado. solo permite que una gorutina a la vez acceda al valor de `count`. así se soluciona el problema de *race condition*, y al ejecutar el programa se obtiene el resultado esperado:

```bash
❯ go run -race million2.go 
the expected count is 5 million, but the actual count is 5000000
```

### 9. variables de condición

en esta subsección menciono un tipo de variable de sincronización que no se encuentra en la guía (`sync.Cond`).

> imagina que tú y tus amigos comparten un buzón de correo. el mutex es la llave del buzón y  `mu.Lock()` asegura que solo una persona pueda revisar el buzón a la vez.
> pero, ¿qué pasa si tú andas esperando un paquete en específico? un día vas, abres el buzón con la llave y tu paquete no está.
> sin una condición de variable, tendrías que cerrar el buzón, esperar unas horas y volver a revisar, lo que es ineficiente porque nada te garantiza que el paquete estará ahí la próxima vez que revises.
> la condición de variable actuaría así: si la primera vez que tuviste acceso tu paquete no estaba, entonces puedes irte a dormir y esperar a que el cartero te avise apenas tu paquete llegue, de modo que de forma inmediata puedas recuperar el acceso al buzón y sacar tu paquete.

esta analogía es medio forzada, pero la idea es que `sync.Cond` le permite a una gorutina esperar (de manera eficiente) a que le avisen que cierta condición se haya cumplido, y por lo tanto, pueda recuperar el acceso exclusivo al recurso compartido.

ahora muestro la solu al ejercicio de la página 14, el cual es un problema de productor-consumidor. el código se encuentra en [prodcom.go](./prodcom.go) y se resuelve usando `sync.Cond`:

```go
package main

// 1inf29-lab4-guia.pdf, page 14
// the goal of this script is to show the producer-consumer problem using sync.Cond

import (
	"fmt"
	"sync"
)

var (
	buff      [5]int = [5]int{-1, -1, -1, -1, -1}
	index     int
	empty     int = 5
	wg        sync.WaitGroup
	mu        sync.Mutex
	condFull  *sync.Cond
	condEmpty *sync.Cond
)

func producer() {
	defer wg.Done()
	for n := range 20 {
		mu.Lock()
		for empty == 0 {
			condFull.Wait()
		}
		item := n * n
		index = n % 5
		buff[index] = item
		empty--
		fmt.Printf("produced %d at index %d - %v\n", item, index, buff)
		condEmpty.Signal()
		mu.Unlock()
	}
}

func consumer() {
	defer wg.Done()
	var item int
	for n := range 20 {
		mu.Lock()
		for empty == 5 {
			condEmpty.Wait()
		}
		index = n % 5
		item = buff[index]
		buff[index] = -1
		empty++
		fmt.Printf("consumed %d at index %d - %v\n", item, index, buff)
		condFull.Signal()
		mu.Unlock()
	}
}

func main() {
	condFull = sync.NewCond(&mu)
	condEmpty = sync.NewCond(&mu)
	wg.Add(2)
	go consumer()
	go producer()
	wg.Wait()
}
```

se usan dos variables de condición: una para manejar el caso en que el arreglo se encuentre lleno y el productor deba esperar, y otra para cuando esté vacío y el consumidor deba esperar. además, la sección crítica siempre se encuentra protegida por el mismo mutex.

### 10. 20252 - pregunta 1a

continuando con el tema de variables de condición, incluyo aquí la [respuesta](./../20252/solution/sincrovar.go) a la pregunta 1a del laboratorio del 2025-2:

```go
package main

import (
	"fmt"
	"sync"
)

var (
	x    int
	mu   sync.Mutex
	cond *sync.Cond
)

func child() {
	mu.Lock()
	defer mu.Unlock()
	fmt.Println("hijo")
	x++
	cond.Signal()
}

func main() {
	x = 0
	cond = sync.NewCond(&mu)
	mu.Lock()
	defer mu.Unlock()
	fmt.Println("padre - inicio")
	go child()
	for x == 0 {
		cond.Wait()
	}
	fmt.Println("padre - fin")
}
```

se sincronizan las impresiones únicamente con variables de condición (`sync.Cond` y `sync.Mutex`).

la idea de solución es la siguiente:
1. se usa la variable `x` para indicar si el mensaje del hijo ya ha sido impreso o no (sugerencia del enunciado).
2. el padre bloquea el mutex y corre la gorutina del hijo, quien se queda bloqueado esperando que el mutex se desbloquee.
3. el padre entra a su bucle de espera y como `x` sigue siendo 0, se bloquea con `cond.Wait()`, lo que a su vez desbloquea el mutex y permite que el hijo ejecute su código.
4. el hijo imprime su mensaje, incrementa `x` a 1 y llama a `cond.Signal()` para desbloquear al padre.
5. el padre continúa su ejecución, imprime su mensaje y termina el programa.

> notas sobre el uso de mutex:
> - bad: si uno intenta desbloquear un mutex que no ha sido bloqueado, el *runtime* de Go lanza un `panic: sync: unlock of unlocked mutex`. este es un error fatal de programación y demuestra un error en la lógica de sincronización.
> - good: si uno intenta bloquear un mutex que ya ha sido bloqueado por otra gorutina, solamente la gorutina se bloquea y espera. este es el comportamiento esperado y es el propósito de los mutex.
> - bad: si uno intenta bloquear un mutex que ya ha sido bloqueado por la **misma** gorutina, el programa se bloquea indefinidamente (deadlock), pues se queda esperando a ser desbloqueado por sí mismo. este error comúnmente genera el mensaje `fatal error: all goroutines are asleep - deadlock!`.