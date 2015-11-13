cpps = boundingbox.cpp camera.cpp renderer.cpp
glstuff = -lGL -lGLEW -DGL_GLEXT_PROTOTYPES -lglfw

all:
	$(CXX) -std=c++14 $(cpps) main.cpp -o program.out $(glstuff)
test:
	$(CXX) -std=c++14 -g -I. $(cpps) tests/test.cpp -o test.out $(glstuff)

clean:
	rm program.out test.out
