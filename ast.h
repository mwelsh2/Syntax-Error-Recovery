#include <vector>
#include <string>
#include "scan.h"

using namespace std;

class Terminal {
    public:
        token c;
        Terminal(token c = t_null);
        bool operator==(Terminal& other) const
         {
             if(this->c == other.c) return true;
             else return false;
         };
};


class NonTerminal {
    public:
        nonterminal X;
        NonTerminal(nonterminal X = nt_null);
        bool operator==(NonTerminal& other) const
         {
             if(this->X == other.X) return true;
             else return false;
         };

};

// A symbol can be a terminal, nonterminal, or epsilon
class Symbol {
    public:
        // Default is null, unless symbol is of given type
        Terminal c;
        NonTerminal X;
        EPSILON e;

        // Constructors
        Symbol();
        Symbol(token c); 
        Symbol(nonterminal X);
        Symbol(EPSILON e);

        // Functions
        bool isTerminal(); // returns true if symbol is terminal
        bool isNonTerminal(); // returns true if symbol is nonterminal
        bool isEpsilon(); // returns true if symbol is epsilon

        bool operator==(Symbol& other) const
         {
             if(this->c == other.c && this->X == other.X
             && this->e == other.e) return true;
             else return false;
         };
};

/* Node for an abstract syntax tree */
class AST_Node {
    public:
        string terminal; // what will actually be printed
        vector <AST_Node*> children; // list of children nodes

        /* Constructor for a single node with no children */
        AST_Node(string terminal = "");
        /* Constructors take the first parameter as the parent node, the rest are added as children */
        AST_Node(AST_Node* p, AST_Node* c);
        AST_Node(AST_Node* p, AST_Node* l, AST_Node* r);
        AST_Node(AST_Node* p, AST_Node* l, AST_Node* m, AST_Node* r);

        /* Public methods for pretty printing */
        void setPrintType(string s);
        virtual void printAST_Node(int indent);
    protected:
        /* Private methods for pretty printing */
        void printIndent(int indent);
};

/* A statement list node is derived from an AST node */
class SL_Node : public AST_Node {
    public:
        SL_Node(string terminal = "") : AST_Node(terminal) { }
        SL_Node(AST_Node* p, AST_Node* c) : AST_Node(p, c) { }
        SL_Node(AST_Node* p, AST_Node* l, AST_Node* r) : AST_Node(p, l, r) { }
        SL_Node(AST_Node* p, AST_Node* l, AST_Node* m, AST_Node* r) : AST_Node(p, l, m, r) { }
        virtual void printAST_Node(int indent);
};

/* A branch statement node is derived from an AST node */
class B_Node : public AST_Node {
    public:
        B_Node(string terminal = "") : AST_Node(terminal) { }
        B_Node(AST_Node* p, AST_Node* c) : AST_Node(p, c) { }
        B_Node(AST_Node* p, AST_Node* l, AST_Node* r) : AST_Node(p, l, r) { }
        B_Node(AST_Node* p, AST_Node* l, AST_Node* m, AST_Node* r) : AST_Node(p, l, m, r) { }
        virtual void printAST_Node(int indent);
};