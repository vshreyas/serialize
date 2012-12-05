all:
	g++ test_text.cpp -o xtest.o --std=c++11 -g -Wextra

cl:
	clang++ test_text.cpp -o xtest.o --std=c++11 -g -Wextra	

r:
	make all && ./xtest.o; cat out.txt

uninstall:
	rm xtest.o

