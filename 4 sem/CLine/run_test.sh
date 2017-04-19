#!/bin/bash

count=1
for i in $( ls tests/); do
	tstres="$(./test < tests/$i | gawk '/^\(.*\)$/')"
	answer="$(gawk -v count=$count 'NR == count {printf $0}' testres.txt)"
	if [ "$tstres" == "$answer" ]; then
		echo test $count ran correctly
	else
		echo test $count ran uncorrectly
		echo output:
		echo "$tstres"
		echo "$answer" '\t' '<-' supposed result
	fi;
	let count+=1
done
