/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _hashtable_
#define _hashtable_
#include "hashTabledef.h"
//calculating hash value for a string
int hash(char *str);
// initialising an hash table
void hash_init(hashtable t);

void hash_insert(hashtable t,char *str,int value);
//searching in the hash table
int find_value(hashtable t,char *str);
bool is_str_present(hashtable t,char*str);
#endif
