#!/bin/bash

make -B CRect
cd CLine/
./make-lib.sh
mv CPoint.o ../
mv CLine.o ../
cd ../
ar -rc libfig.a CRect.o CLine.o CPoint.o
ranlib libfig.a
mv libfig.a lib/
#rm CPoint.o
#rm CLine.o
#rm CRect.o
