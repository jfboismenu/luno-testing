
compile:
	mkdir -p build
	clang++ testing.cpp -std=c++17 -o build/testing

tests: compile
	build/testing

clean:
	rm -rf build
