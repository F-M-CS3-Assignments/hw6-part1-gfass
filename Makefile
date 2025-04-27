all: 
	g++ -std=c++11 -Wall -g RedBlackTree.cpp RedBlackTreeTests.cpp -o rbt-tests
	
run: 
	./rbt-tests

valgrind: 
	valgrind --leak-check=full ./rbt-tests

clean:
	rm -rf rbt-tests