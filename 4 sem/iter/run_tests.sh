#!/bin/bash

for i in $( ls tests/); do
	./test < tests/$i
done
