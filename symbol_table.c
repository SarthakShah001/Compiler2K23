#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
symbol_table global_symbol_table ;
void s_hash_init(s_hashtable t){
    for(int i=0;i<HASHSIZE;i++){
    t[i].is_present=false;
    t[i].s=NULL;
    }
}
void s_hash_insert(s_hashtable t, ast_symbol s){
    int prob = 0;
    int hash_val = hash(s->name);
    while (t[hash_val].is_present)
    {
        prob++;
        hash_val = (hash_val + prob) % HASHSIZE;
    }
    t[hash_val].is_present = true;
    t->s=s;
}
bool is_symbol_present(s_hashtable t, ast_symbol s){
    int hash_val = hash(s->name);
    int prob = 0;
    while (t[hash_val].is_present == true)
    {
        if (strcmp(t[hash_val].s->name, s->name) == 0)
        {
            return true;
        }
        prob++;
        hash_val = (hash_val + prob) % HASHSIZE;
    }
    return false;
}
ast_symbol s_find_value(s_hashtable t, char* str){
    int hash_val = hash(str);
    int prob = 0;
    while (t[hash_val].is_present == true)
    {
        if (strcmp(t[hash_val].s->name, str) == 0)
        {
            return t[hash_val].s;
        }
        prob++;
        hash_val = (hash_val + prob) % HASHSIZE;
    }
    return NULL;
}
symbol_table symbol_table_init(){
symbol_table t=(symbol_table)malloc(sizeof(struct SYMBOL_TABLE));
t->parent=NULL;
t->head=NULL;
return t;
}
void insert_symbol(symbol_table t, ast_symbol s)
{
    if(t!=NULL){
        ast_symbol temp = t->head;
        if(temp == NULL){
            t->head = s;
            return;
        }
        while(temp->next!=NULL){
            temp = temp->next;
        }
        temp->next = s;
        return;
    }
    else{
        return;
    }
}
ast_symbol find_symbol(symbol_table t,char *str){
    return s_find_value(t->t,str);
}
// void delete_symbol(symbol_table t, ast_symbol s){
//     ast_symbol temp= find_symbol(t,s);
//     if(temp==NULL){
//     return;
//     }
//     else{
    
    
//     }

// }


