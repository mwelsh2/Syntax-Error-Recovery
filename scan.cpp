/*  Adapted from:
    Simple ad-hoc scanner for the calculator language.
    Michael L. Scott, 2008-2020.

*/

#include "scan.h"

string token_image;

token scan() {
    string c;
    cin >> c; /* Gets next available string (excluding white space) */
    token_image = c; /* Saves string int global variable */

    if (cin.eof()) {
        return t_eof;
    } else {
        if (isalpha(c[0])) { /* If string of alphabetic characters */
        if (!token_image.compare("read")) { return t_read; }
        else if (!token_image.compare("write")) { return t_write; }
        else if (!token_image.compare("if")) { return t_if; }
        else if (!token_image.compare("while")) { return t_while;}
        else if (!token_image.compare("end")) { return t_end;}
        else { return t_id; }
        }
        else if (isdigit(c[0])) { /* If string of digits */
            return t_literal;
        } else { /* All other valid input */
            if (!token_image.compare(":=")) { return t_gets; }
            else if (!token_image.compare("(")) { return t_lparen; }
            else if (!token_image.compare(")")) { return t_rparen; }
            else if (!token_image.compare("+")) { return t_add; }
            else if (!token_image.compare("-")) { return t_sub; }
            else if (!token_image.compare("*")) { return t_mul; }
            else if (!token_image.compare("/")) { return t_div; }
            else if (!token_image.compare("=")) { return t_eq; }
            else if (!token_image.compare("<>")) { return t_neq; }
            else if (!token_image.compare("<")) { return t_less; }
            else if (!token_image.compare(">")) { return t_great; }
            else if (!token_image.compare("<=")) { return t_leq; }
            else if (!token_image.compare(">=")) { return t_geq; }
            else {
                cout << "Scan Error. " << c << "\n";
                exit(1);
            }
        }
    }
}
