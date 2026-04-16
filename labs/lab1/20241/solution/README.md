# how to test the scripts

## `script1.sh`

run the script:
```shell
./script1.sh 100 130 2
```

## `script2.sh`

move all the `.odt` files from [here](https://github.com/superflash41/1INF29/tree/main/labs/lab1/20241) to the same directory as the script and run it with:

```shell
./script2.sh _.txt
```
then check the new names:

```shell
ls 
```

## `script3.sh`

move the file named [`data`](https://github.com/superflash41/1INF29/tree/main/labs/lab1/20241/data) to the same directory as the script and run it with:

```shell
./script3.sh
```
then verify the changes:

```shell
cat data
```
