# notas

este lab trata de concurrencia en Go, por lo que lo más importante será evitar generar *race conditions*.

## concurrencia

la concurrencia es una **abstracción** para describir la ejecución de múltiples tareas al mismo tiempo. al igual que en el lab pasado, trabajaremos con hilos (pero esta vez en Go) llamados **gorutinas** (*goroutines* en inglés).

la clave será evitar que todas las gorutinas accedan a un mismo recurso al mismo tiempo y que logren sincronizarse correctamente en caso se requiera cierto orden de ejecución.


## gorutinas y canales

### 1 ejemplo simple

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

### 2 ejemplo simple con sincronización

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


### 3 ejemplo con canales

los canales son el **medio de comunicación entre gorutinas**. estos pueden ser **con búfer** (cuando pueden tener más de un elemento a la vez) o **sin búfer** (cuando solo aguantan un elemento).

lo chevere de los canales sin búfer es que se bloquean si intentas meterles más de un elemento a la vez, o si intentas sacar un elemento cuando no tienen nada. por esto, es que se pueden usar como herramientas de sincronización.

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

la moraleja es que si usas canales sin búfer, puedes sincronizar gorutinas sin necesidad de `sync.WaitGroup`, y puedes extraer uno a uno los elementos con un bucle `for range` **sin olvidar cerrar el canal** para que este termine.


### 4 satoru gojo is the strongest

sabiendo cómo usar canales sin búfer se pueden sincronizar gorutinas de forma interesante. por ejemplo, chequea el siguien código:

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

    time.Sleep(1 * time.Second) // recuerda que esto no sincroniza, solo es por el ejemplo
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

como se ve (y como se esperaba) el orden de ejecución es diferente cada vez. el objetivo será sincronizar las gorutinas para imprimir el mensaje en orden. en [gojo.go](./gojo.go) se muestra una alternativa de solu usando canales sin búfer. esta pregunta ha venido varias veces.

en la guía hay un ejemplo en la página 7 sobre cómo usar un canal con `struct{}` para demostrar que el tipo de dato del canal no importa. generalmente se trabaja con `struct{}`, `int` y `bool`.

en las páginas 7 y 8 se mencionan los canales con búfer. puedes considerarlos iguales a los canales que ya usamos, salvo que estos tienen mayor capacidad. igual se bloquean si intentas meterles más elementos de los que pueden aguantar, o si intentas sacar un elemento cuando no tienen nada.


### 5 bucle infinito con select

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

todos sabemos de funpro (y calint) las fórmulas para aproximar pi y e.

$$ \pi = 4 \sum_{k=0}^{\infty} \frac{(-1)^k}{2k+1} = 4 \left(1 - \frac{1}{3} + \frac{1}{5} - \frac{1}{7} + \cdots\right) $$

$$ e = \sum_{k=0}^{\infty} \frac{1}{k!} = 1 + \frac{1}{1} + \frac{1}{2} + \frac{1}{6} + \frac{1}{24} + \cdots $$


en este ejemplo, usamos gorutinas para calcular estos valores de modo que cada cierto tiempo envíen actualizaciones a `main` a través de canales. va así:

1. `main` crea 4 canales independientes y llama a cada gorutina con sus respectivos número de términos (`terms`), número de pasos entre cada actualización (`step`) y el canal correspondiente.

2. cada gorutina calcula aproximaciones (de pi o e) con un bucle y cuando llega a un múltiplo de `step` envía una actualización al canal. al finalizar, cada gorutina cierra su canal.

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

veamos otro problema (clásico en los labs) sobre sincronización de muchas gorutinas:

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


### 8 ejemplo con mutex

