CRect:
	cp CRect.h include/
	clang -c -g2 CRect.cpp -I./include
test:
	cp CRect.h include/
	clang -o test -g2 test.cpp -I./include -L./lib -lfig -lstdc++
