all:
	g++ common.cpp streamreader.cpp streamwriter.cpp text_streamreader.cpp text_streamwriter.cpp test_text.cpp -o test_text.o --std=c++11 -g -Wextra

r:
	make all && ./test_text.o; cat out.txt

uninstall:
	rm test_text.o

