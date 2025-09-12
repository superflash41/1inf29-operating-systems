#!/bin/bash
# Saymon N. - 20211866
while read -r LINE
do
    # notice the separator is a tab (\t)
    CODE=${LINE%%$'\t'*}
    STUDENT=${LINE#*$'\t'}
    # before and after the comma
    LASTNAME=${STUDENT%,*}
    NAME=${STUDENT#*,}
    # to lowercase
    LOWERNAME=${NAME,,}
    LOWLASTNAME=${LASTNAME,,}
    NEW=( $LOWERNAME )
    # mail construction
    DOM1=${NEW%% *}
    NEW="${NEW[@]^}"
    DOM2=${LOWLASTNAME%% *}
    STUDENT="${LASTNAME},${NEW}"
    MAIL="${DOM1}.${DOM2}@pucp.edu.pe"
    echo "$CODE:$STUDENT:$MAIL" >> Lista2.txt
done < "$1"
