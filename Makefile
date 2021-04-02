# Trivial makefile for the calculator scanner/parser.
# Depends on default (built-in) rules for C compilation.

# Note that rule for goal (parse) must be the first one in this file.
CXX = g++
CXXFLAGS = -Wall -g

parse: parse.o scan.o ast.o
	$(CXX) $(CXXFLAGS)  -o parse parse.o scan.o ast.o

clean:
	rm *.o parse

test:
	./parse < ex1.txt
	./parse < ex2.txt
	./parse < err1.txt

parse.o: ast.h
scan.o: scan.h
ast.o: ast.h scan.h