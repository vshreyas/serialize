all:
	g++ text_streamreader.cpp text_streamwriter.cpp test_text.cpp -o text_test.o --std=c++11 -g -Wextra

	g++ binarystreamreader.cpp test_binary.cpp -o binary_test.o --std=c++11 -g -Wextra


uninstall:
	rm test_text.o binary_test.o

