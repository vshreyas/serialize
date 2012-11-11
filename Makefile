all:
	g++ text_streamreader.cpp text_streamwriter.cpp test.cpp -o primitive_test.o --std=c++11 -g -Wextra

uninstall:
	rm primitive_test.o

