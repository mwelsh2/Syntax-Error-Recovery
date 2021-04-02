/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2020.
*/
#include <iostream>
#include <functional>
#include <algorithm>
#include "ast.h"
                       
string names[] = {"read", "write", "id", "literal", ":=",
                       "+", "-", "*", "/", "(", ")", 
                       "if", "while", "end", "=", "<>", "<", ">", "<=", ">=", "eof"};

string nt_names[] = {"program", "stmt_list", "stmt", "cond", 
                        "expr", "term", "factor", "term_tail", 
                        "factor_tail", "ro", "ao", "mo"};

static token input_token;

bool error = false; // if true, don't print the AST

string input;

/* If successful, returns an AST node represented by the token_image */
AST_Node* match (token expected) {
    if (input_token == expected) {
        AST_Node* return_node;
        if (input_token == t_id || input_token == t_literal)    {
            return_node = new AST_Node("\"" + string(token_image) + "\"");
        } else {
            return_node = new AST_Node(token_image);
        }
        input = input + token_image + " ";
        input_token = scan ();
        return return_node;
        
    } else {
        error = true;
        cout << "Syntax error occurred during match. Expected "; 
        if (expected == t_id || expected == t_literal) { cout << names[expected]; }
        else { cout << "\"" << names[expected]<< "\""; }
        cout << ". Received ";
        if (input_token == t_id || input_token == t_literal) { cout << names[input_token] << " (\"" << token_image << "\").\n"; }
        else { cout << "\"" << names[input_token] << "\".\n";} 
       
        if (input_token == t_eof) {
            cout << "End of file reached. Transformed input.\n" << input;
            exit(1);
        }
        input = input + names[expected] + " ";
        return (new AST_Node(names[expected]));
    }
}

/* All nonterminals return AST nodes of a syntax tree. */
AST_Node* program ();
SL_Node* stmt_list (SL_Node* s1);
AST_Node* stmt ();
AST_Node* cond ();
AST_Node* expr ();
AST_Node* term ();
AST_Node* term_tail (AST_Node* t1);
AST_Node* factor ();
AST_Node* factor_tail (AST_Node* f1);
AST_Node* rel_op ();
AST_Node* add_op ();
AST_Node* mul_op ();

/* Returns a list of tokens in FIRST and FOLLOW sets of the given nonterminal */
vector <token> FIRST(nonterminal X);
vector <token> FOLLOW(nonterminal X);

/* Method to help printing of error messages */
string vecToString(vector <token> T);

/* Recovers from an error when inside the given subroutine of the given nonterminal */
void recover (std::function<AST_Node* ()> subroutine, nonterminal X) {
    error = true;

    vector <token> first = FIRST(X);
    vector <token> follow = FOLLOW(X);

    cout << "Syntax error during " << nt_names[X] << ".";
    cout << " Expected " << vecToString(first) << ". Received " << names[input_token] << ".\n";
    
    input_token = scan ();
    while (input_token != t_eof) {
        if (find(first.begin(), first.end(), input_token) != first.end()) {
            subroutine();
            return;
        } else if (find(follow.begin(), follow.end(), input_token) != follow.end()) {
            return;
        } else {
            input_token = scan ();
        }
    }
    cout << "End of file reached. Could not find suitable token.\n";
}


AST_Node* program () {
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_eof: {           /* program -> stmt_list $$ */
            AST_Node* p_node = new AST_Node("program");
            SL_Node* sl_node = stmt_list (new SL_Node());
            AST_Node* eof_node = match (t_eof);
            AST_Node* root = new AST_Node(p_node, sl_node);
            return root;
        }
        default: 
            //matchError ();
            return (new AST_Node("ERROR"));
    }
}

SL_Node* stmt_list (SL_Node* s1) {
    try {
        switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while: {         /* stmt_list -> stmt stmt_list */
            AST_Node* s2 = stmt ();
            s1->children.push_back(s2);
            return stmt_list (s1);
        }
        case t_end:
        case t_eof:
            return s1;          /* stmt_list -> epsilon */
        default: throw input_token;
        }
    } catch (token input_token) {
        recover(stmt, S);
        return stmt_list(new SL_Node());
    }  
}

AST_Node* stmt () {
    try {
        switch (input_token) {
        case t_id: {            /* stmt -> id := expr */
            AST_Node* id_node = match (t_id);
            AST_Node* g_node = match (t_gets);
            AST_Node* e_node = expr ();
            return (new AST_Node(g_node, id_node, e_node));
        }
        case t_read: {          /* stmt -> read id */
            AST_Node* r_node = match(t_read);
            AST_Node* id_node = match(t_id);
            return (new AST_Node(r_node, id_node));
        }
        case t_write: {         /* stmt -> write expre */
            AST_Node* w_node = match(t_write);
            AST_Node* e_node = expr ();
            return (new AST_Node(w_node, e_node));
        }
        case t_if: {            /* stmt -> if cond stmt_list end */
            AST_Node* if_node = match (t_if);
            AST_Node* c_node = cond ();
            SL_Node* sl_node = stmt_list (new SL_Node());
            AST_Node* e_node = match (t_end);
            return (new B_Node(if_node, c_node, sl_node));
        }
        case t_while: {         /* stmt -> while cond stmt_list end */
            AST_Node* w_node = match(t_while);
            AST_Node* c_node = cond ();
            SL_Node* sl_node = stmt_list (new SL_Node());
            AST_Node* end_node = match (t_end);
            return (new B_Node(w_node, c_node, sl_node));
        }
        default: throw input_token;
        }
    } catch (token input_token) {
        recover(stmt, S);
        return (new AST_Node("ERROR"));
    }
    
}

AST_Node* cond () {
    try {
        switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen: {        /* cond -> expr ro expr */
            AST_Node* e1 = expr ();
            AST_Node* rel_node = rel_op ();
            AST_Node* e2 = expr ();
            return (new AST_Node(rel_node, e1, e2));
        }
        default: throw input_token;
        }
    } catch (token input_token) {
        recover(cond, C);
        return (new AST_Node("ERROR"));
    }
}

AST_Node* expr () {
    try {
        switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen: {        /* expr -> term term_tail */
            AST_Node* t_node = term ();
            return term_tail(t_node);
        }
        default: throw input_token;
        }
    } catch (token input_token) {
        recover(expr, E);
        return (new AST_Node("ERROR"));
    }
}

AST_Node* term () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen: {        /* term -> factor factor_tail */
            AST_Node* f_node = factor ();
            return factor_tail(f_node);
        }
        default: //matchError ();
        return (new AST_Node("ERROR"));
    }
}

AST_Node* term_tail (AST_Node* t1) {
    switch (input_token) {
        case t_add:
        case t_sub: {           /* term_tail -> ado term term_tail */
            AST_Node* add_node = add_op ();
            AST_Node* t2 = term ();
            AST_Node* n = new AST_Node(add_node, t1, t2);
            return term_tail (n);
        }
        case t_eq:
        case t_neq:
        case t_less:
        case t_great:
        case t_leq:
        case t_geq:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_end:
        case t_eof:
            return t1;          /* term_tail -> epsilon */
        default: 
            //matchError ();
            return (new AST_Node("ERROR"));
    }
}

AST_Node* factor () {
    switch (input_token) {
        case t_literal: {       /* factor -> lit */
            AST_Node* t_node = match (t_literal);
            AST_Node* lit_node = new AST_Node("num");
            return (new AST_Node(lit_node, t_node));
        }   
        case t_id : {           /* factor -> id */
            AST_Node* t_node = match (t_id);
            AST_Node* id_node = new AST_Node("id");
            return (new AST_Node(id_node, t_node));
        }
        case t_lparen: {        /* factor -> ( expr ) */
            AST_Node* lp_node = match (t_lparen);
            AST_Node* e_node = expr ();
            AST_Node* rp_node = match (t_rparen);
            return e_node;
        }   
        default: 
            //matchError ();
            return (new AST_Node("ERROR"));
    }
}

AST_Node* factor_tail (AST_Node* f1) {
    switch (input_token) {
        case t_mul:
        case t_div: {           /* factor_tail -> mo factor factor_tail */
            AST_Node* mul_node = mul_op ();
            AST_Node* f2 = factor ();
            AST_Node* n = new AST_Node(mul_node, f1, f2);
            return factor_tail (n);
        }
        case t_add:
        case t_sub:
        case t_rparen:
        case t_eq:
        case t_neq:
        case t_less:
        case t_great:
        case t_leq:
        case t_geq:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_end:
        case t_eof:
            return f1;          /* factor -> epsilon */
        default: 
            //matchError ();
            return (new AST_Node("ERROR"));
    }
}

AST_Node* rel_op () {
    switch (input_token) {
        case t_eq: {            /* ro -> = */
            AST_Node* eq_node = match (t_eq);
            return eq_node;
        }
        case t_neq: {           /* ro -> <> */
            AST_Node* neq_node = match (t_neq);
            return neq_node;
        }
        case t_less: {          /* ro -> < */
            AST_Node* less_node = match (t_less);
            return less_node;
        }
        case t_great: {         /* ro -> > */
            AST_Node* great_node = match (t_great);
            return great_node;
        }
        case t_leq: {           /* ro -> <= */
            AST_Node* leq_node = match (t_leq);
            return leq_node;
        }
        case t_geq: {           /* ro -> >= */
            AST_Node* geq_node = match (t_geq);
            return geq_node;
        }
        default: 
            //matchError ();
            return (new AST_Node("ERROR"));
    }
}

AST_Node* add_op () {
    switch (input_token) {
        case t_add: {           /* ao -> + */
            AST_Node* add_node = match (t_add);
            return add_node;
        }
        case t_sub: {           /* ao -> - */
            AST_Node* sub_node = match (t_sub);
            return sub_node;
        }
        default: 
           // matchError ();
            return (new AST_Node("ERROR"));
    }
}

AST_Node* mul_op () {
    switch (input_token) {
        case t_mul: {           /* mo -> * */
            AST_Node* mul_node = match (t_mul);
            return mul_node;
        }
        case t_div: {           /* mo -> / */
            AST_Node* div_node = match(t_div);
            return div_node;
        }
        default: 
            //matchError ();
            return (new AST_Node("ERROR"));
    }
}

/* Returns a list of tokens in the FIRST set of the given nonterminal */
vector <token> FIRST(nonterminal X) {
    switch (X) {
        case P:
            return {t_id, t_read, t_write, t_eof};
            break;
        case SL:
        case S:
            return {t_id, t_read, t_write, t_if, t_while};
            break;
        case C:
        case E:
        case T:
        case F:
            return {t_lparen, t_id, t_literal};
            break;
        case TT:
        case ao:
            return {t_add, t_sub};
            break;
        case FT: 
        case mo:
            return {t_mul, t_div};
            break;
        case ro:
            return {t_eq, t_neq, t_less, t_great, t_leq, t_geq};
            break;
        default: return {t_null};
    }
}

/* Returns a list of tokens in the FOLLOW set of the given nonterminal */
vector <token> FOLLOW(nonterminal X) {
    switch (X) {
        case P:
            return {t_null};
            break;
        case SL:
            return {t_eof};
            break;
        case S:
            return {t_id, t_read, t_write, t_if, t_while, t_eof};
            break;
        case C:
            return {t_id, t_read, t_write, t_if, t_while, t_end};
        case E:
        case TT:
            return {t_rparen, t_id, t_read, t_write, t_if, t_while, t_eof};
            break;
        case T:
        case FT:
            return {t_add, t_sub, t_rparen, t_id, t_read, t_write, t_if, t_while, t_eof};
            break;
        case F: 
            return {t_add, t_sub, t_mul, t_div, t_rparen, t_id, t_read, t_write, t_if, t_while, t_eof};
            break;
        case ro:
        case ao:
        case mo:
            return {t_lparen, t_id, t_literal};
            break;
        default: return {t_null};
    }
}

/* Converts a vector to a string for pretty printing */
string vecToString(vector <token> T) {
    string s = names[T.at(0)];
    int n = T.size();
    if (n > 2) {
        for (int i = 1; i < n - 1; i++) {
            s = s.append(", " + names[T.at(i)]);
        }
    }
    s = s.append(" or " + names[T.at(n - 1)]);
    return s;
}

int main () {
    input_token = scan ();
    AST_Node* p = program();
    if (!error) {
        p->printAST_Node(0);
    } else {
        cout << input;
    }
    cout << "\n\n";
    return 0;
}
