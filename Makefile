all:
	g++ text_streamreader.cpp text_streamwriter.cpp test_text.cpp -o text_test.o --std=c++11 -g -Wextra

r:
	make all && ./text_test.o; cat out.txt

uninstall:
	rm text_test.o

