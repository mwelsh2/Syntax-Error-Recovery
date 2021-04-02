# To Run
Type `make` into your command line to compile.
- To run example tests: type `make test` into your command line.
- To run your own tests: type `./parse < yourfile.txt`.

# Content
- parse.cpp
- scan.cpp
- scan.h
- ast.cpp
- ast.h 

# Features
- parse.cpp, scan.cpp, and scan.h modified from C code to C++.
- Program prints an AST on successful input, or a list of syntax errors along 
with the final transformed input.
- Method match inserts the expected token when encountering an error.
- Exception handlers are added to statement, condition, and expression which 
trigger a recover method if a syntax error occurs. Statement list also has a 
handler, though it triggers recovery as if it were in a statement subroutine.
- ast.cpp, ast.h define classes and methods for an abstract syntax tree. A node 
is represented by a string and a vector of children nodes. There are two 
derived classes from AST_Node (SL_Node and B_Node) that specify different 
printing methods for statement list and branch statement nodes.
- Method match and all subroutines in parse.cpp are modified to return AST
nodes to help build the syntax tree. 

# Limitations
- Make sure there are spaces between input characters. For example, `(A + B)` will
not work, but `( A + B )` will.
