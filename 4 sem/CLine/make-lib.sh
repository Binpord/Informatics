#!/bin/bash

cd ../CPoint/
make -B CPoint
mv CPoint.o ../CLine/tmp/
cd ../CLine/
make -B CLine
mv CLine.o ./tmp/
ar -rc libfig.a ./tmp/CLine.o ./tmp/CPoint.o
ranlib libfig.a
# rm ./tmp/CLine.o ./tmp/CPoint.o
mv libfig.a ../lib/
