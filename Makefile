all: src/object.hpp src/eval.hpp src/builtin.hpp src/parser.hpp src/main.cpp
	g++ -Wall -Wextra -Wpedantic -std=c++17 -O2 src/main.cpp -o smallisp
