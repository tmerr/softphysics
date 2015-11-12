all:
	$(CXX) -std=c++14 boundingbox.cpp camera.cpp renderer.cpp main.cpp -o program.out -lglut -lGL -lm -lGLEW -DGL_GLEXT_PROTOTYPES

test:
	$(CXX) -std=c++14 boundingbox.cpp camera.cpp renderer.cpp tests.cpp -o test.out -lglut -lGL -lm -lGLEW -DGL_GLEXT_PROTOTYPES

clean:
	rm program.out test.out
