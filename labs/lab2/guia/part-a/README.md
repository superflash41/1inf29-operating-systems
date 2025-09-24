# 1INF29 Laboratorio 2 - Guía Parte A

## Tarea 1

a) Para modificar el código se puede usar `sleep` en el padre o `waitpid`. De esta forma hay tiempo para que se muestre la salida del `pstree` ejecutado por el hijo.

b) Aparece el proceso `sh` porque al ejecutar `system(cmd)` se crea un proceso hijo que invoca al *shell* (`sh`) para ejecutar el comando `pstree`.

c) Para mostrar solo la rama del proceso hijo se usa `pstree` con el PID del hijo.

d) El código modificado se encuentra en [`fork_pstree.c`](./fork_pstree.c).

## Tarea 2

Los archivos modificados son [`chainp.c`](./chainp.c) y [`fanp.c`](./fanp.c).

## Tarea 3

Los archivos modificados son [`multifork.c`](./multifork.c) y [`isengfork.c`](./isengfork.c).

## Tarea 4

a) El código modificado del profesor se encuentra en [`btree.c`](./btree.c). Una forma alternativa de solución se muestra en [`binarytree.c`](./binarytree.c).

b) El programa para crear el árbol primero-en-profundidad es [`dftree.c`](./dftree.c).