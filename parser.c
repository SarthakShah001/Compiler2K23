#include "parser.h"
void fill_terminals(hashtable terminals){
    hash_init(terminals);
    for(int i=0;i<num_terminals;i++){
        hash_insert(terminals,terminal_str[i],i);
    }
}
void fill_nonterminals(hashtable nonterminals){
    hash_init(nonterminals);
    for(int i=0;i<num_nonterminals;i++){
        hash_insert(nonterminals,nonterminal_str[i],i);
    }
}
void fill_grammer(){
    FILE *fp=fopen("./grammar.txt","r");
    
}