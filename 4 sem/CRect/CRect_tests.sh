#!/bin/bash

count=1
for i in $( ls tests/); do
	tstres="$(./test < tests/$i | gawk '/\(.*\)$/')"
	answer="$(gawk -v count=$count 'NR == count || NR == count + 1' testres)"
	if [ "$tstres" == "$answer" ]; then
		echo $i ran correctly
	else
		echo $i ran uncorrectly
		echo output:
		echo "$tstres"
		echo "$answer" '\t' '<-' supposed result
	fi;
	let count+=2
done
