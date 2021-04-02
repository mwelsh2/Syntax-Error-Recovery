#include <iostream>
#include "ast.h"

// Constructors for terminal and nonterminal
Terminal::Terminal(token c) { this->c = c; };
NonTerminal::NonTerminal(nonterminal X) { this->X = X; };

// Intializes empty symbol
Symbol::Symbol() {
    this->c = t_null;
    this->X = nt_null;
    this->e = e_null;
}

// Initializes symbol as terminal
Symbol::Symbol(token c) {
    this->c = Terminal(c);
    this->X = nt_null;
    this->e = e_null;
};

// Initializes symbol as nonterminal
Symbol::Symbol(nonterminal X) {
    this->c = t_null;
    this->X = NonTerminal(X);
    this->e = e_null;
};

// Initializes symbol as epsilon
Symbol::Symbol(EPSILON e) {
    this->c = t_null;
    this->X = nt_null;
    this->e = EPS;
};

// Returns true if symbol is terminal
bool Symbol::isTerminal() {
    return this->c.c != t_null;
};

// Returns true if symbol is nonterminal
bool Symbol::isNonTerminal() {
    return this->X.X != nt_null;
};

// Returns true if symbol is epsilon
bool Symbol::isEpsilon() {
    return this->e == EPS;
};

/* AST node constructor */
AST_Node::AST_Node(string terminal) {
    this->terminal = terminal;
    this->children = vector <AST_Node*> ();
};
/* Constructor for one child */
AST_Node::AST_Node(AST_Node* p, AST_Node* c) {
    this->terminal = p->terminal;
    this->children = vector <AST_Node*> ();
    children.push_back(c);
}

/* Constructor for two children */
AST_Node::AST_Node(AST_Node* p, AST_Node* l, AST_Node* r) {
    this->terminal = p->terminal;
    this->children = vector <AST_Node*> ();
    children.push_back(l);
    children.push_back(r);
}

/* Constructor for three children */
AST_Node::AST_Node(AST_Node* p, AST_Node* l, AST_Node* m, AST_Node* r) {
    this->terminal = p->terminal;
    this->children = vector <AST_Node*> ();
    children.push_back(l);
    children.push_back(m);
    children.push_back(r);
}

/* Prints the given node */
void AST_Node::printAST_Node(int indent) {
    if (children.empty()) {
        cout << terminal;
    } else {  
        cout << "(" << terminal << " ";
        int n = children.size();
        if (n > 1) {
            for (int i = 0; i < n - 1; i++) {
                children.at(i)->printAST_Node(indent);
                cout << " ";
            }
        }
    if (n > 0) {
        children.back()->printAST_Node(indent);
    }
    cout << ")";
    }
}

/* Prints a statement list node */
void SL_Node::printAST_Node(int indent) {
    indent += 2;
    int n = children.size();
    cout << "\n";
    printIndent(indent);
    cout << "[ ";
    children.front()->printAST_Node(indent);
    if (n > 1) {
        for (int i = 1; i < n; i++) {
            cout << "\n";
            printIndent(indent+2);
            children.at(i)->printAST_Node(indent); 
        }
    }
    cout << "\n";
    printIndent(indent);
    cout << "]\n";
}

/* Prints a branch statement node */
void B_Node::printAST_Node(int indent) {
    indent += 2;
    cout << "(" << terminal << "\n";
    for (int i = 0; i < int(children.size()) ; i++) {
        printIndent(indent+2);
        children.at(i)->printAST_Node(indent);
    }
    printIndent(indent);
    cout << ")";
}

/* Prints a given number of indents */
void AST_Node::printIndent(int indent) {
    for (int i = 0; i < indent; i++) {
        cout << " ";
    }
}