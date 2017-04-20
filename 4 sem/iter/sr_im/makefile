all:
	g++ -c image.cpp `wx-config --cflags` -O2 -s 
	ar -rc libimage.a image.o
	ranlib libimage.a
clear:
	rm *.o
