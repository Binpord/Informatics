#!/bin/bash

make CPoint
ar -rc libfig.a CPoint.o
ranlib libfig.a
