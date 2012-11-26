#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: <exec> <inputFile> <outputFile>"
fi

g++ -g -Wall maxPlanarSubset.cpp 
(/usr/bin/time ./a.out < $1 > tmp) >& $2
./format $1 tmp >> $2
rm -f tmp
