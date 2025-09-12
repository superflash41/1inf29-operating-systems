# compile
```sh
gcc -o sv sv.c
gcc -o cl cl.c
```
# send the output to b
```sh
./sv > b &
```
# save the code to a
```sh
cat *.c > a
```
# run the client with a as input
```sh
./cl < a
```
# kill the background execution of the sv
```sh
kill %1
```
# check for differences
```sh
diff a b
```
# no output as both files have the same content
