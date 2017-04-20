#!/bin/bash

cd ../CLine/
./make-lib.sh
mv ./tmp/CPoint.o ../CRect/tmp
mv ./tmp/CLine.o ../CRect/tmp
cd ../CRect
make -B CRect
mv ./CRect.o ./tmp
ar -rc libfig.a ./tmp/CRect.o ./tmp/CLine.o ./tmp/CPoint.o
ranlib libfig.a
mv libfig.a ../lib/
#rm CPoint.o
#rm CLine.o
#rm CRect.o
