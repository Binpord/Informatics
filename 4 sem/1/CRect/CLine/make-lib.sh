#!/bin/bash

make -B CLine
cd ../../CPoint
make -B CPoint
mv CPoint.o ../CRect/CLine/
cd ../CRect/CLine/
#ar -rc libfig.a CLine.o CPoint.o
#ranlib libfig.a
