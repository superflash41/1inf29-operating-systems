#!/bin/bash
# Saymon N. - 20211866
for file in "$PWD"/*; do
	if [ -f "$file" ]; then
		fn=$(basename "$file")
		new_fn="${fn//$1}"
		if [ "$fn" != "$new_fn" ]; then
			mv "$file" "$PWD/$new_fn"
		fi
	fi
done
