#!/bin/bash
# Saymon N. - 20211866
while IFS= read -r line; do
	words=($line)
	first="${words[0]}"
	unset 'words[0]'
	words+=("$first")
	new_ln=$(IFS=' '; echo "${words[@]^}")
	echo $new_ln >> newdata
done < data
mv newdata data
