all:
	$(CXX) -std=c++14 boundingbox.cpp renderer.cpp main.cpp -o program.out -lglut -lGL -lm -lGLEW -DGL_GLEXT_PROTOTYPES

test:
	$(CXX) -std=c++14 boundingbox.cpp tests.cpp -o test.out

clean:
	rm program.out test.out
