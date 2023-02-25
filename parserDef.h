#ifndef _parserdef_
#define _parserdef_
#include "lexer.h"

#define terminal tkType
#define num_terminals 56
#define num_nonterminals
#define num_rules 

typedef enum NONTERMINAL{

}nonterminal;

typedef struct SYMBOL* symbol;
struct SYMBOL{
    union element
    {
        terminal t;
        nonterminal nt;
    };
    bool is_terminal;
};
typedef struct RULES{
    nonterminal LHS;
    symbol head;
    symbol tail;
}rule;
rule grep[num_rules];
#endif