LIB_FLAGS = -r
ifeq ("$(wildcard ../lib/libclass.a)", "")
	LIB_FLAGS += -c
endif

CList_Debug:
	cp ./CList.h ../include/
	clang -g2 CList.cpp -c -I../include -D__DEBUG
	ar $(LIB_FLAGS) ../lib/libclass.a CList.o
	ranlib ../lib/libclass.a
	rm CList.o

CList_Release:
	cp ./CList.h ../include/
	clang CLict.cpp -c -I../include
	ar $(LIB_FLAGS) ../lib/libclass.a CList.o
	ranlib ../lib/libclass.a
	rm CList.o

test:
	make -B CList_Debug
	clang -g2 test.cpp -o test -I../include -L../lib -lclass -lstdc++ -D__DEBUG


image:
	cp ./sr_im/image.h ../include/
	g++ -c ./sr_im/image.cpp -I../include -I./sr_im `wx-config --cflags` -std=c++11
	ar $(LIB_FLAGS) ../lib/libclass.a image.o
	ranlib ../lib/libclass.a
	rm image.o

RCList_Debug:
	cp ./RCList.h ../include/
	g++ -g2 RCList.cpp -c -I../include -I./sr_im `wx-config --cflags` -D__DEBUG -std=c++11
	ar $(LIB_FLAGS) ../lib/libclass.a RCList.o
	ranlib ../lib/libclass.a
	rm RCList.o

RCList_Release:
	cp ./RCList.h ../include/
	g++ RCList.cpp -c -I../include -I./sr_im `wx-config --cflags`
	ar $(LIB_FLAGS) ../lib/libclass.a RCList.o
	ranlib ../lib/libclass.a
	rm RCList.o

GetRects_Debug:
	make RCList_Debug
	make image
	g++ -o GetRects -g2 GetRects.cpp -I../include -L../lib -lclass `wx-config --cflags` `wx-config --libs` -lstdc++ -D__DEBUG -std=c++11
