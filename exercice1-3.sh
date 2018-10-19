#!/bin/bash

FILENAME=exercice1-3

echo "Padding,Elapsed time"

for i in {1..2}
do
    rm -f $FILENAME
    make $FILENAME PADDING=$i &> /dev/null
    if [ $? -ne 0 ]; then
        >&2 echo "Compilation failed."
        exit 1
    fi
    echo -n "$i,"
    /usr/bin/time -f "%e" ./$FILENAME > /dev/null
done