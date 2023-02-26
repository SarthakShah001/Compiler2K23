#ifndef _hashtable_
#define _hashtable_
#include "hashTabledef.h"

int hash(char *str);
void hash_init(hashtable t);
void hash_insert(hashtable t,char *str,int value);
int find_value(hashtable t,char *str);
bool is_str_present(hashtable t,char*str);

#endif
