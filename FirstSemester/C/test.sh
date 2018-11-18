#!/bin/bash
g++ -Wall -std=c++14 -O0 "./clock.cpp" -o "./clock"
g++ -Wall -std=c++14 -O0 "./generate.cpp" -o "./generate"

for i in `seq 4 7`;
do
	echo "Testing for 10^"$i
	./generate "output"$1 $((10**$i))
	./clock "output"$1 $((10**$i))
	echo
done