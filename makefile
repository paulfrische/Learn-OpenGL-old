all:
	mkdir -p build
	g++ main.cpp util.cpp -o build/run -lGL -lGLEW -lglfw

run: all
	./build/run
