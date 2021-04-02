/* Definitions the scanner shares with the parser
    Michael L. Scott, 2008-2020.
*/
#include <iostream>
#include <string>

using namespace std;

/* Enumeration of all tokens (represent all terminals of the grammar) */
typedef enum {t_read, t_write, t_id, t_literal, t_gets,
              t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, t_if, t_while, t_end, 
              t_eq, t_neq, t_less, t_great, t_leq, t_geq, t_eof, t_null} token;

/* Enumeration of all nonterminals of the grammar */
typedef enum {P, SL, S, C, E, T, F, TT, FT, ro, ao, mo, nt_null} nonterminal;

/* Enumeration of the empty string */
typedef enum {EPS, e_null} EPSILON;

extern string token_image;
extern token scan();
