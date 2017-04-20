CRect:
	cp CRect.h ../include/
	clang -c -g2 CRect.cpp -I../include
test:
	./final_make_lib.sh
	cp CRect.h ../include/
	clang -o test -g2 test.cpp -I../include -L../lib -lfig -lstdc++
