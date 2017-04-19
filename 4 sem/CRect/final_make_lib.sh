#!/bin/bash

cd CLine/
./make-lib.sh
mv ./objects/CPoint.o ../objects
mv ./objects/CLine.o ../objects
cd ../
make -B CRect
mv ./CRect.o ./objects
ar -rc libfig.a ./objects/CRect.o ./objects/CLine.o ./objects/CPoint.o
ranlib libfig.a
mv libfig.a lib/
#rm CPoint.o
#rm CLine.o
#rm CRect.o
