all:
	$(CXX) -std=c++14 boundingbox.cpp main.cpp -o program.out

test:
	$(CXX) -std=c++14 boundingbox.cpp tests.cpp -o test.out

clean:
	rm program.out test.out
