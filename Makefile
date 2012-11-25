all:
	g++ common.cpp streamreader.cpp streamwriter.cpp text_streamreader.cpp text_streamwriter.cpp test_text.cpp -o xtest.o --std=c++11 -g -Wextra

cl:
	clang++ common.cpp streamreader.cpp streamwriter.cpp text_streamreader.cpp text_streamwriter.cpp test_text.cpp -o xtest.o --std=c++11 -g -Wextra

r:
	make all && ./xtest.o; cat out.txt

uninstall:
	rm xtest.o

