CList_Debug:
	cp ./CList.h ../include/
	clang -g2 CList.cpp -c -I../include -D__DEBUG
	ar -rc libclass.a CList.o
	ranlib libclass.a
	rm CList.o
	mv libclass.a ../lib/

CList_Release:
	cp ./CList.h ../include/
	clang CLict.cpp -c -I../include

test:
	make -B CList_Debug
	clang -g2 test.cpp -o test -I../include -L../lib -lclass -lstdc++ -D__DEBUG
