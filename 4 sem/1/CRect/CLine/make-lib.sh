#!/bin/bash

cd ../../CPoint
make -B CPoint
mv CPoint.o ../CRect/CLine/objects
cd ../CRect/CLine/
make -B CLine
mv CLine.o ./objects
ar -rc libfig.a ./objects/CLine.o ./objects/CPoint.o
ranlib libfig.a
mv libfig.a lib/
